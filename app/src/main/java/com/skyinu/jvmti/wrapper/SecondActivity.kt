package com.skyinu.jvmti.wrapper

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import androidx.appcompat.app.AppCompatActivity
import androidx.core.os.bundleOf
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentActivity
import androidx.fragment.app.FragmentController
import androidx.fragment.app.FragmentHostCallback
import androidx.fragment.app.FragmentPagerAdapter
import androidx.lifecycle.Lifecycle
import androidx.viewpager2.adapter.FragmentStateAdapter
import com.skyinu.jvmti.wrapper.databinding.ActivitySecondBinding
import com.skyinu.jvmti.wrapper.fragment.TextFragment

class SecondActivity : AppCompatActivity() {
    private lateinit var binding: ActivitySecondBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivitySecondBinding.inflate(LayoutInflater.from(this))
        setContentView(binding.root)
        initFlFragment()
        initVpFragment()
    }

    private fun initFlFragment() {
        val fragA = TextFragment()
        fragA.arguments = bundleOf("title" to "title_a")
        val fragB = TextFragment()
        fragB.arguments = bundleOf("title" to "title_b")
        supportFragmentManager.beginTransaction().apply {
            add(binding.flContainer.id, fragA)
            add(binding.flContainer.id, fragB)
            hide(fragB)
            commitNow()
        }
        binding.flTv1.setOnClickListener {
            showFlFragment(fragA, fragB)
        }
        binding.flTv2.setOnClickListener {
            showFlFragment(fragB, fragA)
        }
        binding.flTv3.setOnClickListener {
            if(binding.flContainer.childCount > 0){
            binding.flContainer.removeAllViews()
                }else{
                binding.flContainer.addView(fragA.view)
                binding.flContainer.addView(fragB.view)
            }
        }
        binding.flTv4.setOnClickListener {
            supportFragmentManager.beginTransaction().apply {
                detach(fragB)
                show(fragA)
                commitNow()
            }
        }
    }

    private fun showFlFragment(showFragment: Fragment, hideFragment: Fragment) {
        supportFragmentManager.beginTransaction().apply {
            hide(hideFragment)
            show(showFragment)
            setMaxLifecycle(hideFragment, Lifecycle.State.STARTED)
            setMaxLifecycle(showFragment, Lifecycle.State.RESUMED)
            commitNow()
        }
    }

    private fun initVpFragment() {
        val adapter = object : FragmentStateAdapter(this) {
            override fun getItemCount(): Int {
                return 4
            }

            override fun createFragment(position: Int): Fragment {
                val frag = TextFragment()
                frag.arguments = bundleOf("title" to "title_$position")
                return frag
            }

        }
        binding.vpContainer.offscreenPageLimit = 1
        binding.vpContainer.adapter = adapter
        binding.vpTv1.setOnClickListener {
            binding.vpContainer.setCurrentItem(0)
        }
        binding.vpTv2.setOnClickListener {
            binding.vpContainer.setCurrentItem(1)
        }
        binding.vpTv3.setOnClickListener {
            binding.vpContainer.setCurrentItem(2)
        }
        binding.vpTv4.setOnClickListener {
            binding.vpContainer.setCurrentItem(3)
        }
    }
}