/*
   MINISUMO_XMOTION_MICROSTART_CORE400_TRIMPOT_TACTICS_0128
   BLACK MAGIC ROBOT version0128
*/

//SENSOR LINE
#include <xmotionV3.h>
#define LLINE A2
#define RLINE A4
// SENSOR DETECT
#define LSEN_RIGH A4 //D1 o A4
#define LSEN_LEFT 1
#define FSEN_CENTER A5
#define FSEN_RIGH 4
#define FSEN_LEFT 2
//MOTORS
#define LPWM 10
#define LDIR 12
#define RPWM 11
#define RDIR 13

//SWITCHS, BUTONS, TRIMPOTS ,BUZZER &LEDS
#define DS1 5
#define DS2 6
#define DS3 7

#define BTN A0
#define LLED 8
#define RLED 9
#define SPD A3
#define LSS 650 //LINE SENSOR SENSIBILITY 0~1023 - v1 90

//VARIABLES
int LastValue = 3, SPED = 100;
bool TACTIC = HIGH;
bool x;
int speed_val = 70;

void setup() {
  //INPUTS
  pinMode(LLINE, INPUT);
  pinMode(RLINE, INPUT);
  pinMode(LSEN_RIGH, INPUT);
  pinMode(LSEN_LEFT, INPUT);
  pinMode(FSEN_CENTER, INPUT);
  pinMode(FSEN_RIGH, INPUT);
  pinMode(FSEN_LEFT, INPUT);
  pinMode(DS1, INPUT);
  pinMode(DS2, INPUT);
  pinMode(DS3, INPUT);
  pinMode(BTN, INPUT);
  pinMode(SPD, INPUT);
  //OUTPUTS
  pinMode(LPWM, OUTPUT);
  pinMode(LDIR, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(RDIR, OUTPUT);
  pinMode(LLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  //INITIAL VALUES
  digitalWrite(LLINE, HIGH);
  digitalWrite(RLINE, HIGH);
  digitalWrite(LSEN_RIGH, HIGH);
  digitalWrite(LSEN_LEFT, HIGH);
  digitalWrite(FSEN_CENTER, HIGH);
  digitalWrite(FSEN_RIGH, HIGH);
  digitalWrite(FSEN_LEFT, HIGH);
  digitalWrite(LPWM, LOW);
  digitalWrite(LDIR, LOW);
  digitalWrite(RPWM, LOW);
  digitalWrite(RDIR, LOW);
  digitalWrite(DS1, HIGH);
  digitalWrite(DS2, HIGH);
  digitalWrite(DS3, HIGH);
  digitalWrite(BTN, LOW);
  digitalWrite(LLED, LOW);
  digitalWrite(RLED, LOW);
  LEDS(HIGH, LOW);
  delay(500);
  LEDS(LOW, HIGH);
  delay(500);
  LEDS(HIGH, HIGH);
  delay(500);
  LEDS(LOW, LOW);
  delay(500);
  Serial.begin(9600);
}

void MOVE( float Lval, float Rval, int timex) {
  Lval = Lval * 2.55;
  Rval = Rval * 2.55;
  if ( Lval >= 0 )
    digitalWrite(LDIR, HIGH);
  else {
    Lval = abs(Lval);
    digitalWrite(LDIR, LOW);
  }
  analogWrite(LPWM, Lval);
  if ( Rval >= 0 )
    digitalWrite(RDIR, HIGH);
  else {
    Rval = abs(Rval);
    digitalWrite(RDIR, LOW);
  }
  analogWrite(RPWM, Rval);
  delay(timex);
}

void LEDS(bool D2, bool D1) {
  digitalWrite(LLED, D2);
  digitalWrite(RLED, D1);
}

void DEBUG() {
  Serial.print(" LineSensors: ");
  Serial.print(" LLINE:");
  Serial.print(analogRead(LLINE));
  Serial.print(" RLINE:");
  Serial.print(analogRead(RLINE));
  Serial.print("  OpponentSensors:");
  Serial.print(" LSEN_RIGH:");
  Serial.print(digitalRead(LSEN_RIGH));
  Serial.print(" LSEN_LEFT:");
  Serial.print(digitalRead(LSEN_LEFT));
  Serial.print(" FSEN_CENTER:");
  Serial.print(digitalRead(FSEN_CENTER));
  Serial.print(" FSEN_RIGH:");
  Serial.print(digitalRead(FSEN_RIGH));
  Serial.print(" FSEN_LEFT:");
  Serial.print(digitalRead(FSEN_LEFT));
  Serial.print("  DIPSWITCHS:");
  Serial.print(" DS1:");
  Serial.print(digitalRead(DS1));
  Serial.print(" DS2:");
  Serial.print(digitalRead(DS2));
  Serial.print(" DS3:");
  Serial.print(digitalRead(DS3));
  Serial.print("  BUTONS:");
  Serial.print(" BTN:");
  Serial.print(digitalRead(BTN));
  SPED = float(analogRead(SPD) / 10,23);
  Serial.print(" SPED: ");
  Serial.print(SPED);
  Serial.print("  TRIMPOTS:");
  Serial.print(" SPD: ");
  Serial.println(analogRead(SPD));
  delay(100);
}

void loop() {
INIT:
  DEBUG();
  if (digitalRead(LLINE) == 0 || digitalRead(LSEN_RIGH) == 1 || digitalRead(LSEN_LEFT) == 1 || digitalRead(FSEN_CENTER) == 1 || digitalRead(FSEN_RIGH) == 1 || digitalRead(FSEN_LEFT) == 1 || digitalRead(RLINE) == 0)
    digitalWrite(RLED, HIGH);
  else
    digitalWrite(RLED, LOW);
  x = !x;
  digitalWrite(LLED, x);
  while (digitalRead(BTN) == HIGH) {
    if (TACTIC == HIGH) {
      TACTIC = LOW;
      if (digitalRead(DS1) == LOW && digitalRead(DS2) == LOW && digitalRead(DS3) == LOW) {//111
        goto START;
      }
      else if (digitalRead(DS1) == HIGH && digitalRead(DS2) == HIGH && digitalRead(DS3) == HIGH) {//000
        goto START;
      }
      else if (digitalRead(DS1) == HIGH && digitalRead(DS2) == LOW && digitalRead(DS3) == HIGH) {//010
        MOVE(100, 100, 80);
        LastValue = 3;
      }
      else if (digitalRead(DS1) == HIGH && digitalRead(DS2) == LOW && digitalRead(DS3) == LOW) {//011 ZIGZAG  RIGHT  TO LEFT
        MOVE( 100, -100, 70);
        MOVE(-100, 100, 30);

        MOVE( 100, 100, 100);
        MOVE(-100, -100, 30);

        MOVE( -100, 100, 120);
        MOVE(100, -100, 30);

        MOVE( 100, 100, 200);
        MOVE(-100, -100, 30);
        LastValue = 3;
      }
      else if (digitalRead(DS1) == LOW && digitalRead(DS2) == LOW && digitalRead(DS3) == HIGH) { //110 ZIGZAG LEFT TO RIGHT
        MOVE(-100, 100, 70);
        MOVE( 100, -100, 30);

        MOVE( 100, 100, 100);
        MOVE(-100, -100, 30);

        MOVE( 100, -100, 120);
        MOVE(-100, 100, 30);

        MOVE( 100, 100, 200);
        MOVE(-100, -100, 30);
        LastValue = 3;
      }
      else if (digitalRead(DS1) == HIGH && digitalRead(DS2) == HIGH && digitalRead(DS3) == LOW) { //001 TO RIGHT
        MOVE(100, -100, 105);
        MOVE(-100, 100, 40);

        MOVE( 30, 30, 1);
        LastValue = 3;
      }
      else if (digitalRead(DS1) == LOW && digitalRead(DS2) == HIGH && digitalRead(DS3) == HIGH) { //100 TO LEFT
        MOVE(-100, 100, 105);
        MOVE(100, -100, 40);

        MOVE( 30, 30, 1);
        LastValue = 3;
      }
      else if (digitalRead(DS1) == LOW && digitalRead(DS2) == HIGH && digitalRead(DS3) == LOW) { //101 TO BEHIND
        MOVE(100, -100, 180);
        MOVE(-100, 100, 40);

        MOVE( 30, 30, 1);
        LastValue = 3;
      }
    }
START:
    SPED = float(analogRead(SPD) / 10,23 );
    if (analogRead(LLINE) < LSS && analogRead(RLINE) < LSS) {
      MOVE(-100, -100, 1);
      LEDS(LOW, HIGH);
      while (analogRead(LLINE) < LSS && analogRead(RLINE) < LSS && digitalRead(BTN) == HIGH);
      MOVE(-100, -100, 30);
      MOVE(100, -100, 300);     // 150 - 200 - 250
      MOVE(-100, 100, 30);
      LastValue = 3;
    }
    else if (analogRead(LLINE) < LSS && analogRead(RLINE) > LSS) {
      MOVE(-100, -100, 1);
      LEDS(LOW, HIGH);
      while (analogRead(LLINE) < LSS && analogRead(RLINE) > LSS && digitalRead(BTN) == HIGH);
      MOVE(-100, -100, 30);
      MOVE(100, -100, 200);   // 120 - 160 - 200 - 240
      MOVE(-100, 100, 30);
      LastValue = 3;
    }
    else if (analogRead(LLINE) > LSS && analogRead(RLINE) < LSS) {
      MOVE(-100, -100, 1);
      LEDS(LOW, HIGH);
      while (analogRead(LLINE) > LSS && analogRead(RLINE) < LSS && digitalRead(BTN) == HIGH);
      MOVE(-100, -100, 30);
      MOVE( -100, 100, 200);  // 120 - 160 - 200 - 240
      MOVE(100, -100, 30);
      LastValue = 3;
    }
    else if (digitalRead(LSEN_RIGH) == HIGH || digitalRead(LSEN_LEFT) == HIGH || digitalRead(FSEN_CENTER) == HIGH || digitalRead(FSEN_RIGH) == HIGH || digitalRead(FSEN_LEFT) == HIGH) {
      // --- Local Pid PID Controller ---
      static float Kp = 35.0;  // 10.0 - 15.0 | - 20.0 - 25.0 - 30 - 35
      static float Kd = 10.0; // 20.0 - 25.0 - 30.0
      static float lastError = 0;
      int baseSpeed = 80; // base speed at % percent. Make it 0 for stopping.
      
      // Read Sensors
      int sL = digitalRead(LSEN_RIGH);
      int sLF = digitalRead(LSEN_LEFT);
      int sM = digitalRead(FSEN_CENTER);
      int sRF = digitalRead(FSEN_RIGH);
      int sR = digitalRead(FSEN_LEFT);

// Flag/Deceptive Protection (Weight Change) 
// Normally, the outermost sensors should have a higher weight (+5 / -5)
      float weightL = 5.0;  // 4.0
      float weightR = 5.0;  // 4.0

// If the opponent is in the middle or diagonally in front, 
//we reduce the weight of the external sensors (flags, etc.)
// to prevent the robot from suddenly rotating 90 degrees.
      if(sM == 1 || sLF == 1 || sRF == 1) {
        weightL = 1.5;  // 1.0
        weightR = 1.5;  // 1.0
      }

      // Error Calculation: Left negative (-), Right positive (+)
      float error = (sL * -weightL) + (sLF * -2.0) + (sM * 0.0) + (sRF * 2.0) + (sR * weightR);

      // PD Formula
      float pidOutput = (Kp * error) + (Kd * (error - lastError));
      lastError = error;

      // Motor Speed Update
      int leftSpeed = baseSpeed + pidOutput;
      int rightSpeed = baseSpeed - pidOutput;

      // Speed Contraints
      leftSpeed = constrain(leftSpeed, -100, 100);
      rightSpeed = constrain(rightSpeed, -100, 100);

      // Send Speed Variables to Motors
      MOVE(leftSpeed, rightSpeed, 1);
      LEDS(HIGH, HIGH);

      // If no opponents seen
      // We register the last seen value.
      if (error <= -2.0) LastValue = 1;       // Left Turning
      else if (error >= 2.0) LastValue = 5;   // Right Turning
      else LastValue = 3;                     // Going Forward
    }
    
    else if (LastValue == 1) {
      MOVE((-speed_val), speed_val, 1); // SPED
      LEDS(LOW, LOW);
    }
    else if (LastValue == 2) {
      MOVE(0, speed_val, 1);      // SPED
      LEDS(LOW, LOW);
    }
    else if (LastValue == 3) {
      MOVE(speed_val, speed_val, 1);    // SPED
      LEDS(LOW, LOW);
    }
    else if (LastValue == 4) {
      MOVE(speed_val, 0, 1);      // SPED
      LEDS(LOW, LOW);
    }
    else if (LastValue == 5) {
      MOVE(speed_val, (-speed_val), 1); // SPED
      LEDS(LOW, LOW);
    }
    while (digitalRead(BTN) == LOW) {
      MOVE( 0, 0, 1);
      LEDS(LOW, LOW);
      LastValue = 3;
      TACTIC = HIGH;
      goto INIT;
    }
  }
}
