/*
 * usb_serial.h
 *
 *  Created on: 23-Feb-2016
 *      Author: Harpreet
 */

#include"termios.h"
#include"unistd.h"
#include"errno.h"
#include"sys/types.h"
#include"sys/stat.h"
#include"fcntl.h"

#ifndef USB_SERIAL_H_
#define USB_SERIAL_H_


int set_interface_attribs (int fd, int speed, int parity);// Sets the attributes and returns 0 on success and -1 on failure
/*Function set_blocking
 * should_block: 0 - Non blocking, 1 - blocking
 * fd : file descriptor for usb serial communication
 */
void set_blocking (int fd, int should_block);// sets blocking or nonblocking i/o operation
int usb_init();								// Initialize and open Usb serial Port and returns the file descriptor
#endif /* USB_SERIAL_H_ */
