/*
 * comm.h
 *
 *  Created on: 23-Feb-2016
 *      Author: root
 */
#ifndef COMM_H_
#define COMM_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "stdint.h"
#include"modbus.h"
#include <pthread.h>
#include "usb_serial.h"
#include "signal.h"
#include <semaphore.h>

modbus_t *ctx;			//pointer to variable modbus
int rc;

void *server_thread (void *arg);	// thread to handle request from IOT client
void *usb_thread (void *arg);		// thread to handle modbus request from usb adndroid device
int listenfd = 0, connfd = 0;		// Used as file descriptor
sem_t test_semaphore;				// Semaphore declaration

#endif /* COMM_H_ */
