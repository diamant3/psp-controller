# psp-controller

use psp to control built-in led of esp8266 through C raw socket

[**CUSTOM FIRMWARE PSP IS REQUIRED!**](https://revive.today/psp/cfw/)

## PSP Controls

- Button ``X`` - Off state
- Button ``O`` - On state

## Building

> **Note**
> Use WSL2 or Ubuntu

### psp-controller

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

### psp-esp8266

**REQUIRED**

- [Arduino software](https://www.arduino.cc/en/software)
- [Nodemcu ESP8266 installation](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
- [Learn how to program esp8266](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/)

Download the arduino sketch file here [psp-esp8266](https://gist.github.com/diamant3/74d8ecb0d56b62a36000a3f1879ec2b8)

## Todo

Priority: 

- [x] Usable controller for PSP to esp8266 - [Video Showcase](https://twitter.com/__diamant3/status/1531199473027670016?s=20&t=UiEpDK-PkNDVUV-hazLGkA)
- [ ] Delay bug - *Need to wait for 5-10 seconds before pushing the ``O``/``X`` button*

Optional:

**Required a fix for the Delay bug before this**

- [ ] Ready code for medium to large esp8266 projects
- [ ] Compatible to any wifi enabled Microcontrollers
- [ ] Become an Arduino/ESP8266 Libraries 

## Contribution

Open to any type of contributions, i have Todo section if you want to take, thanks!

## Credits

[PSPDEV](https://github.com/pspdev/)
