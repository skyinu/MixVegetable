package com.skyinu.jvmti.libwrapper

import android.content.Context
import android.os.Debug
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
        Files.copy(Paths.get(agentPath), Paths.get(copyAgentSoPath))
        Debug.attachJvmtiAgent(copyAgentSoPath, context.packageResourcePath, classLoader)
    }
}