#include <Arduino.h>

class Robot {
  private:
    int in1, in2, in3, in4, en1, en2;
  public:
    Robot(int p1, int p2, int p3, int p4, int e1, int e2) {
      in1 = p1;
      in2 = p2;
      in3 = p3;
      in4 = p4;
      en1 = e1;
      en2 = e2;
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(in3, OUTPUT);
      pinMode(in4, OUTPUT);
      pinMode(en1, OUTPUT);
      pinMode(en2, OUTPUT);
    }
    ~Robot(){}
    void tr() {
      digitalWrite(in1, 1);
      digitalWrite(in2, 0);
      digitalWrite(in3, 1);
      digitalWrite(in4, 0);
    }
    void tl() {
      digitalWrite(in1, 0);
      digitalWrite(in2, 1);
      digitalWrite(in3, 0);
      digitalWrite(in4, 1);
    }
    void gostr() {
      digitalWrite(in1, 1);
      digitalWrite(in2, 0);
      digitalWrite(in3, 0);
      digitalWrite(in4, 1);
    }
    void power_l(int a) {
      analogWrite(en1, a);
    }
    void power_r(int a) {
      analogWrite(en2, a);
    }
};
int base = 90;
int corr;
float i, e;
int steer_l;
int steer_r;
float kp = 0.1331;
float ki = 0.001;
float kd = 0.18;
float le = 0;
Robot lfr(4, 5, 6, 7, 11, 3);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  TCCR2B = TCCR2B & B11111000 | B00000101;  // Bit 2:0 – CS22:0: Clock Select
}

void loop() {
  e = (analogRead(A0) - analogRead(A1));
  corr = (e * kp) + ((e - le) * kd) + ((e + i) * ki);
  le = e;
  steer_l = base + corr;
  steer_r = base - corr;
  if (steer_l > 255) {
    steer_l = 255;
  }
  if (steer_l < 0 && steer_l >= -255) {
    steer_l = -steer_l;
    lfr.tr();
  }
  if (steer_l < -255) {
    steer_l = 255;
    lfr.tr();
  }
  if (steer_r > 255) {
    steer_r = 255;
  }
  if (steer_r < 0 && steer_r >= -255) {
    steer_r = -steer_r;
    lfr.tl();
  }
  if (steer_r < -255) {
    steer_r = 255;
    lfr.tl();
  }
  lfr.power_l(steer_l);
  lfr.power_r(steer_r);
  lfr.gostr();
}