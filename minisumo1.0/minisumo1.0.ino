//Pines libres D2 A5 A4 A3 A2 A1 A0 A7 A6 D12 D4
//Pines libres adicionales D13 y D5

//sensores de PRESENCIA
#define IzquierdaFrontal A3
#define DerechaFrontal A4
#define IzquierdaLateral 4  //A5
#define DerechaLateral A5   //4

#define modulo 8

//sensores de LINEA
#define seLineaIzq A0
#define seLineaDer A1

//motores
#define pwmIzquierdo 11  //11
#define direcIzq1 10     //10
#define pwmDerecho 3     //3
#define direcDer1 9      // 9

int initime = 0;  //tiempo de inicio
//int modulo = 8; //modulo
int start = 0;  // modulo status
int st;
int i = 0;  //contador

//pulsadores
int pushizquierdo = 6;
int valpushizquierdo;
int pushderecho = 7;
int valpushderecho;

//LEDS
int ledIzquierdo = 0;
int ledDerecho = 0;


void setup() {
  TCCR2B = TCCR2B & B11111000 | B00000010;  // PWM FREC
  Serial.begin(9600);
  pinMode(ledIzquierdo, OUTPUT);
  pinMode(ledDerecho, OUTPUT);

  pinMode(pwmIzquierdo, OUTPUT);
  pinMode(direcIzq1, OUTPUT);
  pinMode(pwmDerecho, OUTPUT);
  pinMode(direcDer1, OUTPUT);

  pinMode(IzquierdaFrontal, INPUT);
  pinMode(DerechaFrontal, INPUT);
  pinMode(IzquierdaLateral, INPUT);
  pinMode(DerechaLateral, INPUT);

  pinMode(modulo, INPUT);
  pinMode(seLineaIzq, INPUT);
  pinMode(seLineaDer, INPUT);

  while (!digitalRead(modulo)) {
    delay(50);
    digitalWrite(ledIzquierdo, HIGH);
    digitalWrite(ledDerecho, HIGH);
    delay(50);
    digitalWrite(ledIzquierdo, LOW);
    digitalWrite(ledDerecho, LOW);
  }
  for (int i = 0; i < 1; i++) {
    delay(5000);
  }
  arranque();
}

void motor_derecho(int velocidad) {
  if (velocidad > 255) velocidad = 255;
  else if (velocidad < -255) velocidad = -255;

  if (velocidad >= 0) {  //100
    digitalWrite(direcDer1, HIGH);
  } else {
    digitalWrite(direcDer1, LOW);
    velocidad = abs(velocidad);
  }
  analogWrite(pwmDerecho, velocidad);
}

void motor_izquierdo(int velocidad) {
  if (velocidad > 255) velocidad = 255;
  else if (velocidad < -255) velocidad = -255;

  if (velocidad >= 0) {
    digitalWrite(direcIzq1, HIGH);
  } else {
    digitalWrite(direcIzq1, LOW);
    velocidad = abs(velocidad);
  }
  analogWrite(pwmIzquierdo, velocidad);
}

void motores(int V_izquierdo, int V_derecho) {
  motor_derecho(V_derecho);
  motor_izquierdo(V_izquierdo);
}

void controlPD() {
  // 1. Definir Kp, Kd, lastError y baseSpeed
  static float kp = 35.0;  //35
  static float kd = 10.0;  //10
  static float lastError = 0;
  int BaseSped = 150;  //801

  int SLI = digitalRead(IzquierdaLateral);
  int SFI = digitalRead(IzquierdaFrontal);
  int SDF = digitalRead(DerechaFrontal);
  int SLD = digitalRead(DerechaLateral);


  float error = ((SFI * -3.5) + (SDF * 3.5) + (SLI * -5.0) + (SLD * 5.0));
  // Calculo de PD
  float pidOutput = (kp * error) + (kd * (error - lastError));
  lastError = error;


  int leftspeed = BaseSped + pidOutput;
  int rightspeed = BaseSped - pidOutput;

  leftspeed = constrain(leftspeed, -150, 150);
  rightspeed = constrain(rightspeed, -150, 150);

  motores(leftspeed, rightspeed);
}

void frenoDuro(bool izq, bool der, int duracion) {
  if (izq) {
    digitalWrite(direcIzq1, !digitalRead(direcIzq1));
    analogWrite(pwmIzquierdo, 255);
  }
  if (der) {
    digitalWrite(direcDer1, !digitalRead(direcDer1));
    analogWrite(pwmDerecho, 255);
  }
  delay(duracion);
  analogWrite(pwmIzquierdo, 0);
  analogWrite(pwmDerecho, 0);
}

