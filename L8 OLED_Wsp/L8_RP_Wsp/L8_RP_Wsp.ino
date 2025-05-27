// Incluimos las librerías necesarias para manejar la pantalla OLED por I2C
#include <Wire.h>                  // Comunicación I2C
#include <Adafruit_GFX.h>         // Librería gráfica de Adafruit (texto, formas, etc.)
#include <Adafruit_SSD1306.h>     // Librería específica para pantallas OLED con chip SSD1306

// Dimensiones de la pantalla OLED (usualmente 128x64 píxeles)
#define ANCHO_PANTALLA 128
#define ALTO_PANTALLA 64

// Definimos el pin donde está conectado el LED (puede ser el interno o uno externo)
#define LED 29  // GPIO 29, puedes cambiarlo si conectaste el LED a otro pin

// Creamos el objeto 'display' para manejar la OLED
// Usa el bus I2C predeterminado (Wire) y sin pin de reset (-1)
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

// Función de configuración, se ejecuta una sola vez al iniciar el RP2040
void setup() {
  Serial.begin(115200);       // Comunicación serial con la computadora (depuración vía USB)
  Serial1.begin(115200);      // Comunicación UART con el ESP32 (usa pines TX1/RX1 del RP2040)
  Wire.begin();               // Inicializamos la comunicación I2C para la OLED

  pinMode(LED, OUTPUT);       // Configuramos el pin del LED como salida
  digitalWrite(LED, LOW);     // Apagamos el LED inicialmente

  // Inicializamos la pantalla OLED (dirección I2C: 0x3C)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("No se encontró la pantalla OLED"));
    while (true);  // Si no se detecta la pantalla, el programa se detiene
  }

  // Limpiamos pantalla y configuramos estilos de texto
  display.clearDisplay();                // Borra cualquier contenido en la pantalla
  display.setTextSize(1);               // Tamaño de texto normal
  display.setTextColor(SSD1306_WHITE);  // Texto en blanco (OLED monocromática)
}

// Función principal, se ejecuta en bucle constantemente
void loop() {
  // Verificamos si llegó un mensaje desde el ESP32 por el puerto UART (Serial1)
  if (Serial1.available() > 0) {
    // Leemos la línea completa (hasta salto de línea) enviada desde el ESP32
    String mensaje = Serial1.readStringUntil('\n');
    mensaje.trim();  // Eliminamos espacios extra o saltos de línea

    // Mostramos el mensaje en el monitor serial (depuración)
    Serial.print("ACK: ");     // Prefijo de confirmación
    Serial.println(mensaje);   // Mostramos el mensaje recibido

    // Encendemos o apagamos el LED según el contenido del mensaje recibido
    if (mensaje == "LUZ ENCENDIDA ;)") {
      digitalWrite(LED, HIGH);  // Encendemos el LED
    }
    else if (mensaje == "LUZ APAGADA :C") {
      digitalWrite(LED, LOW);   // Apagamos el LED
    }

    // Mostramos el mensaje en la pantalla OLED
    display.clearDisplay();        // Limpiamos la pantalla
    display.setCursor(0, 0);       // Colocamos el cursor en la esquina superior izquierda
    display.println(mensaje);     // Escribimos el mensaje
    display.display();            // Mostramos el contenido

    delay(3000);  // Esperamos 3 segundos antes de actualizar la pantalla
  }
}
