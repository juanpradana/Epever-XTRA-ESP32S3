# Epever-XTRA-ESP32S3
This repository explain how to parse data from epever solar charge controller use ESP32S3 from RS485 ModBus.

## Register Reference
[ModBus Register Reference](https://github.com/juanpradana/Epever-XTRA-ESP32S3/blob/main/ControllerProtocolV2.3.pdf)

## Diagram
- Wiring
![EPEVER XTRA 4415N](https://user-images.githubusercontent.com/30497994/224992728-606a311b-c903-4a65-a154-52b0e8b32664.png)

- Device
![IMG_20230314_185140-min](https://user-images.githubusercontent.com/30497994/224994149-62e250bc-a056-43a6-ba85-973a52fead6b.jpg)

## Hardware
- SCC Epever Xtra
- ESP32 S3
- 2 way rs-485 to TTL converter

## Wire
- microcontroller to rs485 module

| ESP32 S3 | 2 Way RS485 to TTL |
| --- | --- |
| 3V | VCC |
| GND | GND |
| Pin 4 | RX |
| Pin 5 | TX |

- rs485 module to SCC

| RS485 Module | SCC |
| --- | --- |
| A+ | Green Cable |
| B- | Blue Cable |

## Result

![result](https://user-images.githubusercontent.com/30497994/224994304-6e362049-2dbe-4dad-ba6b-b7c004cc81b5.png)
Tested on MPPT 40A 48v EPEVER XTRA 4415N


## thanks to
[firebitlab](https://github.com/firebitlab)
