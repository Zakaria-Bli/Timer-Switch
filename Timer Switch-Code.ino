
//to wirelessly controle our switch we chose t use the 'BLYNK' app
//which provides,stable and diverse comntrole
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <WiFiClient.h>
#include <Wire.h> 
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
//WE define the lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);

//this part is to setup the timers
hw_timer_t * timer0 = NULL;
hw_timer_t * timer1 = NULL;
hw_timer_t * timer2 = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux2 = portMUX_INITIALIZER_UNLOCKED;

//here we define the pins used like buttons and switches
int select_button = 4;
int set_button    = 16;
int mode_pin      = 17;
int switch1       = 12;
int switch2       = 14;
int switch3       = 27;



// here we define the other  variables

int cnt   = 1;
int time0 = 1;
int time1 = 1;
int time2 = 1;
int time_delay = 400;

// here we define the boolean values
boolean setting = false;
boolean mod     = false;
boolean was_pnline = false;
//name and password of network 
//auth is the authentification to our blynk server
char auth[] = "MPrvBmQLbitdI1Y-GAMAJDm0nW1aobJw";
char ssid[] = "celec";
char pass[] = "celeccelec";
String us= "Walid Zaki";



void setup() 
{
//start the erial communication  
Serial.begin(9600);
Serial.println("");
//start the lcd module
lcd.begin();
lcd.setCursor(0,0);
lcd.clear();
lcd.print("POWERED BY :");
delay(100);
//some credit for the makers
for (int i=0;i<10;i++)
{
delay(300);
lcd.setCursor(i,1);
lcd.print(us[i]);
}
delay(400);
lcd.clear();

// here we define the pin modes 
pinMode(select_button,INPUT);
pinMode(set_button,   INPUT);
pinMode(mode_pin,     INPUT); 
pinMode(switch1,     OUTPUT);
pinMode(switch2,     OUTPUT);
pinMode(switch3,     OUTPUT);
 
// we start the interrupt for switching from online and offline
attachInterrupt(mode_pin,mode_switch,RISING);

}

//these 3 functions get executed each time the timer runs out
void IRAM_ATTR onTimer0()
{
//we turn the switch1 off and display it
digitalWrite(switch1,LOW);
Serial.println("SWITCH 1 OFF"); 
//stops the timer or else it will keep counting and executing this function  
stop_timer0();
}

void IRAM_ATTR onTimer1()
{
//we turn the switch2 off and display it
digitalWrite(switch2,LOW);   
Serial.println("SWITCH 2 OFF");
//stops the timer or else it will keep counting and executing this function  
stop_timer1();
}

void IRAM_ATTR onTimer2()
{
//we turn the switch2 off and display it
digitalWrite(switch3,LOW);
Serial.println("SWITCH 3 OFF");
//stops the timer or else it will keep counting and executing this function  
stop_timer2();
}

//the following functions handle the timing part of the code,we inject how many hours we want
// and it configures each time interrupt
void t0 (int time0)
{
digitalWrite(switch1,HIGH);
Serial.println("SWITCH 1 ON");
timer0 = timerBegin(0, 80, true); 
timerAttachInterrupt(timer0, &onTimer0, true); 
timerAlarmWrite(timer0,time0*1000000*3600, true); 
timerAlarmEnable(timer0);
}
void t1 (int time1)
{
digitalWrite(switch2,HIGH);
Serial.println("SWITCH 2 ON");
timer1 = timerBegin(1, 80, true);  
timerAttachInterrupt(timer1, &onTimer1, true); 
timerAlarmWrite(timer1, time1*1000000, true); 
timerAlarmEnable(timer1);
}
void t2 (int time2)
{
digitalWrite(switch3,HIGH);
Serial.println("switch 3 ON"); 
timer2 = timerBegin(2, 80, true);  
timerAttachInterrupt(timer2, &onTimer2, true); 
timerAlarmWrite(timer2, time2*1000000, true);
timerAlarmEnable(timer2);  
}

//functions that are called at the end of each timer to stop it from repeating
void stop_timer0()
{
timerAlarmDisable(timer0);   
timerDetachInterrupt(timer0);  
}
void stop_timer1()
{
timerAlarmDisable(timer1);   
timerDetachInterrupt(timer1);  
}
void stop_timer2()
{
timerAlarmDisable(timer2);   
timerDetachInterrupt(timer2);  
}

