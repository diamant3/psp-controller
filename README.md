# psp-controller

## Introduction

psp-controller is just a prototype and inspired from the project of [iketsj's PSP Robot Controller](https://www.youtube.com/watch?v=do1674d6Rbo). 

I want to try his project, but there's no source code available. That's why I decided to create it on my own and make it open-source for others to learn from and modify on their own for those who want to try it.

## Video Demonstration

https://user-images.githubusercontent.com/71203851/210231258-2ea60264-ba62-42f9-b278-e22711ffaea5.mp4

> The video is about the PlayStation Portable (PSP), which can turn on and off the LED connected to the ESP8266.

[**CUSTOM FIRMWARE PSP IS REQUIRED!**](https://revive.today/psp/cfw/)

## PSP Controls

- Button ``X`` - Off state of LED
- Button ``O`` - On state of LED

## Building

> **Note**
> Use WSL2 or Ubuntu

### `src` directory

**REQUIRED**

- [PSPDEV Main Website](https://pspdev.github.io/)(*PSPSDK Installation guide and PSP apps/games programming examples here*.)
- CMake(psp-cmake)
- Make

run to build:

```bash
mkdir build && cd build
psp-cmake ..
make
```
> Check [Releases](https://github.com/diamant3/psp-controller/releases) for the latest pre compiled EBOOT.PBP

psp directory:

```
ms0:/PSP/GAME/<foldername>/EBOOT.PBP
```

> `<foldername>` is an arbitrary name and put the EBOOT.PBP inside

### `psp-controller` directory

**REQUIRED**

- [Arduino software](https://www.arduino.cc/en/software#future-version-of-the-arduino-ide)
- [Nodemcu ESP8266 installation](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
- [Learn how to program esp8266](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/)

Download the arduino sketch file here [psp-esp8266](https://github.com/diamant3/psp-controller/blob/main/psp-controller/psp-controller.ino)

## Contribution

I'm open to any type of contribution. I have issues if you want to take them. Thanks!

## Credits

[PSPDEV](https://github.com/pspdev/)
