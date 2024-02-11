# psp-controller
![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/diamant3/psp-controller/build.yml)
![GitHub all releases](https://img.shields.io/github/downloads/diamant3/psp-controller/total)

## Introduction
psp-controller is just a prototype and inspired from the project of [iketsj's PSP Robot Controller](https://www.youtube.com/watch?v=do1674d6Rbo). 

I want to try his project, but there's no source code available. That's why I decided to create it on my own and make it open-source for others to learn from and modify on their own for those who want to try it.

## Video Demonstration
https://github.com/diamant3/psp-controller/assets/71203851/8f25e59e-b11e-4d7e-b2d6-7286f4b058bc

> The video is about the PlayStation Portable (PSP), which can turn on and off the LED connected to the ESP8266.

## PSP Controls
- Button ``X`` - Off state of LED
- Button ``O`` - On state of LED

## Building
> [!Note]
> Use WSL2 or Ubuntu

### `src` directory
Requirements:
- [PSPDEV portal](https://pspdev.github.io/)
- CMake(psp-cmake)
- Make

run to build:
```shell
mkdir build && cd build
psp-cmake ..
make
```
> Check [Releases](https://github.com/diamant3/psp-controller/releases) for the latest pre compiled EBOOT.PBP

## Run
> [!Important]
> [Custom firmware](https://youtu.be/h-pZeWV5Q8E?feature=shared) is required to run this, please see [issue](https://github.com/diamant3/psp-controller/issues/1).

> [!Caution]
> Custom firmware can cause damage to your PSP.

Now connect your PSP to your Computer and copy the `EBOOT.PBP` to your PSP's directory like this:

```
PSP/GAME/<dirname>/EBOOT.PBP
```

> `<dirname>` is an arbitrary name and put the EBOOT.PBP inside

### `psp-controller` directory
Requirements:
- [Arduino software](https://www.arduino.cc/en/software#future-version-of-the-arduino-ide)
- [Nodemcu ESP8266 installation](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
- [Learn how to program esp8266](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/)

Download the arduino sketch file here [psp-esp8266](https://github.com/diamant3/psp-controller/blob/main/psp-controller/psp-controller.ino)

## Contribution
I'm open to any type of contribution. I have issues if you want to take them. Thanks!

## Credits
[PSPDEV](https://github.com/pspdev/)