//an indication that the mode changed
void mode_switch()
{
mod =false;
setting == false;
was_pnline = true;
all_off();
Serial.println("----|you are now offline|----");
delay(100);
online_mode();

}
//this is the function that handles the offline functions like chosing time or turning everything on
void time_setup()
{ 

while (setting == false){
//before starting it checks if its the online mode,if the condition is true it runs the online function
//and breaks the offline one(tried multiple methods and this one worked the best) + it turnes all switches off to assure safety
if (mod==true)
{
t0(0);
t1(0);
t2(0);
while (mod == true)
{

Blynk.begin(auth, ssid, pass);
Serial.println("test");
}
return;
}


// in here we write the main logic for switch and time slection
if(digitalRead(select_button)==HIGH)
{
if(cnt < 6)
{

cnt=cnt+1;
delay(time_delay);
}
else
{
cnt=1;
delay(time_delay);
}

Serial.print("position :");
Serial.println(cnt);
if (cnt <4)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("SWITCH :");
lcd.print(cnt);
}
if (cnt==4)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("ALL-ON MODE");
}
if (cnt==5)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("ALL-OFF MODE");
}
if (cnt==6)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("ONLINE MODE");
}
}
if(digitalRead(set_button)== HIGH)
{

delay(time_delay);
setting = true; 
if(cnt<4)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("select time :"); 
Serial.print("you just selected switch n :");
Serial.println(cnt);
Serial.println("h");
delay(time_delay);
}
if(cnt == 4)
{
lcd.clear();
lcd.setCursor(0,1);
lcd.print("|turn all-on|");
Serial.println("you just selected switch n :4");
}
if(cnt == 5)
{
lcd.clear();
lcd.setCursor(0,1);
lcd.print("|turn all-off|");
}
if(cnt == 6)
{
lcd.clear();
lcd.setCursor(0,1);
lcd.print("press to connect");
}
} 
while(setting == true)
{     
if (cnt==1)
{
if(digitalRead(select_button) == HIGH)
{
if (time0 < 12)
{
time0=time0+1;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("time 1 is :");
lcd.print(time0);
lcd.print("h");
delay(time_delay);
}
else
{
time0=1;
delay(time_delay);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("time 1 is :");
lcd.print(time0);
lcd.print("h");
}    
}
if (digitalRead(set_button) == HIGH)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("switch 1 is on");

delay(time_delay);
setting = false;
//here  you write the first interrupt program
t0(time0);
Serial.print("first switch on for : ");
Serial.print(time0);
Serial.println("h");
}
}
if (cnt==2)
{
if(digitalRead(select_button) == HIGH)
{
if (time1< 12)
{
time1=time1+1;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("time 2 is :");
lcd.print(time1);
lcd.print("h");
delay(time_delay);
}
else
{
time1=1;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("time 2 is :");
lcd.print(time1);
lcd.print("h");
delay(time_delay);
}
     
}
if (digitalRead(set_button) == HIGH)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("switch 2 is on");

setting = false;
delay(time_delay);
//here  you write the second interrupt program
t1(time1);
Serial.print("second switch on for : ");
Serial.print(time1);
Serial.println("h");    
}
}
if (cnt==3)
{
if(digitalRead(select_button) == HIGH)
{
if (time2< 12)
{
time2=time2+1;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("time 3 is :");
lcd.print(time2);
lcd.print("h");
delay(time_delay);
}
else
{
time2=1;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("time 3 is :");
lcd.print(time2);
lcd.print("h");
delay(time_delay);
}
}
if (digitalRead(set_button) == HIGH)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("switch 3 is on");
delay(time_delay);
setting = false;
t2(time2);
Serial.print("third switch on for : ");
Serial.print(time2);
Serial.println("h");   
}
}
if (cnt==4)
{
if(digitalRead(set_button) == HIGH)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("|ALL ARE ON|");

delay(time_delay);
setting=false;
Serial.println("|ALL ARE ON|");
all_on();
}
}

if (cnt==5)
{
if(digitalRead(set_button) == HIGH)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("|ALL ARE OFF|");
delay(time_delay);
setting = false;
Serial.println("ALL OFF");
all_off();
}
}
if (cnt==6)
{
if(digitalRead(set_button) == HIGH)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("|NOW ON-LINE|");
delay(time_delay);
setting = false;
Serial.println("ALL OFF");
all_off();
mod=true;
}
}
}
}
}
//here we configure the online mode
void online_mode()
{
// it checks if the offline mode is on,if it is all switches will be turned off for safety
if (mod==false)
{
all_off();
return;
}   
}

//turns all switches off
void all_off()
{
//the names is clear no need to explain it again
digitalWrite(switch1,LOW);
digitalWrite(switch2,LOW);
digitalWrite(switch3,LOW);
}

//turns all switches on
void all_on()
{
digitalWrite(switch1,HIGH);
digitalWrite(switch2,HIGH);
digitalWrite(switch3,HIGH);
}


//in the loop weeee call the main functions
void loop()
{
if (mod == true)
{
 lcd.clear();
lcd.print("ON-LINE");
delay(200);
}
while (mod == false)
{
lcd.clear();
lcd.print("OFF-LINE");
delay(200);
time_setup();
}

}
