# Mavlink Test
## Platform
Ubuntu 16.04 with gcc 5.4.0

## Mavlink Version
version : 1.0

## Autopilot
This example had been tested on Pixhawk(v 1.7.3).

## example




## Importing mavlink to STM32 board on mbed OS
1. Import the following repository to your online compiler:
[mbed](https://os.mbed.com/users/wupinxian/code/mavlink/)
2. Compiling and burning your boards

3. The LED1 on STM32 Board will start blinking after you pressing the reset button.

4. Running the program on Ubuntu.
```
	cd build 
	./mavlink_test -b 921600 -d /dev/ttyACM0
```
5. The terminal will show the following output.
```
	...
	system_id : 3
	component_id : 255
	custom_mode: 555
	type: 2
	autopilot: 12
	base_mode: 43
	system_status: 32
	mavlink_version: 3
	...
```








