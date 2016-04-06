/*
 * main.c
 *
 *  Created on: 20-Jan-2016
 *      Author: Harpreet
 */

#include "comm.h"

void sigproc();

void main() {

    struct sockaddr_in serv_addr;
    pthread_t tid [10];
    void *status;
    init_modbus("169.254.0.1",1502,1);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
    }

    modbus_set_debug(ctx, 1);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);		//Create Socket
        memset(&serv_addr, '0', sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(1010);
        bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));//bind socket with local address

        signal(SIGINT, sigproc);					// Signal Handler for ctrl+c

        if(sem_init (&test_semaphore, 0, 1) == -1)	//Initializing the semaphore
        {
        perror ("sem_init");
        exit (1);
        }

        if(-1 == pthread_create (&tid [0], NULL, server_thread,NULL))// Creating the posix server and usb thread
        perror("pthread:");

        if(-1 == pthread_create (&tid [1], NULL, usb_thread,NULL))
        perror("pthread:");

        if(-1 == pthread_join(tid[0], status))			//wait for termination of the thread
        perror("pthread_join:");
        else printf("status of tid[0](server thread): %c\n",*(char *)status);

        if(-1 == pthread_join(tid[1], status))
        perror("pthread_join:");
        else printf("status of tid[0](usb thread): %c\n",*(char *)status);

}

void sigproc()
{
if(sem_destroy(&test_semaphore) == -1)	//destroy seaphore
	{
		perror ("sem_init");
		exit (1);
	}
printf(" you have pressed ctrl+c\n");
exit(0);
}
