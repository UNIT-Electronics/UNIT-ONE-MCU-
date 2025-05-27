// Define el pin analógico donde está conectado el sensor LM35
#define LM35 28  // GPIO28 → pin analógico A2 en algunas placas como la DUAL MCU ONE

// Variable para almacenar el valor calculado de temperatura
float temp = 0.0;

void setup() {
  // Configura el puerto serial para enviar datos al monitor serial
  Serial.begin(9600);  // Velocidad de comunicación: 9600 baudios

  // Configura el pin del LM35 como entrada analógica
  pinMode(LM35, INPUT);
}

void loop() {
  // Lee el valor analógico del pin (0 a 1023 para placas de 10 bits de resolución)
  // Luego convierte ese valor a voltaje y después a temperatura en °C:
  // - El LM35 entrega 10 mV por cada grado Celsius
  // - Si el voltaje máximo es 3.3V y se usa una calibración aproximada → divisor 310
  temp = ((analogRead(LM35) / 310.0) * 100);

  // Imprime el valor de temperatura en grados Celsius por el monitor serial
  Serial.println(temp);

  // Espera 800 milisegundos antes de hacer otra lectura
  delay(800);
}


/*
#define pot 26 // GPIO26 es decir el pin A0 en la DUAL MCU ONE

// Definición de los pines donde están conectados los LEDs
#define ledVerde    16 // D12
#define ledAmarillo 19 // D11
#define ledAzul     17 // D10
#define ledRojo     3  // D9

// Variable para almacenar el valor de voltaje leído del potenciómetro
float voltaje = 0.0;

void setup() {
  // Inicializa la comunicación serial para ver los datos por el monitor serial
  Serial.begin(9600);         

  // Define el pin del potenciómetro como entrada
  pinMode(pot, INPUT);

  // Define los pines de los LEDs como salidas
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledRojo, OUTPUT);
}

void loop() {
  // Lee el valor analógico del potenciómetro (0 a 1023) y lo convierte a voltaje
  // Aquí se divide entre 310 como forma de calibración personalizada
  // (por ejemplo, si el voltaje máximo esperado es ~3.3V)
  voltaje = analogRead(pot) / 310.0;

  // Muestra el voltaje por el monitor serial
  Serial.println(voltaje);

  // Apaga todos los LEDs antes de decidir cuál encender
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledAzul, LOW);
  digitalWrite(ledRojo, LOW);

  // Lógica de comparación para encender un LED dependiendo del nivel de voltaje
  if (voltaje < 1.0) {
    // Nivel bajo → LED verde
    digitalWrite(ledVerde, HIGH);
  } else if (voltaje >= 1.0 && voltaje < 2.0) {
    // Nivel medio-bajo → LED amarillo
    digitalWrite(ledAmarillo, HIGH);
  } else if (voltaje >= 2.0 && voltaje < 3.0) {
    // Nivel medio-alto → LED azul
    digitalWrite(ledAzul, HIGH);
  } else {
    // Nivel alto → LED rojo
    digitalWrite(ledRojo, HIGH);
  }

  // Espera un tiempo antes de hacer la siguiente lectura (para evitar parpadeos rápidos)
  delay(800);
}*/