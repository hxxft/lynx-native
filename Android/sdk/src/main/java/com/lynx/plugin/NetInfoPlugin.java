package com.lynx.plugin;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import com.lynx.content.LynxApplicationDelegate;
import com.lynx.plugin.PluginImpl;

import java.util.Locale;


public class NetInfoPlugin extends PluginImpl {

    public static int NOT_REACHABLE = 0;
    public static int REACHABLE_VIA_CARRIER_DATA_NETWORK = 1;
    public static int REACHABLE_VIA_WIFI_NETWORK = 2;

    public static final String WIFI = "wifi";
    public static final String WIMAX = "wimax";
    // mobile
    public static final String MOBILE = "mobile";

    // Android L calls this Cellular, because I have no idea!
    public static final String CELLULAR = "cellular";
    // 2G network types
    public static final String TWO_G = "2g";
    public static final String GSM = "gsm";
    public static final String GPRS = "gprs";
    public static final String EDGE = "edge";
    // 3G network types
    public static final String THREE_G = "3g";
    public static final String CDMA = "cdma";
    public static final String UMTS = "umts";
    public static final String HSPA = "hspa";
    public static final String HSUPA = "hsupa";
    public static final String HSDPA = "hsdpa";
    public static final String ONEXRTT = "1xrtt";
    public static final String EHRPD = "ehrpd";
    // 4G network types
    public static final String FOUR_G = "4g";
    public static final String LTE = "lte";
    public static final String UMB = "umb";
    public static final String HSPA_PLUS = "hspa+";
    // return type
    public static final String TYPE_UNKNOWN = "unknown";
    public static final String TYPE_ETHERNET = "ethernet";
    public static final String TYPE_ETHERNET_SHORT = "eth";
    public static final String TYPE_WIFI = "wifi";
    public static final String TYPE_2G = "2g";
    public static final String TYPE_3G = "3g";
    public static final String TYPE_4G = "4g";
    public static final String TYPE_NONE = "none";

    private static final String LOG_TAG = "NetworkManager";


    BroadcastReceiver mReceiver;

    public NetInfoPlugin(String name, long ptr) {
        super(name, ptr);
    }

    public void getConnectInfo(long clientId, long methodId) {

            ConnectivityManager manager = (ConnectivityManager)
                    LynxApplicationDelegate.getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo info = manager.getActiveNetworkInfo();
        Object[] args = new Object[1];
        args[0] = this.getType(info);
        returnBack(clientId, methodId, true, args);
    }


    @Override
    public void addEvent(String event) {

        // We need to listen to connectivity events to update navigator.connection
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        final String e = event;
        if (this.mReceiver == null) {
            this.mReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                        ConnectivityManager manager = (ConnectivityManager)
                            LynxApplicationDelegate.getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
                        Object[] args = new Object[1];
                        args[0] = getType(manager.getActiveNetworkInfo());
                        dispatchEvent(e, args);
                }
            };
            LynxApplicationDelegate.getApplicationContext().registerReceiver(this.mReceiver, intentFilter);
        }
    }

    @Override
    public void removeEvent(String event) {
        if (this.mReceiver != null) {
            try {
                LynxApplicationDelegate.getApplicationContext().unregisterReceiver(this.mReceiver);
            } catch (Exception e) {

            } finally {
                mReceiver = null;
            }
        }
    }



    private String getType(NetworkInfo info) {
        if (info != null) {
            String type = info.getTypeName().toLowerCase(Locale.US);

            if (type.equals(WIFI)) {
                return TYPE_WIFI;
            }
            else if (type.toLowerCase().equals(TYPE_ETHERNET) || type.toLowerCase().startsWith(TYPE_ETHERNET_SHORT)) {
                return TYPE_ETHERNET;
            }
            else if (type.equals(MOBILE) || type.equals(CELLULAR)) {
                type = info.getSubtypeName().toLowerCase(Locale.US);
                if (type.equals(GSM) ||
                        type.equals(GPRS) ||
                        type.equals(EDGE) ||
                        type.equals(TWO_G)) {
                    return TYPE_2G;
                }
                else if (type.startsWith(CDMA) ||
                        type.equals(UMTS) ||
                        type.equals(ONEXRTT) ||
                        type.equals(EHRPD) ||
                        type.equals(HSUPA) ||
                        type.equals(HSDPA) ||
                        type.equals(HSPA) ||
                        type.equals(THREE_G)) {
                    return TYPE_3G;
                }
                else if (type.equals(LTE) ||
                        type.equals(UMB) ||
                        type.equals(HSPA_PLUS) ||
                        type.equals(FOUR_G)) {
                    return TYPE_4G;
                }
            }
        }
        else {
            return TYPE_NONE;
        }
        return TYPE_UNKNOWN;
    }
}

