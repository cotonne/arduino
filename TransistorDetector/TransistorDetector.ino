/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */
static int BUTTON_PIN=8;
static int TRANS_PIN1=2;
static int TRANS_PIN2=3;
static int TRANS_PIN3=4;

boolean  highStates[3];
boolean  lowStates[3];

int nbHighState = 0;
int nbLowState = 0;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // Set the pin for the button
  pinMode(BUTTON_PIN, INPUT);
  // Set the pins which controls the transistor
  pinMode(TRANS_PIN1, OUTPUT);
  pinMode(TRANS_PIN2, OUTPUT);
  pinMode(TRANS_PIN3, OUTPUT);
  // Disable everythin
  setState(LOW, LOW, LOW); 
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println("Waiting for button");
  // Start on HIGH
  while(!digitalRead(BUTTON_PIN)){
   // Empty 
  }
  
  Serial.println("Starting process");
  setState(HIGH, HIGH, LOW); 
  highStates[0] = readAndDisplaySensors();
  setState(HIGH, LOW, HIGH); 
  highStates[1] = readAndDisplaySensors();
  setState(LOW, HIGH, HIGH); 
  highStates[2] = readAndDisplaySensors();
  
  setState(HIGH, LOW, LOW); 
  lowStates[0] = readAndDisplaySensors();
  setState(LOW, HIGH, LOW); 
  lowStates[1] = readAndDisplaySensors();
  setState(LOW, LOW, HIGH); 
  lowStates[2] = readAndDisplaySensors();
  
  setState(LOW, LOW, LOW); 
  
  for(int i = 0; i < 3; i++){
    nbHighState += highStates[i]?1:0;
  }

  for(int i = 0; i < 3; i++){
    nbLowState += lowStates[i]?1:0;
  }
  
  if( (nbHighState == 2) && (nbLowState == 1 ) ){   // Is it an NPN?
    Serial.println("It is a NPN transistor");
  } else if( (nbHighState == 1) && (nbLowState == 2 ) ){  // Is it an PNP?
    Serial.println("It is a PNP transistor");
  } else {
    Serial.println("Transistor does not work!");  
  }
}

void setState(int statePin1, int statePin2, int statePin3){
  digitalWrite(TRANS_PIN1, statePin1); 
  digitalWrite(TRANS_PIN2, statePin2); 
  digitalWrite(TRANS_PIN3, statePin3);
  delay(1000); 
}

boolean readAndDisplaySensors(){
    // read the input on analog pins
  int sensorValuePin3 = analogRead(A0);
  int sensorValuePin2 = analogRead(A1);
  int sensorValuePin1 = analogRead(A2);
  // print out the value you read:
  Serial.print("PIN 1 = ");  
  Serial.println(sensorValuePin1);
  Serial.print("PIN 2 = ");  
  Serial.println(sensorValuePin2);
  Serial.print("PIN 3 = ");  
  Serial.println(sensorValuePin3);
  
  return ( (sensorValuePin1 == 0) || (sensorValuePin1 == 1023) ) &&
     ( (sensorValuePin2 == 0) || (sensorValuePin2 == 1023) ) &&
     ( (sensorValuePin3 == 0) || (sensorValuePin3 == 1023) );
}
