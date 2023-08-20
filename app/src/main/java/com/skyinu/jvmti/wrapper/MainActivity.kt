package com.skyinu.jvmti.wrapper

import android.app.ActivityManager
import android.app.Dialog
import android.content.Context
import android.content.Intent
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Debug
import android.os.Environment
import android.os.Handler
import android.os.Looper
import android.os.Process
import android.util.Log
import android.view.View
import com.example.nativelib.NativeLib
import com.skyinu.jvmti.libwrapper.NativeTiBridge
import kotlinx.android.synthetic.main.activity_main.*
import java.util.Date

class MainActivity : AppCompatActivity() {

    private var count = 0
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        EventCenter.register<MainActivity> {
            Log.e("TAG1", "YES IT IS ")
        }
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
        tvHello2.setOnClickListener {
//            Log.e("TAG1", "hello worl".get(100)+"hhhh")
            val dialog = Dialog(this)
            dialog.setContentView(R.layout.dialog_main)
            dialog.show()
            Handler(Looper.myLooper()!!).postDelayed({
                val intent = Intent(this, SecondActivity::class.java)
                startActivity(intent)
            }, 3000)
        }
        tvHello3.setOnClickListener {
            val intent = Intent(this, SecondActivity::class.java)
            startActivity(intent)
            Handler(Looper.myLooper()!!).postDelayed({
                val dialog = Dialog(this)
                dialog.setContentView(R.layout.dialog_main)
                dialog.show()
            }, 3000)
        }
        val default = Thread.getDefaultUncaughtExceptionHandler()
        Thread.setDefaultUncaughtExceptionHandler { t, e ->
            val activityService = this.getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                val list = activityService.getHistoricalProcessExitReasons(
                    this.packageName,
                    0,
                    10
                )
                Log.e("TAG1", "hhhhh}" + this.packageName)
                list.forEachIndexed { index, info ->
                    Log.e(
                        "TAG1", "$index -${Date(info.timestamp).toString()} ${info.description} -" +
                                " ${info.processStateSummary.contentToString()}"
                    )
                }
            }
            default.uncaughtException(t, e)
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
        val activityService = this.getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            activityService.getHistoricalProcessExitReasons(this.packageName, Process.myPid(), 10)
        }
//        hitException()
    }

    private fun hitException() {
        throw Exception()
    }
}
