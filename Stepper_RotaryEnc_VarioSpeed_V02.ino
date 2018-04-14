/*
PIN USED

D9  Rotary encoder DT (end stop X on CNC shield)
D10 Rotary encoder CLK (end stop Y on CNC shield)
D11 Rotary encoder SW (end stop Z on CNC shield)
D4  speedbutton (Z on CNC shield)
D2  STEP on a4988
D5  DIR on a4988
D8  ENABLE on a4988
D3  ISR number (1 =INT1) - VIRTUAL - RESERVED

CONTROL LED :
D13  Green LED on SpnDir
D12  Red LED on SpnEn
D7    Yellow on Z Dir

*/


#include <AccelStepper.h>

int motorSpeed = 10000; //maximum steps per second (about 5rps / at 8 microsteps)
int motorAccel = 180; //steps/second/second to accelerate 3600 default

const unsigned char ttable[7][4] = {
{0x0, 0x2, 0x4, 0x0}, {0x3, 0x0, 0x1, 0x10},
{0x3, 0x2, 0x0, 0x0}, {0x3, 0x2, 0x1, 0x0},
{0x6, 0x0, 0x4, 0x0}, {0x6, 0x5, 0x0, 0x20},
{0x6, 0x5, 0x4, 0x0},
};

// Rotary encoder connections
#define DT 9
#define CLK 10
#define SW 11
#define DIR_CCW 0x10
#define DIR_CW 0x20

// CONTROL LED
#define LedYellow 7
#define LedRed 12
#define LedGreen 13

int speed;
int counter;
 
// Stepper connections
int motorStepPin = 2;
int motorDirPin = 5;
int speedbutton = 4;
const int Enable_PIN = 8;

AccelStepper stepper(1, motorStepPin, motorDirPin);
volatile unsigned char state = 0;

void intervitesse(){
 switch (digitalRead (speedbutton)) {
      case HIGH:
//      stepper.stop(); 
        speed=100;  // fast speed
         break;

      case LOW:
//      stepper.stop(); 
        speed=1;  // slow speed
        break;
                                      }  
                  }

void setup(){
// Rotary encoder
  pinMode(DT, INPUT);
  pinMode(CLK, INPUT);
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  pinMode(speedbutton, INPUT); 

// CONTROL LED
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  
// Motor
  pinMode(Enable_PIN, OUTPUT);
  digitalWrite(Enable_PIN, LOW);
  stepper.setMaxSpeed(motorSpeed);
  stepper.setSpeed(motorSpeed);
  stepper.setAcceleration(motorAccel);

//  stepper.moveTo(8000); //move 32000 steps (should be 10 rev)
  Serial.begin(9600);

// ISR on speed button selection
  attachInterrupt(1, intervitesse, CHANGE);

}

void loop(){
 

 unsigned char result;
  /* Reset the counter */
 counter = 0;

  while(1)
  {/* Read the status of the dial */
    unsigned char pinstate = (digitalRead(CLK) << 1) | digitalRead(DT);
    state = ttable[state & 0xf][pinstate];
   result=state&0x30;
    if(result==DIR_CCW) counter++;
        if(result==DIR_CW) counter--;

/* If needed, the number of steps can be modified with the rotary encoder :
   stepper.moveTo(counter*vitesse);
*/


   if (counter != 0) {
                      if (counter > 0) {digitalWrite(LedRed, HIGH); digitalWrite(LedGreen, LOW); digitalWrite(LedYellow, LOW);} 
                      else
                      {digitalWrite(LedRed, LOW); digitalWrite(LedGreen, LOW); digitalWrite(LedYellow, HIGH);}
                      }
         else {digitalWrite(LedRed, LOW); digitalWrite(LedYellow, LOW); digitalWrite(LedGreen, HIGH);}
   
   stepper.move(counter*speed);   
   stepper.run();
     }


}


