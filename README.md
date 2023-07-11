# psp-controller

This small project can turn on and off the LED using the ESP8266 as a microcontroller, but
It can be used for other purposes (e.g., controlling motors to build a simple robot) with little to medium modifications in the ESP8266 sketch file and C file.

## Video Demonstration

https://user-images.githubusercontent.com/71203851/210231258-2ea60264-ba62-42f9-b278-e22711ffaea5.mp4

[**CUSTOM FIRMWARE PSP IS REQUIRED!**](https://revive.today/psp/cfw/)

## PSP Controls

- Button ``X`` - Off state of LED
- Button ``O`` - On state of LED

## Building

> **Note**
> Use WSL2 or Ubuntu

### psp-controller(C file)

**REQUIRED**

- [PSPDEV Main Website](https://pspdev.github.io/)(*PSPSDK Installation guide and PSP apps/games programming examples here*.)
- CMake(psp-cmake)

run to build:

```bash
mkdir build && cd build
psp-cmake ..
make
```
> Check `build` directory for pre compiled EBOOT.PBP

psp directory:

```
PSP > GAME > PSP-CONTROLLER > EBOOT.PBP
```

> Create a folder named ``psp-controller`` then put the ``EBOOT.PBP`` inside.

### psp-controller(ESP8266)

**REQUIRED**

- [Arduino software](https://www.arduino.cc/en/software#future-version-of-the-arduino-ide)
- [Nodemcu ESP8266 installation](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
- [Learn how to program esp8266](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/)

Download the arduino sketch file here [psp-esp8266](https://github.com/diamant3/psp-controller/blob/main/psp-controller.ino)

## Contribution

Open to any type of contributions, i have Todo section if you want to take, thanks!

## Credits

[PSPDEV](https://github.com/pspdev/)
