// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.cookie;

import android.content.Context;
import android.content.SharedPreferences;
import android.text.TextUtils;
import android.util.Log;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.net.CookieStore;
import java.net.HttpCookie;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class LynxCookieStore implements CookieStore {


    private static final String COOKIE_PREFS = "Lynx_CookiePrefsFile";//文件名称
    private static final String COOKIE_NAME_STORE = "names";//cookie 名key
    private static final String COOKIE_NAME_PREFIX = "cookie_";//cookie 名前缀


    /** this map may have null keys! */
    private final Map<URI, List<HttpCookie>> map = new HashMap<URI, List<HttpCookie>>();


    private final SharedPreferences cookiePrefs;
    private final ConcurrentHashMap<String, HttpCookie> cookies;
    public LynxCookieStore(Context context){
        cookiePrefs = context.getSharedPreferences(COOKIE_PREFS, Context.MODE_PRIVATE);
        cookies = new ConcurrentHashMap<>();
        loadCookiesFromFile(context);
        LynxCookieStore.sInstance = this;
    }
    public static LynxCookieStore sInstance;

    public synchronized void add(URI uri, HttpCookie cookie) {
        if (cookie == null) {
            throw new NullPointerException("cookie == null");
        }

        uri = cookiesUri(uri);
        if(cookie.getDomain() == null && uri.getHost() != null){
            cookie.setDomain(uri.getHost());
        }

        addCookie(uri,cookie);

        if(cookie.getDomain() != null && !cookie.getDomain().startsWith(".")){
            HttpCookie cookieDot = (HttpCookie) cookie.clone();
            cookieDot.setDomain("."+cookie.getDomain());
            addCookie(uri,cookieDot);
        }else if(cookie.getDomain()== null){
            Log.e("CookieStore",cookie.toString());
        }
    }

    private synchronized void addCookie(URI uri, HttpCookie cookie) {
        if (cookie == null) {
            throw new NullPointerException("cookie == null");
        }

        uri = cookiesUri(uri);
        List<HttpCookie> cookies = map.get(uri);
        if (cookies == null) {
            cookies = new ArrayList<HttpCookie>();
            map.put(uri, cookies);
        } else {
            cookies.remove(cookie);
        }
        cookies.add(cookie);

        saveCookie(uri,cookie);
    }

    private URI cookiesUri(URI uri) {
        if (uri == null) {
            return null;
        }
        try {
            return new URI("http", uri.getHost(), null, null);
        } catch (URISyntaxException e) {
            return uri; // probably a URI with no host
        }
    }

    //cookie 对象编码
    protected String encodeCookie(SerializableCookie cookie) {
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        try {
            ObjectOutputStream outputStream = new ObjectOutputStream(os);
            outputStream.writeObject(cookie);
        } catch (Throwable e) {
            return null;
        }

        return byteArrayToHexString(os.toByteArray());
    }
    //cookie 对象解码
    protected HttpCookie decodeCookie(String cookieStr) {
        byte[] bytes = hexStringToByteArray(cookieStr);
        ByteArrayInputStream is = new ByteArrayInputStream(bytes);
        HttpCookie cookie = null;
        try {
            ObjectInputStream ois = new ObjectInputStream(is);
            cookie = ((SerializableCookie) ois.readObject()).getCookie();
        } catch (Throwable e) {
            Log.e("Exception", e.getMessage());
        }

        return cookie;
    }

    private void clearExpired(){
        Set<String> keys = cookies.keySet();
        for(String key:keys){
            HttpCookie cookie = cookies.get(key);
            if(cookie.hasExpired()){
                remove(key,cookie);
            }
        }
    }

    private void addCookieToMap(URI uri,HttpCookie cookie){
        if (cookie == null) {
            throw new NullPointerException("cookie == null");
        }

        uri = cookiesUri(uri);
        List<HttpCookie> cookies = map.get(uri);
        if (cookies == null) {
            cookies = new ArrayList<HttpCookie>();
            map.put(uri, cookies);
        } else {
            cookies.remove(cookie);
        }
        cookies.add(cookie);
    }

    // 从文件中读取cookie
    private void loadCookiesFromFile(Context context){
        // Load any previously stored cookies into the store
        String storedCookieNames = cookiePrefs.getString(COOKIE_NAME_STORE, null);
        if (storedCookieNames != null) {
            String[] cookieNames = TextUtils.split(storedCookieNames, ", ");
            for (String name : cookieNames) {
                String encodedCookie = cookiePrefs.getString(COOKIE_NAME_PREFIX + name, null);
                if (encodedCookie != null) {
                    HttpCookie decodedCookie = decodeCookie(encodedCookie);
                    if (decodedCookie != null) {
                        cookies.put(name, decodedCookie);
                        String url = getURLByCookieKey(name);

                        addCookieToMap(URI.create(url),decodedCookie);
                    }
                }
            }

            // Clear out expired cookies
            clearExpired();
        }
    }

    // 将当前的HashMap存到 文件中
    private void saveCookie(URI uri,HttpCookie cookie){
        String hexURI = byteArrayToHexString(uri.toString().getBytes());
        // Save cookie into persistent store
        String name = hexURI+"_"+cookie.getName()+ "_"+cookie.getDomain()+"_"+cookie.hashCode();

        if (!cookie.hasExpired()) {
            cookies.put(name, cookie);
        } else {
            cookies.remove(name);
        }
        SharedPreferences.Editor editor = cookiePrefs.edit();
        editor.putString(COOKIE_NAME_STORE, TextUtils.join(", ", cookies.keySet()));
        editor.putString(COOKIE_NAME_PREFIX + name, encodeCookie(new SerializableCookie(cookie)));
        editor.commit();
    }

    //从cookie Key 中提取url
    private String getURLByCookieKey(String key){
        String[] names = key.split("_");
        return new String(hexStringToByteArray(names[0]));
    }
    public synchronized List<HttpCookie> get(URI uri) {
        if (uri == null) {
            throw new NullPointerException("uri == null");
        }
        uri = cookiesUri(uri);
        List<HttpCookie> result = new ArrayList<>();

        // get cookies associated with given URI. If none, returns an empty list
        List<HttpCookie> cookiesForUri = map.get(uri);
        if (cookiesForUri != null) {
            for (Iterator<HttpCookie> i = cookiesForUri.iterator(); i.hasNext(); ) {
                HttpCookie cookie = i.next();
                if (cookie.hasExpired()) {
                    i.remove(); // remove expired cookies
                } else {
                    result.add(cookie);
                }
            }
        }

        // get all cookies that domain matches the URI
        for (Map.Entry<URI, List<HttpCookie>> entry : map.entrySet()) {
            if (uri.equals(entry.getKey())) {
                continue; // skip the given URI; we've already handled it
            }

            List<HttpCookie> entryCookies = entry.getValue();
            for (Iterator<HttpCookie> i = entryCookies.iterator(); i.hasNext(); ) {
                HttpCookie cookie = i.next();
                if (!secureMatches(cookie, uri)) {
                    continue;
                }
                if (!HttpCookie.domainMatches(cookie.getDomain(), uri.getHost())) {
                    //Log.v("Cookie domain",cookie.getDomain()+"===="+uri.getHost());
                    continue;
                }
                if (cookie.hasExpired()) {
                    i.remove(); // remove expired cookies
                } else if (!result.contains(cookie)) {
                    result.add(cookie);
                }
            }
        }

        // 如果在当前cookie中没有获得cookie,则进行系统内cookie获取
//        if (result == null || (result != null && result.size() == 0)) {
//            result = SystemCookieUtil.getCookies(uri);
//            if (result == null) result = new ArrayList<>();
//        }

        return Collections.unmodifiableList(result);
    }

    /**
     * Returns true if {@code cookie} should be sent to {@code uri} with respect to the cookie's
     * secure attribute. Secure cookies should not be sent in insecure (ie. non-HTTPS) requests.
     * @param cookie
     * @param uri
     * @return
     */
    boolean secureMatches(HttpCookie cookie, URI uri) {
        return !cookie.getSecure() || "https".equalsIgnoreCase(uri.getScheme());
    }

    public synchronized List<HttpCookie> getCookies() {
        List<HttpCookie> result = new ArrayList<HttpCookie>();
        for (List<HttpCookie> list : map.values()) {
            for (Iterator<HttpCookie> i = list.iterator(); i.hasNext(); ) {
                HttpCookie cookie = i.next();
                if (cookie.hasExpired()) {
                    i.remove(); // remove expired cookies
                } else if (!result.contains(cookie)) {
                    result.add(cookie);
                }
            }
        }
        // 如果没有,则获取系统的
//        if (result == null || (result != null && result.size() == 0)) {
//            result = SystemCookieUtil.getCookies();
//        }
        return Collections.unmodifiableList(result);
    }

    public synchronized List<URI> getURIs() {
        List<URI> result = new ArrayList<URI>(map.keySet());
        result.remove(null); // sigh

        // 获取系统的
//        if (result == null || (result != null && result.size() == 0)) {
//            result = SystemCookieUtil.getURIs();
//            if (result == null) result = new ArrayList<>();
//        }
        return Collections.unmodifiableList(result);
    }

    public synchronized boolean remove(URI uri, HttpCookie cookie) {
        if (cookie == null) {
            throw new NullPointerException("cookie == null");
        }

        List<HttpCookie> cookies = map.get(cookiesUri(uri));
        if (cookies != null) {
            String hexURI = byteArrayToHexString(uri.toString().getBytes());
            // Save cookie into persistent store
            String name = hexURI+"_"+cookie.getName()+ "_"+cookie.getDomain()+"_"+cookie.hashCode();

            remove(name,cookie);
            return cookies.remove(cookie);
        } else {
            return false;
        }
    }

    public synchronized boolean remove(String name, HttpCookie cookie) {
        cookies.remove(name);

        SharedPreferences.Editor editor = cookiePrefs.edit();

        editor.putString(COOKIE_NAME_STORE, TextUtils.join(", ", cookies.keySet()));
        //editor.putString(COOKIE_NAME_PREFIX + name, encodeCookie(new SerializableCookie(cookie)));
        editor.remove(COOKIE_NAME_PREFIX + name);
        editor.apply();
        return false;
    }

    public synchronized boolean removeAll() {
        boolean result = !map.isEmpty();
        map.clear();
        SharedPreferences.Editor editor = cookiePrefs.edit();
        Set<String> keys = cookies.keySet();
        editor.putString(COOKIE_NAME_STORE, TextUtils.join(", ", cookies.keySet()));
        //editor.putString(COOKIE_NAME_PREFIX + name, encodeCookie(new SerializableCookie(cookie)));
        for(String key:keys){
            editor.remove(key);
        }
        editor.apply();
        cookies.clear();

        return result;
    }

    //cookie 序列化处理相关操作
    // Using some super basic byte array <-> hex conversions so we don't have
    // to rely on any large Base64 libraries. Can be overridden if you like!
    protected String byteArrayToHexString(byte[] b) {
        StringBuffer sb = new StringBuffer(b.length * 2);
        for (byte element : b) {
            int v = element & 0xff;
            if (v < 16) {
                sb.append('0');
            }
            sb.append(Integer.toHexString(v));
        }
        return sb.toString().toUpperCase();
    }

    protected byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }
    public class SerializableCookie implements Serializable {
        private static final long serialVersionUID = 6374381828722046732L;

        private transient final HttpCookie cookie;
        private transient HttpCookie clientCookie;

        public SerializableCookie(HttpCookie cookie) {
            this.cookie = cookie;
        }

        public HttpCookie getCookie() {
            HttpCookie bestCookie = cookie;
            if (clientCookie != null) {
                bestCookie = clientCookie;
            }
            return bestCookie;
        }

        private void writeObject(ObjectOutputStream out) throws IOException {
            out.writeObject(cookie.getName());
            out.writeObject(cookie.getValue());
            out.writeObject(cookie.getComment());
            out.writeObject(cookie.getDomain());
            out.writeObject(cookie.getMaxAge());
            out.writeObject(cookie.getPath());
            out.writeInt(cookie.getVersion());
            out.writeBoolean(cookie.getSecure());
        }

        private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
            String name = (String) in.readObject();
            String value = (String) in.readObject();
            clientCookie = new HttpCookie(name, value);
            clientCookie.setComment((String) in.readObject());
            clientCookie.setDomain((String) in.readObject());
            clientCookie.setMaxAge((long) in.readObject());
            clientCookie.setPath((String) in.readObject());
            clientCookie.setVersion(in.readInt());
            clientCookie.setSecure(in.readBoolean());
        }
    }

    public synchronized static LynxCookieStore initInstance(Context context) {
        if (sInstance == null) {
            sInstance = new LynxCookieStore(context);
        }
        return sInstance;
    }

    public static LynxCookieStore getDefault() {
        if (sInstance == null) {
            throw new RuntimeException("LynxCookieStore has not been inited !");
        }
        return sInstance;
    }
}