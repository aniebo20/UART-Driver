# uartdriver
A Pin-specifc UART Driver for STM32F767xx

In main.c, it is implemented in a password-security application in which the recieved password over RX is checked against the value stored in the password char array and results in either the green LED lighting up or the red.

**Include chip_header files when building the project**
