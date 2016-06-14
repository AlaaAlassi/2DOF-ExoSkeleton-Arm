

int FSR_Pin = A0; //analog pin 0
int x,c,y,D,Dx,I,X,Y,X_1,Y_1,Y1,X1,m1=1;
byte var=1,h,l,TA,TB,TC,TG;
int arm_load = 290, compinsation=50;
int p1= 7.4 , p2= -308.4;
int T, Fx,Fy,a1 =3, a2 =3.5;
float t_sh,t_e, Th1= 0 , Th2= 0,  Th1_1 , Th2_1,x_1=0,y_1=0 ,t_e_1,t_sh_1,lc1=1.75,lc2=1.75,g1,g2,m2=0.5,g=9.81;
byte b;
word th;
//int zero_load = 10;
float w=0;
//---------- filter 
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int readings1[numReadings];      // the readings from the analog input
int index1 = 0;                  // the index of the current reading
int total1 = 0;                  // the running total

void setup(){
 //--------------
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
 //--------------  PWM control bits intial state
   digitalWrite(5,LOW);
  analogWrite(6,0);
  digitalWrite(8,LOW);
  analogWrite(9,0);
  digitalWrite(14,LOW);
  digitalWrite(2,LOW);
 //-------------- Prot L , k as input
 for (byte i=49 ; i >= 42 ; i--)
 { pinMode(i, INPUT);}
   pinMode(41, INPUT);
   DDRK = B00000000;
   
   DDRC = B00000000;  //  for encoder 1  low byte
   DDRA = B00000000;  //  for encoder 1
// --------------- GND pionts
   pinMode(12, OUTPUT);  
   digitalWrite(12,HIGH);      // for the encoder VCC
   pinMode(7, OUTPUT);        
   digitalWrite(7,LOW); 
  
}




