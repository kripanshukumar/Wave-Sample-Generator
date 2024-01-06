# Description:
This project implements an audio recording system using the ESP32 microcontroller, combining real-time clock synchronization, I2S interface for audio input, and SD card storage for efficient data handling.

# Parts Used:
ESP32	32-bit dual core MCU clocked at 320Mhz
INMP441	Omnidirectional Microphone with Bottom Port and I2 S Digital Output

# Operation:
•	The system utilizes an I2S interface for continuous audio data capture.
•	It synchronizes with an NTP server for accurate timekeeping.
•	Audio data is written to an SD card.
•	Recording is initiated by a button press and indicated by an LED.
•	FreeRTOS multitasking framework manages concurrent tasks:
-	Reading audio data
-	Controlling the LED
-	Handling file operations
•	Dynamically generates WAV file headers based on specified audio parameters.
•	Creates finalized WAV files on the SD card.
•	The code structure is designed for flexibility and can be extended for various audio applications.

# Learning/ Sills gained:
1.	Real-Time Clock (RTC) Integration: Establishing a connection to a Wi-Fi network and obtaining accurate time information from an NTP server.
2.	SD Card File Handling: Initializing and interfacing with an SD card for data storage. Performing file operations such as reading, writing, and listing directories.
3.	Multitasking with FreeRTOS: Implementing concurrent tasks using FreeRTOS to manage different aspects of the system simultaneously.
4.	Audio Data Processing: Capturing and processing audio data from an I2S interface. Generating WAV file headers dynamically based on audio parameters.
5.	I2C Communication Protocol: Learned how to communicate with the MCP4725 DAC via the I2C protocol, understanding the principles of serial communication.
6.	User Interface Design: Incorporating a button press as a trigger for specific actions Using an LED for visual feedback on the system's state.
7.	Custom File Format Handling: Creating and managing WAV file headers to ensure compatibility with audio playback software.
8.	Project Structuring and Modularity: Organizing code into functions and tasks for better readability and maintainability. Building modular components for easy integration and potential reuse in other projects
9.	Project Integration: Integrating multiple components (Wi-Fi, SD card, I2S, LED) into a cohesive system. Ensuring proper interaction between hardware and software components.

# Schematics:
<img src="https://github.com/kripanshukumar/Wave-Sample-Generator/blob/main/Image/Schematics.png" width=60% height=60%>
