# IPA2X: Intelligent Pedestrian Assistant to Everyone

This repository contains contributions of ČVUT to the [IPA2X
project][].

[IPA2X project]: https://rtsl.cps.mw.tum.de/ipa2x

The main goal is to create an Android application that will run on a
phone in the car. The application will:

- communicate with the rover via [Fast DDS]
- show warnings on car infotainment screen, hopefully via Android
  Auto screen mirroring.

[Fast DDS]: https://github.com/eProsima/Fast-DDS

## Discovery server setup

### Network
Devices should be connected to Wireguard VPN with subnet `192.168.162.0/24`. The Server is hardcoded to the address `192.168.162.10` (this address can be modified in the source code, but the Android application needs to be recompiled). Other devices can have any address in that subnet.

### Server
The source codes are located in the `warning_cpp\CrossingInfoServer\src` folder. To compile them, you need to have the [Fast DDS](https://fast-dds.docs.eprosima.com/en/latest/installation/sources/sources_linux.html) library installed. Run the Server on a device with an IP address of `192.168.162.10`.

#### Compilation
Create a build folder in the `warning_cpp\CrossingInfoServer` folder and run CMake and make in it
```
mkdir build
cd build
cmake ..
make
```
You can now start the `DDSServer` program. There should be no need to do anything else.

### Publisher and Subscriber
The procedure is the same as for the Server. It is necessary to have the [Fast DDS](https://fast-dds.docs.eprosima.com/en/latest/installation/sources/sources_linux.html) library installed. The IP addresses of these devices do not matter (as long as they are on the Wireguard network).

#### Compilation
Create a build folder in the `warning_cpp\CrossingInfoServer` folder and run CMake and make in it
```
mkdir build
cd build
cmake ..
make
```
You can now run `DDSPublisher`. Use the `x, c, d` keys to send and `q` to exit the application. The options need to be confirmed with the enter key.

### Android Subscriber
Install the app from the `warning_cpp.apk` file attached in the `apk` folder. For custom compilation and running from Android Studio, follow [this](https://gitlab.fel.cvut.cz/marunluk/fastdds-shapes) guide.

## External resources

- [Shapes DDS demo for
  Android](https://gitlab.fel.cvut.cz/marunluk/fastdds-shapes)
  (currently accessible only to selected people). This is our initial
  result of "porting" FastDDS to Android. It will be used as a
  baseline for developing IPA2X Android app.
