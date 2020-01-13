const int potPin = A0;
const int masterSwitchPin = 5;
const int dirSwitchPin1 = 4;
const int HBonPin = 9;
const int dir2Pin = 3;
const int dir1Pin = 2;

//tfghi

int masterSwitch = 0;
int dirSwitch = 0;

int motorOn = 0;
int motorSpeed1 = 0;
int motorDir1 = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(dir1Pin,OUTPUT);
  pinMode(dir2Pin,OUTPUT);
  pinMode(HBonPin,OUTPUT);
  pinMode(masterSwitchPin, INPUT);
  pinMode(dirSwitchPin1, INPUT);

  digitalWrite(HBonPin,LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  masterSwitch = digitalRead(masterSwitchPin);
  delay(1);
  dirSwitch = digitalRead(dirSwitchPin1);
  motorSpeed1 = map(analogRead(potPin),0,1023,0,255);

  if (masterSwitch == HIGH){
    motorOn = !motorOn;
  }
  while (digitalRead(masterSwitchPin) == HIGH){
    delay(20);
  }

  if (dirSwitch == HIGH){
    motorDir1 = !motorDir1;
  }
  while (digitalRead(dirSwitchPin1) == HIGH){
    delay(20);
  }

  if (motorDir1 == 1){
    digitalWrite(dir1Pin, HIGH);
    digitalWrite(dir2Pin, LOW);
  }else {
    digitalWrite(dir1Pin, LOW);
    digitalWrite(dir2Pin, HIGH);
  }

  if (motorOn == 1){
    analogWrite(HBonPin,motorSpeed1);
    delay(20);
  }else {
    analogWrite(HBonPin,0);
    delay(20);
  }
}
