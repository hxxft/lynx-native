// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.cookie;

import com.lynx.core.LynxRuntime;

import java.net.CookieStore;
import java.net.HttpCookie;
import java.net.URI;
import java.util.List;

public class LynxCookie {

    private LynxRuntime mRuntime;

    public LynxCookie(LynxRuntime context) {
        mRuntime = context;
    }

    public String getCookie(){
        StringBuffer rs = new StringBuffer();
        if(mRuntime.getPageURL() != null &&
                mRuntime.getPageURL().length()>5){
            CookieStore cookieStore = LynxCookieStore.getDefault();
            List<HttpCookie> cookies = cookieStore.get(URI.create(mRuntime.getPageURL()));
            int max = cookies.size()-1;
            int index = 0;

            if(cookies != null && cookies.size()>0){
                for(HttpCookie cookieItem:cookies){
                    if(index == max){
                        rs.append(cookieItem.toString());
                    }else{
                        rs.append(cookieItem.toString()+"; ");
                    }
                    index++;
                }
            }
        }

        return rs.toString();
    }

    public void setCookie(String data){
        if(data != null && data.length()>0 && mRuntime.getPageURL()  != null
                && mRuntime.getPageURL().length()>5){
            CookieStore cookieStore = LynxCookieStore.getDefault();
            List<HttpCookie> httpCookies = HttpCookie.parse("Set-Cookie:"+data);
            cookieStore.add(URI.create(mRuntime.getPageURL()), httpCookies.get(0));
        }
    }
}