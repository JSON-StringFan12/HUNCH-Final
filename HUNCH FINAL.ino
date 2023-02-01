#include <Servo.h>

Servo Scan;
Servo MoveX;
Servo MoveY;

// Pin Configurations
const int Pin_X = 2
const int Pin_Y = 3
const int Pin_Scan = 4

// Variables to modify:
// Servo Center Positions:
// Center position of Servo
const int Scan_Center = 94;
const int X_Center = 70;
const int Y_Center = 61;

// Time between Measurements (ms)
const int Measurement_Time = 6000;

// Y Max Position (To prevent hitting the floor)
const int Y_Max = 120;

// Panel Measurements Per Degree (higher = more accurate but slower measurements)
const int Measure_Per_Degree = 1000;

// Range of Scan and Main Panels
const int Range = 90;


float Max[2][2] = {{0, 1}, {0, 1}};

int lastX = X_Center;
int lastY = Y_Center;

void setup() {
  Serial.begin(9600);
  Serial.begin(9601);
  MoveX.attach(Pin_X);
  MoveY.attach(Pin_Y);
  Scan.attach(Pin_Scan);
  Scan.write(Scan_Center);
  MoveX.write(X_Center);
  MoveY.write(Y_Center);
}

int readScan (int pin, int pos, int max, bool scan, int delay) {
  float avg = 0;
  for (int i = 0; i < Measure_Per_Degree; i++) {
    float read = analogRead(pin);
    read = read * 5/1023;
    avg += read;
    delayMicroseconds(delay);
  }
  avg /= Measure_Per_Degree;
  if (avg > Max[max][0] && scan == true) {
    Max[max][0] = avg;
    Max[max][1] = pos;
  } else if (scan == false) {
    Serial.println(avg * 5);
  }
  avg = 0;
}

int Move (int XPos, int YPos) {
  if (lastX > XPos) {
    for (int i = lastX; i >= XPos; i--) {
      MoveX.write(i);
      delay(15);
    }
  } else {
    for (int i = lastX; i <= XPos; i++) {
      MoveX.write(i);
      delay(15);
    }
  }
  if (lastY > YPos) {
    for (int i = lastY; i >= YPos; i--) {
      MoveY.write(i);
      delay(15);
    }
  } else {
    for (int i = lastY; i <= YPos; i++) {
      MoveY.write(i);
      delay(15);
    }
  }
  MoveY.write(YPos);
  MoveX.write(XPos);
  lastX = XPos;
  lastY = YPos;
}

void loop() {
  delay(Measurement_Time);
  for (int i = Scan_Center - (Range/2); i < Scan_Center + (Range/2); i++) {
    Scan.write(i);
    readScan(A4, i, 0, true, 1);
    readScan(A1, i, 1, true, 1);
  }
  if (Max[1][1] > Y_Max) {
    Max[1][1] = Y_Max;
  }
  Scan.write(Max[0][1]);
  delay(2000);
  Scan.write(Max[1][1]);
  delay(2000);
  Move((Max[0][1] - Scan_Center) + X_Center, (Max[1][1] - Scan_Center) + Y_Center);
  MoveY.write((Max[1][1] - Scan_Center) + Y_Center);
  MoveX.write((Max[0][1] - Scan_Center) + X_Center);
  Max[0][0] = 0;
  Max[0][1] = 1;
  Max[1][0] = 0;
  Max[1][1] = 1;
}
