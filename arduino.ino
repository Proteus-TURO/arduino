const int LED_PIN = 13;
const int MOTOR_V_L[] = {6/*EN1*/,4/*IN1*/,2/*IN2*/};     /*Motor vorne links D6(PWM)->EN1,D1->IN1,D0->IN2*/
const int MOTOR_V_R[] = {5/*EN2*/,0/*IN3*/,1/*IN4*/};     /*Motor vorne rechts D5(PWM)->EN2,D4->IN3,D2->IN4*/
const int MOTOR_H_L[] = {10/*EN2*/,8/*IN3*/,7/*IN4*/};    /*Motor hinten links D10(PWM)->EN2,D8->IN3,D7->IN4 */
const int MOTOR_H_R[] = {11/*EN1*/,13/*IN1*/,12/*IN2*/};  /*Motor hinten rechts D11(PWM)->EN1,D13->IN1,D12->IN2*/
const int WHEEL_SEPARATION_WIDTH = 1  /*Distance between the two wheels on the same axis (meters)*/;
const int WHEEL_SEPARATION_LENGTH = 1 /*Distance between the front and rear axis (meters)*/;
const int ticksPerMeter = 1;          /*Number of encoder ticks per meter of travel*/

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_V_L[0],OUTPUT);
  pinMode(MOTOR_V_L[1],OUTPUT);
  pinMode(MOTOR_V_L[2],OUTPUT);
  pinMode(MOTOR_V_L[0],OUTPUT);
  pinMode(MOTOR_V_R[1],OUTPUT);
  pinMode(MOTOR_V_R[2],OUTPUT);
  pinMode(MOTOR_V_L[0],OUTPUT);
  pinMode(MOTOR_H_L[1],OUTPUT);
  pinMode(MOTOR_H_L[2],OUTPUT);
  pinMode(MOTOR_V_L[0],OUTPUT);
  pinMode(MOTOR_H_R[1],OUTPUT);
  pinMode(MOTOR_H_R[2],OUTPUT);
}

void led(int brightness) {
  Serial.println(brightness);
  analogWrite(LED_PIN, brightness);
}

void brake(const int motor[]) {

    digitalWrite(motor[0],HIGH);
    digitalWrite(motor[1],LOW);
    digitalWrite(motor[2],LOW); 
  
}

void set_duty_cycle(const int motor[],int duty_cycle /*in Prozent*/) {

  /*range: -100% bis 100%*/
  
  if (duty_cycle < 0) {
    duty_cycle = abs(duty_cycle);
    if (duty_cycle > 100) {
    analogWrite(motor[0],255);
    digitalWrite(motor[1],LOW);
    digitalWrite(motor[2],HIGH); 
    }else {
    analogWrite(motor[0],(duty_cycle*2.55));
    digitalWrite(motor[1],LOW);
    digitalWrite(motor[2],HIGH);
    }
  }else if (duty_cycle > 100) {
    analogWrite(motor[0],255);
    digitalWrite(motor[1],HIGH);
    digitalWrite(motor[2],LOW);
  }else if (duty_cycle == 0) {
    analogWrite(motor[0],0);
    digitalWrite(motor[1],LOW);
    digitalWrite(motor[2],LOW);
  }else {
    analogWrite(motor[0],(duty_cycle*2.55));
    digitalWrite(motor[1],HIGH);
    digitalWrite(motor[2],LOW);
  }
    
}

void set_all_duty_cycle(int duty_cycle/*in Prozent*/) {
  set_duty_cycle(MOTOR_V_L,duty_cycle);
  set_duty_cycle(MOTOR_V_R,duty_cycle);
  set_duty_cycle(MOTOR_H_L,duty_cycle);
  set_duty_cycle(MOTOR_H_R,duty_cycle);
}

void set_v_duty_cycle(int duty_cycle/*in Prozent*/){
  set_duty_cycle(MOTOR_V_L,duty_cycle);
  set_duty_cycle(MOTOR_V_R,duty_cycle);
}

void set_h_duty_cycle(int duty_cycle/*in Prozent*/) {
  set_duty_cycle(MOTOR_H_L,duty_cycle);
  set_duty_cycle(MOTOR_H_R,duty_cycle);
}

void drive (float linear_x, float linear_y, float angular_z) {

  int frontLeft = ticksPerMeter * (linear_x - linear_y - (WHEEL_SEPARATION_WIDTH + WHEEL_SEPARATION_LENGTH)*angular_z);
  int frontRight = ticksPerMeter * (linear_x + linear_y + (WHEEL_SEPARATION_WIDTH + WHEEL_SEPARATION_LENGTH)*angular_z);
  int rearLeft = ticksPerMeter * (linear_x + linear_y - (WHEEL_SEPARATION_WIDTH + WHEEL_SEPARATION_LENGTH)*angular_z);
  int rearRight = ticksPerMeter * (linear_x - linear_y + (WHEEL_SEPARATION_WIDTH + WHEEL_SEPARATION_LENGTH)*angular_z);

  set_duty_cycle(MOTOR_V_L,frontLeft);
  set_duty_cycle(MOTOR_V_R,frontRight);
  set_duty_cycle(MOTOR_H_L,rearLeft);
  set_duty_cycle(MOTOR_H_R,rearRight);
  
  Serial.println(frontLeft);
  Serial.println(frontRight);
  Serial.println(rearLeft);
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
