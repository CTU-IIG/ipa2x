package cz.cvut.fel.marunluk.ipa2xwarning

import android.Manifest
import android.content.pm.PackageManager
import android.content.res.Configuration
import android.graphics.drawable.Animatable2
import android.graphics.drawable.AnimatedVectorDrawable
import android.graphics.drawable.Drawable
import android.location.Location
import android.location.LocationListener
import android.location.LocationManager
import android.media.MediaPlayer
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import cz.cvut.fel.marunluk.ipa2xwarning.databinding.ActivityMainBinding
import java.lang.Double.doubleToLongBits
import java.lang.Double.longBitsToDouble
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong


class MainActivity : AppCompatActivity(), LocationListener {

    private val locationPermissionCode = 2
    lateinit var binding: ActivityMainBinding
    private lateinit var locationManager: LocationManager
    private lateinit var mp1: MediaPlayer
    private lateinit var mp2: MediaPlayer

    private var currentState: Int = 0

    var subscribers: AtomicInteger = AtomicInteger(0)
    var longitude: AtomicLong = AtomicLong(0)
    var latitude: AtomicLong = AtomicLong(0)
    var clongitude: AtomicLong = AtomicLong(0)
    var clatitude: AtomicLong = AtomicLong(0)
    var speed: AtomicInteger = AtomicInteger(0)

    private val pubHandler = InfoHandler(longitude, latitude, speed)
    private val subHandler = CrossingHandler(this)

    private val pubHandlerT = Thread(pubHandler)
    private val subHandlerT = Thread(subHandler)

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        configure()

        locationManager = getSystemService(LOCATION_SERVICE) as LocationManager
        if ((ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED)) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), locationPermissionCode)
        }
        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 500, 0.5f, this)

        mp1 = MediaPlayer.create(this, R.raw.chime1a)
        mp2 = MediaPlayer.create(this, R.raw.chime2a)

        pubHandlerT.start()
        subHandlerT.start()

    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        configure()
        drawDangerImage(currentState)
    }

    override fun onDestroy() {
        super.onDestroy()
        pubHandler.terminate()
        subHandler.terminate()
        mp1.stop()
        mp1.release()
        mp2.stop()
        mp2.release()
        pubHandlerT.join()
        subHandlerT.join()
    }

    override fun onLocationChanged(location: Location) {

        longitude.set(doubleToLongBits(location.longitude))
        latitude.set(doubleToLongBits(location.latitude))
        binding.latitudeText.text = "Latitude: ${location.latitude}"
        binding.longtitudeText.text = "Longtitude: ${location.longitude}"

        if (location.hasSpeed()) {
            binding.speedText.text = "Rychlost: ${(location.speed * 3.6).toInt()}"
            speed.set((location.speed * 3.6).toInt())
        }

    }

    private fun configure() {

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.latitudeText.text = "Latitude: ${longBitsToDouble(latitude.get())}"
        binding.longtitudeText.text = "Longtitude: ${longBitsToDouble(longitude.get())}"
        binding.speedText.text = "Speed: ${speed.get()}"
        binding.warningsText.text = "${subscribers.get()} publishers connected"
        binding.crossingLatitudeText.text = "Crossing Latitude: ${longBitsToDouble(clatitude.get())}"
        binding.crossingLongitudeText.text = "Crossing Longitude: ${longBitsToDouble(clongitude.get())}"

        binding.button.setOnClickListener {
            finishAndRemoveTask()
        }
    }

    fun drawDanger(danger: Boolean, crossing: Boolean) {

        var cs: Int
        if (danger) {
            cs = 2
        } else if (crossing) {
            cs = 1
        } else {
            cs = 0
        }

        if (cs != currentState) {
            currentState = cs

            stopPlayer(mp1)
            stopPlayer(mp2)

            when (cs) {
                1 -> { mp1.start() }
                2 -> { mp2.start() }
            }

            drawDangerImage(cs)
        }
    }

    private fun stopPlayer(mp: MediaPlayer) {
        if (mp.isPlaying) { mp.pause() }
        mp.seekTo(0)
    }

    private fun drawDangerImage(state: Int) {
        when (state) {
            1 -> {
                runOnUiThread {
                    binding.image.setImageResource(R.drawable.blue)
                    val a = binding.image.drawable as AnimatedVectorDrawable
                    a.registerAnimationCallback(object : Animatable2.AnimationCallback() {
                        override fun onAnimationEnd(drawable: Drawable?) {
                            super.onAnimationEnd(drawable)
                            binding.image.post {a.start()}
                        }
                    } )
                    a.start()
                }
            }
            2 -> {
                runOnUiThread {
                    binding.image.setImageResource(R.drawable.red)
                    val a = binding.image.drawable as AnimatedVectorDrawable
                    a.registerAnimationCallback(object : Animatable2.AnimationCallback() {
                        override fun onAnimationEnd(drawable: Drawable?) {
                            super.onAnimationEnd(drawable)
                            binding.image.post {a.start()}
                        }
                    } )
                    a.start()

                }
            }
            else -> runOnUiThread { binding.image.setImageResource(android.R.color.transparent) }
        }
    }


}