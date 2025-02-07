//in order for this code to work there are 2 steps we need to do.
// first thing we need to install and select the arduino 2560 board : step1: tool>>board"arduino uno">> step2: boards manager>>search emoro and we install "EMORO 2560" step3: we repeat step 1>>>> then select EMORO 2560>>> select it a second time and we changed our board
//second thing we need to ionstall the emoro 2560 library and we do as follows : sketch>>include libray>> manage libraries>>search emoro inn search bar>> install 2560 and we installed the library , now this code will have no troble compiling  
static int Lfront = 500;
static int Rfront = 2500;
static int Lback = 2500;
static int Rback = 500;
static int SMstop = 1500;
//intilaise values for moving the robot.  
static float  x = 0;
static double  m1 = 0 ; 
static double  m2 = 0 ;
static double itd = 0;
static double itds = 0;
static float  th = 0;
static int t = 550 ;
static int uls =0;
static int m1_ano =0;
static int m2_ano =0;
static int p = 0;
int cali1 = 30 ;
int cali2 = 30 ;
int S = 1 ;
int LSR;
int LCR0; 
int LCR; 
int LST = 500; 
int LCT = 275;
int i=4;
int br;
char *results[]= {"buttom right" , "top right" , "top left" , "buttom left", "nan"};
//intilaies other code relevant values.

// we find the theta using the (ITD * 34300) /  88.9(failed process)

//setup fucntion 
void setup() { 
Serial.begin(9600);
InitEmoro();
InitSwitch();
Ultrasonic.attach(GPP_0);
//intilaies ultrasonic sensor 
EmoroServo.attach(SERVO_0);
EmoroServo.attach(SERVO_1);
//intilaies sevo motors 
pinMode(ADC_0, INPUT);
pinMode(ADC_1, INPUT);
//intilaies sound sensors 
}


//function for report on time since program started and the last program result.
void th_report(){
for (int z=0; z<1;){
 x = micros();
 Lcd.locate(1,0);
 Lcd.print("loc:");
 Lcd.print(results[i]);
 Lcd.print ("       ");
 Lcd.locate(0, 0);
 Lcd.print("micro:");
 Lcd.print(x);  
 Lcd.print("  ");
 if(ReadSwitch(SW_3)){
  delay(1000);
  }
 if(ReadSwitch(SW_4)){
  z=1;
  Lcd.clear();
  }
 }
}

//function for sound reading and ultrasonic obstcale detection 
void sound_report() {
for (int z=0; z<1;){
 delay(100);
 m1_ano = analogRead (ADC_0);
 m2_ano = analogRead (ADC_1);
 uls = Ultrasonic.read(GPP_0);
 Lcd.locate(0,0);
 Lcd.print("m1:");
 Lcd.print(m1_ano);
 Lcd.print("    ");
 Lcd.print("m2:");
 Lcd.print(m2_ano);
 Lcd.print("  ");
 Lcd.locate(1,0);
 Lcd.print("uls:");
 Lcd.print(uls);
 Lcd.print("           ");
 if(ReadSwitch(SW_4)){
  z=1;
  }
 }
}
//function to find ITD 
int itimed (int calibration1 , int calibration2){
Lcd.clear();
for (int z=0; z<1;){
 m1_ano = analogRead (ADC_0);
 m2_ano = analogRead (ADC_1); 
 Lcd.locate(0,0); 
 Lcd.print("1");
 if (m1_ano > calibration1){ // m1 recived the sound 
  m1 = (micros());
  for (int x=0; x<1;){
   m1_ano = analogRead (ADC_0);
   m2_ano = analogRead (ADC_1); 
   Lcd.locate(0,0); 
   Lcd.print("2");  
  if (m2_ano > calibration2){ // m2 recived the sound 
   m2 = (micros());
   itd = m2-m1;  // calculate itd 
   x=1;
   z=1;  
   }      
  }
 }
 else if (m2_ano > calibration2){ // m2 recived the sound 
  m2 = (micros());
  for (int x=0; x<1;){
   m1_ano = analogRead (ADC_0);
   m2_ano = analogRead (ADC_1); 
   Lcd.locate(0,0); 
   Lcd.print("2"); 
   if (m1_ano > calibration1){ // m1 recived the sound 
    m1 = (micros());
    itd= m2-m1; // calculate itd
    x=1;
    z=1;  
    }      
   }
  } 
 }
}

