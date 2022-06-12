# PSP-Controller

use psp to control built-in led of esp8266 

## Controls

- Button X - Off
- Button O - On

## Directory guide

- ESP8266 - arduino sketch (.ino)
- PSP-Controller - C source file (.c)

## Building - PSP-Controller

> **Note**
> Use Linux or Windows(WSL) - Latest Ubuntu

**REQUIRED**
- [pspsdk](https://github.com/pspdev/pspdev) - instruction guide installing pspsdk
- make

run to build:

```bash
make
```

psp directory: 

```
-->PSP
    L--> GAME
           L--> <FOLDER>
                    L--> EBOOT.PBP
```

**FOLDER** -> Create a folder then put the ```EBOOT.PBP``` inside.

## Building - ESP8266

> **Note**
> Use Windows

**REQUIRED**

- [Arduino software](https://www.arduino.cc/en/software)
- [Nodemcu ESP8266 installation](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
- [Learn how to program esp8266](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/)

Upload the ```psp-controller.ino``` sketch using Arduino IDE.

## Credits

[PSPDEV](https://github.com/pspdev/)

## Bugs

- need to wait for 5-10 seconds before pushing the button

## Contribution

Open to any type of contributions, thanks!

## License

[PSPSDK License](https://github.com/pspdev/pspsdk/blob/master/LICENSE)