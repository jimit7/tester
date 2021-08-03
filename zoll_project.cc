  // IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// Se1 RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
// Here we have multimeter click to measure voltage, current, resistance, and capacitance.
// channel0 - voltage
//channel1 - current
// channel2 - capacitance
// channel3 - resistance - To mesuare the resistance we have to do some changes im multimeter click. we have to put A , B to HIGH(5V) , C to LOW(GND)
 

#include <Wire.h>
#define ad1ress1 0x50
#include <SPI.h>
#include <SD.h>
#include <UTFT.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif
#define ADC_CS 20
extern unsigned int virtek[];
extern uint8_t Retro8x16[];
extern uint8_t mykefont2[];
UTFT myDisplay(ST7735, A5, A6, A3, A2, A4);


int volts;


// This one is the selection of pins on arduino for SPI communication

#define SELPIN 20 //Selection Pin
#define DATAOUT 51//MOSI // SDI of Multimeter click is connected to here
#define DATAIN  50//MISO // SDO of Multimeter click is connected to here
#define SPICLOCK  52//Clock

int ADC_of1set = 2048;
int buttonState = 0;
int count = 0;
int pass_count = 0;
int state=HIGH;
int failflag=0;
float analogInput = 1.00;
int button = 0;
int buttonhold = 0;
 float vout,vout2,vout3,vout9,readvalue, readvalue2,readvalue3;
int next = 0;
int a;
int vtest, vtest1, vtest2, vtest3, led, temp = 0;
float x_c,y_c,x_r,y_r,y_r_r;
float vout_c;
float vout_v;
 
float vin = 0.00;
float R1 = 33150.0;    // !! resistance of R1 !!
float R2 = 4699.0;     // !! resistance of R2 !!

// this is determined emiprically, putting the input into short and reading the ADC readout

float volt = 0.000;
float volt2 = 0.000;

float multim_measureV;
//float volt1 = 0.000;
int read_adc(int channel){
 int adcvalue = -2045;
 byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)

 //allow channel selection
 commandbits|=((channel-2)<<3);

 digitalWrite(SELPIN,LOW); //Select adc
 // setup bits to be written
 for (int i=7; i>=3; i--){
   digitalWrite(DATAOUT,commandbits&1<<i);
   //cycle clock
   
   digitalWrite(SPICLOCK,HIGH);
   digitalWrite(SPICLOCK,LOW);    
 }

 digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
 digitalWrite(SPICLOCK,LOW);
 
 digitalWrite(SPICLOCK,HIGH);  
 digitalWrite(SPICLOCK,LOW);

 //read bits from adc
 for (int i=11; i>=0; i--){
   adcvalue+=digitalRead(DATAIN)<<i;
   //cycle clock
   
   digitalWrite(SPICLOCK,HIGH);
   digitalWrite(SPICLOCK,LOW);
 }
 digitalWrite(SELPIN, HIGH); //turn of1 device
 return adcvalue;

}

int read_adc2(int channel2){
 int adcvalue2 = -2045;
 byte commandbits2 = B11000000; //command bits - start, mode, chn (3), dont care (3)

 //allow channel selection
 commandbits2|=((channel2-3)<<3);

 digitalWrite(SELPIN,LOW); //Select adc
 // setup bits to be written
 for (int i=7; i>=3; i--){
   digitalWrite(DATAOUT,commandbits2&1<<i);
   //cycle clock
   
   digitalWrite(SPICLOCK,HIGH);
   digitalWrite(SPICLOCK,LOW);    
 }

 digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
 digitalWrite(SPICLOCK,LOW);
 
 digitalWrite(SPICLOCK,HIGH);  
 digitalWrite(SPICLOCK,LOW);

 //read bits from adc
 for (int i=11; i>=0; i--){
   adcvalue2+=digitalRead(DATAIN)<<i;
   //cycle clock
   
   digitalWrite(SPICLOCK,HIGH);
   digitalWrite(SPICLOCK,LOW);
 }
 digitalWrite(SELPIN, HIGH); //turn of1 device
 return adcvalue2;

}

int read_adc3(int channel3){
 int adcvalue3 = -2045;
 byte commandbits3 = B11000000; //command bits - start, mode, chn (3), dont care (3)

 //allow channel selection
 commandbits3|=((channel3-0)<<3);

 digitalWrite(SELPIN,LOW); //Select adc
 // setup bits to be written
 for (int i=7; i>=3; i--){
   digitalWrite(DATAOUT,commandbits3&1<<i);
   //cycle clock
   
   digitalWrite(SPICLOCK,HIGH);
   digitalWrite(SPICLOCK,LOW);    
 }

 digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
 digitalWrite(SPICLOCK,LOW);
 
 digitalWrite(SPICLOCK,HIGH);  
 digitalWrite(SPICLOCK,LOW);

 //read bits from adc
 for (int i=11; i>=0; i--){
   adcvalue3+=digitalRead(DATAIN)<<i;
   //cycle clock
   
   digitalWrite(SPICLOCK,HIGH);
   digitalWrite(SPICLOCK,LOW);
 }
 digitalWrite(SELPIN, HIGH); //turn of1 device
 return adcvalue3;

}

void setup(){
//set pin modes
pinMode(46, INPUT);
  pinMode(47, OUTPUT);  
   pinMode(40, OUTPUT);// COMMUNICATION POWER
  pinMode(38, OUTPUT); // TURN HIGH AND THEN LOW TO START PIN 46 AND 47 TO MEASURE VOLTAGE, BUT IN COMM TEST DO HIGH, DO COMM TEST AND THEN TURN IT LOW 
  pinMode(39, OUTPUT); // TURN HIGH AND THEN LOW TO START PIN 46 AND 47 TO MEASURE VOLTAGE, BUT IN COMM TEST DO HIGH, DO COMM TEST AND THEN TURN IT LOW 
  
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  button = digitalRead(A7);

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  

  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);


    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
   pinMode(A4, OUTPUT);
pinMode(A5, OUTPUT);
 pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  
digitalWrite(36, HIGH);
pinMode(SELPIN, OUTPUT);
pinMode(DATAOUT, OUTPUT);
pinMode(DATAIN, INPUT);
pinMode(SPICLOCK, OUTPUT);
//disable device to start with
digitalWrite(SELPIN,HIGH);
digitalWrite(DATAOUT,LOW);
digitalWrite(SPICLOCK,LOW);
digitalWrite(A8, HIGH);
digitalWrite(A9, HIGH);
digitalWrite(A10, HIGH);
digitalWrite(A11, HIGH);
digitalWrite(A12, HIGH);
digitalWrite(A13, HIGH);
digitalWrite(A14, HIGH);


Serial.begin(9600);    
myDisplay.InitLCD();
  myDisplay.clrScr();
  myDisplay.fillScr(255, 255, 255);
  myDisplay.drawBitmap(20, 1, 126, 126, virtek, 1);
 // Serial.println("Push button to start test");
/* Serial.print("ZOLL MEDICAL CORPORATION");
  Serial.println();
 Serial.print("MODEL: 9301-010001-TRD");*/
  while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
  }
}


