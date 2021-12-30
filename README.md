# MAX ESSENTIAL TOOLKIT

MaxEssentialToolkit package includes arduino driver and usecase examples for MAXIM/ADI  RTC, temperature and current-sense amplifier sensors.
Supported part numbers:

- [MAX31328](https://datasheets.maximintegrated.com/en/ds/MAX31328.pdf)
- [MAX31341](https://datasheets.maximintegrated.com/en/ds/MAX31341B-MAX31341C.pdf)
- [MAX31343](https://datasheets.maximintegrated.com/en/ds/MAX31343.pdf)
- [MAX31825](https://datasheets.maximintegrated.com/en/ds/MAX31825.pdf)
- [MAX31827](https://datasheets.maximintegrated.com/en/ds/MAX31827-MAX31829.pdf)
- [MAX31889](https://datasheets.maximintegrated.com/en/ds/MAX31889.pdf)
- [MAX40080](https://datasheets.maximintegrated.com/en/ds/MAX40080.pdf)


## How to install
There are two main options to install library:
### Option 1:
Note: Not works yet, The project is not linked with Arduino library database yet.
 1. Open Arduino IDE
 2. Go into Tools -> Manage Libraries...
 3. Search for MaxEssentialToolkit
 4. Click install button

### Option 2: 
 1. Dowload repository as .zip file
 2. Rename .zip file as "MaxEssentialToolkit.zip" 
 3. Open Arduino IDE
 4. Go into Sketch -> Include Library -> Add .ZIP Library...
 5. Browse the MaxEssentialToolkit.zip location
 6. Click Open

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
The software is provided under the [Apache-2.0 license](LICENSE-apache-2.0.txt). 
Contributions to this project are accepted under the same license.
