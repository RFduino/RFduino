void setup() {
  Serial.begin(57600);

  Serial.println("The device id is:");
  uint64_t id = getDeviceId();
  Serial.println(getDeviceIdLow(), HEX);
  Serial.println(getDeviceIdHigh(), HEX);
}

void loop() {
}
