# psp-controller

This small project can turn on and off the LED using the ESP8266 as a microcontroller, but
It can be used for other purposes (e.g., controlling motors to build a simple robot) with little to medium modifications in the ESP8266 sketch file and C file.

[**CUSTOM FIRMWARE PSP IS REQUIRED!**](https://revive.today/psp/cfw/)

## PSP Controls

- Button ``X`` - Off state of LED
- Button ``O`` - On state of LED

## Building

> **Note**
> Use WSL2 or Ubuntu

### psp-controller(C file)

**REQUIRED**

- [PSPSDK Installation Guide](https://github.com/pspdev/pspdev)
- Make

run to build:

```bash
cd psp-controller/src/ && make
```

psp directory:

```
PSP > GAME > PSP-CONTROLLER > EBOOT.PBP
```

> Create a folder named ``psp-controller`` then put the ``EBOOT.PBP`` inside.

If you don't want to build it on your own, you can download the `EBOOT.PBP` file here [psp-controller](https://github.com/diamant3/psp-controller/blob/main/src/EBOOT.PBP)

### psp-controller(ESP8266)

**REQUIRED**

- [Arduino software](https://www.arduino.cc/en/software)
- [Nodemcu ESP8266 installation](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
- [Learn how to program esp8266](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/)

Download the arduino sketch file here [psp-esp8266](https://github.com/diamant3/psp-controller/blob/main/psp-controller.ino)

## Todo

- [x] Usable controller for PSP to esp8266 - [Video Showcase](https://twitter.com/__diamant3/status/1609030769246343175?s=20&t=RkDNyktUGP9mrwhqDLX7kg)
- [x] Delay bug - *Need to wait for 5-10 seconds before pushing the ``O``/``X`` button*
- [ ] PSP WiFi turn off automatically after 5-10 seconds :(

## Contribution

Open to any type of contributions, i have Todo section if you want to take, thanks!

## Credits

[PSPDEV](https://github.com/pspdev/)
