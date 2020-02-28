# Arduino music player with Web GUI
Arudino runs a web server serving a web GUI used to control music playback.

This is a proof of a concept project made to test the possibility of controlling Arduino via web GUI.

# Required equipment
* Arduino (i.e. Uno)
* a small speaker (i.e. taken from an old laptop)
  * connect speaker to GND and output pin 9
* Ethernet Shield using ENC28J60 chip (i.e. DK Ethernet Shield v2.0)
* a computer with USB cable and Android IDE installed
* an network switch/hub

# Usage
Solution uses DHCP to obtain the IP address on which it will be listening for web requests.
The address will be logged to serial monitor (Tools > Serial monitor). Open it in you web browser and you will see the web UI.

# References
Created by combining the following examples:
* [*backSoon* example](https://github.com/njh/EtherCard/blob/master/examples/backSoon/backSoon.ino) by <jc@wippler.nl>
* [*Jingle Bells* example](http://repairmypc.net/2017/08/jingle-bells-arduino-sketch/) by Emmanuel
* [*SSDP.ino*](https://github.com/njh/EtherCard/blob/master/examples/SSDP/SSDP.ino) by Nicholas Humfrey

# License
This work is licensed under GPLv2
