const int PHOTO_PIN = A2;

const int btn_A_PIN = 2;
const int btn_B_PIN = 4;

int btn_A_value = 0;
int btn_B_value = 0;

int previous_btn_A_value = 0;
int previous_btn_B_value = 0;

int myLEDs[2] = {6, 8};

int actuatorValue = 0;
int previousValue = 0;

void setup() {
  pinMode(btn_A_PIN, INPUT);
  pinMode(btn_B_PIN, INPUT);
  
  for(int i = 0; i < 2; i++){
  pinMode(myLEDs[i], OUTPUT);
  }  
  Serial.begin(9600); 
}

void loop() {
   if(Serial.available() > 0) {
    receivingData();
  } else {
    sendingData();
  }
  delay(10);
}

void sendingData() {
  actuatorValue = analogRead(PHOTO_PIN);
    
  if (previousValue != actuatorValue) {
    sendSerialMessage(actuatorValue, btn_A_value, btn_B_value);
    previousValue = actuatorValue;
  }

  if (previous_btn_A_value != btn_A_value) {
    sendSerialMessage(actuatorValue, btn_A_value, btn_B_value);
    previous_btn_A_value = btn_A_value;
  }

    if (previous_btn_B_value != btn_B_value) {
    sendSerialMessage(actuatorValue, btn_A_value, btn_B_value);
    previous_btn_B_value = btn_B_value;
  }
  delay(100);
}

void sendSerialMessage(int actuatorValue, int btn_A_value, int btn_B_value) {
  Serial.print(actuatorValue);
  Serial.print(' ');
  Serial.print(btn_A_value);
  Serial.print(' ');
  Serial.print(btn_B_value);
  Serial.print(' ');
  Serial.println();
  
}

void receivingData() {
  char inByte = Serial.read();

  switch(inByte){
    case 'S':
      digitalWrite(myLEDs[1], HIGH);
      digitalWrite(myLEDs[2], HIGH);
      delay(500);
      digitalWrite(myLEDs[1], LOW);
      digitalWrite(myLEDs[2], LOW);
      break;
    case 'L':
      digitalWrite(myLEDs[1], HIGH);
      digitalWrite(myLEDs[2], HIGH);
      break;
  }
  Serial.flush();
}
