# LEGO DUPLO remote controlled locomotive build log

This log captures the process of modifying a Lego DUPLO machine to be remote controlled.


## Features

* go forward/backward, stop
* front and rear lights
* automatic shutdown (as the original engine)
* radio remote control
* adjustable (trim potentiometer) transmit power
* power & TX indication LEDs
* RX indication LED
* preserve the audio - power the original PCB on the engine; top button wired to MCU to allow using the engine without remote
 * short press - go forward
 * long press - go backward


# Build log

Timelapse of the build


## 2016-07-01

Ordered LEGO bricks from The_Duplo_Store via bricklink.com.

* [Used] Dark Bluish GrayDuplo, Train Base 4 x 8 with Light Bluish Gray Train Wheels and Moveable Hook
* [Used] OrangeDuplo, Train Locomotive Cabin Roof 2 x 4 Studs, 4 Medium Windows
* [Used] OrangeDuplo, Train Locomotive Front with Silver Headlight, 10508 and V Stripes Pattern
* [Used] RedDuplo, Train Passenger Locomotive Base with Black Battery Compartment, Light Bluish Gray Wheels
* [Used] YellowDuplo, Train Cab / Tender Base with Bottom Tubes with Box and Arrows Pattern on Both Sides


## 2016-07-08

Received the LEGO bricks.

![](img_20160709.jpg)

After _thorough_ cleanup, but hey, batteries included..


### Radio control
I've bought 433Mhz transmitter + receiver modules (http://www.ebay.com/itm/380717845396)

Technical parameters of the transmitter:

* Product Model: MX-FS-03V 
* Launch distance :20-200 meters (different voltage, different results) 
* Operating voltage :3.5-12V 
* Dimensions: 19 * 19mm
* Operating mode: AM 
* Transfer rate: 4KB/s 
* Transmitting power: 10mW 
* Transmitting frequency: 433M
* Pinout from left to right: (DATA; VCC; GND)


Technical parameters of the receiver

* Product Model MX-05V 
* Operating voltage: DC5V 
* Quiescent current: 4mA
* Receiving frequency: 433MHZ
* Receiver sensitivity:-105DB
* Size: 30 * 14 * 7mm
* An extra antenna is needed, please use spiral 50Ω a quarter wavelength antenna
  * http://www.csgnetwork.com/freqwavelengthcalc.html
  * 433MHz 1/4 wavelength antenna: 0.173m long

This is the first time I'm dealing with RF, so some troubles are expected, but I'm not rolling my own transmitter or receiver so they should be small.

#### Transmitter schematics

![](transmitter_1e9f973b79bf1e60976f5d36f49e2bdb1212c23c.pdf)
![](https://github.com/simonfojtu/LegoDuploLocomotive/raw/master/log/transmitter_1e9f973b79bf1e60976f5d36f49e2bdb1212c23c.pdf)
