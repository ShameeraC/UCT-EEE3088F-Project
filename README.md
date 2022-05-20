# EEE3088F Group 18 Sensing HAT (Version 1.0)
This project is a group project for the course EEE3088F at UCT. The purpose of the HAT is to detect light and move a motor or another device in the direction of the light.

# Instructions For Use (For Version 1.0)
    
    - Hardware Requirements - 

    To use this HAT, a mini-USB connector is required to interface the HAT with the STM 
    (Or whatever other microcontroller is in use). That being said, an STM32F0 discovery board, 
    or another board containing a microcontroller with read/write/ADC/memory capabilities must be used if 
    the full functionality of the HAT is to be claimed.
    The USB must be capable of data transmission.
    A 5V Li-Ion battery and battery holder are necessary for the powering of the HAT, unless some other 
    power source is to be used. Jumper cables are necessary for the current design, as it does not conform
    to the general standard of board measurements (they are needed to connect the HAT to the board in use).
   
    - Software Requirements -

    The coding of this board can be done via the STM Cube IDE (preferred) or Code Blocks. Other coding
    platforms can work as well. The terminal must be configured to drive the USB ports that connect to the
    discovery board or the HAT. 
    Programs can be uploaded to the STM or to the HAT itself, depending on which method is preferred.
    The EEPROM on the HAT must be coded via the HAT's USB port, as this sends data to and from the FTDI
    and the EEPROM on board the HAT together. 
    Currently the HAT has not been revised, and secondary versions have not yet been created. Because of this,
    the recommended approach to coding is to do so via the STM or the board in use (and not via the HAT).
    The temperature sensor which sends data to and from the HAT's EEPROM was not populated in version 1.0,
    and as such the EEPROM and the FTDI on board the HAT are not needed or useful currently.
    Thus, any code that is written should be uploaded directly to the board in use, after which wiring
    can interface the board with the HAT.

    - Initialization Instructions -

    To initialize the hardware, connect the battery to the HAT to power the STM board (if batteries 
    are not being used, find another way to connect the HAT to power). Connect each 
    pin on the STM to the corresponding pin on the HAT via jumper wires.
    To initialize the software, connect to the board via a USB, and compile and upload the desired 
    code to the board via an appropriate IDE. How to upload code to the board will vary from IDE to
    IDE. Check the IDE's online documentation for further details.

    - Running Your First Program on the HAT - 

    To run your first program on the HAT, refer to the self-explanatory code in the "firstProgram" file
    in the "Simulation/" directory. Comments are included to help you through the process of writing an
    initial program to run on the HAT.

# Directories:
This repo has the following subfolders, which provide a useful common structure for custom electronics projects: \
    * Documentation: Project documentation (including submissions) and component datasheets are stored here.\
    * Firmware: Any software developed for the hat will be stored here.\
    * PCB: This includes all KiCad files (schematics, pcb, gerber files, footprints etc.). The main KiCad folder is here. \
    * Production: The gerber files, BOM, Budget, or anything required by JLCPCB will be stored here. \
    * Simulation: Any simulation files (eg SPICE) or design stage generated results (Eg matlab or excel) will be stored here.

# PCB Designers:
JONATHAN APPS \
SHAMEERA CASSIM \
JAHAR PERSAD 

# License
[Creative Commons Attribution 4.0 International license](https://choosealicense.com/licenses/cc-by-4.0/)
