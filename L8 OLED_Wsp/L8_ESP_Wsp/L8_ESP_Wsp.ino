// Librerías necesarias para conectividad WiFi y para usar la plataforma ThingESP
#include <WiFi.h>
#include <ThingESP.h>

// Creamos un objeto de tipo ThingESP32 los datos ingresados en ThingESP
// Estos datos deben coincidir con los configurados en la plataforma ThingESP
ThingESP32 thing("usuario", "nombre del proyecto", "contraseña del proyecto");

// Creamos una instancia del puerto serial 2 para comunicar al ESP32 con el RP2040 por UART
// Puedes usar cualquier par de pines compatibles del ESP32 para esto
HardwareSerial mensaje_al_RP2040(2);  // UART2

// Variables para control de tiempo (no se usan activamente en este código, pero pueden ser útiles después)
unsigned long previousMillis = 0;
const long INTERVAL = 6000; // 6 segundos

// Función de configuración que se ejecuta una sola vez al encender el ESP32
void setup()
{
  // Iniciamos el puerto serial principal (USB) para depuración con el monitor serie
  Serial.begin(115200);

  // Configuramos el nombre y la contraseña de la red WiFi a la que el ESP32 debe conectarse
  thing.SetWiFi("Nombre de la red WiFi", "contraseña de la red");

  // Inicializamos la conexión del ESP32 con la plataforma ThingESP
  thing.initDevice();

  // Inicializamos la comunicación UART con el RP2040 a través del puerto serial 2
  // Parámetros:
  // - Velocidad de 115200 baudios
  // - Formato 8N1 (8 bits de datos, sin paridad, 1 bit de parada)
  // - RX en el pin GPIO16, TX en el pin GPIO17
  mensaje_al_RP2040.begin(115200, SERIAL_8N1, 16, 17);
}

// Función que maneja los comandos recibidos desde la plataforma ThingESP
// Recibe una cadena ("query") con el comando enviado desde la app o web
// Devuelve un mensaje de respuesta que se mostrará en la app de ThingESP
String HandleResponse(String query)
{
  // Si el usuario envía "encender luz"
  if (query == "encender luz") {
    // Enviamos un mensaje por UART al RP2040 indicando que se debe encender la luz
    mensaje_al_RP2040.write("LUZ ENCENDIDA ;)\n");

    // Enviamos una respuesta de confirmación a ThingESP
    return "HECHO: Luz encendida";
  }

  // Si el usuario envía "apagar luz"
  else if (query == "apagar luz") {
    // Enviamos un mensaje por UART al RP2040 indicando que se debe apagar la luz
    mensaje_al_RP2040.write("LUZ APAGADA :C\n");

    // Enviamos una respuesta de confirmación a ThingESP
    return "HECHO: Luz apagada";
  }

  // Si se recibe un comando que no es reconocido
  else {
    // Enviamos un mensaje al RP2040 indicando que el comando fue inválido
    mensaje_al_RP2040.write("THA FUCK?\n");

    // Enviamos una respuesta de error a ThingESP
    return "Mensaje invalido";
  }
}

// Bucle principal del programa, se ejecuta constantemente
void loop()
{
  // Esta función se encarga de verificar si hay nuevos comandos recibidos desde ThingESP
  // y automáticamente llama a HandleResponse() con el texto del comando
  thing.Handle();
}

