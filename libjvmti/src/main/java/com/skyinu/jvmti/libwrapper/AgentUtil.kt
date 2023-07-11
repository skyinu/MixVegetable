package com.skyinu.jvmti.libwrapper

import android.content.Context
import android.os.Debug
import android.util.Log
import net.lingala.zip4j.ZipFile
import java.io.File
import java.nio.file.Files
import java.nio.file.Paths

object AgentUtil {
    private const val AGENT_SO_DIRECTORY = "agent"
    fun loadAgent(context: Context, agentSoName: String) {
        val classLoader = context.classLoader
        val findLibrary = classLoader.javaClass.methods.find { it.name == "findLibrary" }
        val agentPath = findLibrary!!.invoke(classLoader, agentSoName) as String
        val copyDestPath = File(context.filesDir, AGENT_SO_DIRECTORY)
        if (!copyDestPath.exists()) {
            copyDestPath.mkdir()
        }
        val copyAgentSoPath = "${copyDestPath}${File.separator}${agentSoName}.so"
        val copyAgentSoFile = File(copyAgentSoPath)
        if (copyAgentSoFile.exists()) {
            copyAgentSoFile.delete()
        }
        try {
            Files.copy(Paths.get(agentPath), Paths.get(copyAgentSoPath))
            Debug.attachJvmtiAgent(copyAgentSoPath, context.packageResourcePath, classLoader)
        } catch (th: Throwable) {
            Log.e("TAG_WRAPPER", Log.getStackTraceString(th))
            val apkPath = context.applicationInfo.sourceDir
            val soFileItem = agentPath.replace("$apkPath!/", "")
            ZipFile(context.applicationInfo.sourceDir)
                .extractFile(soFileItem, copyDestPath.absolutePath)
            Debug.attachJvmtiAgent(
                copyAgentSoPath + File.separator + soFileItem,
                context.packageResourcePath,
                classLoader
            )
        }
    }
}