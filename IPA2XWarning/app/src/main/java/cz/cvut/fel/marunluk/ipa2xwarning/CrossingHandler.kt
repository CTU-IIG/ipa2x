package cz.cvut.fel.marunluk.ipa2xwarning

import java.lang.Double.doubleToLongBits

class CrossingHandler(private var context: MainActivity) : Runnable {

    private var running = true;

    override fun run() {
        val sub = initCrossingSubscrier()
        while (running) {
            Thread.sleep(500)
        }
        killCrossingSubscriber(sub)
    }

    fun parseCrossing(danger: Boolean, crossing: Boolean, longitude: Double, latitude: Double) {
        context.drawDanger(danger, crossing)
        context.clatitude.set(doubleToLongBits(latitude))
        context.clongitude.set(doubleToLongBits(longitude))
        context.runOnUiThread {
            context.binding.crossingLatitudeText.text = "Crossing Latitude: $latitude"
            context.binding.crossingLongitudeText.text = longitude.toString()
        }
    }

    fun terminate() {
        running = false;
    }

    fun updateCrossingPublisherInfo(number: Int) {
        if (running) {
            context.subscribers.set(number)
            context.runOnUiThread { context.binding.warningsText.text = "$number publishers connected" }
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