void loop(){
  
 Th2 = 0.017*0.098*get_enc_P(2) - 13.65;   //  max = 2.12 rad = 122 dig
 Th1 = 0.017*0.098*get_enc_P(1) - 1.57;   //  max = 2.12 rad = 122 dig - 1.57

  if (Th1 >= 4)
 { Th1 = Th1-12-1.57-0.01; }
  
 if (Th2 <= -4)
 { Th2 = Th2+13.65+0.01; }
  
 
 
  
  switch (var){
    case 1:
 y = Bias_sensor(600, 400,0);
 x = Bias_sensor(600, 400,1); // suppused ti be -x according to cordinate system but it will produce forcess opposite to my direction.

y = -Filtery() ;
x = -Filterx() ;


X= x*cos(Th1+Th2) - y*sin(Th1+Th2); //x
Y= x*sin(Th1+Th2) + y*cos(Th1+Th2);




 X= X*0.2   ; //(x*0.001+ x_1) (x - x_1)*0.1 + (x*0.01+ x_1)
 Y= Y*0.2 ;//(y*0.001+ y_1 )  (y - y_1)*0.1 + (y*0.01+ y_1);
 
//D = (Y - Y_1);
//Dx = (X - X_1);
// Y1 = Y + D;
// X1= X+Dx;
 
 if (X >= 255 || X <= -255) 
 X=X_1;
 if (Y >=255 || Y <= -255) 
 Y=Y_1;

g1 = (m1*lc1+m2*a1)*g*cos(Th1)+m2*lc2*g*cos(Th1+Th2);
g2 = m2*lc2*g*cos(Th1+Th2);

  t_sh = (- a1*sin(Th1) - a2*sin(Th1 + Th2))*X + (a1*cos(Th1)+a2*cos(Th1+Th2))*Y+g1*2.5 ;// 4.5 gain for gravity 
  t_e = (-a2*sin(Th1+Th2))*X +a2*cos(Th1+Th2)*Y+g2*2.5;

     if (t_sh >= 255){t_sh = 255;}
     if (t_sh <= -255){t_sh = -255;}
     if (t_e >= 255){t_e = 255;}
     if (t_e <= -255){t_e= -255;}
     
   Serial.print(X);
   Serial.print(",");
  Serial.print(Y);
  Serial.print(",");
  Serial.print(Th1);
   Serial.print(",");
  Serial.print(Th2);
  Serial.print(",");
  Serial.print(t_sh);
  Serial.print(",");
  Serial.print(t_e);
  Serial.print(",");
   Serial.print(TB);
  Serial.print(",");
  Serial.println(TG);
  
  
/*  Serial.print(",");
  Serial.print(Th2);
  Serial.print(",");
  Serial.print(X);
  Serial.print(",");
  Serial.print(Y1);
  Serial.print(",");
  Serial.println(D);

  Serial.print(X);
  Serial.print(",");
  Serial.print(Y);
  Serial.print(",");
  Serial.print(Th1);
  Serial.print(",");
  Serial.print(Th2);
  Serial.print(",");
// t_e = byte (t_e);
 Serial.print (t_e/3);
 Serial.print(",");
 Serial.print(TA);
 Serial.print(",");
 Serial.print(-TB);
 Serial.print("  ");
//   t_e = byte (t_sh);
 Serial.print(t_sh/3);
Serial.print(",");
 Serial.print(TC);
 Serial.print(",");
 Serial.println(-TG);*/


  
  Th1_1 = Th1;
  Th2_1 = Th2;
  x_1 = x;
  y_1 = y;
  X_1 = X;
  Y_1 = Y;
// t_e_1;
// t_sh_1;
   
// x=  p1*x*x + p2*x + 3265;
 if (t_e > 0 ){ digitalWrite(8,LOW); 
 TA = byte(t_e);
   if (TA >= 255){TA = 255;}
   analogWrite(9,TA);}
 if (t_e < 0)
  { t_e =  -t_e ;
  TB = byte(t_e);
    if (TB >= 255){ TB = 255;}
    analogWrite(9,LOW);
   analogWrite(8,TB);}
   
    if (t_sh > 0 ){ digitalWrite(6,LOW); 
 TC = byte(t_sh);
   if (TC >= 255){TC = 255;}
   analogWrite(5,TC);}
 if (t_sh < 0)
  { 
  TG = byte(- t_sh );
    if (TG > 255){ TG = 255;}
    analogWrite(5,LOW);
   analogWrite(6,TG);}
      
   //   delay(20);

    break;
 //--------------------------------------   
    case 2:
   
  delay(10); //just here to slow down the output for easier reading

    break;
  //----------------------------------------  
  }
  

}
//-----------------------------------------------------------------------------------
// to biase and scale the force signal 
int Bias_sensor(int upper_limit, int lower_limit, byte ch){
  int Res;
  
  Res = analogRead(ch); 
  if (Res >= upper_limit  ) // supposing that the motor compinsaes for the whole wight of the arm
  {Res = ( Res - (upper_limit - 1) );
}
  else if ( Res <= lower_limit )
  {
    Res= Res - (lower_limit + 1) ;
  }
  else 
  Res=0;
  return Res;
}
//----------------------------------


word get_enc_P( byte chl){
  word res,th,l,h,j,m;
  switch(chl){
   //-----------
  case 1:
  
 j= PINC ;
 m= PINA & B0011111  ;
 th = word (m,j);
 if (th < 0) {th=0;}
 res = th;
 
    break;
  //------------
   case 2:
 l= PINL ;
 h= PINK & B0011111  ;
 th = word (h,l);
 if (th < 0) {th=0;}
 res = th;
   break;
  //------------
  return res;
    }
  }
//---------------------------------------------------------------------------------------------


int Filtery() {
  
    
    
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] =y; 
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
//  y = average;
return average;

}

int Filterx() {
  // subtract the last reading:
  total1= total1 - readings1[index1];         
  // read from the sensor:  
  readings1[index1] = x; 
  // add the reading to the total:
  total1= total1 + readings1[index1];       
  // advance to the next position in the array:  
  index1 = index1 + 1;                    

  // if we're at the end of the array...
  if (index1 >= numReadings)              
    // ...wrap around to the beginning: 
    index1 = 0;                           

  // calculate the average:
  average = total1 / numReadings;  
//  x = average;
  // send it to the computer as ASCII digits

  
return average;

  
}
   
