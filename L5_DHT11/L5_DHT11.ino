#include <DHT.h>

// Definiciones de pines
#define DHTPIN 9       // Pin digital conectado al DHT11 (D4 en la UNIT DUAL MCU ONE)
#define DHTTYPE DHT11  // Tipo de sensor
#define LED_PIN 16      // Pin para el LED (D12 en la UNIT DUAL MCU ONE)

// Inicializa el sensor
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 2000; // Intervalo de lectura en milisegundos (2 segundos)

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Verifica si han pasado 2 segundos desde la última lectura
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Lee temperatura y humedad
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Verifica que la lectura sea válida
    if (isnan(h) || isnan(t)) {
      Serial.println("Error al leer del sensor DHT11");
      return;
    }

    Serial.print("Humedad:");
    Serial.print(h);
    Serial.print("%\t");       //Borra el sigo % si quieres graficar
    Serial.print("Temperatura:");
    Serial.print(t);
    Serial.println(" *C");  // Comenta esta linea si quieres graficar
    //Serial.println();         // Descomenta esta linea si quieres graficar

    // Control del LED según temperatura
    if (t > 28) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  }
}
