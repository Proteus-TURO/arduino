void setup() {
  Serial.begin(9600);
  ledcSetup(0, 5000 ,8);
  ledcAttachPin(2, 0);
}

void led(int brightness) {
  ledcWrite(0, brightness);
}

void loop() {
  if (Serial.available() > 0) {
    int cmd = Serial.read();
    if (cmd == 0) {
      while (Serial.available() == 0) {
        usleep(100);
      }
      int brightness = Serial.read();
      Serial.println(brightness);
      led(brightness);
    }
  }
}
