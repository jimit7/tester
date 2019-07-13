//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
#include <iostream>
#include <cstdio>
#include <signal.h>
#include "libpixyusb2.h"
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdint.h>
#include<linux/spi/spidev.h>
#define SPI_PATH "/dev/spidev1"
#define Xmax 175
#define Xmin 125
const unsigned uint16_t symbols[5] = { 
1010,0101,1001,0110,0000
};

int transfer(int fd, unsigned char send[], unsigned char rec[], int len){
   struct spi_ioc_transfer transfer;        //the transfer structure
   transfer.tx_buf = (unsigned long) send;  //the buffer for sending data
   transfer.rx_buf = (unsigned long) rec;   //the buffer for receiving data
   transfer.len = len;                      //the length of buffer
   transfer.speed_hz = 1000000;             //the speed in Hz
   transfer.bits_per_word = 8;              //bits per word
   transfer.delay_usecs = 0;                //delay in us
   transfer.cs_change = 0;       // affects chip select after transfer
   transfer.tx_nbits = 0;        // no. bits for writing (default 0)
   transfer.rx_nbits = 0;        // no. bits for reading (default 0)
   transfer.pad = 0;             // interbyte delay - check version

   // send the SPI message (all of the above fields, inc. buffers)
   int status = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
   if (status < 0) {
      perror("SPI: SPI_IOC_MESSAGE Failed");
      return -1;
   }
   return status;
}

Pixy2        pixy;
static bool  run_flag = true;


void handle_SIGINT(int unused)
{
  // On CTRL+C - abort! //

  run_flag = false;
}
//void tes_spi()
//{

//}
//void  get_blocks()
//{
  //int  Block_Index;

  // Query Pixy for blocks //
  //pixy.ccc.getBlocks();
 
  // Were blocks detected? //
  //if (pixy.ccc.numBlocks)
  //{
    // Blocks detected - print them! //

    //printf ("Detected %d block(s)\n", pixy.ccc.numBlocks);

    //for (Block_Index = 0; Block_Index < pixy.ccc.numBlocks; ++Block_Index)
    //{
      //printf ("  Block %d: ", Block_Index + 1);
      //pixy.ccc.blocks[Block_Index].print();

    //}
  //}


}

int main()
{
   int fd;                              // file handle and loop counter
   unsigned char null=0x00;                // sending only a single char
   uint8_t mode = 3;                       // SPI mode 3
  int  Result;

  // Catch CTRL+C (SIGINT) signals //
  signal (SIGINT, handle_SIGINT);

  printf ("=============================================================\n");
  printf ("= PIXY2 Get Blocks Demo                                     =\n");
  printf ("=============================================================\n");

  printf ("Connecting to Pixy2...");

  // Initialize Pixy2 Connection //
  {
    Result = pixy.init();

    if (Result < 0)
    {
      printf ("Error\n");
      printf ("pixy.init() returned %d\n", Result);
      return Result;
    	}

    printf ("Success\n");
  	}

  // Get Pixy2 Version information //
  	{
    Result = pixy.getVersion();

    if (Result < 0)
    	{
      printf ("pixy.getVersion() returned %d\n", Result);
      return Result;
    	}

    pixy.version->print();
  	}

  // Set Pixy2 to color connected components program //
  pixy.changeProg("color_connected_components");

  

while(1)
  	{
    	int  Block_Index;
	// Query Pixy for blocks //
  	pixy.ccc.getBlocks();
 
 	 // Were blocks detected? //
 	 if (pixy.ccc.numBlocks)
  		
    	// Blocks detected - print them! //

   	 printf ("Detected %d block(s)\n", pixy.ccc.numBlocks);
	
   	 for (Block_Index = 0; Block_Index < pixy.ccc.numBlocks; ++Block_Index)
    		{
      	printf ("  Block %d: ", Block_Index + 1);
    	 // pixy.ccc.blocks[Block_Index].print();
	
  	

 if ((fd = open(SPI_PATH, O_RDWR))<0){
      perror("SPI Error: Can't open device.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_WR_MODE, &mode)==-1){
      perror("SPI: Can't set SPI mode.");
      return -1;
   }
   if (ioctl(fd, SPI_IOC_RD_MODE, &mode)==-1){
      perror("SPI: Can't get SPI mode.");
      return -1;
   }




if((pixy.ccc.blocks[Block_Index].m_x)<XMin)
	{
if (transfer(fd, (unsigned char*) &symbols[3], &null, 1)==-1)
	{
         perror("Failed to update the display");
         return -1;
      	}
printf("left= 0110\n");
printf("x=%d/n",(pixy.ccc.blocks[Block_Index].m_x));
	}





if((pixy.ccc.blocks[Block_Index].m_x)>XMax)
	{
if (transfer(fd, (unsigned char*) &symbols[2], &null, 1)==-1)
	{
         perror("Failed to update the display");
         return -1;
      	}
printf("x=%d/n",(pixy.ccc.blocks[Block_Index].m_x));
printf("Right= 1001\n");
	}




if(((pixy.ccc.blocks[Block_Index].m_x)>=XMin && (pixy.ccc.blocks[Block_Index].m_x) <=XMax))
	{
	if (transfer(fd, (unsigned char*) &symbols[0], &null, 1)==-1)
	{
         perror("Failed to update the display");
         return -1;
      	}
printf("x=%d/n",(pixy.ccc.blocks[Block_Index].m_x));
printf("forward=1010");
	}

    




if (run_flag == false)
    	{
      // Exit program loop //
      break;
    	}
  	}
 	close(fd);
  printf ("PIXY2 Get Blocks Demo Exit\n");
	}
	}
