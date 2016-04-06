/*
 * comm.c
 *
 *  Created on: 23-Feb-2016
 *      Author: Harpreet
 */
#include"comm.h"

#include "unit-test.h"

/*
 * This Function establishes modbus tcp/ip connection
 * Arguments:
 * char * ip :- IP address of modbus tcp/ip client
 * port :- Port on which modbus server is running
 * slave :- Slave no. of Modbus client
 * Return Value 1:- return successful
 * 				0 :- Error
 */
int init_modbus(const char *ip, int port,int slave)
{
	ctx = modbus_new_tcp(ip, port);
	printf("after ctx \n");
	printf("before modbus_set_slave\n");
	rc = modbus_set_slave(ctx,slave);
	if(rc == 0)printf("slave address set correctly");
	if (rc == -1) {
		printf("modbus_set_slave error\n");
	    fprintf(stderr, "%s\n", modbus_strerror(errno));
	    return 0;
	}
	return 1;
}

/*
 * This Function deinitialize modbus tcp/ip connection
 */
void deinit_modbus()
{
	modbus_close(ctx);
	modbus_free(ctx);
}

/*
 * This Function sends modbus request and returns the response
 * Arguments:
 * unsigned char * fn :- Modbus request array. 1st byte is function code, next 2 bytes is address of register
 * 						4th 5th byte is no. of register. 6th 7th byte is data. Modbus response is sent back on
 * 						6th and 7th byte
 * Return Value :- 0 : On successful
 * 					-1 : on error
 */
int MBSendRequest(unsigned char *fn)
{
	uint16_t address,NIitem,*data,data1 ;
	int ret=0;
	address = *(fn+1);
	address = address<<8;			// This is done so that data recieved is in big indian format.
	address = address | (*(fn+2));	//It should be converted to little indian format

	NIitem = *(fn+3);
	NIitem = NIitem<<8;
	NIitem = NIitem | *(fn+4);

	data1 = *(fn+5);
	data1 = data1<<8;
	data1 = data1 | *(fn+6);

	data = &data1;

	printf("Function code = %d \n",*fn);
	printf("register address = %x \n",address);
	printf("NIitem = %x \n",NIitem);
	printf("Data=%d\n",*data);
	switch(*fn)
	{
	case 1:ret = modbus_read_bits(ctx,address,NIitem,(uint8_t*) data);
			break;
	case 2:ret = modbus_read_input_bits(ctx,address,NIitem,(uint8_t*) data);
			break;
	case 3:ret = modbus_read_registers(ctx,address,NIitem,data);
			break;
	case 4:ret = modbus_read_input_registers(ctx,address,NIitem,data);
			break;
	case 5:ret = modbus_write_bit(ctx,address,*data);
			break;
	case 6:ret = modbus_write_register(ctx,address,*data);
			break;
	case 15:ret = modbus_write_bits(ctx,address,NIitem,(uint8_t*) data);
			break;
	case 16:ret = modbus_write_registers(ctx,address,NIitem,data);
			break;
	}
	*(fn+6)=((*data) >> 8)& 0x00ff;
	*(fn+5)= (*data) & 0x00ff;
	return ret;
}

void *server_thread (void *arg)
{
	int temp=0,iu=0;
	unsigned char readBuff[7];
	listen(listenfd,10);			//listen for 10 connections
	while(1)
	{
		printf("inside server thread\n");
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);//Await a connection on socket FD.When a connection arrives, open a new socket to communicate with it
        printf("\n accepted...\n");
		sem_wait(&test_semaphore);		//Acquire the semaphore
      temp = read(connfd, readBuff, 7);	//Read the Modbus request of 7 bytes
		if(temp ==-1)printf("read fd failed\n");
		for(iu=0;iu<32000;iu++);		//Approx. delay
		if(temp < 7) temp=read(connfd,readBuff+temp,7-temp);
		if(temp != 0)
		{
        printf("\n after read \n");
       printf("readBuff[]=%x %x %x %x %x %x %x\n",readBuff[0],readBuff[1],readBuff[2],readBuff[3],readBuff[4],readBuff[5],readBuff[6]);
        temp = MBSendRequest( readBuff);		//Send the MOdbus tcp/ip request and receive the respose
        if(temp == -1)printf("failed..\n");
        printf("after MBrequest \n");
        printf("readBuff[5]=%x%x\n",readBuff[5],readBuff[6]);
        write(connfd, (uint16_t*)(readBuff+5), 2);	//write back the modbus response to the client
        printf("after write.. \n");
        readBuff[5]=readBuff[6]=0;
		}
		printf("before server semaphor release\n");	//release the semaphore
		if(sem_post(&test_semaphore) == -1)
			{
				perror ("sem_init");
				exit (1);
			}
	}
}

void *usb_thread (void *arg)
{
	int temp=0, fd_usb=0,iu=0;
	unsigned char readBuff[7];
	   temp =access("/dev/ttyACM0",F_OK);		// test for access to io file
	   if(temp == -1) printf("temp = %d \n",temp);
	   if(temp != -1)
	   {
	   fd_usb = usb_init();						//initialize usb serial port
	   if(fd_usb != -1)
	   {
	while(1){
		printf("inside usb thread\n");
		temp=read(fd_usb,readBuff,7);			//read modbus request of 7 bytes
		if(temp ==-1)printf("read fd failed\n");
		for(iu=0;iu<32000;iu++);				//approx. delay
		if(temp < 7) temp=read(fd_usb,readBuff+temp,7-temp);
	   printf("no. of bytes read = %d\n",temp);
	   if(temp != 0)
	   {
	   sem_wait(&test_semaphore);			// Wait for semaphore
	   printf("readBuff[]=%x %x %x %x %x %x %x\n",readBuff[0],readBuff[1],readBuff[2],readBuff[3],readBuff[4],readBuff[5],readBuff[6]);
    temp = MBSendRequest( readBuff);		//send tcp/ip modbus request and recieve the response
    if(temp == -1)printf("MBsendrequest failed..\n");
    printf("readBuff[5]=%x%x\n",readBuff[5],readBuff[6]);

	   temp = write(fd_usb, (uint16_t*)(readBuff+5), 2);// Write back th response
	   printf("no. of bytes write=%d\n",temp);

	   for(iu=0;iu<32000;iu++);
	   read(fd_usb,readBuff,1);				//read the acknowledgment byte
	   while(readBuff[0] != 'k')			//If acknowledgment not received write back modbus response
	   {
		   temp = write(fd_usb, (uint16_t*)(readBuff+5), 2);
		   for(iu=0;iu<32000;iu++);
		   read(fd_usb,readBuff,1);
	   }
	   readBuff[5]=readBuff[6]=0;
	   if(sem_post(&test_semaphore) == -1)	//Release the semaphore
	   	   			{
	   	   				perror ("sem_init");
	   	   				exit (1);
	   	   			}
	   }// if
	}// while(1)
	   temp = close(fd_usb);	//close usb
	   if(temp == -1)printf("error while closing the usb file descriptor\n");
	   }//if
	   }//if
}//function ends