void loop(void) {
Serial.print("ZOLL MEDICAL CORPORATION");
  Serial.println();
 Serial.print("MODEL: 9301-010001-TRD");
/*  myDisplay.clrScr();
  state = HIGH;
  buttonhold = 0;
  while (1) {
    failflag = 0;
   
    delay(1000);
   digitalWrite(38, HIGH);
    digitalWrite(39, HIGH);
    delay(500);
    digitalWrite(47, HIGH);
    delay(500);
    digitalWrite(47, LOW);
    delay(500);
    digitalWrite(38, LOW);
    digitalWrite(39, LOW);
    delay(1000);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    delay(1000);
myDisplay.setColor(255, 255, 255); 

     myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("1. VOLTAGE TEST",CENTER,3);
   delay(250);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("TP1 ",0,18);
   
 
    delay(100);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    
    delay(1000);

voltage_measure();
 
       if ((vout_v <= 100) && (vout_v >= 2)) {
     myDisplay.setColor(0, 255, 0); 
     myDisplay.printNumF(vout_v, 1,36,18);
      vtest++;
    } else {
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(vout_v, 1,36,18);
       }
delay(250);
 myDisplay.setColor(255, 255, 255);
  myDisplay.print("TP2 ",0,36);
   
 
    delay(100);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    
    delay(1000);

voltage_measure();
 
       if ((vout_v <= 100) && (vout_v >= 2)) {
     myDisplay.setColor(0, 255, 0); 
     myDisplay.printNumF(vout_v, 1,36,36);
      vtest++;
    } else {
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(vout_v, 1,36,36);
       }
delay(250);
 myDisplay.setColor(255, 255, 255);
  myDisplay.print("TP3 ",0,54);
   
 
    delay(100);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    
    delay(1000);

voltage_measure();
 
       if ((vout_v <= 100) && (vout_v >= 2)) {
     myDisplay.setColor(0, 255, 0); 
     myDisplay.printNumF(vout_v, 1,36,54);
      vtest++;
    } else {
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(vout_v, 1,36,54);
       }
delay(250);
 myDisplay.setColor(255, 255, 255);
  myDisplay.print("TP4 ",0,72);
   
 
    delay(100);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    
    delay(1000);

voltage_measure();
 
       if ((vout_v <= 100) && (vout_v >= 2)) {
     myDisplay.setColor(0, 255, 0); 
     myDisplay.printNumF(vout_v, 1,36,72);
      vtest++;
    } else {
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(vout_v, 1,36,72);
       }

delay(250);
 myDisplay.setColor(255, 255, 255);
  myDisplay.print("TP5 ",0,90);
   
 
    delay(100);

    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    
    delay(1000);

voltage_measure();
 
       if ((vout_v <= 100) && (vout_v >= 2)) {
     myDisplay.setColor(0, 255, 0); 
     myDisplay.printNumF(vout_v, 1,36,90);
      vtest++;
    } else {
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(vout_v, 1,36,90);
       }


delay(250);

 myDisplay.setColor(255, 255, 255);
  myDisplay.print("TP6 ",0,108);
   
 
    delay(100);

    digitalWrite(6, LOW);
    digitalWrite(7,HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    
    delay(1000);

voltage_measure();
 
       if ((vout_v <= 100) && (vout_v >= 2)) {
     myDisplay.setColor(0, 255, 0); 
     myDisplay.printNumF(vout_v, 1,36,108);
      vtest++;
    } else {
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(vout_v, 1,36,108);
       }

 delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
} 
 break;
} */
 myDisplay.clrScr();
  state = HIGH;
  buttonhold = 0;
  while (1) {
    failflag = 0;
   
    delay(1000);
   myDisplay.setColor(255, 255, 255); 

  
     myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("1A.RESISTANCE TEST",CENTER,3);
  Serial.println();
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01A ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

    resistance();
    measurement();
    limit();
    if(a == 1){
      Serial.print("01A: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
     
   
    delay(105);

  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01B ",0,32);
   
    delay(10);
     resistance1();
     measurement(); 
    limit2();
    if(a == 1){
      Serial.print("01B: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01B: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    delay(105);
  
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("01C ",0,46);
   
   delay(10);
  
    resistance2();
    measurement(); 
    limit3();
    if(a == 1){
      Serial.print("01C: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01C: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);

   myDisplay.setColor(255, 255, 255);
   myDisplay.print("01D ",0,60);
   
   delay(10);
   
    resistance3();
    measurement(); 
    limit4();
    if(a == 1){
      Serial.print("01D: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01D: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
   
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("01E ",0,74);
   
   delay(10);
   
   resistance4();
   measurement(); 
   limit5();
   if(a == 1){
      Serial.print("01E: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01E: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01F ",0,88);
   
 
    delay(10);
   
    resistance5();
   measurement();
   limit6();
   if(a == 1){
      Serial.print("01F: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01F: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01G ",0,102);
   
   delay(10);

    resistance6();
   measurement();
   limit7();
  if(a == 1){
      Serial.print("01G: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01G: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01H ",0,116);
   
  delay(10);
  
  resistance7();
  measurement(); 
  limit8();
 if(a == 1){
      Serial.print("01H: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01H: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01I ",81,18);
  
  delay(10);
  
  resistance8();
  measurement(); 
  limit9();
  if(a == 1){
      Serial.print("01I: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01J ",81,32);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit10();
if(a == 1){
      Serial.print("01J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01K ",81,46);
  
  delay(10);

   resistance10();
measurement(); 
limit11();
if(a == 1){
      Serial.print("01K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01L ",81,60);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit12();
 if(a == 1){
      Serial.print("01L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 

   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01M ",81,74);
 
  delay(10);
  
  resistance12();
  measurement(); 
  limit13();
 if(a == 1){
      Serial.print("01M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01N ",81,88);
  
  delay(10);
  
  resistance13();
measurement(); 
limit14();
if(a == 1){
      Serial.print("01N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01O ",81,102);
  
  delay(10);
  
  resistance14();
  measurement();
  limit15();
  if(a == 1){
      Serial.print("01O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01P ",81,116);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit16();
 if(a == 1){
      Serial.print("01P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
} 

  myDisplay.clrScr(); 
  myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("1B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01Q ",0,18);

  delay(10); 
  
    resistance16();
    measurement(); 
    limit();
   if(a == 1){
      Serial.print("01Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
    delay(105);

  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01R ",0,32);
   
    delay(10);
     resistance17();
     measurement();
   limit2();
  if(a == 1){
      Serial.print("01R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
    delay(105);
  
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("01S ",0,46);
   
   delay(10);
  
    resistance18();
    measurement(); 
   limit3();
   if(a == 1){
      Serial.print("01S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);

   myDisplay.setColor(255, 255, 255);
   myDisplay.print("01T ",0,60);
   
   delay(10);
   
    resistance19();
    measurement();
    limit4();
   if(a == 1){
      Serial.print("01T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
   
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("01U ",0,74);
   
   delay(10);
   
   resistance20();
   measurement(); 
   limit5();
   if(a == 1){
      Serial.print("01U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01V ",0,88);
   
 
    delay(10);
   
    resistance21();
   measurement(); 
   limit6(); 
  if(a == 1){
      Serial.print("01V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01W ",0,102);
   
   delay(10);

    resistance22();
   measurement(); 
   limit7();
  if(a == 1){
      Serial.print("01W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01X ",0,116);
   
  delay(10);
  
  resistance23();
  measurement(); 
  limit8(); 
 if(a == 1){
      Serial.print("01X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01Y ",81,18);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit9();
 if(a == 1){
      Serial.print("01Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01Z ",81,32);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit10(); 
 if(a == 1){
      Serial.print("01Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01a ",81,46);
  
  delay(10);

   resistance26();
measurement(); 
limit11();
if(a == 1){
      Serial.print("01a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01b ",81,60);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit12();
 if(a == 1){
      Serial.print("01b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01c ",81,74);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit13(); 
  if(a == 1){
      Serial.print("01c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01d ",81,88);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit14();
  if(a == 1){
      Serial.print("01d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01e ",81,102);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit15();
 if(a == 1){
      Serial.print("01e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("01f ",81,116);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit16();
  if(a == 1){
      Serial.print("01f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("01f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
 
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("2A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02B ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

    
     resistance1();
     measurement(); 
     limit1(); // limit1();

  if(a == 1){
      Serial.print("02B: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
      
    delay(105);
  
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("02C ",0,32);
   
   delay(10);
  
    resistance2();
    measurement(); 
    limit2();
   if(a == 1){
      Serial.print("02C: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02C: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);

   myDisplay.setColor(255, 255, 255);
   myDisplay.print("02D ",0,46);
   
   delay(10);
   
    resistance3();
    measurement(); 
    limit3();
   if(a == 1){
      Serial.print("02D: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02D: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   delay(105);
   
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("02E ",0,60);
   
   delay(10);
   
   resistance4();
   measurement(); 
   limit4();
  if(a == 1){
      Serial.print("02E: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02E: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02F ",0,74);
   
 
    delay(10);
   
    resistance5();
   measurement(); 
   limit5();
  if(a == 1){
      Serial.print("02F: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02F: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02G ",0,88);
   
   delay(10);

    resistance6();
   measurement(); 
   limit6();
  if(a == 1){
      Serial.print("02G: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02G: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02H ",0,102);
   
  delay(10);
  
  resistance7();
  measurement(); 
  limit7();
if(a == 1){
      Serial.print("02H: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02H: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02I ",0,116);
  
  delay(10);
  
  resistance8();
  measurement(); 
  limit8();
 if(a == 1){
      Serial.print("02I: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02J ",81,18);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit9();
 if(a == 1){
      Serial.print("02J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02K ",81,32);
  
  delay(10);

   resistance10();
   measurement(); 
   limit10();
 if(a == 1){
      Serial.print("02K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02L ",81,46);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit11();
  if(a == 1){
      Serial.print("02L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02M ",81,60);
 
  delay(10);
  
  resistance12();
  measurement(); 
  limit12();
 if(a == 1){
      Serial.print("02M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02N ",81,74);
  
  delay(10);
  
  resistance13();
measurement(); 
limit13();
if(a == 1){
      Serial.print("02N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02O ",81,88);
  
  delay(10);
  
  resistance14();
  measurement();
  limit14();
 if(a == 1){
      Serial.print("02O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02P ",81,102);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit15();
  if(a == 1){
      Serial.print("02P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   

       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02Q ",81,116);
  
  delay(10);
  
  resistance16();
  measurement();
  limit16();
 if(a == 1){
      Serial.print("02Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("2B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02R ",0,18);
   
 
    delay(10);

     resistance17();
     measurement();
     limit();
    if(a == 1){
      Serial.print("02R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
    delay(105);
  
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("02S ",0,32);
   
   delay(10);
  
    resistance18();
    measurement(); 
    limit2();
    if(a == 1){
      Serial.print("02S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);

   myDisplay.setColor(255, 255, 255);
   myDisplay.print("02T ",0,46);
   
   delay(10);
   
    resistance19();
    measurement(); 
    limit3();
   if(a == 1){
      Serial.print("02T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    

   delay(105);
   
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("02U ",0,60);
   
   delay(10);
   
   resistance20();
   measurement(); 
   limit4();
  if(a == 1){
      Serial.print("02U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02V ",0,74);
   
 
    delay(10);
   
    resistance21();
   measurement(); 
   limit5();
  if(a == 1){
      Serial.print("02V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02W ",0,88);
   
   delay(10);

    resistance22();
   measurement(); 
   limit6();
  if(a == 1){
      Serial.print("02W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02X ",0,102);
   
  delay(10);
  
  resistance23();
  measurement(); 
  limit7();
  if(a == 1){
      Serial.print("02X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02Y ",0,116);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit8();
  if(a == 1){
      Serial.print("02Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02Z ",81,18);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit9();
 if(a == 1){
      Serial.print("02Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02a ",81,32);
  
  delay(10);

   resistance26();
   measurement(); 
   limit10();
   if(a == 1){
      Serial.print("02a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02b ",81,46);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit11();
  if(a == 1){
      Serial.print("02b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02c ",81,60);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit12();
 if(a == 1){
      Serial.print("02c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02d ",81,74);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit13();
  if(a == 1){
      Serial.print("02d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02e ",81,88);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit14();
 if(a == 1){
      Serial.print("02e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("02f ",81,102);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit15();
  if(a == 1){
      Serial.print("02f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("02f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }

 myDisplay.clrScr(); 
 myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("3A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03C ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   delay(10);
  
    resistance2();
    measurement(); 
    limit1();

    if(a == 1){
      Serial.print("03C: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   delay(105);

   myDisplay.setColor(255, 255, 255);
   myDisplay.print("03D ",0,32);
   
   delay(10);
   
    resistance3();
    measurement(); 
    limit2();
   if(a == 1){
      Serial.print("03D: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03D: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
   
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("03E ",0,46);
   
   delay(10);
   
   resistance4();
   measurement(); 
   limit3();
  if(a == 1){
      Serial.print("03E: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03E: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03F ",0,60);
   
 
    delay(10);
   
    resistance5();
   measurement(); 
   limit4();
   if(a == 1){
      Serial.print("03F: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03F: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03G ",0,74);
   
   delay(10);

    resistance6();
   measurement();
   limit5();
   if(a == 1){
      Serial.print("03G: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03G: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03H ",0,88);
   
  delay(10);
  
  resistance7();
  measurement(); 
  limit6();
 if(a == 1){
      Serial.print("03H: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03H: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03I ",0,102);
  
  delay(10);
  
  resistance8();
  measurement(); 
  limit7();
  if(a == 1){
      Serial.print("03I: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03J ",0,116);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit8();
  if(a == 1){
      Serial.print("03J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03K ",81,18);
  
  delay(10);

   resistance10();
   measurement(); 
   limit9();
  if(a == 1){
      Serial.print("03K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03L ",81,32);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit10();
  if(a == 1){
      Serial.print("03L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03M ",81,46);
 
  delay(10);
  
  resistance12();
  measurement();
  limit11();
 if(a == 1){
      Serial.print("03M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03N ",81,60);
  
  delay(10);
  
  resistance13();
  measurement(); 
  limit12();
  if(a == 1){
      Serial.print("03N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03O ",81,74);
  
  delay(10);
  
  resistance14();
  measurement(); 
  limit13();
  if(a == 1){
      Serial.print("03O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03P ",81,88);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit14();
 if(a == 1){
      Serial.print("03P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03Q ",81,102);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit15();
 if(a == 1){
      Serial.print("03Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03R ",81,116);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit16();
  if(a == 1){
      Serial.print("03R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("3B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03S ",0,18);
   
 
    delay(10);
  
    resistance18();
    measurement(); 
    limit();
    if(a == 1){
      Serial.print("03S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);

   myDisplay.setColor(255, 255, 255);
   myDisplay.print("03T ",0,32);
   
   delay(10);
   
    resistance19();
    measurement();
    limit2();
    if(a == 1){
      Serial.print("03T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    

   delay(105);
   
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("03U ",0,46);
   
   delay(10);
   
   resistance20();
   measurement();
   limit3();
   if(a == 1){
      Serial.print("03U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03V ",0,60);
   
 
    delay(10);
   
    resistance21();
   measurement(); 
   limit4();
  if(a == 1){
      Serial.print("03V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03W ",0,74);
   
   delay(10);

    resistance22();
   measurement();
   limit5();
  if(a == 1){
      Serial.print("03W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03X ",0,88);
   
  delay(10);
  
  resistance23();
  measurement(); 
  limit6();
 if(a == 1){
      Serial.print("03X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03Y ",0,102);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit7();
  if(a == 1){
      Serial.print("03Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03Z ",0,116);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit8();
  if(a == 1){
      Serial.print("03Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03a ",81,18);
  
  delay(10);

   resistance26();
   measurement(); 
   limit9();
  if(a == 1){
      Serial.print("03a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03b ",81,32);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit10();
 if(a == 1){
      Serial.print("03b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03c ",81,46);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit11();
  if(a == 1){
      Serial.print("03c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03d ",81,60);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit12();
  if(a == 1){
      Serial.print("03d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03e ",81,74);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit13();

  if(a == 1){
      Serial.print("03e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("03f ",81,88);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit14();
 if(a == 1){
      Serial.print("03f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("03f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }

  
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("4A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04D ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
   
    resistance3();
    measurement(); 
    limit1();

    if(a == 1){
      Serial.print("04D: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
     

   delay(105);
   
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("04E ",0,32);
   
   delay(10);
   
   resistance4();
   measurement(); 
   limit2();

 if(a == 1){
      Serial.print("04E: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04E: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04F ",0,46);
   
 
    delay(10);
   
    resistance5();
   measurement(); 
   limit3();
   if(a == 1){
      Serial.print("04F: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04F: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04G ",0,60);
   
   delay(10);

    resistance6();
   measurement(); 
   limit4();
   if(a == 1){
      Serial.print("04G: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04G: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04H ",0,74);
   
  delay(10);
  
  resistance7();
  measurement(); 
  limit5();
   if(a == 1){
      Serial.print("04H: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04H: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04I ",0,88);
  
  delay(10);
  
  resistance8();
  measurement(); 
  limit6();
   if(a == 1){
      Serial.print("04I: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04J ",0,102);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit7();
   if(a == 1){
      Serial.print("04J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04K ",0,116);
  
  delay(10);

   resistance10();
measurement();
limit8();
 if(a == 1){
      Serial.print("04K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04L ",81,18);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit9();
  if(a == 1){
      Serial.print("04L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04M ",81,32);
 
  delay(10);
  
  resistance12();
  measurement(); 
  limit10();
    if(a == 1){
      Serial.print("04M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04N ",81,46);
  
  delay(10);
  
  resistance13();
measurement(); 
limit11();
  if(a == 1){
      Serial.print("04N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04O ",81,60);
  
  delay(10);
  
  resistance14();
measurement(); 
limit12();
  if(a == 1){
      Serial.print("04O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04P ",81,74);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit13();
    if(a == 1){
      Serial.print("04P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04Q ",81,88);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit14();
  if(a == 1){
      Serial.print("04Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04R ",81,102);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit15();
   if(a == 1){
      Serial.print("04R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04S ",81,116);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit16();
    if(a == 1){
      Serial.print("04S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("4B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04T ",0,18);
   
 
    delay(10);
    
    resistance19();
    measurement(); 
    limit();
    if(a == 1){
      Serial.print("04T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    

   delay(105);
   
   myDisplay.setColor(255, 255, 255);
   myDisplay.print("04U ",0,32);
   
   delay(10);
   
   resistance20();
   measurement(); 
   limit2();
    if(a == 1){
      Serial.print("04U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04V ",0,46);
   
 
    delay(10);
   
    resistance21();
   measurement(); 
   limit3();
    if(a == 1){
      Serial.print("04V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04W ",0,60);
   
   delay(10);

    resistance22();
   measurement(); 
   limit4();
     if(a == 1){
      Serial.print("04W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04X ",0,74);
   
  delay(10);
  
  resistance23();
  measurement();
  limit5();
   if(a == 1){
      Serial.print("04X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04Y ",0,88);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit6();
 if(a == 1){
      Serial.print("04Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04Z ",0,102);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit7();
   if(a == 1){
      Serial.print("04Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04a ",0,116);
  
  delay(10);

   resistance26();
measurement(); 
limit8();
  if(a == 1){
      Serial.print("04a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04b ",81,18);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit9();
   if(a == 1){
      Serial.print("04b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04c ",81,32);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit10();
   if(a == 1){
      Serial.print("04c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04d ",81,46);
  
  delay(10);
  
  resistance29();
measurement(); 
limit11();
  if(a == 1){
      Serial.print("04d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04e ",81,60);
  
  delay(10);
  
  resistance30();
measurement(); 
limit12();
  if(a == 1){
      Serial.print("04e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("04f ",81,74);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit13();
  if(a == 1){
      Serial.print("04f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("04f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }


myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("5A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05E ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    
   
   resistance4();
   measurement();
   limit1();

    if(a == 1){
      Serial.print("05E: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05F ",0,32);
   
 
    delay(10);
   
    resistance5();
   measurement(); 
   limit2();
    if(a == 1){
      Serial.print("05F: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05F: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05G ",0,46);
   
   delay(10);

    resistance6();
   measurement(); 
   limit3();
  if(a == 1){
      Serial.print("05G: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05G: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05H ",0,60);
   
  delay(10);
  
  resistance7();
  measurement(); 
  limit4();
 if(a == 1){
      Serial.print("05H: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05H: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05I ",0,74);
  
  delay(10);
  
  resistance8();
  measurement(); 
  limit5();
  if(a == 1){
      Serial.print("05I: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05J ",0,88);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit6();
  if(a == 1){
      Serial.print("05J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05K ",0,102);
  
  delay(10);

   resistance10();
measurement(); 
limit7();
if(a == 1){
      Serial.print("05K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05L ",0,116);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit8();
 if(a == 1){
      Serial.print("05L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05M ",81,18);
 
  delay(10);
  
  resistance12();
  measurement(); 
  limit9();
  if(a == 1){
      Serial.print("05M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05N ",81,32);
  
  delay(10);
  
  resistance13();
measurement(); 
limit10();
if(a == 1){
      Serial.print("05N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05O ",81,46);
  
  delay(10);
  
  resistance14();
measurement(); 
limit11();
if(a == 1){
      Serial.print("05O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05P ",81,60);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit12();
  if(a == 1){
      Serial.print("05P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05Q ",81,74);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit13();
 if(a == 1){
      Serial.print("05Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05R ",81,88);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit14();
 if(a == 1){
      Serial.print("05R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05S ",81,102);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit15();
  if(a == 1){
      Serial.print("05S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05T ",81,116);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit16();
 if(a == 1){
      Serial.print("05T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("5B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05U ",0,18);
   
 
    delay(10);

   resistance20();
   measurement();
   limit();
   if(a == 1){
      Serial.print("05U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05V ",0,32);
   
 
    delay(10);
   
    resistance21();
   measurement(); 
   limit2();
   if(a == 1){
      Serial.print("05V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05W ",0,46);
   
   delay(10);

    resistance22();
   measurement(); 
   limit3();
   if(a == 1){
      Serial.print("05W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05X ",0,60);
   
  delay(10);
  
  resistance23();
  measurement(); 
  limit4();
 if(a == 1){
      Serial.print("05X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05Y ",0,74);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit5();
  if(a == 1){
      Serial.print("05Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05Z ",0,88);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit6();
  if(a == 1){
      Serial.print("05Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05a ",0,102);
  
  delay(10);

   resistance26();
measurement(); 
limit7();
if(a == 1){
      Serial.print("05a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05b ",0,116);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit8();
 if(a == 1){
      Serial.print("05b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

 
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05c ",81,18);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit9();
 if(a == 1){
      Serial.print("05c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05d ",81,32);
  
  delay(10);
  
  resistance29();
measurement(); 
limit10();
if(a == 1){
      Serial.print("05d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05e ",81,46);
  
  delay(10);
  
  resistance30();
measurement(); 
limit11();
if(a == 1){
      Serial.print("05e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("05f ",81,60);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit12();
  if(a == 1){
      Serial.print("05f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("05f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }


myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("6A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06F ",0,18);
   
 
    delay(10);

      digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

    resistance5();
   measurement(); 
   limit1();

   if(a == 1){
      Serial.print("06F: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06G ",0,32);
   
   delay(10);

    resistance6();
   measurement();
   limit2();
  if(a == 1){
      Serial.print("06G: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06G: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06H ",0,46);
   
  delay(10);
  
  resistance7();
  measurement(); 
  limit3();
 if(a == 1){
      Serial.print("06H: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06H: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06I ",0,60);
  
  delay(10);
  
  resistance8();
  measurement();
  limit4();
  if(a == 1){
      Serial.print("06I: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06J ",0,74);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit5();
if(a == 1){
      Serial.print("06J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06K ",0,88);
  
  delay(10);

   resistance10();
measurement(); 
limit6();
if(a == 1){
      Serial.print("06K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06L ",0,102);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit7();
  if(a == 1){
      Serial.print("06L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06M ",0,116);
 
  delay(10);
  
  resistance12();
  measurement(); 
   limit8();
  if(a == 1){
      Serial.print("06M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06N ",81,18);
  
  delay(10);
  
  resistance13();
measurement(); 
limit9();
if(a == 1){
      Serial.print("06N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06O ",81,32);
  
  delay(10);
  
  resistance14();
measurement(); 
limit10();
if(a == 1){
      Serial.print("06O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06P ",81,46);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit11();
 if(a == 1){
      Serial.print("06P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06Q ",81,60);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit12();
 if(a == 1){
      Serial.print("06Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06R ",81,74);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit13();
  if(a == 1){
      Serial.print("06R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06S ",81,88);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit14();
  if(a == 1){
      Serial.print("06S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06T ",81,102);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit15();
 if(a == 1){
      Serial.print("06T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06U ",81,116);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit16();
 if(a == 1){
      Serial.print("06U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("6B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06V ",0,18);
   
 
    delay(10);

    resistance21();
   measurement(); 
   limit();
   if(a == 1){
      Serial.print("06V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06W ",0,32);
   
   delay(10);

    resistance22();
   measurement(); 
   limit2();
  if(a == 1){
      Serial.print("06W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06X ",0,46);
   
  delay(10);
  
  resistance23();
  measurement(); 
  limit3();
  if(a == 1){
      Serial.print("06X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06Y ",0,60);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit4();
  if(a == 1){
      Serial.print("06Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06Z ",0,74);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit5();
  if(a == 1){
      Serial.print("06Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06a ",0,88);
  
  delay(10);

   resistance26();
measurement(); 
limit6();
if(a == 1){
      Serial.print("06a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06b ",0,102);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit7();
 if(a == 1){
      Serial.print("06b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06c ",0,116);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit8();
 if(a == 1){
      Serial.print("06c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06d ",81,18);
  
  delay(10);
  
  resistance29();
measurement(); 
limit9();
if(a == 1){
      Serial.print("06d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06e ",81,32);
  
  delay(10);
  
  resistance30();
measurement(); 
limit10();
if(a == 1){
      Serial.print("06e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("06f ",81,46);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit11();
  if(a == 1){
      Serial.print("06f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("06f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("7A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07G ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

  
    resistance6();
   measurement(); 
   limit1();

   if(a == 1){
      Serial.print("07G: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07H ",0,32);
   
  delay(10);
  
  resistance7();
  measurement(); 
  limit2();

  if(a == 1){
      Serial.print("07H: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07H: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07I ",0,46);
  
  delay(10);
  
  resistance8();
  measurement(); 
  limit3();
  if(a == 1){
      Serial.print("07I: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07J ",0,60);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit4();
  if(a == 1){
      Serial.print("07J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07K ",0,74);
  
  delay(10);

   resistance10();
measurement();
limit5();
 if(a == 1){
      Serial.print("07K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07L ",0,88);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit6();
  if(a == 1){
      Serial.print("07L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07M ",0,102);
 
  delay(10);
  
  resistance12();
  measurement(); 
  limit7();
  if(a == 1){
      Serial.print("07M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07N ",0,116);
  
  delay(10);
  
  resistance13();
measurement(); 
limit8();
 if(a == 1){
      Serial.print("07N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07O ",81,18);
  
  delay(10);
  
  resistance14();
measurement(); 
limit9();
 if(a == 1){
      Serial.print("07O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07P ",81,32);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit10();
  if(a == 1){
      Serial.print("07P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07Q ",81,46);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit11();
   if(a == 1){
      Serial.print("07Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07R ",81,60);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit12();
   if(a == 1){
      Serial.print("07R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07S ",81,74);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit13();
   if(a == 1){
      Serial.print("07S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07T ",81,88);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit14();
  if(a == 1){
      Serial.print("07T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07U ",81,102);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit15();
   if(a == 1){
      Serial.print("07U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
       delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07V ",81,116);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit16();
  if(a == 1){
      Serial.print("07V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
       
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("7B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07W ",0,18);
   
 
    delay(10);

  
    resistance22();
   measurement(); 
   limit();
    if(a == 1){
      Serial.print("07W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07X ",0,32);
   
  delay(10);
  
  resistance23();
  measurement(); 
  limit2();
 if(a == 1){
      Serial.print("07X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07Y ",0,46);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit3();
  if(a == 1){
      Serial.print("07Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07Z ",0,60);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit4();
  if(a == 1){
      Serial.print("07Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07a ",0,74);
  
  delay(10);

   resistance26();
measurement(); 
limit5();
 if(a == 1){
      Serial.print("07a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07b ",0,88);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit6();
  if(a == 1){
      Serial.print("07b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07c ",0,102);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit7();
   if(a == 1){
      Serial.print("07c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07d ",0,116);
  
  delay(10);
  
  resistance29();
measurement(); 
limit8();
 if(a == 1){
      Serial.print("07d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07e ",81,18);
  
  delay(10);
  
  resistance30();
measurement(); 
limit9();
 if(a == 1){
      Serial.print("07e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("07f ",81,32);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit10();
  if(a == 1){
      Serial.print("07f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("07f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("8A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08H ",0,18);
   
 
    delay(10);

     digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
   
  
  resistance7();
  measurement(); 
  limit1();

  if(a == 1){
      Serial.print("08H: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08I ",0,32);
  
  delay(10);
  
  resistance8();
  measurement(); 
  limit2();
   if(a == 1){
      Serial.print("08I: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08J ",0,46);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit3();
 if(a == 1){
      Serial.print("08J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08K ",0,60);
  
  delay(10);

   resistance10();
measurement(); 
limit4();
if(a == 1){
      Serial.print("08K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08L ",0,74);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit5();
 if(a == 1){
      Serial.print("08L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08M ",0,88);
 
  delay(10);
  
  resistance12();
  measurement(); 
  limit6();
if(a == 1){
      Serial.print("08M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08N ",0,102);
  
  delay(10);
  
  resistance13();
measurement();
limit7();
if(a == 1){
      Serial.print("08N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08O ",0,116);
  
  delay(10);
  
  resistance14();
measurement(); 
limit8();
if(a == 1){
      Serial.print("08O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08P ",81,18);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit9();
 if(a == 1){
      Serial.print("08P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08Q ",81,32);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit10();
  delay(105);
 if(a == 1){
      Serial.print("08Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08R ",81,46);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit11();
 if(a == 1){
      Serial.print("08R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08S ",81,60);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit12();
if(a == 1){
      Serial.print("08S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08T ",81,74);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit13();
  if(a == 1){
      Serial.print("08T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08U ",81,88);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit14();
  if(a == 1){
      Serial.print("08U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08V ",81,102);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit15();
  if(a == 1){
      Serial.print("08V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08W ",81,116);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit16();
  if(a == 1){
      Serial.print("08W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("8B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08X ",0,18);
   
 
    delay(10);
   
  
  resistance23();
  measurement(); 
  limit();
 if(a == 1){
      Serial.print("08X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08Y ",0,32);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit2();
 if(a == 1){
      Serial.print("08Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08Z ",0,46);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit3();
if(a == 1){
      Serial.print("08Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08a ",0,60);
  
  delay(10);

   resistance26();
measurement(); 
limit4();
if(a == 1){
      Serial.print("08a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08b ",0,74);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit5();
 if(a == 1){
      Serial.print("08b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08c ",0,88);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit6();
 if(a == 1){
      Serial.print("08c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08d ",0,102);
  
  delay(10);
  
  resistance29();
measurement();
limit7();
if(a == 1){
      Serial.print("08d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08e ",0,116);
  
  delay(10);
  
  resistance30();
measurement(); 
limit8();
if(a == 1){
      Serial.print("08e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("08f ",81,18);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit9();
  if(a == 1){
      Serial.print("08f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("08f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
    
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("9A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09I ",0,18);
   
 
    delay(10);

   digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

 
  
  resistance8();
  measurement(); 
  limit1();

  if(a == 1){
      Serial.print("09I: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09J ",0,32);
   
  delay(10);
  
  resistance9();
  measurement(); 
  limit2();
 if(a == 1){
      Serial.print("09J: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09K ",0,46);
  
  delay(10);

   resistance10();
measurement(); 
limit3();
if(a == 1){
      Serial.print("09K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09L ",0,60);
  
  delay(10);
  
   resistance11();
  measurement(); 
  limit4();
 if(a == 1){
      Serial.print("09L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09M ",0,74);
 
  delay(10);
  
  resistance12();
  measurement(); 
  limit5();
  if(a == 1){
      Serial.print("09M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09N ",0,88);
  
  delay(10);
  
  resistance13();
measurement(); 
limit6();
if(a == 1){
      Serial.print("09N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09O ",0,102);
  
  delay(10);
  
  resistance14();
measurement();
limit7();
if(a == 1){
      Serial.print("09O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09P ",0,116);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit8();
  if(a == 1){
      Serial.print("09P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09Q ",81,18);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit9();
  if(a == 1){
      Serial.print("09Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09R ",81,32);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit10();
 if(a == 1){
      Serial.print("09R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09S ",81,46);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit11();
 if(a == 1){
      Serial.print("09S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09T ",81,60);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit12();
  if(a == 1){
      Serial.print("09T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09U ",81,74);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit13();
 if(a == 1){
      Serial.print("09U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09V ",81,88);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit14();
 if(a == 1){
      Serial.print("09V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09W ",81,102);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit15();
 if(a == 1){
      Serial.print("09W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09X ",81,116);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit16();
 if(a == 1){
      Serial.print("09X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }
  myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("9B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09Y ",0,18);
   
 
    delay(10);

  
  resistance24();
  measurement(); 
  limit();
 if(a == 1){
      Serial.print("09Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09Z ",0,32);
   
  delay(10);
  
  resistance25();
  measurement(); 
  limit2();
 if(a == 1){
      Serial.print("09Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09a ",0,46);
  
  delay(10);

   resistance26();
measurement(); 
limit3();
if(a == 1){
      Serial.print("09a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09b ",0,60);
  
  delay(10);
  
   resistance27();
  measurement(); 
  limit4();
 if(a == 1){
      Serial.print("09b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
 
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09c ",0,74);
 
  delay(10);
  
  resistance28();
  measurement(); 
  limit5();
  if(a == 1){
      Serial.print("09c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09d ",0,88);
  
  delay(10);
  
  resistance29();
measurement(); 
limit6();
if(a == 1){
      Serial.print("09d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

    
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09e ",0,102);
  
  delay(10);
  
  resistance30();
measurement();
limit7();
if(a == 1){
      Serial.print("09e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("09f ",0,116);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit8();
 if(a == 1){
      Serial.print("09f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("09f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
 }
 break;
  }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("10A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010J ",0,18);
   
 
    delay(10);

   digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

 
  resistance9();
measurement(); 
limit17();
if(a == 1){
      Serial.print("010J: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010K ",0,32);
   
  delay(10);
  
  resistance10();
measurement(); 
limit18();
if(a == 1){
      Serial.print("010K: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010L ",0,46);
   
  delay(10);
  
  resistance11();
  measurement(); 
  limit19();
 if(a == 1){
      Serial.print("010L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010M ",0,60);
  
  delay(10);
  
  resistance12();
measurement(); 
limit20();
if(a == 1){
      Serial.print("010M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010N ",0,74);
  
  delay(10);

  resistance13();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("010N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010O ",0,88);
  
  delay(10);
  
  resistance14();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("01OK: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010P ",0,102);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit23();
 if(a == 1){
      Serial.print("010P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }


 delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010Q ",0,116);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit24();
 if(a == 1){
      Serial.print("010Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010R ",81,18);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("010R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010S ",81,32);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit26();
 if(a == 1){
      Serial.print("010S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010T ",81,46);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit27();
 if(a == 1){
      Serial.print("010T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010U ",81,60);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit28();
  if(a == 1){
      Serial.print("010U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010V ",81,74);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit29();
 if(a == 1){
      Serial.print("010V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010W ",81,88);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit30();
 if(a == 1){
      Serial.print("010W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010X ",81,102);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit31();
  if(a == 1){
      Serial.print("010X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010Y ",81,116);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit32();
 if(a == 1){
      Serial.print("010Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("10B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010Z ",0,18);
   
 
    delay(10);
 
  resistance25();
  measurement(); 
  limit0();
 if(a == 1){
      Serial.print("010Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010a ",0,32);
   
  delay(10);
  
  resistance26();
measurement(); 
limit18();
if(a == 1){
      Serial.print("010a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010b ",0,46);
   
  delay(10);
  
  resistance27();
  measurement(); 
  limit19();
 if(a == 1){
      Serial.print("010b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010c ",0,60);
  
  delay(10);
  
  resistance28();
measurement(); 
limit20();
if(a == 1){
      Serial.print("010c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010d ",0,74);
  
  delay(10);

  resistance29();
  measurement(); 
  limit21();
 if(a == 1){
      Serial.print("010d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010e ",0,88);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("010e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("010f ",0,102);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit23();
 if(a == 1){
      Serial.print("010f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("010f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("11A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011K ",0,18);
   
 
    delay(10);

   digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
 
  
resistance10();
measurement(); 
limit17();
if(a == 1){
      Serial.print("011K: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011L ",0,32);
   
  delay(10);
  
  resistance11();
  measurement(); 
  limit18();
 if(a == 1){
      Serial.print("011L: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011M ",0,46);
  
  delay(10);
  
  resistance12();
measurement(); 
limit19();
 if(a == 1){
      Serial.print("011M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011N ",0,60);
  
  delay(10);

  resistance13();
  measurement();
  limit20();
 if(a == 1){
      Serial.print("011N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011O ",0,74);
  
  delay(10);
  
  resistance14();
  measurement();
  limit21();
  if(a == 1){
      Serial.print("011O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011P ",0,88);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("011P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011Q ",0,102);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit23();
  if(a == 1){
      Serial.print("011Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011R ",0,116);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit24();
  if(a == 1){
      Serial.print("011R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011S ",81,18);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("011S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011T ",81,32);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit26();
  if(a == 1){
      Serial.print("011T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011U ",81,46);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit27();
  if(a == 1){
      Serial.print("011U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011V ",81,60);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit28();
   if(a == 1){
      Serial.print("011V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011W ",81,74);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit29();
   if(a == 1){
      Serial.print("011W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011X ",81,88);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit30();
  if(a == 1){
      Serial.print("011X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011Y ",81,102);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit31();
   if(a == 1){
      Serial.print("011Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011Z ",81,116);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit32();
  if(a == 1){
      Serial.print("011Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}

  
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("11B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011a ",0,18);
   
 
    delay(10);

resistance26();
measurement(); 
limit0();
if(a == 1){
      Serial.print("011a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011b ",0,32);
   
  delay(10);
  
  resistance27();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("011b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011c ",0,46);
  
  delay(10);
  
  resistance28();
measurement(); 
limit19();
 if(a == 1){
      Serial.print("011c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011d ",0,60);
  
  delay(10);

  resistance29();
  measurement();
  limit20();
  if(a == 1){
      Serial.print("011d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011e ",0,74);
  
  delay(10);
  
  resistance30();
  measurement();
  limit21();
   if(a == 1){
      Serial.print("011e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("011f ",0,88);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit22();
   if(a == 1){
      Serial.print("011f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("011f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("12A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012L ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

 resistance11();
  measurement(); 
  limit17();
  if(a == 1){
      Serial.print("012L: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012M ",0,32);
  
  delay(10);
  
  resistance12();
measurement(); 
limit18();
if(a == 1){
      Serial.print("012M: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012N ",0,46);
  
  delay(10);

  resistance13();
  measurement();
  limit19();
 if(a == 1){
      Serial.print("012N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012O ",0,60);
  
  delay(10);
  
  resistance14();
  measurement(); 
  limit20();
 if(a == 1){
      Serial.print("012O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012P ",0,74);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit21();
 if(a == 1){
      Serial.print("012P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }


   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012Q ",0,88);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("012Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012R ",0,102);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit23();
  if(a == 1){
      Serial.print("012R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  


   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012S ",0,116);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit24();
 if(a == 1){
      Serial.print("012S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012T ",81,18);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("012T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
 
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012U ",81,32);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit26();
  if(a == 1){
      Serial.print("012U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012V ",81,46);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit27();
 if(a == 1){
      Serial.print("012V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012W ",81,60);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit28();
  if(a == 1){
      Serial.print("012W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }



   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012X ",81,74);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit29();
 if(a == 1){
      Serial.print("012X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012Y ",81,88);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit30();
  if(a == 1){
      Serial.print("012Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012Z ",81,102);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit31();
  if(a == 1){
      Serial.print("012Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012a ",81,116);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit32();
  if(a == 1){
      Serial.print("012a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(250);
  
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}

   myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("12B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012b ",0,18);
   
 
    delay(10);

 resistance27();
  measurement(); 
  limit0();
 if(a == 1){
      Serial.print("012b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012c ",0,32);
  
  delay(10);
  
  resistance28();
measurement(); 
limit18();
if(a == 1){
      Serial.print("012c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }


  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012d ",0,46);
  
  delay(10);

  resistance29();
  measurement();
  limit19();
  if(a == 1){
      Serial.print("012d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012e ",0,60);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit20();
 if(a == 1){
      Serial.print("012e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("012f ",0,74);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit21();
 if(a == 1){
      Serial.print("012f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("012f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}



myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("13A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013M",0,18);
   
 
    delay(10);

   digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

resistance12();
measurement(); 
limit17();
if(a == 1){
      Serial.print("013M: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013N ",0,32);
  
  delay(10);

  resistance13();
  measurement();
  limit18();
 if(a == 1){
      Serial.print("013N: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013O ",0,46);
  
  delay(10);
  
  resistance14();
  measurement();
  limit19();
 if(a == 1){
      Serial.print("013O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013P ",0,60);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit20();
 if(a == 1){
      Serial.print("013P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013Q ",0,74);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("013Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013R ",0,88);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("013R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013S ",0,102);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit23();
 if(a == 1){
      Serial.print("013S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013T ",0,116);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit24();
 if(a == 1){
      Serial.print("013T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013U ",81,18);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("013U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013V ",81,32);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit26();
 if(a == 1){
      Serial.print("013V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013W ",81,46);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit27();
  if(a == 1){
      Serial.print("013W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013X ",81,60);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit28();
 if(a == 1){
      Serial.print("013X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013Y ",81,74);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit29();
  if(a == 1){
      Serial.print("013Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  


   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013Z ",81,88);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit30();
  if(a == 1){
      Serial.print("013Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013a ",81,102);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit31();
  if(a == 1){
      Serial.print("013a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013b ",81,116);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit32();
  if(a == 1){
      Serial.print("013b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(250);
  
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}


 

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("13B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013c",0,18);
   
 
    delay(10);

resistance28();
measurement(); 
limit0();
if(a == 1){
      Serial.print("013c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013d ",0,32);
  
  delay(10);

  resistance29();
  measurement();
  limit18();
 if(a == 1){
      Serial.print("013d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013e ",0,46);
  
  delay(10);
  
  resistance30();
  measurement();
  limit19();
  if(a == 1){
      Serial.print("013e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("013f ",0,60);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit20();
 if(a == 1){
      Serial.print("013f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("013f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}


myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("14A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014N ",0,18);
   
 
    delay(10);

   
     digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    
   resistance13();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("014N: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014O ",0,32);
  
  delay(10);
  
  resistance14();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("014O: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014P ",0,46);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit19();
  if(a == 1){
      Serial.print("014P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014Q ",0,60);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit20();
  if(a == 1){
      Serial.print("014Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014R ",0,74);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("014R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014S ",0,88);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit22();

   if(a == 1){
      Serial.print("014S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014T ",0,102);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit23();
  if(a == 1){
      Serial.print("014T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  


   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014U ",0,116);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit24();
  if(a == 1){
      Serial.print("014U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014V ",81,18);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit25();
   if(a == 1){
      Serial.print("014V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014W ",81,32);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit26();
   if(a == 1){
      Serial.print("014W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014X ",81,46);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit27();
    if(a == 1){
      Serial.print("014X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014Y ",81,60);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit28();
   if(a == 1){
      Serial.print("014Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014Z ",81,74);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit29();
   if(a == 1){
      Serial.print("014Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  


   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014a ",81,88);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit30();
    if(a == 1){
      Serial.print("014a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014b ",81,102);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit31();
   if(a == 1){
      Serial.print("014b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014c ",81,116);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit32();
    if(a == 1){
      Serial.print("014c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("14B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014d ",0,18);
   
 
    delay(10);

   resistance29();
  measurement();
  limit0();
   if(a == 1){
      Serial.print("014d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014e ",0,32);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit18();
   if(a == 1){
      Serial.print("014e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("014f ",0,46);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit19();
    if(a == 1){
      Serial.print("014f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("014f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
  
}




myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("15A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015O ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance14();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("015O: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015P ",0,32);
  
  delay(10);
  
  resistance15();
  measurement(); 
  limit18();
   if(a == 1){
      Serial.print("015P: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015Q ",0,46);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit19();
  if(a == 1){
      Serial.print("015Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015R ",0,60);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit20();
 if(a == 1){
      Serial.print("015R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015S ",0,74);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit21();
   if(a == 1){
      Serial.print("015S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015T ",0,88);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit22();
   if(a == 1){
      Serial.print("015T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015U ",0,102);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit23();
   if(a == 1){
      Serial.print("015U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015V ",0,116);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit24();
 if(a == 1){
      Serial.print("015V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015W ",81,18);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("015W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015X ",81,32);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit26();
   if(a == 1){
      Serial.print("015X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015Y ",81,46);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit27();
  if(a == 1){
      Serial.print("015Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  


   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015Z ",81,60);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit28();
  if(a == 1){
      Serial.print("015Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015a ",81,74);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit29();
  if(a == 1){
      Serial.print("015a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015b ",81,88);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit30();
   if(a == 1){
      Serial.print("015b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015c ",81,102);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit31();
   if(a == 1){
      Serial.print("015c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015d ",81,116);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit32();
   if(a == 1){
      Serial.print("015d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
}

  myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("15B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015e ",0,18);
   
 
    delay(10);

   resistance30();
  measurement();
  limit0();
 if(a == 1){
      Serial.print("015e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("015f ",0,32);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit18();
 if(a == 1){
      Serial.print("015f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("015f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
}


myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("16A.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016P ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance15();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("016P: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016Q ",0,32);
  
  delay(10);
  
  resistance16();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("016Q: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016R ",0,46);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit19();
  if(a == 1){
      Serial.print("016R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016S ",0,60);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit20();
   if(a == 1){
      Serial.print("016S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016T ",0,74);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit21();
   if(a == 1){
      Serial.print("016T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016U ",0,88);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("016U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016V ",0,102);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit23();
  if(a == 1){
      Serial.print("016V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016W ",0,116);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit24();
   if(a == 1){
      Serial.print("016W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016X ",81,18);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("016X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016Y ",81,32);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit26();
  if(a == 1){
      Serial.print("016Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016Z ",81,46);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit27();
  if(a == 1){
      Serial.print("016Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016a ",81,60);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit28();
  if(a == 1){
      Serial.print("016a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016b ",81,74);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit29();
   if(a == 1){
      Serial.print("016b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016c ",81,88);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit30();
   if(a == 1){
      Serial.print("016c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016d ",81,102);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit31();
   if(a == 1){
      Serial.print("016d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016e ",81,116);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit32();
   if(a == 1){
      Serial.print("016e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("16B.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("016f ",0,18);
   
 
    delay(10);
    
   resistance31();
  measurement();
  limit0();
   if(a == 1){
      Serial.print("016f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("016f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("17.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017Q ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance16();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("017Q: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017R ",0,32);
  
  delay(10);
  
  resistance17();
  measurement(); 
  limit18();
   if(a == 1){
      Serial.print("017R: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017S ",0,46);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit19();
   if(a == 1){
      Serial.print("017S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017T ",0,60);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit20();
   if(a == 1){
      Serial.print("017T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017U ",0,74);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit21();
   if(a == 1){
      Serial.print("017U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017V ",0,88);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit22();
   if(a == 1){
      Serial.print("017V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017W ",0,102);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit23();
  if(a == 1){
      Serial.print("017W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017X ",0,116);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit24();
   if(a == 1){
      Serial.print("017X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017Y ",81,18);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit25();
   if(a == 1){
      Serial.print("017Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017Z ",81,32);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit26();
  if(a == 1){
      Serial.print("017Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017a ",81,46);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit27();
   if(a == 1){
      Serial.print("017a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017b ",81,60);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit28();
   if(a == 1){
      Serial.print("017b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017c ",81,74);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit29();
  if(a == 1){
      Serial.print("017c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017d ",81,88);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit30();
  if(a == 1){
      Serial.print("017d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017e ",81,102);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit31();
   if(a == 1){
      Serial.print("017e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("017f ",81,116);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit32();
  if(a == 1){
      Serial.print("017f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("017f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("18.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018R ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance17();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("018R: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018S ",0,32);
  
  delay(10);
  
  resistance18();
  measurement(); 
  limit18();
   if(a == 1){
      Serial.print("018S: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018T ",0,46);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit19();
   if(a == 1){
      Serial.print("018T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018U ",0,60);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit20();
   if(a == 1){
      Serial.print("018U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018V ",0,74);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("018V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018W ",0,88);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("018W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018X",0,102);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit23();
  if(a == 1){
      Serial.print("018X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018Y ",0,116);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit24();
  if(a == 1){
      Serial.print("018Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018Z ",81,18);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("018Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018a ",81,32);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit26();
  if(a == 1){
      Serial.print("018a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018b ",81,46);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit27();
  if(a == 1){
      Serial.print("018b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018c ",81,60);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit28();
   if(a == 1){
      Serial.print("018c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018d ",81,74);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit29();
   if(a == 1){
      Serial.print("018d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018e ",81,88);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit30();
  if(a == 1){
      Serial.print("018e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("018f ",81,102);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit31();
   if(a == 1){
      Serial.print("018f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("018f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("19.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019S ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance18();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("019S: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019T ",0,32);
  
  delay(10);
  
  resistance19();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("019T: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019U ",0,46);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit19();
   if(a == 1){
      Serial.print("019U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019V ",0,60);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit20();
   if(a == 1){
      Serial.print("019V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019W ",0,74);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit21();
   if(a == 1){
      Serial.print("019W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019X ",0,88);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("019X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019Y",0,102);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit23();
   if(a == 1){
      Serial.print("019Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019Z ",0,116);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit24();
  if(a == 1){
      Serial.print("019Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019Z ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019a ",81,18);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("019a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019b ",81,32);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit26();
  if(a == 1){
      Serial.print("019b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019c ",81,46);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit27();
  if(a == 1){
      Serial.print("019c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019d ",81,60);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit28();
  if(a == 1){
      Serial.print("019d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019e ",81,74);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit29();
   if(a == 1){
      Serial.print("019e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("019f ",81,88);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit30();
   if(a == 1){
      Serial.print("019f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("019f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("20.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020T ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance19();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("020T: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020U ",0,32);
  
  delay(10);
  
  resistance20();
  measurement(); 
  limit18();
   if(a == 1){
      Serial.print("020U: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020V ",0,46);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit19();
  if(a == 1){
      Serial.print("020V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020W ",0,60);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit20();
  if(a == 1){
      Serial.print("020W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020X ",0,74);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit21();
   if(a == 1){
      Serial.print("020X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020Y ",0,88);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit22();
 if(a == 1){
      Serial.print("020Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020Z",0,102);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit23();
   if(a == 1){
      Serial.print("020Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020a ",0,116);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit24();
   if(a == 1){
      Serial.print("020a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020b ",81,18);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("020b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020c ",81,32);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit26();
   if(a == 1){
      Serial.print("020c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020d ",81,46);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit27();
   if(a == 1){
      Serial.print("020d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020e ",81,60);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit28();
   if(a == 1){
      Serial.print("020e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("020f ",81,74);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit29();
  if(a == 1){
      Serial.print("020f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("020f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("21.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021U ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance20();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("021U: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021V ",0,32);
  
  delay(10);
  
  resistance21();
  measurement(); 
  limit18();
   if(a == 1){
      Serial.print("021V: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021W ",0,46);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit19();
  if(a == 1){
      Serial.print("021W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021X ",0,60);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit20();
   if(a == 1){
      Serial.print("021X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021Y ",0,74);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit21();
   if(a == 1){
      Serial.print("021Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021Z ",0,88);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("021Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021a",0,102);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit23();
   if(a == 1){
      Serial.print("021a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021b ",0,116);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit24();
  if(a == 1){
      Serial.print("021b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021c ",81,18);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit25();
   if(a == 1){
      Serial.print("021c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021d ",81,32);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit26();
   if(a == 1){
      Serial.print("021d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021e ",81,46);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit27();
  if(a == 1){
      Serial.print("021e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("021f ",81,60);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit28();
   if(a == 1){
      Serial.print("021f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("021f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }


  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("22.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022V ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance21();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("022V: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022W ",0,32);
  
  delay(10);
  
  resistance22();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("022W: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022X ",0,46);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit19();
 if(a == 1){
      Serial.print("022X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022Y ",0,60);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit20();
 if(a == 1){
      Serial.print("022Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022Z ",0,74);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("022Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022a ",0,88);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("022a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022b",0,102);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit23();
 if(a == 1){
      Serial.print("022b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022c ",0,116);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit24();
 if(a == 1){
      Serial.print("022c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022d",81,18);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("022d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022e ",81,32);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit26();
 if(a == 1){
      Serial.print("022e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("022f ",81,46);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit27();
  if(a == 1){
      Serial.print("022f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("022f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }


myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("23.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023W ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance22();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("023W: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023X ",0,32);
  
  delay(10);
  
  resistance23();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("023X: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023Y ",0,46);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit19();
   if(a == 1){
      Serial.print("023Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023Z ",0,60);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit20();
   if(a == 1){
      Serial.print("023Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023a ",0,74);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("023a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023b ",0,88);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit22();
   if(a == 1){
      Serial.print("023b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023c",0,102);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit23();
   if(a == 1){
      Serial.print("023c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023d ",0,116);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit24();
   if(a == 1){
      Serial.print("023d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023e",81,18);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit25();
  if(a == 1){
      Serial.print("023e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("023f ",81,32);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit26();
   if(a == 1){
      Serial.print("023f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("023f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("24.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024X ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance23();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("024X: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024Y ",0,32);
  
  delay(10);
  
  resistance24();
  measurement(); 
  limit18();
   if(a == 1){
      Serial.print("024Y: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("024Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024Z ",0,46);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit19();
  if(a == 1){
      Serial.print("024Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("024Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024a ",0,60);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit20();
  if(a == 1){
      Serial.print("024a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("024a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024b ",0,74);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("024b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("024b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024c ",0,88);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("024c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("024c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024d",0,102);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit23();
 if(a == 1){
      Serial.print("024d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("024d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024e ",0,116);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit24();
  if(a == 1){
      Serial.print("024e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("024e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
 

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("024f",81,18);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit25();
 if(a == 1){
      Serial.print("024f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("024f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("25.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("025Y ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance24();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("025Y: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("025Z ",0,32);
  
  delay(10);
  
  resistance25();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("025Z: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("025Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("025a ",0,46);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit19();
  if(a == 1){
      Serial.print("025a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("025a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("025b ",0,60);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit20();
 if(a == 1){
      Serial.print("025b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("025b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("025c ",0,74);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("025c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("025c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("025d ",0,88);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit22();
  if(a == 1){
      Serial.print("025d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("025d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("025e",0,102);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit23();
  if(a == 1){
      Serial.print("025e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("025e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("025f ",0,116);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit24();
 if(a == 1){
      Serial.print("025f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("025f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }


myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("26.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("026Z ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance25();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("026Z: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("026a ",0,32);
  
  delay(10);
  
  resistance26();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("026a: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("026a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("026b ",0,46);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit19();
   if(a == 1){
      Serial.print("026b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("026b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("026c ",0,60);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit20();
  if(a == 1){
      Serial.print("026c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("026c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("026d ",0,74);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit21();
   if(a == 1){
      Serial.print("026d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("026d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("026e ",0,88);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit22();
   if(a == 1){
      Serial.print("026e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("026e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("026f",0,102);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit23();
  if(a == 1){
      Serial.print("026f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("026f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }



myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("27.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("027a ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance26();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("027a: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("027b ",0,32);
  
  delay(10);
  
  resistance27();
  measurement(); 
  limit18();
   if(a == 1){
      Serial.print("027b: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("027b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("027c ",0,46);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit19();
  if(a == 1){
      Serial.print("027c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("027c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("027d ",0,60);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit20();
  if(a == 1){
      Serial.print("027d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("027d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("027e ",0,74);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("027e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("027e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("027f ",0,88);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit22();
   if(a == 1){
      Serial.print("027f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("027f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
 
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }


myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("28.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("028b ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance27();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("028b: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("028c ",0,32);
  
  delay(10);
  
  resistance28();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("028c: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("028c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("028d",0,46);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit19();
 if(a == 1){
      Serial.print("028d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("028d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("028e ",0,60);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit20();
 if(a == 1){
      Serial.print("028e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("028e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("028f ",0,74);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit21();
  if(a == 1){
      Serial.print("028f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("028f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("29.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("029c ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance28();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("029c: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("029d ",0,32);
  
  delay(10);
  
  resistance29();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("029d: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("029d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("029e",0,46);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit19();
   if(a == 1){
      Serial.print("029e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("029e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("029f ",0,60);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit20();
  if(a == 1){
      Serial.print("029f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("029f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }
 
myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("30.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("030d ",0,18);
   
 
    delay(10);

    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance29();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("030d: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("030e ",0,32);
  
  delay(10);
  
  resistance30();
  measurement(); 
  limit18();
  if(a == 1){
      Serial.print("030e: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("030e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("030f",0,46);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit19();
   if(a == 1){
      Serial.print("030f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("030f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 }

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("31.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("031e ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance30();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("031e: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

   delay(105);
  
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("031f ",0,32);
  
  delay(10);
  
  resistance31();
  measurement(); 
  limit18();
 if(a == 1){
      Serial.print("031f: ");
      a = 0;
      Serial.print(x_r,1);
      Serial.print("  FAIL");
      }
      else if(a == 1){
      Serial.print("031f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }
  
  delay(250);
while (1) {
    button = digitalRead(A7);

    if (button == 0) {
      break;
    }
}
 break;
 

myDisplay.clrScr(); 
myDisplay.setColor(255, 255, 255); 

while(1){

  myDisplay.setColor(100, 255, 255);
  myDisplay.setFont(mykefont2);
  myDisplay.print("32.RESISTANCE TEST",CENTER,3);
   delay(105);
  myDisplay.setColor(255, 255, 255);
  myDisplay.print("032f ",0,18);
   
 
    delay(10);

    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW
    );
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

   resistance31();
  measurement();
  limit17();
  if(a == 1){
      Serial.print("030f: ");
      a = 0;
      Serial.print(y_r_r,2);
      Serial.print("  FAIL");
      }

  delay(250);

 if(vtest==496)
  {

      myDisplay.setColor(0, 255, 0);
       myDisplay.print("ALL TEST PASSED",15,110);
       Serial.println();
       Serial.print ("  01A-032f  "); 
       Serial.print ("   ***ALL TEST PASSED***  "); 
  delay(1000);
  Serial.print("END");
      
   
  }
else
{  
  myDisplay.print(" TEST FAILED",15,110);
  Serial.println();
  Serial.println();
  Serial.print("      ***TEST FAILED***");
     delay(1000);
     Serial.print("END");
  }
 break;
 }

 resetFunc();
}

}

 void resistance(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);
}
      
void resistance1(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
  void resistance2(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
   void resistance3(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    delay(1000);

   }
  void resistance4(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, HIGH);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
 void  resistance5(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, HIGH);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
   void resistance6(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
  void resistance7(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
 void  resistance8(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);
    digitalWrite(47, LOW);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
  void resistance9(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);
    digitalWrite(47, LOW);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    
    delay(1000);

    
   }
  void resistance10(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);
    digitalWrite(47, LOW);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
void  resistance11(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);
    digitalWrite(47, LOW);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
  void resistance12(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, LOW);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
 void  resistance13(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, LOW);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);
  
   }
 void  resistance14(void)
{
  
   digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, LOW);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
 void  resistance15(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, LOW);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);
  }
    
   

    void resistance16(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, HIGH);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
void resistance17(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
  void resistance18(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, HIGH);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
   void resistance19(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);
    digitalWrite(46, LOW);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
  void resistance20(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, HIGH);
    digitalWrite(46, LOW);
    digitalWrite(47, HIGH);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
 void  resistance21(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, HIGH);
    digitalWrite(46, LOW);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
   void resistance22(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, LOW);
    digitalWrite(47, HIGH);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
  void resistance23(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, LOW);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
 void  resistance24(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
  void resistance25(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    
    delay(1000);


   }
  void resistance26(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
void  resistance27(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


    
   }
  void resistance28(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


    
   }
 void  resistance29(void)
{
  
    digitalWrite(44, LOW);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);

   }
 void  resistance30(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, LOW);
    digitalWrite(41, HIGH);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
 void  resistance31(void)
{
  
    digitalWrite(44, HIGH);
    digitalWrite(45, HIGH);
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
    digitalWrite(40, HIGH);
    digitalWrite(41, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    
    delay(1000);


   }
   
   void limit(void)

   {
   
       if ((x_r<= 2.5) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,27,18);
     a++;
      vtest++;
    }
      else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,27,18);
      vtest++;
       }
       else {
      myDisplay.setColor(0, 0, 255);
       myDisplay.printNumF(y_r_r, 1,27,18);
       a++;
       }
       
 }

   void limit1(void)

   {

       if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 255, 0); 
     myDisplay.printNumF((x_r), 1,27,18);
      vtest++;
    }        
      else
    {
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,27,18);
      a++;
    
       }

   }

   void limit2(void)
   {
   
      if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,27,32);

      vtest++;
     a++;
      }
   else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,27,32);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,27,32);
      a++;
       } 
   }

   void limit3(void)
   {
  
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,27,46);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,27,46);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,27,46);
      a++;
       } 
    }
   void limit4(void)
   {
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,27,60);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,27,60);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,27,60);
      a++;
       } 
    }
   void limit5(void)
   {
   
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,27,74);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,27,74);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,27,74);
      a++;
       } 
    }
   void limit6(void)
   {
  
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,27,88);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,27,88);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,27,88);
      a++;
       } 
    }
   void limit7(void)
   {
  
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,27,102);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,27,102);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,27,102);
      a++;
       } 
    }
   void limit8(void)
   {
  
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,27,116);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,27,116);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,27,116);
      a++;
       } 
    }
   void limit9(void)
   {
  
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,108,18);
      vtest++;
      a++;
    }
      else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,108,18);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,108,18);
      a++;
       } 
    }
   void limit10(void)
   
   {
  
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,108,32);
      vtest++;
      a++;
    } else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,108,32);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,108,32);
      a++;
       } 
    }
   void limit11(void)
   {
  
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,108,46);
      vtest++;
      a++;
    } else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,108,46);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,108,46);
      a++;
       }
    }
   void limit12(void)
   {

    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,108,60);
      vtest++;
      a++;
    } else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,108,60);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,108,60);
      a++;
       } 
    }
   void limit13(void)
   {
    
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,108,74);
      vtest++;
      a++;
    }
       else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,108,74);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,108,74);
      a++;
       } 
    }
   void limit14(void)
   {
    
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,108,88);
      vtest++;
      a++;
    } else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,108,88);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,108,88);
      a++;
       } 
    }
   void limit15(void)
   {

    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,108,102);
      vtest++;
      a++;
    } else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,108,102);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,108,102);
      a++;
       } 
    }
   void limit16(void)
   
  {
    
   if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,108,116);
      vtest++;
      a++;
    } else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,108,116);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,108,116);
      a++;
       }
    }

   void limit0(void)
   {
    
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,34,18);
     a++;
      vtest++;
    } else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,34,18);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,18);
      a++;
       } 
    }

   void limit17(void)
   {
  
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 255, 0); 
     myDisplay.printNumF(x_r, 1,34,18);
      vtest++;
    }
       else {
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,18);
      a++;
       } 
    }
   void limit18(void)
   {
   
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,34,32);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,34,32);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,32);
      a++;
       } 
    }
   void limit19(void)
   {
  
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,34,46);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,34,46);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,46);
      a++;
       } 
    }
   void limit20(void)
   {
   
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,34,60);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,34,60);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,60);
      a++;
       }
    }
   void limit21(void)
   {
   
    if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,34,74);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,34,74);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,74);
      a++;
       } 
    }
   void limit22(void)
   {
   
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,34,88);
      vtest++;
      a++;
    }
       else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,34,88);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,88);
      a++;
       } 
    }
   void limit23(void)
   {
   
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,34,102);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,34,102);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,102);
      a++;
       } 
    }
   void limit24(void)
   {
  
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,34,116);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,34,116);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,34,116);
      a++;
       } 
    }
   void limit25(void)
   {
   
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,115,18);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,115,18);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,115,18);
      a++;
       } 
    }
   void limit26(void)
   {
  
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,115,32);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,115,32);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,115,32);
      a++;
       } 
    }
   void limit27(void)
   {
  
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,115,46);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,115,46);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,115,46);
      a++;
       } 
    }
   void limit28(void)
   {
  
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,115,60);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,115,60);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,115,60);
      a++;
       } 
    }
   void limit29(void)
   {
   
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,115,74);
      vtest++;
      a++;
    }
      else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,115,74);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,115,74);
      a++;
       } 
    }
   void limit30(void)
   {
   
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,115,88);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,115,88);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,115,88);
      a++;
       } 
    }
   void limit31(void)
   {
  
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,115,102);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,115,102);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,115,102);
      a++;
       } 
    }
   void limit32(void)
   {
   
     if ((x_r<= 3) && (x_r>= -0.5)) {
     myDisplay.setColor(0, 0, 255); 
     myDisplay.printNumF(x_r, 1,115,116);
      vtest++;
      a++;
    }else if(y_r_r>=6410 && y_r_r<=6480){
     myDisplay.setColor(0, 255, 0);
      myDisplay.printNumF(y_r_r, 1,115,116);
      vtest++;
       }
       else{
     myDisplay.setColor(0, 0, 255);
      myDisplay.printNumF(y_r_r, 1,115,116);
      a++;
       } 
    }
   
   
  void measurement(void)
{
 
    readvalue2 = read_adc2(1);
    vout2 = readvalue2 *-1;

    x_r = (((vout2 + 2045)*30 ) / 1000)-1  ;
    y_r = ((0.0001831802*(pow(x_r, 3)))+(0.0012553126*(pow(x_r, 2)))+(0.8752914940*(pow(x_r, 1)))+(0.01));
 
    delay(3);

   if(y_r >= 42.500)
   {
      y_r_r = ((0.000074*(pow(y_r, 3))) - (0.014584*(pow(y_r, 2))) + (5.466445*(pow(y_r, 1))) - (9.064197));// 2M
   }
   else  
  {
      y_r_r = ((4.7057029*(pow(y_r, 1))) + (0.1308606));
  }

}   

void voltage_measure(void)
{
readvalue3 = read_adc3(1);

vout3 = readvalue3 *-1;

vout_v = vout3 / -121;
}


void resetFunc() {
     wdt_disable();
     wdt_enable(WDTO_15MS);
     while (1) {}
  }
