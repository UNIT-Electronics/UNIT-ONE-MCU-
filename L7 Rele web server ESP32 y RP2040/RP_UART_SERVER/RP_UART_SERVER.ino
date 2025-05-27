// Define el pin del relevador (GPIO10 en el RP2040, corresponde al pin D7 en la DUAL MCU ONE)
#define RELE 10  

void setup() {
  Serial.begin(9600);      // Inicia la comunicación por USB (UART0) para el monitor serial (depuración)
  Serial1.begin(9600);     // Inicia UART1 (TX = GPIO0, RX = GPIO1), usado para recibir datos del ESP32
  pinMode(RELE, OUTPUT);   // Configura el pin del relevador como salida
  Serial.println("RP2040 UART listo");  // Mensaje de confirmación
}

void loop() {
  // Verifica si hay datos disponibles para leer desde el ESP32 (vía UART1)
  if (Serial1.available()) {  
    // Lee la cadena de texto recibida hasta el salto de línea '\n'
    String message = Serial1.readStringUntil('\n');  

    // Elimina espacios en blanco o caracteres no deseados al inicio/final
    message.trim();  

    // Muestra el mensaje recibido en el monitor serial
    Serial.println("Received: " + message);  

    // ---------------------------
    // Lógica para controlar el RELE
    // ---------------------------

    // Si el mensaje es "ON", activa el relevador
    if (message == "ON") {  
      digitalWrite(RELE, HIGH);  // Enciende el relevador (activa salida)
      Serial.println("RELE ACTIVADO DEBIDO A MENSAJE EN UART");  // Mensaje de confirmación

      // Envía respuesta de confirmación por UART de regreso al ESP32
      Serial1.write("DATO RECIBIDO : RELE desactivado"); 
    }

    // Si el mensaje es "OFF", apaga el relevador
    else if (message == "OFF") {  
      digitalWrite(RELE, LOW);  // Apaga el relevador (desactiva salida)
      Serial.println("RELE APAGADO DEBIDO A MENSAJE EN UART"); 

      // Envía respuesta de confirmación por UART de regreso al ESP32
      Serial1.write("DATO RECIBIDO : RELE ACTIVADO");  
    }

    // Si se recibe un mensaje diferente a "ON" u "OFF"
    else {  
      Serial.println("Comando no reconocido");  // Mensaje de advertencia en consola
    }
  }
}
