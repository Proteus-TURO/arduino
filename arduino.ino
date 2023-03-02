const int LED_PIN = 13;
const int MOTOR_V_L = 6; const int IN1_V = 4; const int IN2_V = 2;/*Motor vorne links D5(PWM)->EN2,D1->IN3,D0->IN4*/
const int MOTOR_V_R = 5; const int IN3_V = 1; const int IN4_V = 0;/*Motor vorne rechts   D6(PWM)->EN1,D4->IN1,D2->IN2*/
const int MOTOR_H_L = 11; const int IN1_H = 13; const int IN2_H = 12;/*Motor hinten links D10(PWM)->EN2,D8->IN3,D7->IN4 */
const int MOTOR_H_R = 10; const int IN3_H = 8; const int IN4_H = 7; /*Motor hinten rechts D11(PWM)->EN1,D13->IN1,D12->IN2*/

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(IN1_V,OUTPUT);
  pinMode(IN2_V,OUTPUT);
  pinMode(IN3_V,OUTPUT);
  pinMode(IN4_V,OUTPUT);
  pinMode(IN1_H,OUTPUT);
  pinMode(IN2_H,OUTPUT);
  pinMode(IN3_H,OUTPUT);
  pinMode(IN4_H,OUTPUT);
}

void led(int brightness) {
  Serial.println(brightness);
  analogWrite(LED_PIN, brightness);
}

void set_duty_cycle(int motor,int duty_cycle /*in Prozent*/) {

  int IN1;
  int IN2;

  switch(motor) {

    case MOTOR_V_L:

        IN1 = IN1_V;
        IN2 = IN2_V;
        break;

    case MOTOR_V_R:

        IN1 = IN3_V;
        IN2 = IN4_V;
        break;

    case MOTOR_H_L:

        IN1 = IN1_H;
        IN2 = IN2_H;
        break;

    case MOTOR_H_R:

        IN1 = IN3_H;
        IN2 = IN4_H;
        break;
    
  }

  if (duty_cycle < 0) {
    duty_cycle = abs(buty_cycle);
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    analogWrite(motor,255);
  }else if (duty_cycle > 100) {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    analogWrite(motor,255);
  }else if (duty_cycle == 0) {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    analogWrite(motor,duty_cycle);
  }else {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    analogWrite(motor,(duty_cycle*2,55));
  }
  
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
