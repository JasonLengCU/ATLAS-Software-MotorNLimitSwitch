#include <SPI.h>
#include <AMIS30543.h>

const uint8_t amisDirPin = 2;
const uint8_t amisStepPin = 3;
const uint8_t amisSlaveSelect = 4;
const uint8_t LEDpin = 13;
const uint8_t buttonPin = 9;
int stepDelay = 10;
char receivedChar;
char lastChar;
boolean newData = false;
boolean buttonState;

AMIS30543 stepper;

void setup()
{
  SPI.begin();
  Serial.begin(9600);
  stepper.init(amisSlaveSelect);
  
  Serial.write("Startup...\n");
  // Drive the NXT/STEP and DIR pins low initially.
  digitalWrite(amisStepPin, LOW);
  pinMode(amisStepPin, OUTPUT);
  digitalWrite(amisDirPin, LOW);
  digitalWrite(LEDpin, LOW);
  pinMode(amisDirPin, OUTPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(buttonPin, INPUT);

  // Give the driver some time to power up.
  digitalWrite(LEDpin,HIGH);
  delay(1000);
  digitalWrite(LEDpin,LOW);
  
  // Reset the driver to its default settings.
  stepper.resetSettings();
  Serial.write("Reset Complete\n");
  
  // Set the current limit.  You should change the number here to
  // an appropriate value for your particular system.
  stepper.setCurrentMilliamps(1500);
  Serial.write("Current Limit Set\n");
  
  // Set the number of microsteps that correspond to one full step.
  stepper.setStepMode(64);
  Serial.write("Step Mode Set\n");

  //apply settings
  //stepper.applySettings();
  //Serial.write("Settings Applied\n");
  
  // Enable the motor outputs.
  stepper.enableDriver();
  Serial.write("Driver Enabled\n");
  
  // Verify settings and driver connection
  bool verify = stepper.verifySettings();
  Serial.write("...Attempting Settings Verification\n");
  if(verify){
     Serial.write("......Settings set correctly, driver responding!\n");
  }else{
     Serial.write("......Settings not verified!\n");
  }
  Serial.write("Done!");
}

void loop()
{
  recvOneChar();
  showNewData();
  lastChar = receivedChar;
  buttonState = digitalRead(buttonPin);
  switch(lastChar){
    case 'f':
      stepDelay = 500;
      break;
    case 'c':
      stepDelay = 100;
      break;
    case 'w':
        setDirection(1);
        step();
      break;
    case 's':
      setDirection(0);
      step();
      break;
  }
}

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void step()
{
  // The NXT/STEP minimum high pulse width is 2 microseconds.
  digitalWrite(amisStepPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(amisStepPin, LOW);
  delayMicroseconds(3);

  // The delay here controls the stepper motor's speed.  You can
  // increase the delay to make the stepper motor go slower.  If
  // you decrease the delay, the stepper motor will go fast, but
  // there is a limit to how fast it can go before it starts
  // missing steps.
  delayMicroseconds(stepDelay);
}

// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void setDirection(bool dir)
{
  // The NXT/STEP pin must not change for at least 0.5
  // microseconds before and after changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(amisDirPin, dir);
  delayMicroseconds(1);
}

void recvOneChar() {
 if (Serial.available() > 0) {
  receivedChar = Serial.read();
  newData = true;
 }
}

void showNewData() {
 if (newData == true) {
 //Serial.print("");
 Serial.write(receivedChar);
 newData = false;
 }
}
