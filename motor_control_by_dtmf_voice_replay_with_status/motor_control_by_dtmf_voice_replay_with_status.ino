 #include <EEPROM.h>
#include <SoftwareSerial.h> 
#include "Talkie.h"
#include "Vocab_US_Large.h"
#include "Vocab_Special.h"
Talkie voice;
SoftwareSerial GSM(6, 7);   //connect D6 to gsm tx , D7 to gsm rx
char *phone_no = "+917602977090"; 
unsigned long currentTime;
unsigned long loopTime1;
unsigned long loopTime2;
#define Relay1 A0          //connect to relay
int load1;
char n[3];                 //connect D3 to gsm MIC +
void setup() {
pinMode(Relay1, OUTPUT); digitalWrite(Relay1, 1); 

Serial.begin(9600);  
GSM.begin(9600);   

Serial.println("Initializing....");
initModule("AT","OK",300);          
initModule("ATE0","OK",300);
initModule("AT+CPIN?","READY",300);
initModule("AT+CLIP=1","OK",300);  
initModule("AT+DDET=1","OK",300);
Serial.println("Initialized Successfully"); 

load1 = EEPROM.read(1);

relays();
}

void loop(){

currentTime = millis(); 
if(currentTime >= (loopTime1 + 2000)){
initModule("AT","OK",300); 
loopTime1 = currentTime;   
}
  
if(GSM.find("CLIP:")) {
 Serial.println("RING!");
 delay(200);
    
while(1){     
if(GSM.find(phone_no)){ 
initModule("ATA","OK",300);
delay(1500);voice.say(sp2_PRESS);voice.say(sp2_ONE);voice.say(sp4_FOR);voice.say(sp2_MOTOR);delay(100);voice.say(sp4_ON);
delay(1000);voice.say(sp2_PRESS);voice.say(sp2_TWO);voice.say(sp4_FOR);voice.say(sp2_MOTOR);delay(100);voice.say(sp4_OFF);
delay(1000);voice.say(sp2_PRESS);voice.say(sp2_THREE);voice.say(sp4_FOR);voice.say(sp2_MOTOR);delay(100);voice.say(sp4_CHECK);
delay(500);
loopTime2 = currentTime; 
while (1){   
   currentTime = millis();  
if(currentTime >= (loopTime2 + 25000)){
  Serial.println("Call End");
  loopTime2 = currentTime; 
  break;
} 
      
if(GSM.find("+DTMF:")){
  int Data = GSM.parseInt();
  switch(Data){                                

case 1: {
   load1=0;
   delay(1500);voice.say(sp2_MOTOR);voice.say(sp4_ON);delay(1000);
   eeprom_write();
   relays();  
loopTime2 = currentTime;
   break;}

case 2: { 
   load1=1;
   delay(1500);voice.say(sp2_MOTOR);voice.say(sp4_OFF);delay(1000);
   eeprom_write();
   relays();  
loopTime2 = currentTime;   
   break;}

case 3: {
   initModule("AT+DDET=0","OK",300);  
   if(load1==0){ 
       delay(1000);voice.say(sp2_MOTOR);voice.say(sp4_IS);voice.say(sp4_ON);delay(500); 
   }else{   
    if(load1==1){
     delay(1000);voice.say(sp2_MOTOR);voice.say(sp4_IS);delay(200);voice.say(sp4_OFF);delay(500);
    }
   } 
   initModule("AT+DDET=1","OK",300);   
loopTime2 = currentTime;   
   break;}

    } 
   }
  }   
 } 
else {
initModule("AT","OK",300); 
break;            
 } 
}
 initModule("AT+CHUP;","OK",300); 
 }
 
}


void eeprom_write(){
EEPROM.write(1,load1);
 
}

void relays(){  
digitalWrite(Relay1, load1); 

}

 // Init GSM Module 
void initModule(String cmd, char *res, int t){
while(1){
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while(GSM.available()>0){
       if(GSM.find(res)){
        Serial.println(res);
        delay(t);
        return;
       }else{Serial.println("Error");}}
    delay(t);
  }
}   
  
