#include <Wire.h>


bool isFlashing = false;
const int a = 4;
const int b = 5; 
const int c = 6;
const int input = A1;
const int led = 3; 
double temps[8];

// TODO: Set a cutoff voltage under which the resistor is hotter than 60 degrees celsius.
const int CUTOFF = 0;
int reg = 0;

// for testing purposes. these all need to be chekced
int millivoltDropPerDegreeC=-6;
double degreeCPerStep = 0.6667;
double restingVoltage = 533.9;


void setup() {
  // digital selection pins for the multiplexer

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void receiveEvent(int numBytes){
  if (numBytes < 1) {return;}
  reg = Wire.read();
}

void requestEvent(){
  double value = temps[reg];
  Wire.write((const uint8_t*)&value, sizeof(double));
  reg = (reg + 1);
}

void loop() {
  // track the minimum voltage (highest temperature)
  double maxTemp = -1024;
  for (int currentSelection = 0; currentSelection < 8; ++currentSelection) {

    digitalWrite(a, bitRead(currentSelection, 0));
    digitalWrite(b, bitRead(currentSelection, 1));
    digitalWrite(c, bitRead(currentSelection, 2));

    // Serial.println("current selection: ");
    // Serial.println(currentSelection);
    // Serial.println("in binary: ");
    // Serial.print(bitRead(currentSelection, 2));
    // Serial.print(bitRead(currentSelection, 1));
    // Serial.print(bitRead(currentSelection, 0));
    // Serial.print("\n");
    delay(30);

    // int from 0-1023
    int sumVoltage = 0;
    const int NUM_SAMPLES = 10;
    
    for (int j = 0; j < NUM_SAMPLES; j++){
      int voltage = analogRead(input);
      // Serial.println("voltage of selection: ");
      // Serial.println(voltage);
      sumVoltage += voltage;
      delay(1);
    }
    
    double average = sumVoltage / (float)NUM_SAMPLES;
    

    double temperature = 22+(degreeCPerStep*(average - restingVoltage));
    temps[currentSelection] = temperature;
    if (temperature > maxTemp){
      maxTemp = temperature;
    }
    // Serial.println(temps[currentSelection]);
  }
  
  Serial.println(maxTemp);
  if (maxTemp > 60){
    isFlashing = true;
  } else {
    isFlashing = false;
  }

  if (isFlashing) {
    digitalWrite(led, HIGH);
    delay(100);
    Serial.println("Turn on");
    digitalWrite(led, LOW);
    delay(100);
    Serial.println("Turn off");
  }
  
}
