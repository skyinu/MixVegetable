package com.skyinu.jvmti.wrapper;

import android.util.Log;

import java.lang.reflect.ParameterizedType;

public class EventCenter {
    interface Callback<T>{
        void onCall(T event);
    }

    public static  <Event> void register(Callback<Event> callback){
        try {
            ParameterizedType type = (ParameterizedType) callback.getClass().getGenericInterfaces()[0];
            Log.e("TAG2","YES YES");

        }catch (Throwable throwable){
            Log.e("TAG2","DD" + Log.getStackTraceString(throwable));
        }
    }
}
