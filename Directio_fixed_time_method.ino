#include <TimerOne.h>


const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int sens=0;
int inputPin = A0;
volatile long v=0;
word c=0;
char A=0,B=0,A_1=0,B_1=0;
int y=0,y_1=0,k1=0.5;
 int v_1 , v_2 , v_3,s_1,s,sc;
 byte th;
void setup(void)
{
  Timer1.initialize(10000);
//  Timer1.attachInterrupt(dt); // dt = 0.01 seconds
  Serial.begin(9600);
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  attachInterrupt(0, enRead, CHANGE);
  attachInterrupt(1, enRead, CHANGE);
   // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0; 
    DDRD = B11100000;
    DDRB = B00011111;
    DDRC=  B11111111;
  
}

void loop() {
//  Serial.write(c*0.095);
Serial.write(c);

}

/*void dt (void)
{
}*/


//---------------------------
void enRead()
{
B=digitalRead(3);
A= digitalRead(2);
  if(B ^ A_1){
    c=c-1;
                      }
    else {c=c+1;} 
  A_1=A;
 PORTB = lowByte(c);
 PORTD = lowByte(c);
 PORTC = highByte(c)  ;
   
}

int Filter() {
  // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = v; 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;  
  average = average;
  // send it to the computer as ASCII digits
return average;
  
}

/*void enRead()
{
B=digitalRead(3);
A= digitalRead(2);
  if(B ^ A_1){
    c=c-50;
    if ( c <= 0 )
    {th = th - 1;}
                      }
    else {c=c+50;if ( c >= 255 )
    {th = th + 1;}} 
  A_1=A;
 PORTB = th;
 PORTD = th; */


