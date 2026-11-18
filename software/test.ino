bool isFlashing = false;
const int a = 4;
const int b = 5; 
const int c = 6;
const int input = A1;
const int led = 3; 
double temps[8];

// TODO: Set a cutoff voltage under which the resistor is hotter than 60 degrees celsius.
const int CUTOFF = 0;

// for testing purposes. these all need to be chekced
int millivoltDropPerDegreeC=-6;
double restingVoltage = 593;


void setup() {
  // digital selection pins for the multiplexer

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // track the minimum voltage (highest temperature)
  double maxTemp = -1024;
  for (int currentSelection = 0; currentSelection < 8; ++currentSelection) {
    int isAOn = (int) (currentSelection & 4) == 4;
    int isBOn = (int) (currentSelection & 2) == 2;
    int isCOn = (int) (currentSelection & 1) == 1;

    digitalWrite(a, isAOn);
    digitalWrite(b, isBOn);
    digitalWrite(c, isCOn);
    Serial.println("current selection: ");
    Serial.println(currentSelection);
    delay(30);

    // int from 0-1023
    int sumVoltage = 0;
    
    for (int j = 0; j < 10; j++){
      int voltage = analogRead(input);
      Serial.println("voltage of selection: ");
      Serial.println(voltage);
      sumVoltage += voltage;
      delay(3);
    }
    
    double average = sumVoltage / 15.0;
    temps[currentSelection] = 20-(millivoltDropPerDegreeC*(average - restingVoltage));
    if (average > maxTemp){
      maxTemp = average;
    }
  }
  
  if (maxTemp > 60){
    isFlashing = true;
  } else {
    isFlashing = false;
  }

  if (isFlashing) {
    digitalWrite(led, HIGH);
    delay(100);
    // Serial.println("Turn on");
    digitalWrite(led, LOW);
    delay(100);
    // Serial.println("Turn off");
  }
}
