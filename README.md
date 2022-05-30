# PSP-Controller

use psp as a controller for esp8266 

## Directory guide

- ESP8266 - .ino directory for esp8266
- PSP-Controller - .c source directory for psp-controller 

## Building - PSP-Controller

*Recommended OS/platform: Linux or WSL(Windows)*

**REQUIRED**
- [pspsdk](github.com/pspdev/pspsdk)
- make

run to build:

```bash
make
```

```
--> PSP
     L--> GAME
         L--> <DIRECTORY NAME>
               L--> EBOOT.PBP
```

> DIRECTORY NAME - Need to create first a folder before putting the EBOOT.PBP in GAME directory.

## Building - ESP8266

**REQUIRED**

- [Arduino software](https://www.arduino.cc/en/software)
- [Nodemcu ESP8266 installation](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)
- [Learn how to program esp8266](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/)

Just click psp-controller.ino then upload sketch

## Credits

[PSPDEV](github.com/pspdev)

## Bugs
- need to wait for 5-10 seconds before pushing button
- messy code and readme huhu :( 
- just for fun hehehe awit
- this is a naive code, it uses http response to blink the esp8266(or just a substring hahah)

you can improve this, but no further development happens to this repo and it's just for learning purposes, thank you

## License
same as [pspsdk license](https://github.com/pspdev/pspsdk/blob/master/LICENSE) because mostly of the code are from their samples