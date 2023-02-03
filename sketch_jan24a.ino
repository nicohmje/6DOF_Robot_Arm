#include <AccelStepper.h>
#include <ezButton.h> 

const int enable1 = 4;
const int step1 = 3;
const int dir1 = 2; 

const int enable2 = 5;
const int step2 = 6;
const int dir2 = 7; 

const int enable3 = 8;
const int step3 = 9;
const int dir3 = 10; 

int Homed_offset_SHOULDER;
int Homed_offset_ELBOW;

ezButton Elbow_limit(12);
ezButton Shoulder_limit(13);

AccelStepper shoulder(AccelStepper::DRIVER, step1, dir1);
AccelStepper elbow(AccelStepper::DRIVER, step2, dir2);
AccelStepper base(AccelStepper::DRIVER, step3, dir3);


void setup() {
    Serial.begin(115200);

    Elbow_limit.setDebounceTime(50);
    Shoulder_limit.setDebounceTime(50);// set debounce time to 50 milliseconds

    //shoulder
    shoulder.setEnablePin(enable1);
    shoulder.setPinsInverted(false, false, true);
    shoulder.enableOutputs();
    shoulder.setAcceleration(250);
    shoulder.setMaxSpeed(900);

    //elbow
    elbow.setEnablePin(enable2);
    elbow.setPinsInverted(false, false, true);
    elbow.enableOutputs();
    elbow.setAcceleration(300);
    elbow.setMaxSpeed(900);

    //base
    base.setEnablePin(enable3);
    base.setPinsInverted(true, false, true);
    base.enableOutputs();
    base.setAcceleration(800);
    base.setMaxSpeed(1400);



    
    Serial.println("Homing shoulder in 5 seconds");
    delay(5000); 


     
    Serial.println(Shoulder_limit.getState());
    shoulder.setMaxSpeed(350);
    shoulder.setCurrentPosition(15000); 
    shoulder.moveTo(0);
    while(Shoulder_limit.getState() == HIGH) {
      Shoulder_limit.loop();
      shoulder.run();
    }
    Serial.println("Shoulder homed 1");
    Homed_offset_SHOULDER = shoulder.currentPosition();
    shoulder.setCurrentPosition(0); 
    shoulder.moveTo(600);
    while(shoulder.distanceToGo() > 0) {
        shoulder.run();
    } 
    shoulder.setCurrentPosition(0);

     
    shoulder.setMaxSpeed(900);
    Serial.println("Shoulder homed 2");
    Serial.println(Homed_offset_SHOULDER); 


    

    Serial.println("Homing elbow in 2 seconds");
    delay(2000); 

     
    Serial.println(Elbow_limit.getState());
    elbow.setMaxSpeed(450);
    elbow.setCurrentPosition(15000); 
    elbow.moveTo(0);
    while(Elbow_limit.getState() == HIGH) {
      Elbow_limit.loop();
      elbow.run();
    }
    Serial.println("Elbow homed 1");
    Homed_offset_ELBOW = shoulder.currentPosition();
    elbow.setCurrentPosition(0); 
    elbow.moveTo(600);
    while(elbow.distanceToGo() > 0) {
        elbow.run();
    } 
    elbow.setCurrentPosition(0); 
    elbow.setMaxSpeed(900);
    Serial.println("ELBOW homed 2");

    
    Serial.println(Homed_offset_ELBOW); 
    
    delay(2000);
    Serial.println("Commencing movement");
    checkpoint(1);
}

int num = 2; 

void loop() {
      
      shoulder.run();
      base.run();
      elbow.run();
      if (shoulder.distanceToGo() == 0 && elbow.distanceToGo() == 0 && base.distanceToGo() == 0) {
          if (num == 1) {
            num = 2;
          }
          else {
            num = 1;
            
          }
          checkpoint(num); 
      }
}



int checkpoint(int number) {
  Serial.println("CHECKPOINTCHANGE");
  Serial.println(number); 
  if (number == 1) {
      shoulder.moveTo(3300);
      elbow.moveTo(1100);
      base.moveTo(0);
  }
  if (number == 2) {
    elbow.moveTo(0);
    shoulder.moveTo(0);
    base.moveTo(0);
  }
}
