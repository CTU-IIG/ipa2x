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
import android.text.InputFilter
import android.text.Spanned
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

    // Stored values
    var subscribers: AtomicInteger = AtomicInteger(0)
    var longitude: AtomicLong = AtomicLong(0)
    var latitude: AtomicLong = AtomicLong(0)
    var clongitude: AtomicLong = AtomicLong(0)
    var clatitude: AtomicLong = AtomicLong(0)
    var speed: AtomicInteger = AtomicInteger(0)
    var IPaddress: IntArray = intArrayOf(192, 168, 162, 10, 11811)
    var started: Boolean = false

    // Threads runnables
    private val pubHandler = InfoHandler(longitude, latitude, speed)
    private val subHandler = CrossingHandler(this)

    // Thread instances
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

        // Start handler threads
        //pubHandlerT.start()
        //subHandlerT.start()

    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        configure()
        // Restore previous image
        drawDangerImage(currentState)
    }

    override fun onDestroy() {
        super.onDestroy()
        // Send terminate singal to threads
        pubHandler.terminate()
        subHandler.terminate()
        // Stop and release players
        mp1.stop()
        mp1.release()
        mp2.stop()
        mp2.release()
        // Wait for threads
        pubHandlerT.join()
        subHandlerT.join()
    }

    override fun onLocationChanged(location: Location) {

        // Update stored locations
        longitude.set(doubleToLongBits(location.longitude))
        latitude.set(doubleToLongBits(location.latitude))
        // Update onscreen location
        binding.latitudeText.text = "Latitude: ${location.latitude}"
        binding.longtitudeText.text = "Longtitude: ${location.longitude}"

        if (location.hasSpeed()) {
            // Update onscreen speed
            binding.speedText.text = "Rychlost: ${(location.speed * 3.6).toInt()}"
            // Update stored speed
            speed.set((location.speed * 3.6).toInt())
        }

    }

    private fun configure() {

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Init labels
        binding.latitudeText.text = "Latitude: ${longBitsToDouble(latitude.get())}"
        binding.longtitudeText.text = "Longtitude: ${longBitsToDouble(longitude.get())}"
        binding.speedText.text = "Speed: ${speed.get()}"
        binding.warningsText.text = "${subscribers.get()} publishers connected"
        binding.crossingLatitudeText.text = "Crossing Latitude: ${longBitsToDouble(clatitude.get())}"
        binding.crossingLongitudeText.text = "Crossing Longitude: ${longBitsToDouble(clongitude.get())}"

        binding.ipA.filters = arrayOf<InputFilter>(IPFilter())
        binding.ipB.filters = arrayOf<InputFilter>(IPFilter())
        binding.ipC.filters = arrayOf<InputFilter>(IPFilter())
        binding.ipD.filters = arrayOf<InputFilter>(IPFilter())
        binding.ipA.isEnabled = false
        binding.ipB.isEnabled = false
        binding.ipC.isEnabled = false
        binding.ipD.isEnabled = false
        binding.port.isEnabled = false

        if (started) {
            /*binding.ipA.isEnabled = false
            binding.ipB.isEnabled = false
            binding.ipC.isEnabled = false
            binding.ipD.isEnabled = false
            binding.port.isEnabled = false*/
            binding.startButton.isEnabled = false
            binding.modeSwitch.isEnabled = false
        }

        binding.button.setOnClickListener {
            finishAndRemoveTask()
        }
        
        binding.modeSwitch.setOnCheckedChangeListener { buttonView, isChecked ->  //Line A
            if (isChecked) {
                binding.ipA.isEnabled = true
                binding.ipB.isEnabled = true
                binding.ipC.isEnabled = true
                binding.ipD.isEnabled = true
                binding.port.isEnabled = true
                binding.ipA.setText(IPaddress[0].toString())
                binding.ipB.setText(IPaddress[1].toString())
                binding.ipC.setText(IPaddress[2].toString())
                binding.ipD.setText(IPaddress[3].toString())
                binding.port.setText(IPaddress[4].toString())
            } else {
                binding.ipA.isEnabled = false
                binding.ipB.isEnabled = false
                binding.ipC.isEnabled = false
                binding.ipD.isEnabled = false
                binding.port.isEnabled = false
                binding.ipA.text.clear()
                binding.ipB.text.clear()
                binding.ipC.text.clear()
                binding.ipD.text.clear()
                binding.port.text.clear()
            }
        }

        binding.startButton.setOnClickListener {

            started = true
            readIP()
            binding.ipA.isEnabled = false
            binding.ipB.isEnabled = false
            binding.ipC.isEnabled = false
            binding.ipD.isEnabled = false
            binding.port.isEnabled = false
            binding.startButton.isEnabled = false
            binding.modeSwitch.isEnabled = false

            pubHandler.setIP(binding.modeSwitch.isChecked, IPaddress)
            subHandler.setIP(binding.modeSwitch.isChecked, IPaddress)

            // Start handler threads
            pubHandlerT.start()
            subHandlerT.start()
        }

    }

    // Read and parse IP addresses
    fun readIP() {
        val ip_A = binding.ipA.text.toString().toIntOrNull()
        val ip_B = binding.ipB.text.toString().toIntOrNull()
        val ip_C = binding.ipC.text.toString().toIntOrNull()
        val ip_D = binding.ipD.text.toString().toIntOrNull()
        val port = binding.port.text.toString().toIntOrNull()

        if ((port != null) && (port in 1024..65535)) {
            IPaddress[4] = port
        } else {
            binding.port.setText(IPaddress[4].toString())
        }

        if ((ip_A == null) || (ip_B == null) || (ip_C == null) || (ip_D == null)) {
            binding.ipA.setText(IPaddress[0].toString())
            binding.ipB.setText(IPaddress[1].toString())
            binding.ipC.setText(IPaddress[2].toString())
            binding.ipD.setText(IPaddress[3].toString())
        } else {
            IPaddress[0] = ip_A
            IPaddress[1] = ip_B
            IPaddress[2] = ip_C
            IPaddress[3] = ip_D
        }
    }

    // View and play warning based on boolean states
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

    // Stop and rewind player
    private fun stopPlayer(mp: MediaPlayer) {
        if (mp.isPlaying) { mp.pause() }
        mp.seekTo(0)
    }

    // Displays proper animation based on state
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

    inner class IPFilter() : InputFilter {
        private var ipMin: Int = 0
        private var ipMax: Int = 255

        override fun filter(source: CharSequence, start: Int, end: Int, dest: Spanned, dStart: Int, dEnd: Int): CharSequence? {
            try {
                val input = Integer.parseInt(dest.toString() + source.toString())
                if ((ipMin <= input) && (input <= ipMax)) {
                    return null
                }
            } catch (e: NumberFormatException) {
                e.printStackTrace()
            }
            return ""
        }
    }

}