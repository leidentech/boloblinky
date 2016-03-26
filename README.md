# Boloblinky retro game device made with a max7219 LED matrix and Arduino Nano

![boloblinky](/images/boloblinky.png?raw=true "Boloblinky")

I originally made this as a swag bag tchotchke for my 11-year-old son's birthday party.  It is designed so that the kids could build it themselves and get an introduction to soldering and acquainted with micro-controllers, programming and making stuff. My only two design parameters is that it had to be easy and relatively cheap - it had to be under/around €10 and came in at €9.81 including the PLA for the case and 4 batteries.

I decided on the nano and the max7219 because I've used them both before and I knew there were libraries for them that were easy to use and I could get both for under €3 each, leaving around €4 for the rest of the parts.

The program has different functions, which are incremented or decremented by long pressing the left or right button:

* 6-side die - hit either button to roll the die

* magic 8-ball type random advice giver - either button to get feedback

* image editor - hit the left button to go through the matrix pixel by pixel and the right button to turn it on or off.  When finished, long press either button to display the image

* race game - hit left or right to navigate through the random road.

* eyeball - animated eye that blinks and does crazy spins (eye code heavily borrows from work at https://github.com/michaltj/LedEyes/blob/master/LedEyes.ino)


The race game is something similar to one of the first programs I ever wrote on the Sinclair ZX81 a long time ago and reminiscent of the Mattel Electronics games I played in the '70s.  I had ideas of other games but since there are only two buttons the possibilities are limited.

Libraries used are: the Adafruit GFX library -  https://github.com/adafruit/Adafruit-GFX-Library and the Max72xxPanel library https://github.com/markruys/arduino-Max72xxPanel.git

## Build

OpenSCAD and STL files for this device can be found in the case directory.  There is also an alternate design with the micro switches inside the case.  My son preferred the switches on the outside.

Battery case:

place battery ends in the case so that the spring is at the end nearest the square plug bracket at the end where the switch is. Wire the nearest spring end underneath the bracket to the middle pin of the slide switch.   Wire the nearest pin of the slide switch to the (-) end of the two pin plug passing the wire underneath and through the bracket.

Wire the farthest flat end on the same side with a red wire to (+) side of the two pin plug passing the wire underneath and through the bracket.


Switches and Nano

![wiring](/images/wiring.png?raw=true "wiring")

Attach two wires with female plugs to the GND/VIN on the Nano next to the RST pin.

Wire the two pins nearest the bottom wall on the switches together, wire the middle pin on the switch nearest the Nano to D2 and the middle pin of the other switch to D3.  Wire the remaining pin on the switch nearest the Nano to GND next to D2 on the Nano.  Wire the remaining pin on the far switch with a female plug to go into the GND of the max7219.

Wire (with female plugs) the Nano's D10 to go to DIN on the max7219, The D11 to go to the CS pin on the max7219 and D13 to the CLK pin on the max7219.  Wire the 5V pin on the Nano to the 5V on the max7219.

This wiring corresponds to the Nano code and the libraries used.

Plug all the wires into the max7219 correctly and place that in the opening flush with the face of the top. Bend the pins as necessary to make the fit tight.  Slide the Nano into the slot so that the usb port is sticking out of the provided hole.

![schematic](/images/schematic.png?raw=true "schematic")

## PARTS LIST
Prices will undoubtedly change:

| Price | Part | link |
| --- | --- | --- |
| €2.34 | Arduino Nano | http://www.banggood.com/ATmega328P-Nano-V3-Controller-Board-Compatible-Arduino-p-940937.html |
| €2.08 | MAX7219 Red Dot Matrix Module | http://www.banggood.com/5Pcs-MAX7219-Dot-Matrix-Module-MCU-LED-Control-Module-Kit-For-Arduino-p-1031709.html |
| €0.80/4 | Batteries | |
| €0.56 | Slide Switch | http://nl.farnell.com/c-k-components/os102011ma1qn1/switch-spdt-0-1a-12v-pcb-r-a/dp/1201431?CMP=i-bf9f-00001000 - I've since seen cheaper slide switches - such as http://www.banggood.com/20PCS-Black-Mini-Size-SPDT-Slide-Switches-On-Off-PCB-5V-0_3A-DIY-Material-p-1009183.html |
| €1.73 | Battery Contacts 3x€.41 .21 .24 | http://nl.farnell.com/keystone/5212/battery-dual-contact-a-aa/dp/1650703?CMP=i-bf9f-00001000 http://nl.farnell.com/keystone/5224/battery-pos-a-aa/dp/1650704?CMP=i-bf9f-00001000 http://nl.farnell.com/keystone/5201/battery-holder-a-aa-1way/dp/1339845?CMP=i-bf9f-00001000 |
| €0.76/2 | Microswitches | http://nl.farnell.com/multicomp/dm1-01p-30-3/microswitch-spdt-1a-short-lever/dp/1735364?CMP=i-bf9f-00001000 |
| €1.44 | PLA for Case | |
| €0.10 | Wire | |