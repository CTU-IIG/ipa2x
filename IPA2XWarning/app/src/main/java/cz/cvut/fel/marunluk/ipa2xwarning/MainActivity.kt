package cz.cvut.fel.marunluk.ipa2xwarning

import android.Manifest
import android.content.SharedPreferences
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
import android.widget.Toast
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
    private lateinit var prefs: SharedPreferences

    private var currentState: Int = 0

    // Stored values
    var subscribers: AtomicInteger = AtomicInteger(0)
    var longitude: AtomicLong = AtomicLong(0)
    var latitude: AtomicLong = AtomicLong(0)
    var clongitude: AtomicLong = AtomicLong(0)
    var clatitude: AtomicLong = AtomicLong(0)
    var speed: AtomicInteger = AtomicInteger(0)
    var IPaddress: IntArray = intArrayOf(0, 0, 0, 0, 0)
    var started: Boolean = false

    // Threads runnables
    private val pubHandler = InfoHandler(longitude, latitude, speed)
    private val subHandler = CrossingHandler(this)

    // Thread instances
    private val pubHandlerT = Thread(pubHandler)
    private val subHandlerT = Thread(subHandler)

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        prefs = getSharedPreferences("ipa2x_prefs", MODE_PRIVATE)
        IPaddress[0] = prefs.getInt("ip0", 192)
        IPaddress[1] = prefs.getInt("ip1", 168)
        IPaddress[2] = prefs.getInt("ip2", 162)
        IPaddress[3] = prefs.getInt("ip3", 10)
        IPaddress[4] = prefs.getInt("ip4", 11811)

        configure()

        locationManager = getSystemService(LOCATION_SERVICE) as LocationManager
        if ((ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED)) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), locationPermissionCode)
        }
        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 500, 0.5f, this)

        mp1 = MediaPlayer.create(this, R.raw.chime1a)
        mp2 = MediaPlayer.create(this, R.raw.chime2a)


    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        configure()
        // Restore previous image
        drawDangerImage(currentState)
    }

    // Stop and terminte app
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

    // Init all GUI fields when constructing activity
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

        binding.modeSwitch.isChecked = prefs.getBoolean("switch", false)
        binding.ipA.filters = arrayOf<InputFilter>(IPFilter())
        binding.ipB.filters = arrayOf<InputFilter>(IPFilter())
        binding.ipC.filters = arrayOf<InputFilter>(IPFilter())
        binding.ipD.filters = arrayOf<InputFilter>(IPFilter())
        binding.port.filters = arrayOf<InputFilter>(PortFilter())
        manageIPFields(prefs.getBoolean("switch", false))

        if (started) {
            binding.startButton.isEnabled = false
            binding.modeSwitch.isEnabled = false
        }

        binding.button.setOnClickListener {
            finishAndRemoveTask()
        }
        
        binding.modeSwitch.setOnCheckedChangeListener { buttonView, isChecked ->
            prefs.edit().putBoolean("switch", isChecked).commit()
            manageIPFields(isChecked)
        }

        binding.startButton.setOnClickListener {
            verifyAndStartCommunication()
        }
    }

    // Checks if IP is valid and starts DDS handler threads
    fun verifyAndStartCommunication() {
        if (binding.modeSwitch.isChecked) {
            if (readIP()) {
                prefs.edit().putInt("ip0", IPaddress[0]).commit()
                prefs.edit().putInt("ip1", IPaddress[1]).commit()
                prefs.edit().putInt("ip2", IPaddress[2]).commit()
                prefs.edit().putInt("ip3", IPaddress[3]).commit()
                prefs.edit().putInt("ip4", IPaddress[4]).commit()
            } else {
                return
            }
        }
        started = true

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

    // Disable or enable (and put values from memory) IP fields
    fun manageIPFields(status: Boolean) {

        binding.ipA.isEnabled = status
        binding.ipB.isEnabled = status
        binding.ipC.isEnabled = status
        binding.ipD.isEnabled = status
        binding.port.isEnabled = status

        if (status) {
            binding.ipA.setText(IPaddress[0].toString())
            binding.ipB.setText(IPaddress[1].toString())
            binding.ipC.setText(IPaddress[2].toString())
            binding.ipD.setText(IPaddress[3].toString())
            binding.port.setText(IPaddress[4].toString())
        } else {
            binding.ipA.text.clear()
            binding.ipB.text.clear()
            binding.ipC.text.clear()
            binding.ipD.text.clear()
            binding.port.text.clear()
        }
    }

    // Read and parse IP addresses
    fun readIP(): Boolean {
        val ip_A = binding.ipA.text.toString().toIntOrNull()
        val ip_B = binding.ipB.text.toString().toIntOrNull()
        val ip_C = binding.ipC.text.toString().toIntOrNull()
        val ip_D = binding.ipD.text.toString().toIntOrNull()
        val port = binding.port.text.toString().toIntOrNull()

        if ((ip_A == null) || (ip_B == null) || (ip_C == null) || (ip_D == null) || (port == null)) {
            Toast.makeText(this@MainActivity, "Invalid IP or port!", Toast.LENGTH_SHORT).show()
            return false
        }
        IPaddress[0] = ip_A
        IPaddress[1] = ip_B
        IPaddress[2] = ip_C
        IPaddress[3] = ip_D
        IPaddress[4] = port

        return true
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

    // Filter to allow only valid IP values
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

    // Filter to allow only valid port values
    inner class PortFilter() : InputFilter {
        private var portMax: Int = 65535

        override fun filter(source: CharSequence, start: Int, end: Int, dest: Spanned, dStart: Int, dEnd: Int): CharSequence? {
            try {
                val input = Integer.parseInt(dest.toString() + source.toString())
                if ((input <= portMax)) {
                    return null
                }
            } catch (e: NumberFormatException) {
                e.printStackTrace()
            }
            return ""
        }
    }

}