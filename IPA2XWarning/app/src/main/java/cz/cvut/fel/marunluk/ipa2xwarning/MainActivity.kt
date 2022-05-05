package cz.cvut.fel.marunluk.ipa2xwarning

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.graphics.drawable.AnimatedVectorDrawable
import android.location.Location
import android.location.LocationListener
import android.location.LocationManager
import android.media.MediaPlayer
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import cz.cvut.fel.marunluk.ipa2xwarning.databinding.ActivityMainBinding
import java.lang.Double.doubleToLongBits
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong
import kotlin.math.roundToInt


class MainActivity : AppCompatActivity(), LocationListener {

    private lateinit var binding: ActivityMainBinding
    var sub = 0L
    var current: Boolean = false
    private lateinit var locationManager: LocationManager
    private val locationPermissionCode = 2
    lateinit var mp: MediaPlayer
    var baba = true

    private var longitude: AtomicLong = AtomicLong(0)
    private var latitude: AtomicLong = AtomicLong(0)
    private var speed: AtomicInteger = AtomicInteger(0)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.latitudeText!!.text = "Latitude: x"
        binding.longtitudeText.text = "Longtitude: x"
        binding.speedText.text = "Speed: x"
        binding.warningsText!!.text = "No warning publishers"

        val pubHandler = InfoHandler(longitude, latitude, speed)
        val pubHandlerT = Thread(pubHandler)

        locationManager = getSystemService(Context.LOCATION_SERVICE) as LocationManager
        if ((ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED)) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), locationPermissionCode)
        }
        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 500, 0.5f, this)

        binding.button.setOnClickListener {
            pubHandlerT.start()
            if (sub == 0L) {
                sub = initCrossingSubscrier()
            } else {
                killCrossingSubscriber(sub)
                sub = 0
            }
            Thread.sleep(200)
        }

        binding.button2.setOnClickListener {
            drawDanger(baba)
            baba = !baba
            getLocation()
        }
    }

    private fun getLocation() {

    }
    override fun onLocationChanged(location: Location) {

        longitude.set(doubleToLongBits(location.longitude))
        latitude.set(doubleToLongBits(location.latitude))

        binding.latitudeText!!.text = "Latitude: ${location.latitude}"
        binding.longtitudeText.text = "Longtitude: ${location.longitude}"
        if (location.hasSpeed()) {
            binding.speedText.text = "Rychlost: ${(location.speed * 3.6).toInt()}"
            speed.set((location.speed * 3.6).toInt())
        }
    }

    fun drawDanger(danger: Boolean) {
        if (danger != current) {
            current = danger
            if (this::mp.isInitialized) {
                mp.stop()
                mp.release()
            }

            if (danger) {
                mp = MediaPlayer.create(this, R.raw.chime1a)
                mp.start()
                runOnUiThread {
                    binding.image.setImageResource(R.drawable.blue_anim)
                    (binding.image.drawable as AnimatedVectorDrawable).start()
                }
            } else {
                mp = MediaPlayer.create(this, R.raw.chime2a)
                mp.start()
                runOnUiThread {
                    binding.image.setImageResource(R.drawable.red_anim)
                    (binding.image.drawable as AnimatedVectorDrawable).start()
                }
            }
        }
    }

    external fun initCrossingSubscrier(): Long

    external fun killCrossingSubscriber(pointer: Long): Boolean

    companion object {
        init {
            System.loadLibrary("ipa2xwarning")
        }
    }
}