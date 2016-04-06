# IOT-HMI
Internet of Things with Human Machine Interface

HMI (Human Machine Interface) is the user interface in a manufacturing or
process control system. It provides a graphics-based visualization of an
industrial control and monitoring system. Machine builders are connecting their
automation systems to the Internet of Things (IoT), and many Human Machine
Interface (HMI) software solutions enable IoT connectivity in a variety of ways.
In this project we are using x86 based processor named intel galileo for
development of HMI along with IOT support.

The architecture of this project is as follows:-

![architecture](https://cloud.githubusercontent.com/assets/8409015/14314615/e0fd5e92-fc15-11e5-9072-37a4738e6feb.png)

In this project Intel Galileo Gen 2 processor is used. For local display instead 
of lcd an GUI on rooted Android phone was developed using Qt5.5. The communication
between this local display and processor is through usb serial port. The processor
communicates to the field devices usig Modbus Tcp/ip fieldbus protocol. Instead of
using the original fieldbus equipment, in this project Modbus emulator  ModbusPal
is used. ModbusPal is also an open source Modbus Tcp/ip client emulator.

The processor also act as an embedded server. If ane remote client makes query through internet for any Modbus register value, processor sends the corresponding Modbus request to the modbus tcp/ip client and then recieves the modbus response. Intel Galileo then forwards this response to the remote client over internet. In this project remote client application is also made using Qt 5.5. The processor needs to send the modbus data to local display as well as to remote client, therefore two thread are used one for usb communication and second for the remote client i.e iot communcation. Linux IPC feature is used to accomplish this task. The embedded linux image running on intel galileo was generated using Yocto Build System. The procedure for the same is explained in intel Documents for generating iot-devkit-image.
