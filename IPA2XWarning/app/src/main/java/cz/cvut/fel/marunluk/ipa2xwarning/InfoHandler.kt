package cz.cvut.fel.marunluk.ipa2xwarning

import android.util.Log
import java.lang.Double.longBitsToDouble
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong

class InfoHandler(private val longitude_p: AtomicLong, private val latitude_p: AtomicLong, private val speed_p: AtomicInteger) : Runnable {

    private var running = true;
    private var isServer: Boolean = false;
    private var ip: IntArray = intArrayOf(192, 168, 162, 10, 11811)

    override fun run() {
        val pub = initInfoPublisher(isServer, ip[0], ip[1], ip[2], ip[3], ip[4])
        while (running) {
            sendInfoPublisher(pub, longBitsToDouble(longitude_p.get()), longBitsToDouble(latitude_p.get()), speed_p.get())
            Thread.sleep(500)
        }
        killInfoPublisher(pub)
        Log.d("InfoHandler", "Thread exiting");
    }

    fun terminate() {
        running = false;
    }

    fun setIP(server: Boolean, ip_: IntArray) {
        isServer = server
        if (ip_.size == 5) {
            ip = ip_
        }
    }

    external fun initInfoPublisher(server: Boolean, ipA: Int, ipB: Int, ipC: Int, ipD: Int, port: Int): Long

    external fun killInfoPublisher(pointer: Long): Long

    external fun sendInfoPublisher(pointer: Long, longitude: Double, latitude: Double, speed: Int): Boolean

    companion object {
        init {
            System.loadLibrary("ipa2xwarning")
        }
    }
}