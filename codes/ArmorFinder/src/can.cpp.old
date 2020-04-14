/*
 * cantest.c
 *
 *  Created on: 2017-5-16
 *      Author: simon
 */
#ifndef CAN
#define CAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int canTansfer(unsigned char data[])
{
	int s, nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;
	struct can_filter rfilter[1];

    /* create socket */
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
	    perror("Create socket failed");
	    exit(-1);
	}
	/* set up can interface */
	strcpy(ifr.ifr_name, "can0");
	printf("can port is %s\n",ifr.ifr_name);
	/* assign can device */
	ioctl(s, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
    /* bind can device */
	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Bind can device failed\n");
		close(s);
		exit(-2);
	}
	/* configure sending */
        /* configure can_id and can data length */
	frame.can_id = 0x1F;
        frame.can_dlc = 8;
        printf("%s ID=%#x data length=%d\n", ifr.ifr_name, frame.can_id, frame.can_dlc);
        /* prepare data for sending: 0x11,0x22...0x88 */
         for (int i=0; i<8; i++)
         {
        	frame.data[i] = data[i];
        	printf("%#x ", frame.data[i]);
         }

        printf("Sent out\n");
        /* Sending data */
        if(write(s, &frame, sizeof(frame)) < 0)
		{
            perror("Send failed");
            close(s);
		}
	 close(s);
}
#endif