void arranque() {
  for (int i = 90; i > 5; i -= 10) {
    motores(150, 150);
    delay(10);
    controlPD();
    //Arranque hasta la linea
    if (digitalRead(seLineaIzq) && digitalRead(seLineaDer)) {
      frenoDuro(true, true, 20);
      motores(-50, -50);
      delay(200);
      for (int i = 10; i < 80; i++) {
        motores(i, 0);
        delay(10);
      }
    } else if (digitalRead(seLineaIzq)) {
      frenoDuro(true, true, 20);
      motores(-50, -50);
      delay(200);
      for (int i = 10; i < 80; i++) {
        motores(i, 0);
        delay(10);
      }
    } else if (digitalRead(seLineaDer)) {
      frenoDuro(true, true, 20);
      motores(-80, -80);
      delay(200);
      for (int i = 10; i < 80; i++) {
        motores(0, i);
        delay(10);
      }
    }
  }
  for (int j = 100; j > 5; j--) {
    motores(30, 30);
    delay(10);
    if (digitalRead(seLineaIzq) && digitalRead(seLineaDer)) {
      frenoDuro(true, true, 20);
      motores(-50, -50);
      delay(200);
      for (int i = 10; i < 50; i++) {
        motores(80, 0);
        delay(10);
        controlPD();
      }
      Ronaldo(true, false);
      break;
    } else if (digitalRead(seLineaIzq)) {
      frenoDuro(true, true, 20);
      motores(-50, -50);
      delay(200);
      for (int i = 10; i < 50; i++) {
        motores(80, 0);
        delay(10);
        controlPD();
      }
      Ronaldo(true, false);
      break;
    } else if (digitalRead(seLineaDer)) {
      frenoDuro(true, true, 20);
      motores(-80, -80);
      delay(200);
      for (int i = 10; i < 50; i++) {
        motores(0, 80);
        delay(10);
        controlPD();
      }
      Ronaldo(false, true);
      break;
    }
  }
}
void Ronaldo(bool izquierda, bool derecha) {
  if (izquierda) {
    for (int i = 100; i > 10; i -= 5) {
      motores(i, i);
      delay(20);
      if (digitalRead(IzquierdaLateral) || digitalRead(IzquierdaFrontal) || digitalRead(DerechaFrontal) || digitalRead(DerechaLateral)) {
        controlPD();
      }
    }
    frenoDuro(true, true, 20);
    delay(20);
    motores(100, -90);
    delay(100);
  }
  if (derecha) {
    for (int i = 100; i > 10; i -= 5) {
      motores(i, i);
      delay(20);
      if (digitalRead(IzquierdaLateral) || digitalRead(IzquierdaFrontal) || digitalRead(DerechaFrontal) || digitalRead(DerechaLateral)) {
        controlPD();
      }
    }
    frenoDuro(true, true, 20);
    delay(20);
    motores(-90, 100);
    delay(100);
  }
}

void loop() {

  // if (digitalRead(IzquierdaLateral) || digitalRead(IzquierdaFrontal) || digitalRead(DerechaFrontal) || digitalRead(DerechaLateral)){
  //   controlPD();
  // }else{
  //   motores(0,0);
  // }
  while (digitalRead(modulo)) {
    if (digitalRead(seLineaIzq) && digitalRead(seLineaDer)) {
      frenoDuro(true, true, 20);
      motores(-100, -100);
      delay(250);
      for (int i = 10; i < 80; i++) {
        motores(i, 0);
        delay(10);
      }
    } else if (digitalRead(seLineaIzq)) {
      frenoDuro(true, true, 20);
      motores(-80, -80);
      delay(250);
      for (int i = 10; i < 80; i++) {
        motores(i, 0);
        delay(10);
      }

    } else if (digitalRead(seLineaDer)) {
      frenoDuro(true, true, 20);
      motores(-80, -80);
      delay(250);
      for (int i = 10; i < 80; i++) {
        motores(0, i);
        delay(10);
      }
    } else if (digitalRead(IzquierdaLateral) || digitalRead(IzquierdaFrontal) || digitalRead(DerechaFrontal) || digitalRead(DerechaLateral)) {
      controlPD();
    } else {
      motores(30, 30);
      delay(80);
    }
  }
  motores(0, 0);
}
