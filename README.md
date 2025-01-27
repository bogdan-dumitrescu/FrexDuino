## FrexDuino SimWheel Controller

### About this project

FrexDuino has been specifically developed to support the [Frex SimWHEEL (v1)](https://www.youtube.com/watch?v=tcqOE-FSnFI). The Frex was originally delivered with an Immersion FFB controller. This controller works fine, but only with 32b games. Many 64b games (Asseto Corsa, Automobilista 2, Dirt 2) crash when using the Immersion driver.

An Arduino Leonardo running the FrexDuino firmware will be recognized by Windows as a generic input device with force feedback capabilities, solving the Immersion driver issue by simply not using it.

The FrexDuino project is meant to be a complete replacement for the Immersion controller box. In order to get the Frex wheel working with FrexDuino, you will need:

 - the firmware in this project
 - an [Arduino Leonardo](https://amzn.to/4huuYeO) board to run the firmware
 - a [DC motor controller](https://amzn.to/4jzXH3w) to drive the wheel's motor
 - a [power converer](https://amzn.to/3EcqdIw) to convert from 24V to 5V
 - a [power supply](https://amzn.to/4jzXH3w) to power everything up
 - fuses, wires, project box & the electronics knowledge to put it all together

#### Example of finished product

When it all goes well, you will have a box which connects to the Frex wheel (using the original encoder, motor & button cables). The box will also provide a micro-USB port for connecting to your PC.

![](box.jpg)

### Frex SimWHEEL: what works / what doesn't

- works: wheel position, force feedback, and 8 wheel buttons. 
- doesn't: H-shifter, sequential shifter, hand brake, pedals, LED.

I own the simplest Frex wheel: only 8 buttons on the wheel adapter (including the 2 paddle shifters). And I have no pedals or shifters connected to my Frex wheel controller. So far I implemented the following Frex SimWHEEL connections:

 - optical encoder to keep track of the position
 - DC motor +/- to send the actual FFB signal
 - ethernet cable supporting the button matrix

The basics are all there: position, force feedback and wheel buttons. I intentionally left the wheel LED unconnected for now. 

There are enough inputs left on the Arduino to add support for Frex pedals and/or shifters that were originally delivered with the wheel - PRs for this are welcome.

### Building & uploading the firmware

 - this project requires [PlatformIO](https://platformio.org/platformio-ide). Download and install the PlatformIO IDE. I use the VSCode extension, it works great.
 - clone this GitHub repository. No other dependencies are needed. 
 - open the cloned repository folder as a project in the PlatformIO context (make sure you do it from the PlatformIO Home). You can now build and upload the project to your Arduino Leonardo board. 

### Wiring diagram

![](wiring.svg)

The above diagram shows the basic wiring sketch for controlling the Frex SimWHEEL. There are no fuses shown. I used two for my FrexDuino, one for the 5V components, and another for the 24V ones. 2 amps and 5 amps, respectively. 

Note that the Frex connections drawn represent the actual cables coming from the wheel (and **not** the Immersion box). You should have at least 3 cables coming out the back of your wheel:

- the thicker black & white pair connected to the DC motor
- the thinner 4 wire cable connected to the optical encoder 
- the ethernet cable connected to the wheel adapter / buttons / LED

#### Arduino Pins

The pins connecting the optical encoder (2 & 3) and motor controller (9 & 10) are linked to timers & interrupts in the code. They cannot easily be changed without complex code changes. 

The pins for the button matrix can be changed by changing the variables defined in the [code](src/buttons.cpp). 

#### Optical Encoder 

It's important to get the optical encoder's `5V` and `GND` wires right. Mine were obvious, red and black, respectively. Google the part number on your optical encoder to make sure.

If turning the wheel is moving the axis in the wrong direction, switch the 2 & 3 pins around.

#### Button Matrix

Frex uses an ethernet cable to connect the button matrix. The colors mentioned in the diagram are for the **ethernet wires**. R1 & R2 are the row pins, while C1, C2... are the column ones. 

As each wheel was hand-made, it could be that yours is wired differently than mine. If needed, you can find out which is which for your wheel by tracing the wires for the buttons and following one simple rule:

 - each button is **always** connected to precisely one row and one column.

It's not a problem if you switch the row pins around. You can also switch the columns around. But it is important to not switch a row pin with a column pin. 

The green wire on the ethernet cable is unused for my wheel. It could be used as a third row pin to wire up more buttons (if you have the fancier wheel adapter). 

The Frex uses the orange wire to also wire up the LED, together with the Orange/White wire (which is not used for the buttons). I left the LED unconnected, as previously mentioned.

#### BTS7960 motor controller 

Make sure that the `B+/-` connections are going to the correct polarities of the power supply and use a fuse with the correct rating.

If the force feedback feels like it's in the wrong direction, you can switch around pins 1 & 2 of the BTS7960 controller. 

### FAQ

**Q: Does this actually work?**

**A:** Yes, most definitely. So far I have tested a FrexDuino box with GT Legends, GTR2, Automobilista 2, and iRacing. All work fine and provide the expected force feedback. Looking forward to your feedback as well!

**Q: Will it burn my precious Frex / encoder / house down?**

**A:** This project requires electronics knowledge, and I am not responsible for any damaged directly or indirectly caused by using FrexDuino.

**Q: Why the Arduino Leonardo? Can I use a Uno?**

**A:** The Leonardo has a feature that allows it to be recognized as a USB device. The original Uno will not work for this purpose. Many other boards should work fine (the Arduino Micro, for example). But I have only tested this project with the Arduino Leonardo.

**Q: Can I use this firmware / wiring diagram for any other wheel than Frex?**

**A:** Although FrexDuino has been specifically developed with the Frex SimWHEEL in mind, FFB wheels are quite similar to each other. If your wheel uses a single motor & an optical encoder, you should be easily able to adapt the firmware to work in your situation. Keep in mind though that the Frex uses a simple button matrix wiring for registering the button presses. Other wheels might use a more complex system (like a shift register.)

### Credits & appreciation

This project uses the excelent [ArduinoJoystickWithFFB](https://github.com/YukMingLaw/ArduinoJoystickWithFFBLibrary) library. It also draws heavy inspiration from the herculean effort made within the [Arduino-FFB-wheel](https://github.com/ranenbg/Arduino-FFB-wheel) endeavour. And none of this could have been possible without the amazing work done over the past decade in [many](https://github.com/fernandoigor/BRWheel/tree/alphatest) [other](https://github.com/MHeironimus/ArduinoJoystickLibrary) [projects](https://github.com/hoantv/VNWheel).