//function for stage 'listning'
int Listen (int calibration1 , int calibration2){
for (int z=0; z<1;){ //listenig loop 
 Lcd.clear();
 Lcd.locate(0,1);
 Lcd.print("listening.");
 delay(250);
 Lcd.print(".");
 delay(250);
 Lcd.print(".");
 itimed(calibration1 , calibration2); // find itd fucntion 
 if (itd > -1 ) { // right turning 
  Lcd.clear();
  Lcd.locate(0,0);
  Lcd.print("sound location:              ");
  Lcd.locate(1,0);
  Lcd.print("right                       "); //indicate sound location 
  EmoroServo.write(SERVO_0, Lfront);
  EmoroServo.write(SERVO_1, Rback); // turning 
  delay(LST); // delay to turn 90 dgrees
  EmoroServo.write(SERVO_0, SMstop); // stop
  EmoroServo.write(SERVO_1, SMstop);
  delay(900); 
  LSR = 1; 
  z=1;
  } 
 else if (itd < -1 ){ // left turing 
  itd = - itd;
  Lcd.clear();
  Lcd.locate(0,0);
  Lcd.print("sound location:            ");
  Lcd.locate(1,0);
  Lcd.print("left                       "); //indicate sound location 
  EmoroServo.write(SERVO_0, Lback);
  EmoroServo.write(SERVO_1, Rfront); // turning 
  delay(LST); // delay to turn 90 dgrees
  EmoroServo.write(SERVO_0, SMstop); // stop
  EmoroServo.write(SERVO_1, SMstop);
  delay(900); 
  LSR=2; 
  z = 1;
  }
 }
}

//function for stage 'locating'  
int locate(int calibration1 , int calibration2, int del){
for (int z=0; z<1;){
 Lcd.clear();
 Lcd.locate(0,1);
 Lcd.print("locating.");
 itimed(calibration1 , calibration2); // find itd 
 if (itd > 792 && itd < 2200) { // right turning (only if itd is postive and within range)
  EmoroServo.write(SERVO_0, Lfront);
  EmoroServo.write(SERVO_1, Rback);
  delay(del);
  EmoroServo.write(SERVO_0, SMstop);
  EmoroServo.write(SERVO_1, SMstop);
  z=1;
  LCR0 = 1; 
  } 
 else if (itd < -792 && itd > -2200){ // left turning (only if itd is negative and within range)
  itd = - itd;
  EmoroServo.write(SERVO_0, Lback);
  EmoroServo.write(SERVO_1, Rfront); 
  delay(del);
  EmoroServo.write(SERVO_0, SMstop);
  EmoroServo.write(SERVO_1, SMstop);
  z = 1;
  LCR0= 2; 
  }
  else {
   itd = 137.5*8; // fixed postion if itd is above or below range 
   if (itd < -1){ // left truning 
    EmoroServo.write(SERVO_0, Lback);
    EmoroServo.write(SERVO_1, Rfront); 
    delay(del);
    EmoroServo.write(SERVO_0, SMstop);
    EmoroServo.write(SERVO_1, SMstop);
    z=1;
    LCR0 =2; 
    }
    else if (itd > 1){ // right turning 
     EmoroServo.write(SERVO_0, Lfront);
     EmoroServo.write(SERVO_1, Rback);
     delay(del);
     EmoroServo.write(SERVO_0, SMstop);
     EmoroServo.write(SERVO_1, SMstop);
     z=1;
    LCR0 =1; 
    }
    }
 }
}

// fucntion for stage 'moving' 
void move_uls(){
Lcd.clear(); 
for (int x=0; x<1;){
 Lcd.locate(0,0);
 Lcd.print("moving.");
 delay(250);
 Lcd.print(".     ");
 delay(250);
 Lcd.print(".     ");
 uls = Ultrasonic.read(GPP_0);
 EmoroServo.write(SERVO_0, Lback);
 EmoroServo.write(SERVO_1, Rback); // move forward
 if (uls <= 20){ // obstcale withing range 
  EmoroServo.write(SERVO_0, SMstop); // stop 
  EmoroServo.write(SERVO_1, SMstop);
  Lcd.locate(0,0);
  Lcd.print("sound source reached"); //indicate sound source was reached 
  delay(1000);
  x=1;
  Lcd.clear(); 
  }
 } 
}

// fucntion to autmoatically calibrate microphones to properly indicate when sound recived 
// when calibrating do not touch robot microphones and dont not move robot, make sure you calibrate while servo motors are on  
void autocalibrate(){
for (int x=0; x<1000;){
 x += 1; 
 m1_ano = analogRead (ADC_0);
 m2_ano = analogRead (ADC_1);
 Lcd.locate(0,0);
 Lcd.print("calibarting...");
 if (m1_ano > cali1){
  cali1= m1_ano;
 }
 if (m2_ano > cali2){
  cali2= m2_ano;
 }
}
}

// manual calibration for microphones 
void calibrate(){
  for (int x=0; x<1;){
 m1_ano = analogRead (ADC_0);
 m2_ano = analogRead (ADC_1);
  Lcd.locate(0,0);
  Lcd.print("c1:");
  Lcd.print(cali1);
  Lcd.print(" c2:");
  Lcd.print(cali2);
  Lcd.print("       "); 
  Lcd.locate(1,0);
  Lcd.print("m1:");
  Lcd.print(m1_ano);
  Lcd.print("   m2:");
  Lcd.print(m2_ano);
  Lcd.print("       ");
  if(ReadSwitch(SW_2)){
    if(S==1){
    cali1 += 1;
    delay(100); 
    }
    else if(S==2){
    cali2 += 1;
    delay(100); 
    }
  }
  if(ReadSwitch(SW_1)){
     if(S==1){
    cali1 -= 1;
    delay(100); 
    }
    else if(S==2){
     cali2 -= 1;
    delay(100); 
    }
  }
   if(ReadSwitch(SW_3)){
    if(S==1){
     S = 2;
    delay(100); 
    } 
    else if(S==2){
     S = 1;
    delay(100); 
    } 
  }
   if(ReadSwitch(SW_4)){
    x = 1;  
  }
 }
}

