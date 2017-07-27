#define photodiode  P6_0    // select the input pin for the photodiode
#define Switch1  P2_1 // Define the left switch on the Launchpad
#define Switch2  P1_1 // Define the right switch on the Launchpad
#define pot_inc P1_6
#define pot_ud P6_6

String inputString = ""; // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete

int sensorValue = 0;
int buttonState = LOW;
float trans = 0;
float hugeNumber = 0;
float average = 0;
float absorbance = 0;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(photodiode, INPUT); 
  
  pinMode(Switch1, INPUT_PULLUP); 
  pinMode(Switch2, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Begin");
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(pot_ud, OUTPUT);
  pinMode(pot_inc, OUTPUT);
  digitalWrite(pot_ud, LOW);
  digitalWrite(pot_inc, HIGH);
}

void loop() {
  buttonState = digitalRead(Switch1);
  if(buttonState == LOW)
  {
    for(float i = 0; i < 1; i++){
      // read the value from the sensor:
      sensorValue = analogRead(photodiode); // Read a value from the photodiode.   
      Serial.print("Photodiode ADC count: ");
      Serial.println(sensorValue);
      hugeNumber += sensorValue;
      delay(1);
    } 
    average = hugeNumber/100;
    trans = (average)/sensorValue;
    Serial.print("Transmittance: ");
    Serial.println(trans);
    absorbance = -logf(trans)/logf(10);
    Serial.print("Absorbance ");
    Serial.println(absorbance);
    hugeNumber = 0;
  }
  
  buttonState = digitalRead(Switch2);
  if(buttonState == LOW){
      Serial.println("Calibrating");
      calibration();
      Serial.println("Calibration finished");
  }
  
  
  if (stringComplete) {
    if(inputString == "inc") {
      Serial.println("Incrementing wiper position...");
      incPot();
      Serial.println("Wiper position incremented.");
    }    // clear the string:
    if(inputString == "dec") {
      Serial.println("Decrementing wiper position...");
      decPot();
      Serial.println("Wiper position decremented.");
    }    // clear the string:
    if(inputString == "max") {
      Serial.println("Maximizing wiper position...");
      maxPot();
      Serial.println("Wiper position maximized.");
    }    // clear the string:
    if(inputString == "min") {
      Serial.println("Minimizing wiper position...");
      minPot();
      Serial.println("Wiper position minimized.");
    }    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
    stringComplete = true;
    }
    else {
      // add it to the inputString:
      inputString += inChar;
    }
  }
}

void decPot() {
  digitalWrite(pot_ud, HIGH);
  delay(1);
  digitalWrite(pot_inc, LOW);     
  delay(1);
  digitalWrite(pot_inc, HIGH);
}

void incPot() {
  digitalWrite(pot_ud, LOW);
  delay(1);
  digitalWrite(pot_inc, LOW);
  delay(1);
  digitalWrite(pot_inc, HIGH);
}

void maxPot(){
  for(int i=0; i<100; i++){
    incPot();
  }
}

void minPot(){
  for(int i=0; i<100; i++){
    decPot();
  }
}

void transmittance(){
  trans = sensorValue - 4096;
}

void calibration(){
  for(int i=0; i<100; i++)
  {
    if(analogRead(photodiode)<3000)
    {
    incPot();
    }
    else
    {
    break;
    }
  }
}
