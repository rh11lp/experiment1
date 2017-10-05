#include <NewPing.h>


 // Gobetwino source http://mikmo.dk/gobetwino.html 
 // Twinkle Twinkle song: http://www.orangefreesounds.com/twinkle-twinkle-little-star-song/

 //85-110

//Sensor setup
int proximityTrigger = A0;
int proximityEcho = 9;
int maxDistance = 1000;
int distance;

int proximitySampleRate = 500;
long lastReadingProx;

NewPing proximity1(proximityTrigger, proximityEcho, maxDistance);

bool speakerInProgress = false;
long capTimeStart;
int songLength = 103000; //1m and 43s

void setup() {
  
  Serial.begin(9600);
  
}

void loop() {

  long stopTimer = millis();

  //Proximity sensing
  if(stopTimer-lastReadingProx>=proximitySampleRate){
    
    distance = proximity1.ping_cm();

    //Debugging statements
/*    Serial.print("proximity: "); 
    Serial.println(distance);*/

    
   if(distance >= 85 && distance <= 110){
      if(!speakerInProgress){
        //Send command to GoBetwino to start playing song
        Serial.println("#S|PLAYSONG|[]#");
        speakerInProgress = true;
        capTimeStart = millis();
      }else {
        //debugging printouts
/*        Serial.print(millis());
        Serial.print("ms - ");
        Serial.print(capTimeStart);
        Serial.print("ms = ");
        Serial.print(millis()-capTimeStart);
        Serial.print("ms   ??     ");
        Serial.println(songLength);*/
        if(millis()-capTimeStart>=songLength){
          speakerInProgress = false;
        }
      }
    }
  
    //Recap milliseconds for poll frequency
    lastReadingProx = millis(); 

  }
  
 
}
