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


#include <signal.h>
#include "libpixyusb2.h"
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>

#define XMax 200
#define XMin 70
#define area_d 10000

uint16_t m_width = 0 ;
uint16_t m_height = 0; 
uint16_t area = 0;
uint16_t newarea = 0;
uint16_t maxarea = 0;
uint16_t minarea = 0;
Pixy2        pixy;
static bool  run_flag = true;


void handle_SIGINT(int unused)
{
  // On CTRL+C - abort! //

  run_flag = false;
}




int main()
{
   int sender;                             
      if ((sender = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }
printf ("Success in client\n");
struct termios options;
tcgetattr(sender, &options);
options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
options.c_iflag = IGNPAR | ICRNL;
tcflush(sender, TCIFLUSH);
//fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);  // make reads non-blocking
tcsetattr(sender, TCSANOW, &options);
write(sender,"a",1);
           
                       
	
int  Result,count =0;

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
	if(pixy.ccc.numBlocks == 0)
	{
              write(sender,"f",1);
	}
 	 // Were blocks detected? //
 	 	if(pixy.ccc.numBlocks)
  		//while(1)
		//{
    	// Blocks detected - print them! //

   	 	printf("Detected %d block(s)\n", pixy.ccc.numBlocks);
	
   	 	for(Block_Index = 0; Block_Index < pixy.ccc.numBlocks; ++Block_Index)
    		{
      		printf ("  Block %d: ", Block_Index + 1);
    		//pixy.ccc.blocks[Block_Index].print();
	
  	
		printf("x=%d\n",(pixy.ccc.blocks[Block_Index].m_x));
     		area = ((pixy.ccc.blocks[Block_Index].m_width)*(pixy.ccc.blocks[Block_Index].m_height));
		printf("height=%d\nwidth=%d",pixy.ccc.blocks[Block_Index].m_height,pixy.ccc.blocks[Block_Index].m_width);
		printf("area=%d\n\n",area);
    		maxarea = area + 1000;
     		minarea = area - 1000; 
		printf("maxarea=%d\n",maxarea);
		printf("minarea=%d\n",minarea);
		 
		 
	
if((pixy.ccc.blocks[Block_Index].m_signature)==5)
	{
	newarea = (((pixy.ccc.blocks[Block_Index].m_width)*(pixy.ccc.blocks[Block_Index].m_height)));
	printf("newarea=%d\n\n",newarea);

if((pixy.ccc.blocks[Block_Index].m_x)<XMin)
	{
	write(sender,"a",1);
 	printf("left= 0110\n");
	printf("x=%d\n",(pixy.ccc.blocks[Block_Index].m_x));
	}

if((pixy.ccc.blocks[Block_Index].m_x)>XMax)
	{ 
	write(sender,"b",1);
	printf("x=%d\n",(pixy.ccc.blocks[Block_Index].m_x));
	printf("Right= 1001\n");
	}




if(((pixy.ccc.blocks[Block_Index].m_x)>=XMin && (pixy.ccc.blocks[Block_Index].m_x) <=XMax))
	{
	
	printf("x=%d\n",(pixy.ccc.blocks[Block_Index].m_x));
	printf("center=1010\n");
	

   
	   
	if(newarea < area_d)
	{
		   write(sender,"c",1);
		   printf("new=%d<area=%d\n",newarea,minarea);
		    printf("go forward\n");
	   }
 	if(newarea > area_d)
	   {		
		 write(sender,"d",1);
		   write(sender,"e",1);
		   printf("new=%d>area=%d\n",newarea,maxarea);
		
		   printf("Stop than go reverse\n");
	   }
}
		 }

if (run_flag == false)
    	{
      // Exit program loop //
      break;
    	}
  	}
}
 	close(sender);
  printf ("PIXY2 Get Blocks Demo Exit\n");
	
	}
