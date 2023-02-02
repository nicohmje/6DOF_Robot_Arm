#include <AccelStepper.h>

const int enable1 = 4;
const int step1 = 3;
const int dir1 = 2; 

const int enable2 = 5;
const int step2 = 6;
const int dir2 = 7; 

const int enable3 = 8;
const int step3 = 9;
const int dir3 = 10; 

AccelStepper stepper1(AccelStepper::DRIVER, step1, dir1);
AccelStepper stepper2(AccelStepper::DRIVER, step2, dir2);
AccelStepper stepper3(AccelStepper::DRIVER, step3, dir3);


void setup() {
    Serial.begin(115200);


    //shoulder
    stepper1.setEnablePin(enable1);
    stepper1.setPinsInverted(false, false, true);
    stepper1.enableOutputs();
    stepper1.setAcceleration(150);
    stepper1.setMaxSpeed(700);

    //elbow
    stepper2.setEnablePin(enable2);
    stepper2.setPinsInverted(true, false, true);
    stepper2.enableOutputs();
    stepper2.setAcceleration(550);
    stepper2.setMaxSpeed(600);


    //base
    stepper3.setEnablePin(enable3);
    stepper3.setPinsInverted(true, false, true);
    stepper3.enableOutputs();
    stepper3.setAcceleration(100);
    stepper3.setMaxSpeed(2000);
    Serial.println("startup in 5 seconds");
    delay(5000); 
}

int num = 2; 

void loop() {
      stepper1.run();
      stepper3.run();
      stepper2.run();
      if (stepper1.distanceToGo() == 0) {
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
      stepper1.moveTo(1500);
      stepper2.moveTo(1000);
      stepper3.moveTo(2000);
  }
  if (number == 2) {
    stepper2.moveTo(500);
    stepper1.moveTo(0);
    stepper3.moveTo(0);
  }
}
