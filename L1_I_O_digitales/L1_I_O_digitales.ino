// Pines de entrada para los pulsadores
#define boton1  19 // D11
#define boton2  17 // D10
// GPIO16 que en la DUAL MCU ONE refiere al pin D12
const int led = 16;

void setup() {
  pinMode(boton1, INPUT_PULLUP);
  pinMode(boton2, INPUT_PULLUP);
  pinMode(led, OUTPUT);
}

void loop() {
  int estadoBoton1 = digitalRead(boton1);
  int estadoBoton2 = digitalRead(boton2);

  // Función NAND (&&): El LED solo enciende si ambos botones están presionados (ambos a cero)
  // Prueba cambiar la logica por una funcion NOR (||)
  if (estadoBoton1 == LOW && estadoBoton2 == LOW) {
    digitalWrite(led, HIGH);  // Enciende el LED
  } else {
    digitalWrite(led, LOW);   // Apaga el LED
  }
}


