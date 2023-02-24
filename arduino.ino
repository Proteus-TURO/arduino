const int LED_PIN = 13;
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void led(int brightness) {
  Serial.println(brightness);
  analogWrite(LED_PIN, brightness);
}

void drive(float linear_x, float linear_y, float angular_z) {
  Serial.println(linear_x);
  Serial.println(linear_y);
  Serial.println(angular_z);
}

void loop() {
  if (Serial.available() > 0) {
    int cmd = Serial.read();
    Serial.println(cmd);
    if (cmd == 0) {
      while (Serial.available() == 0) {
        delay(1);
      }
      int brightness = Serial.read();
      led(brightness);
    } else if (cmd == 1) {
      while (Serial.available() < 12) {
        delay(1);
      }
      char buffer[12];
      Serial.readBytes(buffer, 12);
      float x = ((float *) buffer)[0];
      float y = ((float *) buffer)[1];
      float z = ((float *) buffer)[2];
      drive(x, y, z);
    }
  }
}
