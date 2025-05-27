/*// Pines de conexión
#define boton1 10      // GPIO10 Aumentar ángulo al pin D7
#define boton2 8      // GPIO8 Disminuir ángulo al pin D6
#define pinServo 3    // GPIO3 Señal PWM al servo al pin D9
#define led1 18       // GPIO18 pin D13
#define led2 16       // GPIO16 pin D12

int angulo = 0;       // Ángulo inicial del servo (grados)

void setup() {
  pinMode(boton1, INPUT_PULLUP);  // Botón conectado a GND
  pinMode(boton2, INPUT_PULLUP);  // Botón conectado a GND
  pinMode(pinServo, OUTPUT);      // Pin del servo como salida
  pinMode(led1, OUTPUT);      // Pin del led como salida
  pinMode(led2, OUTPUT);      // Pin del led2 como salida
}

void loop() {
  // Si se presiona el botón 1, aumentar ángulo
  if (digitalRead(boton1) == LOW) {
    angulo+=20;
    angulo = constrain(angulo, 0, 180);
     digitalWrite(led1, HIGH);
     digitalWrite(led2, LOW);
    delayMicroseconds(10); // Pequeña pausa para evitar que suba muy rápido
  }

  // Si se presiona el botón 2, disminuir ángulo
  if (digitalRead(boton2) == LOW) {
    angulo-=20;
    angulo = constrain(angulo, 0, 180);
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    delayMicroseconds(10); // Pausa para evitar que baje muy rápido
  }

  // Enviar la señal PWM manual al servo (20 ms cada ciclo)
  for (int i = 0; i < 5; i++) { // Repetir varias veces para asegurar que el servo responda
    int pulso = map(angulo, 0, 180, 500, 2500); // Convertir ángulo a microsegundos (0° = 0.5 ms, 180° = 2.5 ms)

    digitalWrite(pinServo, HIGH);
    delayMicroseconds(pulso);
    digitalWrite(pinServo, LOW);
    delay(20 - (pulso / 1000)); // Resto del ciclo de 20 ms
  }
}

*/

#define ledPWM 3  // Pin D9 PWM conectado al LED

int brillo = 0;         // Nivel de brillo actual (0 a 255)
int direccion = 5;      // Paso del cambio (positivo o negativo)

void setup() {
  pinMode(ledPWM, OUTPUT);
}

void loop() {
  analogWrite(ledPWM, brillo); // Aplica el valor de brillo al LED

  brillo += direccion;         // Aumenta o disminuye el brillo

  // Si llega a los extremos, invierte la dirección
  if (brillo <= 0 || brillo >= 255) {
    direccion = -direccion;
  }

  delay(20);  // Controla la velocidad de la transición (más bajo = más rápido)
}
