<img src="assets/CWS.svg" width=200 align="right">


# Springbot • CWS (Core WiFi Springbot)
This is the Springbot core used in the Domino4 eco-system.
<table>
<tr style="background-color:#f47521;color:#ffffff;">
<th>Green</th><th>Gold</th>
</tr>
<tr style="background-color:#000;">
<td><img src="assets/CWS_Green.png" width=300 ></td><td><img src="assets/CWS_Gold.png" width=300></td>
</tr>
<tr style="background-color:#f47521;color:#ffffff;">
<th>5x5 LED Array • :x: NFC</th><th>128x64 OLED • :white_check_mark: NFC</th>
</tr>
</table>

## Programming in Arduino
To program the Domino4 cores using Arduino, install the board files using the doumentation from [Espressif](https://github.com/espressif/arduino-esp32)

Connection: USB-C (preferred) or PPU V2

Board: 'ESP32S2 Dev Module' - Flash Size: 4MB

Using PPU: Speed: Max 460800 bps - Chose the port where your PPU is inserted.

Using USB-C: USB CDC on Boot: Enabled - Chose the port where your Springbot is inserted.


Please read the notes regarding upload/transfer speed when using the [PPU](https://github.com/domino4com/PPU).
If you cannot see the port, the check out your [PPU installation](https://github.com/domino4com/PPU)

## Pin Usage
### Buttons 
#### Touch Buttons
| Symbol | GPIO | T# |
|:-----------------------------:|:----:|:--:|
| Ⓐ                | IO11 | T11|
| Ⓑ                | IO12 | T12|
| Logo              | IO13 | T13|

#### Push Buttons on the back
| Symbol | GPIO | Function |
|:-----------------------------:|:----:|:--:|
| &#x25B6;              | IO0 | Program |
| &#x21BB;              | n/a | Reset |

#### Special buttons usage: 
- Instead of power cycle your circuit in order to restart your program, you can click the &#x21BB; button.
- By holding down &#x25B6; and clicking &#x21BB;, you can force the ESP32S2 into PROGRAM mode. If you feel you have any issues programming the core, then try this. The <font color="#f00">&#x2600;</font> will illuminate permanently. You have to manually reset the core once programmed, by clicking &#x21BB;.
- You can install a UF2 Bootloaded by visiting https://apps.springbot.co.za
- If you use the UF2 Bootloaded to install a UF2 prepared application, then double-click on &#x21BB;.   The <font color="#f00">&#x2600;</font> will pulse.
- The Springbot comes preloaded with the UF2 bootloader. If you program the core using Arduino, you will overwrite the bootloaded. Simply follow the instructions on https://apps.springbot.co.za to re-install the bootloader.


### LEDs
#### Green only
| Postion | GPIO | 
|:-----------------------------|:----:|
|  Row 1 |IO8 |
|  Row 2 |IO40 |
|  Row 3 |IO10 |
|  Row 4 |IO38 |
|  Row 5 |IO33 |
|  Column 1 |IO3 |
|  Column 2 |IO42 |
|  Column 3 |IO41 |
|  Column 4 |IO6 |
|  Column 5 |IO7 |

#### Green & Gold on the back
| Postion | GPIO | 
|:-----------------------------|:----:|
|  Neopixel | IO39 | 
|  Red | IO17 | 

### Other Pins
| Function |  GPIO |Note|
|:-----------------------------|:----:|:--|
|  I²C SDA |5||
|  I²C SCL |4||
|  Serial TX |43||
|  Serial RX |44||
|  IO(1) pin |1||
|  IO2 pin |9||
|  IO3 pin |18||
|  Buzzer |15||
|  NTC Sensor |14||
|  Phototransistor |16||
|  IO3 |18||
|  IMU Interrupt |21||
|  NFC Interrupt |46|Only on Gold|
|  DblTap |2||

### SPI and SD Card
| SPI |  GPIO | 
|:-----------------------------|:----:|
|  MISO |37|
|  MOSI |35|
|  SCK  |36|
|  SD CS|34|


## Programming in Python.
- Download the MicroPython firmware from [micropython.org](https://micropython.org/download/esp32/)
- It is recommended to download and use the (Mu Editor)(https://codewith.mu/en/download)
- You can use the Mu Editor to upload the MicroPython Firmware.

## Troubleshooting
- If you try to upload code and getting this message ```A fatal error occurred: Timed out waiting for packet content``` or ```A fatal error occurred: Invalid head of packet (0xE0)```, change the transfer speed to 460800 pbs.
- If you try to upload code and getting this message ```[7886] Failed to execute script esptool the selected serial port [7886] Failed to execute script esptool
does not exist or your board is not connected```, your serial port is open by another application. Close the other application and try again.

# License: 
<img src="assets/CC-BY-NC-SA.svg" width=200 align="right">
Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License

[View License Deed](https://creativecommons.org/licenses/by-nc-sa/4.0/) | [View Legal Code](https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode)