// main fucntion / loop fucntion 
void loop() {
delay(100);    
Lcd.locate(0,0);
Lcd.print("1:run 2:report");
Lcd.locate(1,0);
Lcd.print("3:debug"); // main menue 

if(ReadSwitch(SW_1)){ // if sw1 is pressed run sound loclisation 
 Listen(cali1, cali2);// listening 
  locate(cali1, cali2, LCT);// locating 1 
  if (LCR0 ==1) {
    LCR = 1; 
  }
  else if (LCR0 ==2) {
    LCR = 2; 
  }
  delay (100);
   /*for (int z=0; z<500;){
  Lcd.locate(0,0);
  Lcd.print("object distance");
  Lcd.print(uls); 
 uls = Ultrasonic.read(GPP_0);
 z +=1; 
 }*/
  for (int x=0; x<1;){ 
 locate(cali1, cali2, itd/8); //locating 2 (loop)
 for (int z=0; z<500;){ // check for obstcale 
  Lcd.locate(0,0);
  Lcd.print("object distance");
  Lcd.print(uls); 
 uls = Ultrasonic.read(GPP_0);
 z +=1; 
 }
 if (uls<51){ // if obstcale within range break loop 
  x = 1; 
  }
  }
 move_uls(); // moving 
 // check program result 
 if (LSR==1 ) {
  if (LCR==1){
    i = 0; 
    }
  else if (LCR==2){ 
   i = 1; 
   }
  }
 else if (LSR==2 ) {
  Lcd.clear();
  if (LCR==1){
   i = 2; 
   }
  if (LCR==2){
   i = 3;
   }
  }
  //print result 
 Lcd.clear();
 Lcd.locate(0,0); 
 Lcd.print("location:");
 Lcd.locate(0,0); 
 Lcd.print(results[i]);
 delay(1000); 
 }
 
if(ReadSwitch(SW_2)){ // if sw is pressed move to report menue 
 Lcd.clear();
 delay(100);
 for (int x=0; x<1;){
  Lcd.locate(0, 0);
  Lcd.print ("sw1:sound report ");
  Lcd.locate(1, 0);
  Lcd.print ("sw2:sys&th");
  if(ReadSwitch(SW_1)){ // sw1 sound report   
   sound_report();
   Lcd.clear();
   delay(100);
   }
  if(ReadSwitch(SW_2)){ // sw2 th report 
   th_report();
   Lcd.clear();
   delay(100);
   } 
  if(ReadSwitch(SW_4)){ // sw4 back to main menue 
   x=1;
   Lcd.print ("sw1");
   Lcd.clear();   
   }
  }
 }
delay(100);
if(ReadSwitch(SW_3)){ // if sw3 is pressed go to debug menue 
 Lcd.clear();
 delay(100);
 for (int x=0; x<1;){
  Lcd.locate(0, 0);
  Lcd.print ("sw1 calibrate");
  Lcd.locate(1, 0);
  Lcd.print ("sw2 stages");
  delay(100);  
  if(ReadSwitch(SW_1)){ // sw1 calibrate menue 
   for (int z=0; z<1;){
   Lcd.locate(0, 0);
   Lcd.print ("sw1 automatic");
   Lcd.locate(1, 0);
   Lcd.print ("sw2 manual");
   delay(100);
  if(ReadSwitch(SW_4)){ // sw4 back to debug menue 
   z=1;
   Lcd.clear();
   }
  if(ReadSwitch(SW_1)){ // sw1 auto calibrate  
   autocalibrate();
   }
  if(ReadSwitch(SW_2)){ //sw2 manual calibrate    
   calibrate();
   }
   }
  if(ReadSwitch(SW_4)){// sw4 back to main menue 
   x=1;
   Lcd.clear();
   }    
  }
  if(ReadSwitch(SW_2)){ // sw2 debug stages menue 
   delay(100);
   Lcd.clear();
   for (int z=0; z<1;){
    Lcd.locate(1, 0);
    Lcd.print ("Dbug mode  ");
    Lcd.locate(0, 0);
    Lcd.print ("Switch 1-2-3  ");
    if(ReadSwitch(SW_1)){ 
     Listen(cali1, cali2); //run listen 
     }
    if(ReadSwitch(SW_2)){
     locate(cali1, cali2, 275); // run locate 
     }
    if(ReadSwitch(SW_3)){ // run move_uls
     move_uls();
     }
    if(ReadSwitch(SW_4)){// back to debug menue 
     z=1;
     Lcd.clear();
     }
    }
  }
 }
 if(ReadSwitch(SW_4)){ // back to main menue 
  x=1;
  Lcd.clear();
  }
 }
}