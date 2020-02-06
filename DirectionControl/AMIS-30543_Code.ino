/* This example shows basic use of the AMIS-30543 stepper motor
driver.
It shows how to initialize the driver, set the current limit, set
the micro-stepping mode, and enable the driver.  It shows how to
send pulses to the NXT/STEP pin to get the driver to take steps
and how to switch directions using the DIR pin.  The DO pin is
not used and does not need to be connected.
Before using this example, be sure to change the
setCurrentMilliamps line to have an appropriate current limit for
your system.  Also, see this library's documentation for
information about how to connect the driver:
    http://pololu.github.io/amis-30543-arduino/
*/

#include <SPI.h>
#include <AMIS30543.h>

const uint8_t amisDirPin = 2;
const uint8_t amisStepPin = 3;
const uint8_t amisSlaveSelect = 4;
const uint8_t LEDpin = 13;
char receivedChar;
char lastChar;
boolean newData = false;

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

  // Give the driver some time to power up.
  digitalWrite(LEDpin,HIGH);
  delay(1000);
  digitalWrite(LEDpin,LOW);
  
  // Reset the driver to its default settings.
  stepper.resetSettings();
  Serial.write("Reset Complete\n");
  
  // Set the current limit.  You should change the number here to
  // an appropriate value for your particular system.
  stepper.setCurrentMilliamps(1000);
  Serial.write("Current Limit Set\n");
  
  // Set the number of microsteps that correspond to one full step.
  stepper.setStepMode(86);
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
  switch(lastChar){
    case 'f':
      setDirection(1);
      step();
      //Serial.println("STEPPING!");
      break;
    case 'd':
      setDirection(0);
      step();
      break;
  }
  //if(receivedChar=='a'){
    //setDirection(0);
    //while(receivedChar=='a'){
      //Serial.println("STEPPING!");
      //step();
      //recvOneChar();
    //}
  //}
  
  // Step in the default direction 1000 times.
  //setDirection(0);
  //for (unsigned int x = 0; x < 1000; x++)
  //{
  //  step();
  //}

  // Wait for 300 ms.
  //delay(300);

  // Step in the other direction 1000 times.
  //setDirection(1);
  //for (unsigned int x = 0; x < 1000; x++)
  //{
  //  step();
  //}

  // Wait for 300 ms.
  //delay(300);
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
  delayMicroseconds(2000);
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
 Serial.print("");
 Serial.println(receivedChar);
 newData = false;
 }
}
