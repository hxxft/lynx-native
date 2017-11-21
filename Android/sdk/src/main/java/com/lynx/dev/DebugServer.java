package com.lynx.dev;

import android.os.Handler;
import android.os.Message;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by dli on 2017/9/23.
 */

public class DebugServer {
    private final static int DEVDEBUG_COMMAND_RELOAD = 1;

    private static int PORT = 8000;
    private static String DebugURL = "Debug://";

    private ServerSocket mServerSocket;
    private DebugDevHost mHost;
    private Handler mHandler;


    private void runCommand(final String command) {
        if(command.equals("reload")) {
            mHandler.sendEmptyMessage(DEVDEBUG_COMMAND_RELOAD);
        }
    }

    private void service(){
        while(true){
            Socket socket = null;
            try {
                socket = mServerSocket.accept();
                InputStream is = socket.getInputStream();
                InputStreamReader isr =new InputStreamReader(is);
                BufferedReader br =new BufferedReader(isr);
                runCommand(br.readLine());
            } catch (IOException e) {
                e.printStackTrace();
            }finally{
                if(socket!=null){
                    try {
                        socket.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    public void initialize() {
        mHandler = new Handler() {

            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what) {
                    case DEVDEBUG_COMMAND_RELOAD:
                        if(mHost != null)
                            mHost.debugHotReload(DebugURL);
                    default:
                        break;
                }
            }

        };

        try {
            startService();
        }catch (Exception e) {
            e.printStackTrace();
        }

    }

    public void startService() throws Exception{

        if(mServerSocket == null) {
            mServerSocket = new ServerSocket(PORT);

            new Thread(new Runnable() {
                @Override
                public void run() {
                    service();
                }
            }).start();
        }
    }

    public void setDebugDevHost(DebugDevHost host) {
        mHost = host;
    }
}
