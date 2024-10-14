# _WFR ECU 2024 Firmware_

## Setup
Vanilla Espressif-IDF was too yucky, I ported it back to platformio, If you wanna use just ESP-IDF you are on your own. For Platformio, just add the extension to your VSCode intallation and it should handle the rest.

### C/C++ Extension set-up(this might not need to be done with Platformio )
If you want linting to work correctly and not have Intelisense yelling at you constantly you'll need to modify the C/C++ extension settings so that it knows where the compiler and includepath is. Set the compiler path to `<IDF Path>\.espressif\tools\xtensa-esp-elf\esp-13.2.0_20240530\xtensa-esp-elf\bin\xtensa-esp32s3-elf-g++.exe` Where IDF Path is where you set the extension to install the IDF tools. By default it will be in your user folder. For the include path add `<IDF Components path>/esp/v5.3.1/esp-idf/components/**` where the IDF components path is again where you told the installer to put it. Once again by default it's in your user folder. 

### Using the JTAG Interface
For some reason in setting this project up I realized that the drivers that Windows might automatically try to use when you plug in your device are wrong and if you try to [flash/debug over JTAG](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/jtag-debugging/index.html) you will get a bunch of errors. First I tried installing [Zadig](https://zadig.akeo.ie/) and using it to change the drivers as suggested in [this forum post](https://community.platformio.org/t/how-to-use-jtag-built-in-debugger-of-the-esp32-s3-in-platformio/36042) but after that didn't work (it might work for you) just uninstalling the driver from device manager and letting Windows figure it out again from scratch fixed it. [You can look at this post for more details](https://github.com/espressif/idf-installer/issues/133#issuecomment-1098132203)

- ok more lore on this: I tried repeating this on a new device and a new devboard and it didn't work, I remembered that I also tried re-installing the drivers using the [ESP-IDF Tools Installer](https://dl.espressif.com/dl/esp-idf/?idf=4.4). This *still* didn't work, so what I tried after was connecting the device over serial first and flashing a program, then unplugging it again, and then connecting it over the USB/JTAG port again, and then it worked? I'm honestly pretty confused as this point, hopefully this is a quirk with my devboard and won't happen with the actual ECU


## Project Structure
_coming soon_

```
├── CMakeLists.txt
├── Components                  This is where the core project components live
│   ├── CAN                     
│   │   ├── CAN.cpp
│   │   ├── CAN.h
│   │   └── CMakeLists.txt
│   ├── Devices
│   │   ├── BMS.h
│   │   ├── BMS.cpp
│   │   ├── Inverter.h
│   │   ├── Inverter.cpp
│   │   ├── Pedals.h
│   │   ├── Pedals.cpp
│   │   ├── Devices.cpp
│   │   ├── Devices.h
│   │   └── CMakeLists.txt
│   ├── IO
│   │   ├── PinMap.h
│   │   ├── IO.cpp
│   │   ├── IO.h
│   │   └── CMakeLists.txt
│   ├── Sensors
│   │   ├── Sensors.cpp
│   │   ├── Sensors.h
│   │   └── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c                 Program entry point (but not really)
└── README.md                  This is the file you are currently reading
```

## Contribution Guide (Work In Progress)
