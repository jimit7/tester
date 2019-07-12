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
#include <unistd.h>

#include <signal.h>
#include "libpixyusb2.h"
#define XMin 155
#define XMax 165 
#include<string>
#include<fstream>
using std::string;
using std::ofstream;

#define GPIO_PATH "/sys/class/gpio/"

namespace exploringBB {

typedef int (*CallbackType)(int);
enum GPIO_DIRECTION{ INPUT, OUTPUT };
enum GPIO_VALUE{ LOW=0, HIGH=1 };
enum GPIO_EDGE{ NONE, RISING, FALLING, BOTH };

class GPIO {
private:
	int number, debounceTime;
	string name, path;

public:
	GPIO(int number); //constructor will export the pin
	virtual int getNumber() { return number; }

	// General Input and Output Settings
	virtual int setDirection(GPIO_DIRECTION);
	virtual GPIO_DIRECTION getDirection();
	virtual int setValue(GPIO_VALUE);
	virtual int toggleOutput();
	virtual GPIO_VALUE getValue();
	virtual int setActiveLow(bool isLow=true);  //low=1, high=0
	virtual int setActiveHigh(); //default
	//software debounce input (ms) - default 0
	virtual void setDebounceTime(int time) { this->debounceTime = time; }

	// Advanced OUTPUT: Faster write by keeping the stream alive (~20X)
	virtual int streamOpen();
	virtual int streamWrite(GPIO_VALUE);
	virtual int streamClose();

	virtual int toggleOutput(int time); //threaded invert output every X ms.
	virtual int toggleOutput(int numberOfTimes, int time);
	virtual void changeToggleTime(int time) { this->togglePeriod = time; }
	virtual void toggleCancel() { this->threadRunning = false; }

	// Advanced INPUT: Detect input edges; threaded and non-threaded
	virtual int setEdgeType(GPIO_EDGE);
	virtual GPIO_EDGE getEdgeType();
	virtual int waitForEdge(); // waits until button is pressed
	virtual int waitForEdge(CallbackType callback); // threaded with callback
	virtual void waitForEdgeCancel() { this->threadRunning = false; }

	virtual ~GPIO();  //destructor will unexport the pin

private:
	int write(string path, string filename, string value);
	int write(string path, string filename, int value);
	string read(string path, string filename);
//	int exportGPIO();
//	int unexportGPIO();
	ofstream stream;
	pthread_t thread;
	CallbackType callbackFunction;
	bool threadRunning;
	int togglePeriod;  //default 100ms
	int toggleNumber;  //default -1 (infinite)
	friend void* threadedPoll(void *value);
	friend void* threadedToggle(void *value);
};

void* threadedPoll(void *value);
void* threadedToggle(void *value);

} 




















































using namespace std;
using namespace exploringBB                           
Pixy2        pixy;
static bool  run_flag = true;


void handle_SIGINT(int unused)
{


  run_flag = false;
}

void  get_blocks()
{
GPIO outGPIO(44),out1GPIO(26),out2GPIO(46),out3GPIO(65);
outGPIO.setDirection(OUTPUT);
out1GPIO.setDirection(OUTPUT);
out2GPIO.setDirection(OUTPUT);
out3GPIO.setDirection(OUTPUT);
  int  Block_Index;
uint16_t x1,y1,w,h;

  pixy.ccc.getBlocks();

 
  if (pixy.ccc.numBlocks)
  {


    printf ("Detected %d block(s)\n", pixy.ccc.numBlocks);

    for (Block_Index = 0; Block_Index < pixy.ccc.numBlocks; ++Block_Index)
    {
      printf ("  Block %d: ", Block_Index + 1);
      x1=pixy.ccc.blocks[Block_Index].m_x;
y1=pixy.ccc.blocks[Block_Index].m_y;
w=pixy.ccc.blocks[Block_Index].m_width;
h=pixy.ccc.blocks[Block_Index].m_height;
printf("x=%d/n",x1);
printf("y=%d/n",y1);
printf("w=%d/n",w);
printf("h=%d/n",h);


if((pixy.ccc.blocks[Block_Index].m_x)<XMin)
{
                                                                           	//while(!((pixy.ccc.blocks[Block_Index].m_x)>= XMin && (pixy.ccc.blocks[Block_Index].m_x) <= XMax))
										//{
printf("x=%d/n",(pixy.ccc.blocks[Block_Index].m_x));
printf("left= 0110\n");
outGPIO.setValue(LOW);
out1GPIO.setValue(HIGH);
out2GPIO.setValue(HIGH);
out3GPIO.setValue(LOW);

}
                                                                                  	//if(((pixy.ccc.blocks[Block_Index].m_x) >= XMin && (pixy.ccc.blocks[Block_Index].m_x) <= XMax))
                                                   					//{
											//break;     
											//left;
											//}
											// printf("In  L Center\n"); 
											//}

if((pixy.ccc.blocks[Block_Index].m_x)>XMax)
{
//while(!((pixy.ccc.blocks[Block_Index].m_x) <= XMax && (pixy.ccc.blocks[Block_Index].m_x) >= XMin))
//{
printf("x=%d/n",(pixy.ccc.blocks[Block_Index].m_x));
printf("RIght= 1001\n");

//if(((pixy.ccc.blocks[Block_Index].m_x) >= XMin && (pixy.ccc.blocks[Block_Index].m_x) <= XMax))
//{
//break;
//left;
//}
//}
 printf("In R Center"); 
outGPIO.setValue(HIGH);
out1GPIO.setValue(LOW);
out2GPIO.setValue(LOW);
out3GPIO.setValue(HIGH);
}
if(((pixy.ccc.blocks[Block_Index].m_x)>=XMin && (pixy.ccc.blocks[Block_Index].m_x) <=XMax)){
printf("x=%d/n",(pixy.ccc.blocks[Block_Index].m_x));
printf("center= 1010\n");
outGPIO.setValue(HIGH);
out1GPIO.setValue(LOW);
out2GPIO.setValue(HIGH);
out3GPIO.setValue(LOW);
//}
}
}
}
}
int main()
{
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
   get_blocks();
//x1=pixy.ccc.blocks[m_index].m_x;
//y1=pixy.ccc.blocks[m_index].m_y;
//w=pixy.ccc.blocks[m_index].m_width;
//h=pixy.ccc.blocks[m_index].m_height;
//printf("x=%d/n",x1);
//printf("y=%d/n",y1);
//printf("w=%d/n",w);
//printf("h=%d/n",h);

    if (run_flag == false)
    {
      // Exit program loop //
      break;
    }
  }

  printf ("PIXY2 Get Blocks Demo Exit\n");
}

