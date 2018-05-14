package com.lynx.example;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

/**
 * Created by dli on 2018/5/14.
 */

public class DebugActivity extends Activity {

    private Button mButtom;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mButtom = new Button(this);
        this.setContentView(mButtom);

        mButtom.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d("lynx", "test");
                Bundle bundle = new Bundle();
                bundle.putString("page", "HomeDemo.vue");
                Intent intent = new Intent(DebugActivity.this, MainActivity.class);
                intent.putExtras(bundle);
                startActivity(intent);
            }
        });
    }


}
