package com.skyinu.jvmti.libwrapper;

import android.util.Log;

import androidx.annotation.Keep;

@Keep
public class NativeTiBridge {
    void onThreadStart(String name) {
        Log.e("TAG1", "onThreadStart");
    }
}