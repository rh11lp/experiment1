#include <NewPing.h>

/* Melody credits:
 * (cleft) 2005 D. Cuartielles for K3
 *
 * where the different tones are described as in the table:
 *
 * note   frequency   period  timeHigh
 * c          261 Hz          3830  1915  
 * d          294 Hz          3400  1700  
 * e          329 Hz          3038  1519  
 * f          349 Hz          2864  1432  
 * g          392 Hz          2550  1275  
 * a          440 Hz          2272  1136  
 * b          493 Hz          2028  1014  
 * C          523 Hz          1912  956
 *
 * http://www.arduino.cc/en/Tutorial/Melody
 * 
 * Adapted for use with custom timer instead of delay();
 */

//Sensor setup
int proximityTrigger = A0;
int proximityEcho = 9;
int maxDistance = 200;
int distance;

int proximitySampleRate = 200;
long lastReadingProx;

int lastPollResult = 0;
int differenceThreshold = 5; //check against this to see if rocking

NewPing proximity1(proximityTrigger, proximityEcho, maxDistance);

//Speaker setup
int speakerPin = 10;
int speakerSampleRate = 1000;
long lastReadingSpeaker;
bool rocking = false;

bool speakerInProgress = false;

int length = 15; // the number of notes

//twinkle twinkle little star
char notes[] = "ccggaag ffeeddc ggffeed ggffeed ccggaag ffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup() {
  
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {

  long stopTimer = millis();

  //Proximity sensing
  if(stopTimer-lastReadingProx>=proximitySampleRate){
    
    distance = proximity1.ping_cm();

    int difference = distance-lastPollResult;

    //Make the difference positive
    if(difference < 0){
      difference *= -1;
    }

    //Debugging statements
    Serial.print("proximity: "); Serial.print(distance);
    Serial.print("     last result:"); Serial.print(lastPollResult);
    Serial.print("     difference:"); Serial.println(difference);


    if(!speakerInProgress){ //if the speaker code is not currently already in process of being executed, proceed.
      //If the difference is less than 5 between polls, assume rocking has stopped and make the baby cry after a short delay
      if(difference < differenceThreshold){
        
        speakerInProgress = true;
        
        rocking = false;
        lastReadingSpeaker = millis();
  
      }else{ //Else the crib is rocking and the baby quiets after a short delay (TODO: Make lullaby play as well)
        
        speakerInProgress = true;
        
        rocking = true;
        lastReadingSpeaker = millis();
        
      }
    }

    //Recap milliseconds for poll frequency
    lastReadingProx = millis();
    //Recap current distance to test difference
    lastPollResult = distance;     
  }
  
  //Allow a small amount of time to pass before we change states
  if(millis()-lastReadingSpeaker>=speakerSampleRate){
    if(rocking){
      noTone(speakerPin);
      speakerInProgress = false;
      
      lastReadingSpeaker = millis();
      
    }else{
      tone(speakerPin, 500);
      speakerInProgress = false;
      
      lastReadingSpeaker = millis();
      
    }
  } 
}
