# _WFR ECU 2024 Firmware_

## Setup
This project is meant to be set-up using the Espressif IDF "IDE" in VSCode. [Please follow the official instructions to set it up](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md)

Once the IDE is set-up clone this repository onto your computer and open up the folder in VSCode. If you don't know how to clone a repository this is probably not the project for you. Make sure the project builds correctly after cloning. Make sure to set the target chip to esp32s3.

### C/C++ Extension set-up
If you want linting to work correctly and not have Intelisense yelling at you constantly you'll need to modify the C/C++ extension settings so that it knows where the compiler and includepath is. Set the compiler path to `<IDF Path>\.espressif\tools\xtensa-esp-elf\esp-13.2.0_20240530\xtensa-esp-elf\bin\xtensa-esp32s3-elf-g++.exe` Where IDF Path is where you set the extension to install the IDF tools. By default it will be in your user folder. For the include path add `<IDF Components path>/esp/v5.3.1/esp-idf/components/**` where the IDF components path is again where you told the installer to put it. Once again by default it's in your user folder. 

### Using the JTAG Interface
For some reason in setting this project up I realized that the drivers that Windows might automatically try to use when you plug in your device are wrong and if you try to [flash/debug over JTAG](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/jtag-debugging/index.html) you will get a bunch of errors. First I tried installing [Zadig](https://zadig.akeo.ie/) and using it to change the drivers as suggested in [this forum post](https://community.platformio.org/t/how-to-use-jtag-built-in-debugger-of-the-esp32-s3-in-platformio/36042) but after that didn't work (it might work for you) just uninstalling the driver from device manager and letting Windows figure it out again from scratch fixed it. [You can look at this post for more details](https://github.com/espressif/idf-installer/issues/133#issuecomment-1098132203)


## Project Structure
_coming soon_