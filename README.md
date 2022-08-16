# IPA2X: Intelligent Pedestrian Assistant to Everyone

This repository contains contributions of ČVUT to the [IPA2X
project][].

[IPA2X project]: https://rtsl.cps.mw.tum.de/ipa2x

The main goal is to create an Android application that will run on a
phone in the car. The application will:

- communicate with the rover via [Fast DDS]
- show warnings on car infotainment screen, via Android Auto screen
  mirroring.

[Fast DDS]: https://github.com/eProsima/Fast-DDS

## Compilation

### C++ demo programs

Programs for testing the communication with the mobile application can
be compiled as follows:

1. Compile (or install) [Fast DDS
   libraries](https://fast-dds.docs.eprosima.com/en/latest/installation/sources/sources_linux.html).
2. Compile demo programs:
   ```sh
   cd warning_cpp
   mkdir build
   cd build
   cmake -DCMAKE_INSTALL_PREFIX=~/Fast-DDS/install ..
   make
   ```

   The above works with so called [local Fast DDS
   installation](https://fast-dds.docs.eprosima.com/en/latest/installation/sources/sources_linux.html#local-installation).
   If you installed Fast DDS differently, you may need to change
   `CMAKE_INSTALL_PREFIX` or remove it completely.

### Android application

Compiled Android application is available in the [apk
directory](./apk).

To compile the app from source using Android Studio, follow [this
guide](https://gitlab.fel.cvut.cz/marunluk/fastdds-shapes).

## Testing communication with the mobile phone

There are two ways how the communication can be tested:

- **local network with simple discovery** – all communicating devices (mobile phone,
  rover or computer running a demo program) should be connected to the
  same network (e.g. Wi-Fi access point).
- **VPN with a discovery server** – this will be used for final demo -
  all devices need to be connected to CVUT's VPN. We will provide VPN
  credentials for those who need them upon request.

The Android application works differently in portrait and landscape
mode. The **portrait** mode is meant for debugging and testing and display
various controls and settings. The **landscape** mode is meant for
mirroring the screen on the car.

### Local network

1. Run IPA2X Warning Android app.
2. In the portrait mode switch off *Discovery server* toggle.
3. Press *Start*
4. Run `./src/CrossPub` (from the `build` subdirectory) without any arguments.
5. Press `x<Enter>` to show the crossing warning on the phone.

### VPN

#### Connecting to the VPN

TODO

#### Testing

TODO

## External resources

- [Shapes DDS demo for
  Android](https://gitlab.fel.cvut.cz/marunluk/fastdds-shapes)
  (currently accessible only to selected people). This is our initial
  result of "porting" FastDDS to Android. It will be used as a
  baseline for developing IPA2X Android app.
