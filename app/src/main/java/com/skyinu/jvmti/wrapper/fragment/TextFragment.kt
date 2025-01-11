package com.skyinu.jvmti.wrapper.fragment

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.skyinu.jvmti.wrapper.databinding.FragmentTextBinding

class TextFragment : Fragment() {

    private lateinit var binding: FragmentTextBinding
    private val title by lazy {
        arguments?.getString("title") ?: "default"
    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        super.onCreateView(inflater, container, savedInstanceState)
        binding = FragmentTextBinding.inflate(inflater)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        binding.textTv.setText(title)
        Log.e("Fragment","onViewCreated title = " + title + "  and this=" + this)
    }

    override fun onStart() {
        super.onStart()
        Log.e("Fragment","onStart title = " + title + "  and this=" + this)
    }

    override fun onResume() {
        super.onResume()
        Log.e("Fragment","onResume title = " + title + "  and this=" + this)
    }

    override fun onPause() {
        super.onPause()
        Log.e("Fragment","onPause title = " + title + "  and this=" + this)
    }

    override fun onStop() {
        super.onStop()
        Log.e("Fragment","onStop title = " + title + "  and this=" + this)
    }

    override fun onDestroyView() {
        super.onDestroyView()
        Log.e("Fragment","onDestroyView title = " + title + "  and this=" + this)
    }

    override fun onDestroy() {
        super.onDestroy()
        Log.e("Fragment","onDestroy title = " + title + "  and this=" + this)
    }
}