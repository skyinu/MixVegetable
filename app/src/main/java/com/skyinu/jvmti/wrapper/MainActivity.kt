package com.skyinu.jvmti.wrapper

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Debug
import android.os.Environment
import android.os.Process
import android.util.Log
import android.view.View
import com.example.nativelib.NativeLib
import com.skyinu.jvmti.libwrapper.NativeTiBridge
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private var count = 0
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        Thread({
            while (true) {
                Thread.sleep(10000)
                Log.e("TAG1", "TAG:" + Process.myPid() + " vs " + Any())
            }
        }, "dede").start()
        tvHello.setOnClickListener {
            tvHello.text = "${NativeTiBridge.getObjectSize(this)}"
            doOnClick(it)
        }
    }

    private fun doOnClick(view: View) {
        try {
            count += 2
            val result = NativeLib().stringFromJNI()
            LogUtil.loge(
                "lib fork ${result}-vs" + Process.myPid()
            )
            if (result.endsWith("0")) {
                LogUtil.loge("count=" + count + "id=" + Process.myPid())
                Thread {
                    while (true) {
                        LogUtil.loge(
                            "kill " + Process.myPid() + "name " + Thread.currentThread().name +
                                    " vs " + Thread.getAllStackTraces().keys.size
                        )
                        Debug.dumpHprofData(
                            "${Environment.getExternalStorageDirectory()}/Download/downloaded_rom/file.hprof"
                        )
                        Thread.sleep(500_000)
                    }
                }.start()
            }
            clickHello()
        } catch (ignore: Exception) {
            LogUtil.loge("EXEXEXE =" + Log.getStackTraceString(ignore))
        }
    }

    private fun clickHello() {
//        hitException()
    }

    private fun hitException() {
        throw Exception()
    }
}
