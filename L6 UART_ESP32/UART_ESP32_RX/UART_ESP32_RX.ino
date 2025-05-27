// Definición del pin del LED (GPIO25, común en muchas placas ESP32)
#define LED 25  

// Configuración del UART2 (HardwareSerial) para comunicación serial
HardwareSerial mySerial(2);  

void setup() {
  // Inicializa el puerto serial USB (para depuración en el Monitor Serial)
  Serial.begin(9600);  

  // Inicializa el UART2 con:
  // - Baud rate: 9600
  // - Configuración: 8 bits de datos, sin paridad, 1 bit de stop (SERIAL_8N1)
  // - Pines: RX = GPIO16, TX = GPIO17 (pueden cambiarse según necesidades)
  mySerial.begin(9600, SERIAL_8N1, 16, 17);  

  // Configura el pin del LED como salida
  pinMode(LED, OUTPUT);  

  // Apaga el LED inicialmente (HIGH porque es común en lógica de pull-up)
  digitalWrite(LED, HIGH);  

  // Mensaje inicial para confirmar que el programa inició
  Serial.println("ESP32 UART Receiver");  
}

void loop() {
  // Verifica si hay datos disponibles en el UART2
  if (mySerial.available()) {  
    // Lee el mensaje hasta encontrar un salto de línea ('\n')
    String message = mySerial.readStringUntil('\n');  

    // Elimina espacios en blanco al inicio/final del mensaje (para limpieza)
    message.trim();  

    // Muestra el mensaje recibido en el Monitor Serial (USB)
    Serial.println("Received: " + message);  

    // --- Lógica de control del LED ---
    if (message == "ON") {  
      digitalWrite(LED, LOW);  // Enciende el LED (LOW activa, común en lógica pull-up)
      Serial.println("LED ENCENDIDO DEBIDO A MENSAJE EN UART");  
      mySerial.write("DATO RECIBIDO");  // Envía confirmación por UART (opcional)
    }
    else if (message == "OFF") {  
      digitalWrite(LED, HIGH);  // Apaga el LED
      Serial.println("LED APAGADO DEBIDO A MENSAJE EN UART");  
    }
    else {  
      // Mensaje si el comando no es "ON" ni "OFF"
      Serial.println("Comando no reconocido");  
    }
  }
}