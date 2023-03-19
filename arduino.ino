const int LED_PIN = 13;
const int MOTOR_V_L[] = {6/*EN1*/,4/*IN1*/,2/*IN2*/};     /*Motor vorne links D6(PWM)->EN1,D1->IN1,D0->IN2*/
const int MOTOR_V_R[] = {5/*EN2*/,A0/*IN3*/,10/*IN4*/};     /*Motor vorne rechts D5(PWM)->EN2,A0->IN3,A1->IN4*/
const int MOTOR_H_L[] = {3/*EN2*/,8/*IN3*/,7/*IN4*/};    /*Motor hinten links D3(PWM)->EN2,D8->IN3,D7->IN4 */
const int MOTOR_H_R[] = {11/*EN1*/,9/*IN1*/,12/*IN2*/};  /*Motor hinten rechts D11(PWM)->EN1,D13->IN1,D12->IN2*/
const int WHEEL_SEPARATION_WIDTH = 1;  /*Distance between the two wheels on the same axis (meters)*/
const int WHEEL_SEPARATION_LENGTH = 1; /*Distance between the front and rear axis (meters)*/
const int maxSpeed = 10000;

void setup() {
  Serial.begin(9600);
  pwmMode(LED_PIN, PWM_MODE_NORMAL, PWM_FREQ_FAST, 0);
  pwmResolution(LED_PIN, 8);
  pinMode(MOTOR_V_L[0],OUTPUT);
  pinMode(MOTOR_V_L[1],OUTPUT);
  pinMode(MOTOR_V_L[2],OUTPUT);
  pinMode(MOTOR_V_R[0],OUTPUT);
  pinMode(MOTOR_V_R[1],OUTPUT);
  pinMode(MOTOR_V_R[2],OUTPUT);
  pinMode(MOTOR_H_L[0],OUTPUT);
  pinMode(MOTOR_H_L[1],OUTPUT);
  pinMode(MOTOR_H_L[2],OUTPUT);
  pinMode(MOTOR_H_R[0],OUTPUT);
  pinMode(MOTOR_H_R[1],OUTPUT);
  pinMode(MOTOR_H_R[2],OUTPUT);

  for (int i = 2; i <= 13; i++) {
    digitalWrite(i, LOW);
  }
  digitalWrite(A0, LOW);
}

void led(int brightness) {
  Serial.println(brightness);
  if (brightness == 0) {
    pwmTurnOff(LED_PIN);
    digitalWrite(LED_PIN, LOW);
  } else {
    pwmMode(LED_PIN, PWM_MODE_NORMAL, PWM_FREQ_FAST, 0);
    pwmResolution(LED_PIN, 8);
    pwmWrite(LED_PIN, brightness);
  }
}

void brake(const int motor[]) {
    digitalWrite(motor[0],HIGH);
    digitalWrite(motor[1],LOW);
    digitalWrite(motor[2],LOW); 
}

void set_duty_cycle(const int motor[],int duty_cycle) {
  if (duty_cycle == 0) {
    brake(motor);
  } else if (duty_cycle < 0) {
    duty_cycle = abs(duty_cycle);
    analogWrite(motor[0],duty_cycle);
    digitalWrite(motor[1],LOW);
    digitalWrite(motor[2],HIGH);
  } else {
    analogWrite(motor[0],duty_cycle);
    digitalWrite(motor[1],HIGH);
    digitalWrite(motor[2],LOW);
  }
}

void set_all_duty_cycle(int duty_cycle) {
  set_duty_cycle(MOTOR_V_L,duty_cycle);
  set_duty_cycle(MOTOR_V_R,duty_cycle);
  set_duty_cycle(MOTOR_H_L,duty_cycle);
  set_duty_cycle(MOTOR_H_R,duty_cycle);
}

void set_v_duty_cycle(int duty_cycle){
  set_duty_cycle(MOTOR_V_L,duty_cycle);
  set_duty_cycle(MOTOR_V_R,duty_cycle);
}

void set_h_duty_cycle(int duty_cycle) {
  set_duty_cycle(MOTOR_H_L,duty_cycle);
  set_duty_cycle(MOTOR_H_R,duty_cycle);
}

void drive (float linear_x, float linear_y, float angular_z) {
  int frontLeft = 255 * (linear_x - linear_y - angular_z);
  int frontRight = 255 * (linear_x + linear_y + angular_z);
  int rearLeft = 255 * (linear_x + linear_y - angular_z);
  int rearRight = 255 * (linear_x - linear_y + angular_z);

  set_duty_cycle(MOTOR_V_L,frontLeft);
  set_duty_cycle(MOTOR_V_R,frontRight);
  set_duty_cycle(MOTOR_H_L,rearLeft);
  set_duty_cycle(MOTOR_H_R,rearRight);
  
  Serial.print(frontLeft);
  Serial.print(" ");
  Serial.print(frontRight);
  Serial.print(" ");
  Serial.print(rearLeft);
  Serial.print(" ");
  Serial.println(rearRight);
  
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
