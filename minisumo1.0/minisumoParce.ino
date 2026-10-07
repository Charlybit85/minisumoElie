#include <xmotionV3.h>

#define LLINE A2
#define RLINE A1

#define LSEN_RIGH 1  // OJO: mismo pin que RLINE (ver nota)
#define LSEN_LEFT A4
#define FSEN_CENTER A5
#define FSEN_RIGH 4
#define FSEN_LEFT 2

// #define LPWM 10
// #define LDIR 12
// #define RPWM 11
// #define RDIR 13

#define RPWM 10 
#define RDIR 12
#define LPWM 11
#define LDIR 13

#define DS1 5
#define DS2 6
#define DS3 7

#define BTN A0
#define LLED 8
#define RLED 9
#define SPD A3
#define LSS 650


#define BTN_START HIGH

int LastValue = 3;
int speed_val = 70;
bool x;
bool TACTIC = HIGH;
float lastError = 0;


void setup() {
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
 // Motores
  pinMode(LPWM, OUTPUT);
  pinMode(LDIR, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(RDIR, OUTPUT);

  // Leds
  pinMode(LLED, OUTPUT);
  pinMode(RLED, OUTPUT);

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

  LEDS(HIGH, LOW);  delay(500);
  LEDS(LOW, HIGH);  delay(500);
  LEDS(HIGH, HIGH); delay(500);
  LEDS(LOW, LOW);   delay(500);

  Serial.begin(9600);
  MOTORES(0, 0, 1);
}

void MOTORES(float Lval, float Rval, int timex) {
  Lval = Lval * 2.55;
  Rval = Rval * 2.55;
  if (Lval >= 0) digitalWrite(LDIR, HIGH);
  else { Lval = abs(Lval); digitalWrite(LDIR, LOW); }
  analogWrite(LPWM, Lval);

  if (Rval >= 0) digitalWrite(RDIR, HIGH);
  else { Rval = abs(Rval); digitalWrite(RDIR, LOW); }
  analogWrite(RPWM, Rval);
  delay(timex);
}

void LEDS(bool D2, bool D1) {
  digitalWrite(LLED, D2);
  digitalWrite(RLED, D1);
}

void freno(bool left, bool right, int timex){
  if (left) {
    digitalWrite(LDIR, !digitalRead(LDIR));
    analogWrite(LPWM, 255);
  }
  if (right) {
    digitalWrite(RDIR, !digitalRead(RDIR));
    analogWrite(RPWM, 255);
  }
  delay(timex);
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
}

bool DetectoLoser() {
  return digitalRead(LSEN_RIGH) == HIGH || digitalRead(LSEN_LEFT) == HIGH ||
         digitalRead(FSEN_CENTER) == HIGH || digitalRead(FSEN_RIGH) == HIGH ||
         digitalRead(FSEN_LEFT) == HIGH;
}

bool conteo() {
  for (int i = 0; i < 50; i++) {          
    if (digitalRead(BTN) != BTN_START) {  
      MOTORES(0, 0, 1);
      return false;
    }
    MOTORES(0, 0, 1);                     
    LEDS(i % 2, !(i % 2));                
    delay(100);
  }
  LEDS(LOW, LOW);
  return true;
}
void ataqueDirecto(){

  if (digitalRead(BTN) != BTN_START) {
    MOTORES(0, 0, 1);
    LastValue = 3;
    lastError = 0;
    TACTIC = HIGH;
    digitalWrite(RLED, DetectoLoser() || digitalRead(LLINE) == 0 || digitalRead(RLINE) == 0);
    x = !x;
    digitalWrite(LLED, x);
    return;                
  }
  MOTORES(10,10,1);
}
void loop() {

  if (digitalRead(BTN) != BTN_START) {
    MOTORES(0, 0, 1);
    LastValue = 3;
    lastError = 0;
    TACTIC = HIGH;
    digitalWrite(RLED, DetectoLoser() || digitalRead(LLINE) == 0 || digitalRead(RLINE) == 0);
    x = !x;
    digitalWrite(LLED, x);
    return;                
  }

  if (TACTIC == HIGH) {
    if (!conteo()) return;
    TACTIC = LOW;
    //LLegar hasta la linea y captar si hay un oponente de frente.
    while(analogRead(LLINE) > LSS && analogRead(RLINE) > LSS){
      MOTORES(50, 50,  1);
      if (digitalRead(FSEN_CENTER) == HIGH){
        ataqueDirecto();
      }
    }
    freno(true, true, 20);
    while(analogRead(LLINE) < LSS && analogRead(RLINE) < LSS && digitalRead(BTN) == HIGH){
      MOTORES(-40,-40, 25);
      MOTORES(70,-70, 90);
      MOTORES(30,-30, 30);
    }

  }
  if (DetectoLoser()) {
    static float Kp = 35.0;
    static float Kd = 10.0;
    int baseSpeed = 0;

    int sL  = digitalRead(LSEN_RIGH);
    int sLF = digitalRead(LSEN_LEFT);
    int sM  = digitalRead(FSEN_CENTER);
    int sRF = digitalRead(FSEN_RIGH);
    int sR  = digitalRead(FSEN_LEFT);

    float weightL = 5.0;
    float weightR = 5.0;
    if (sM == 1 || sLF == 1 || sRF == 1) {
      weightL = 1.5;
      weightR = 1.5;
    }

    float error = (sL * -weightL) + (sLF * -2.0) + (sM * 0.0) + (sRF * 2.0) + (sR * weightR);
    float pidOutput = (Kp * error) + (Kd * (error - lastError));
    lastError = error;

    int leftSpeed  = constrain(baseSpeed + pidOutput, -20, 20);
    int rightSpeed = constrain(baseSpeed - pidOutput, -20, 20);

    MOTORES(leftSpeed, rightSpeed, 1);
    LEDS(HIGH, HIGH);

    if (error <= -2.0)      LastValue = 1;
    else if (error >= 2.0)  LastValue = 5;
    else                    LastValue = 3;
  }
  else {
    LEDS(LOW, LOW);
    if      (LastValue == 1) MOTORES(-speed_val, speed_val, 1);
    else if (LastValue == 2) MOTORES(0, speed_val, 1);
    else if (LastValue == 3) MOTORES(speed_val, speed_val, 1);
    else if (LastValue == 4) MOTORES(speed_val, 0, 1);
    else if (LastValue == 5) MOTORES(speed_val, -speed_val, 1);
  }
}