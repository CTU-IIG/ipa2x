package cz.cvut.fel.marunluk.ipa2xwarning

import java.lang.Double.doubleToLongBits
import java.lang.Double.longBitsToDouble
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong

class InfoHandler(private val longitude_p: AtomicLong, private val latitude_p: AtomicLong, private val speed_p: AtomicInteger) : Runnable {

    private var running = true;

    override fun run() {
        val publisher = initInfoPublisher()
        while (running) {
            sendInfoPublisher(publisher, longBitsToDouble(longitude_p.get()), longBitsToDouble(latitude_p.get()), speed_p.get())
            Thread.sleep(500)
        }
        killInfoPublisher(publisher)
    }

    fun terminate() {
        running = false;
    }

    external fun initInfoPublisher(): Long

    external fun killInfoPublisher(pointer: Long): Long

    external fun sendInfoPublisher(pointer: Long, longitude: Double, latitude: Double, speed: Int): Boolean

    companion object {
        init {
            System.loadLibrary("ipa2xwarning")
        }
    }
}