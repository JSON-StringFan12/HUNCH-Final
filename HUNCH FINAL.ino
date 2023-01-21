#include <Servo.h>

Servo Scan;
Servo MoveX;
Servo MoveY;

float Max[2][2] = {{0, 1}, {0, 1}};

int lastX = 70;
int lastY = 65;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MoveX.attach(2);
  MoveY.attach(3);
  Scan.attach(4);
  Scan.write(94); // 94 = center (+4)
  MoveX.write(70); // 70 = center (+25)
  delay(100);
  MoveY.write(61); // 61 = center (+20)
}

int readScan (int pin, int pos, int max, bool scan, int delay) {
  float avg = 0;
  for (int i = 0; i < 1000; i++) {
    float read = analogRead(pin);
    read = read * 5/1023;
    avg += read;
    delayMicroseconds(delay);
  }
  avg /= 1000;
  if (avg > Max[max][0] && scan == true) {
    Max[max][0] = avg;
    Max[max][1] = pos;
  } else if (scan == false) {
    Serial.println(avg * 5);
  }
  avg = 0;
}

int Move (int XPos, int YPos) {
  Serial.println(XPos, YPos);
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
  // put your main code here, to run repeatedly:
  delay(6000);
  for (int i = 49; i < 139; i++) {
    Scan.write(i);
    readScan(A0, i, 0, true, 1);
    readScan(A1, i, 1, true, 1);
  }
  if (Max[1][1] > 120) {
    Max[1][1] = 120;
  }
  Scan.write(Max[0][1]);
  delay(1000);
  Scan.write(Max[1][1]);
  delay(1000);
  Move((Max[1][1] - 94) + 70, (Max[0][1] - 94)/4 + 61);
  MoveY.write((Max[0][1] - 94)/4 + 61);
  MoveX.write((Max[1][1] - 94)/3 + 70);
  delay(2-000);
  Serial.println("Tracking:");
  readScan(A2, 0, 0, false, 10000);
  Serial.println("Static:");
  readScan(A3, 0, 0, false, 10000);
  Max[0][0] = 0;
  Max[0][1] = 1;
  Max[1][0] = 0;
  Max[1][1] = 1;
  delay(2000);
}