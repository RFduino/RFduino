int myPinCallback(uint32_t ulPin)
{
  digitalWrite(4, HIGH);
  delay(250);
  digitalWrite(4, LOW);
  return 0;  // don't exit RFduino_ULPDelay
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  // processing handled by exiting RFduino_ULPDelay
  pinMode(5, INPUT);
  RFduino_pinWake(5, HIGH);
  
  // processing handled by myPinCallback
  pinMode(6, INPUT);
  RFduino_pinWakeCallback(6, HIGH, myPinCallback);
}

void loop() {
  RFduino_ULPDelay(SECONDS(1));
  
  if (RFduino_pinWoke(5))
  {
    digitalWrite(3, HIGH);
    delay(250);
    digitalWrite(3, LOW);
    
    RFduino_resetPinWake(5);
  }
  else
  {
    Serial.println("timed out");
    digitalWrite(2, HIGH);
    delay(250);
    digitalWrite(2, LOW);
  }
}
