#include <WiFi.h>  // Librería para manejo de WiFi en ESP32

// Variable para guardar el estado del relevador
String p = "off";

// Credenciales de la red WiFi
const char *ssid = "Escribe aquí la contraseña de dicha red";
const char *password = "Escribe aquí la contraseña de dicha red";

// Crear un servidor en el puerto 80 (HTTP)
WiFiServer server(80);

// Se declara el segundo puerto serial UART (UART2)
// Este se usará para comunicar con el microcontrolador RP2040
HardwareSerial mySerial(2);

void setup() {
  Serial.begin(9600);  // Inicia comunicación serial para depuración por USB
  Serial.println();
  Serial.println("Configurando WiFi...");

  // Conexión al WiFi con las credenciales dadas
  WiFi.begin(ssid, password);

  // Espera hasta que el ESP32 se conecte al WiFi
  Serial.print("Conectandome");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Una vez conectado, se muestra la IP asignada al ESP32
  Serial.println();
  Serial.print("Conectado, IP: ");
  Serial.println(WiFi.localIP());

  // Inicia el servidor web
  server.begin();
  Serial.println("Servidor iniciado");

  // Inicializa UART2 (HardwareSerial 2)
  // Velocidad 9600 baudios, formato 8N1, pines RX = 16, TX = 17
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("Transmisor ESP32 por UART a RP2040");
}

void loop() {
  // Espera por un cliente HTTP
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Nuevo cliente.");
    String currentLine = "";
    bool comandoEnviado = false;  // Flag para asegurarse que solo se envía una vez el comando

    // Mientras el cliente esté conectado
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();  // Lee un carácter del cliente
        Serial.write(c);         // Lo muestra por el monitor serial

        // Si se detecta un salto de línea, se interpreta que terminó una petición HTTP
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Envío de cabecera HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Contenido HTML de la página
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html{font-family: Helvetica; text-align: center;} .button{border:none; color:white; padding:15px 32px; font-size:16px; margin:4px; cursor:pointer;} .button1{background-color:#4CAF50;} .button2{background-color:#808080;}</style>");
            client.println("</head><body><h1>Control de Relevador ESP32</h1>");

            // Botón cambia según estado del relevador
            if (p == "off") {
              client.println("<button class='button button1' onclick=\"location.href='/r1=ON'\">ENCENDER</button>");
            } else {
              client.println("<button class='button button2' onclick=\"location.href='/r1=OFF'\">APAGAR</button>");
            }

            client.println("</body></html>");
            client.println();  // Fin del contenido HTML
            break;             // Sal del bucle después de enviar la respuesta
          } else {
            currentLine = "";  // Limpia la línea actual si no es el final
          }
        } else if (c != '\r') {
          currentLine += c;  // Acumula caracteres para formar la línea completa
        }

        // Si se detecta la petición para encender el relevador
        if (!comandoEnviado && currentLine.indexOf("GET /r1=ON") != -1) {
          p = "on";  // Actualiza estado
          mySerial.write("ON");  // Envia comando ON por UART al RP2040
          comandoEnviado = true;  // Marca como enviado

          // Si hay datos disponibles desde el RP2040, los lee y muestra
          if (mySerial.available() > 0) {
            String mensaje = mySerial.readStringUntil('\n');
            mensaje.trim();  // Elimina espacios o saltos innecesarios
            Serial.println("\n ACK: ");
            Serial.print(mensaje);
            Serial.print("\n");
          }
        }

        // Si se detecta la petición para apagar el relevador
        if (!comandoEnviado && currentLine.indexOf("GET /r1=OFF") != -1) {
          p = "off";  // Actualiza estado
          mySerial.write("OFF");  // Envia comando OFF por UART al RP2040
          comandoEnviado = true;  // Marca como enviado

          // Si hay datos disponibles desde el RP2040, los lee y muestra
          if (mySerial.available() > 0) {
            String mensaje2 = mySerial.readStringUntil('\n');
            mensaje2.trim();
            Serial.println("\n ACK: ");
            Serial.print(mensaje2);
            Serial.print("\n");
          }
        }
      }
    }

    // Espera mínima antes de cerrar la conexión
    delay(1);
    client.stop();  // Termina conexión con el cliente
    Serial.println("Cliente desconectado");
  }
}
