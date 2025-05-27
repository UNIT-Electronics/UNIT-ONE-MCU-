// Define el pin del botón (GPIO16, que corresponde a D12 en la DUAL MCU ONE)
#define BUTTON_PIN 16  

// Variables para el antirrebote (debounce) del botón:
bool lastButtonState = HIGH;   // Estado anterior del botón (HIGH por INPUT_PULLUP)
bool ledState = false;         // Estado virtual del LED (ON/OFF)
unsigned long lastDebounceTime = 0;  // Último tiempo de detección
const unsigned long debounceDelay = 300;  // Tiempo de espera para evitar rebotes (ms)

void setup() {
  Serial.begin(9600);     // Inicia UART0 para depuración (Monitor Serial)
  Serial1.begin(9600);    // Inicia UART1 (GPIO0=TX, GPIO1=RX por defecto)
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Botón con resistencia pull-up interna
  Serial.println("RP2040 Transmisor listo");
}

void loop() {
  // Lee el estado actual del botón
  bool currentButtonState = digitalRead(BUTTON_PIN);

  // --- Recepción de mensajes por UART (opcional, para confirmaciones) ---
  if(Serial1.available() > 0) {
    String mensaje = Serial1.readStringUntil('\n');  // Lee hasta salto de línea
    mensaje.trim();  // Elimina espacios y caracteres especiales
    Serial.print("ACK: ");  // Confirma recepción (ej: "ACK: RECIBIDO")
    Serial.println(mensaje);
  }

  // --- Lógica del botón ---
  // Detecta flanco descendente (botón presionado, pasa de HIGH a LOW)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    // Verifica que haya pasado el tiempo de debounce
    if (millis() - lastDebounceTime > debounceDelay) {
      ledState = !ledState;  // Cambia el estado del LED (toggle)

      // Envía "ON" o "OFF" por UART según el estado
      if (ledState) {
        Serial1.println("ON");   // Envía "ON" por UART1 (GPIO0)
        Serial.println("Enviado: ON");  // Confirma por Monitor Serial
      } else {
        Serial1.println("OFF");  // Envía "OFF" por UART1 (GPIO0)
        Serial.println("Enviado: OFF");
      }

      lastDebounceTime = millis();  // Reinicia el temporizador de debounce
    }
  }

  // Actualiza el estado anterior del botón
  lastButtonState = currentButtonState;
}