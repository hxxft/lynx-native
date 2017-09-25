package com.lynx.dev;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;

/**
 * Created by dli on 2017/9/23.
 */

public class DebugLogger extends Thread{


    private boolean mIsAlive = true;

    private static String DebugLogURL = "http://127.0.0.1:3000/log";

    public DebugLogger() {
        mIsAlive = true;
    }

    public String postLog(String url, String log){
        HttpURLConnection conn = null;
        PrintWriter pw = null ;
        BufferedReader rd = null ;
        StringBuilder sb = new StringBuilder ();
        String line = null ;
        String response = null;
        try {
            conn = (HttpURLConnection) new URL(url).openConnection();
            conn.setRequestMethod("POST");
            conn.setDoOutput(true);
            conn.setDoInput(true);
            conn.setReadTimeout(20000);
            conn.setConnectTimeout(20000);
            conn.setUseCaches(false);
            conn.connect();
            pw = new PrintWriter(conn.getOutputStream());
            pw.print(log);
            pw.flush();
            rd  = new BufferedReader( new InputStreamReader(conn.getInputStream(), "UTF-8"));
            while ((line = rd.readLine()) != null ) {
                sb.append(line);
            }
            response = sb.toString();
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally{
            try {
                if(pw != null){
                    pw.close();
                }
                if(rd != null){
                    rd.close();
                }
                if(conn != null){
                    conn.disconnect();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return response;
    }



    @Override
    public void run() {
        try {
            try {
                //获取logcat日志信息
                String[] cmdLine = new String[]{"logcat", "|", "grep", "lynx"};
                Process mLogcatProc = Runtime.getRuntime().exec(cmdLine);
                BufferedReader reader = new BufferedReader(new InputStreamReader(mLogcatProc.getInputStream()));
                String line;
                while (mIsAlive && (line = reader.readLine()) != null) {
                    int i = line.indexOf("lynx-js-console");
                    if (i != -1) {
                        final String finalLine = line.substring(i, line.length());
                        String content = "log=" + URLEncoder.encode(finalLine, "UTF8");
                        postLog(DebugLogURL, content);
                    }
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }catch (Exception e) {
            e.printStackTrace();
        }
    }

}
