# MAX ESSENTIAL TOOLKIT

MaxEssentialToolkit package includes arduino driver and usecase examples for MAXIM/ADI  RTC, temperature and current-sense amplifier sensors.
Supported part numbers:

- [MAX31328](https://www.maximintegrated.com/en/products/analog/real-time-clocks/MAX31328.html)
- [MAX31341](https://www.maximintegrated.com/en/products/analog/real-time-clocks/MAX31341B.html)
- [MAX31343](https://www.maximintegrated.com/en/products/analog/real-time-clocks/MAX31343.html)
- [MAX31825](https://www.maximintegrated.com/en/products/sensors/MAX31825.html)
- [MAX31827](https://www.maximintegrated.com/en/products/sensors/MAX31827.html)
- [MAX31889](https://www.maximintegrated.com/en/products/sensors/MAX31889.html)
- [MAX40080](https://www.maximintegrated.com/en/products/analog/amplifiers/MAX40080.html)


## How to install
There are two main options to install library:
### Option 1:
 1. Open Arduino IDE
 2. Go into Tools -> Manage Libraries...
 3. Search for MaxEssentialToolkit
 4. Click install button
 5. On pop-up message box click install all to install dependency too

![Installation](./Images/installation.png)

### Option 2: 
 1. Dowload repository as .zip file
 2. Rename .zip file as "MaxEssentialToolkit.zip" 
 3. Open Arduino IDE
 4. Go into Sketch -> Include Library -> Add .ZIP Library...
 5. Browse the MaxEssentialToolkit.zip location
 6. Click Open
 7. [Install OneWire Library](https://www.arduino.cc/reference/en/libraries/onewire/)

## How to build and load an example
 1. After installation open Arduino IDE
 2. Go into Files -> Examples -> MaxEssentialToolkit
 3. Select the part number you would like to use
 4. Select an example
 5. (If needs) Update example pin connection in example to it match with your target board.

 ![Select an example](./Images/how_to_build/1_select_an_example.png)

 6. Plug your Arduino board to PC via USB cable.
 7.	Select board type and serial port by navigating to
		Tools->Board
		Tools->Ports

 ![Select board and port](./Images/how_to_build/2_select_port.png)

 8. Click right arrow button to build and load it

 ![Build and load image](./Images/how_to_build/3_build_and_load_image.png)

 9. Please check output to see whether load succeeded or not
 
 ![Output screen](./Images/how_to_build/4_after_load_output_screen.png)


## License and contributions
The software is provided under the [Apache-2.0 license](LICENSE.txt). 
Contributions to this project are accepted under the same license.
