# psp-controller

use psp to control built-in led of esp8266

[**CUSTOM PSP IS REQUIRED!**](https://revive.today/psp/cfw/)

## PSP Controls

- Button ``X`` - Off state
- Button ``O`` - On state

## Directory guide

- psp-esp8266 - arduino sketch
- psp-controller - C source file

## Building

> **Note**
> Use WSL2 or Ubuntu

### psp-controller

**REQUIRED**
- [PSPSDK installation guide](https://github.com/pspdev/pspdev)
- make

run to build:

```bash
cd psp-controller && make
```

psp directory:

```
PSP > GAME > PSP-CONTROLLER > EBOOT.PBP
```

> Create a folder named ``psp-controller`` then put the ``EBOOT.PBP`` inside.

### psp-esp8266

**REQUIRED**

- [Arduino software](https://www.arduino.cc/en/software)
- [Nodemcu ESP8266 installation](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
- [Learn how to program esp8266](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/)

## Credits

[PSPDEV](https://github.com/pspdev/)

## Bug

- need to wait for 5-10 seconds before pushing the button

## Contribution

Open to any type of contributions, thanks!
