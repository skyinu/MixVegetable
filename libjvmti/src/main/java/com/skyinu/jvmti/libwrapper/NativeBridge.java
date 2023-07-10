package com.skyinu.jvmti.libwrapper;

 import androidx.annotation.Keep;

 @Keep
class NativeBridge {

    static {
        System.loadLibrary("jvmti_wrapper");
    }

    native static long getObjSize(Object object);
}
