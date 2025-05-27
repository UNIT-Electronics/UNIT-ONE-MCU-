/*
Este programa configura el bus CAN (TWAI) en el ESP32, establece una conexión con un transceptor CAN, y envía un mensaje 
cada segundo. Además, monitorea errores y alertas del bus para asegurarse de que la comunicación esté funcionando correctamente.
*/

#include "driver/twai.h"  // Incluye la librería del controlador CAN (TWAI) para ESP32

// Definición de pines usados por el transceptor CAN
#define RX_PIN 26          // Pin para recepción (RX)
#define TX_PIN 25          // Pin para transmisión (TX)

// Tiempos de transmisión y sondeo en milisegundos
#define TRANSMIT_RATE_MS 1000  // Intervalo de envío de mensajes (1 segundo)
#define POLLING_RATE_MS 1000   // Intervalo para revisar alertas (1 segundo)

static bool driver_installed = false;  // Bandera para saber si el driver se instaló correctamente
unsigned long previousMillis = 0;      // Para llevar el control del tiempo entre envíos

void setup() {
  Serial.begin(115200);  // Inicia la comunicación serial para depuración

  // Configura los parámetros del bus CAN (TWAI)
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  // Velocidad de 500 kbps
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // Acepta todos los mensajes

  // Instala el driver TWAI con la configuración anterior
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("Driver instalado");
  } else {
    Serial.println("Error al instalar driver");
    return;
  }

  // Inicia el driver para comenzar a enviar/recibir mensajes CAN
  if (twai_start() == ESP_OK) {
    Serial.println("Driver iniciado");
  } else {
    Serial.println("Error al iniciar driver");
    return;
  }

  // Configura alertas que se quieren monitorear
  uint32_t alerts_to_enable = TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS |
                              TWAI_ALERT_TX_FAILED | TWAI_ALERT_ERR_PASS |
                              TWAI_ALERT_BUS_ERROR;
  if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
    Serial.println("Alertas configuradas");
  } else {
    Serial.println("Error al configurar alertas");
    return;
  }

  driver_installed = true;  // Marca que todo se ha configurado correctamente
}

// Función para enviar un mensaje por el bus CAN al ESP32 + Modulo MCP2515 
static void send_message1() {
  twai_message_t message;
  message.identifier = 0x123;          // ID del mensaje CAN
  message.data_length_code = 4;        // Longitud del mensaje: 4 bytes
  message.extd = 0;                    // Usa formato estándar (11 bits)
  message.rtr = 0;                     // No es una solicitud remota (RTR desactivado)

  // Rellena los datos del mensaje (1, 2, 3, 4)
  for (int i = 0; i < 4; i++) {
    message.data[i] = i + 1;
  }

  // Imprime los datos que se van a enviar
  Serial.print("Enviando: ");
  for (int i = 0; i < 4; i++) {
    Serial.printf("%02X ", message.data[i]);
  }
  Serial.println();

  // Envía el mensaje con un timeout de 1 segundo
  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.println("✅ Mensaje transmitido");
  } else {
    Serial.println("❌ Error al transmitir mensaje");
  }
}

// Función para enviar un mensaje por el bus CAN al ESP32 + Modulo MCP2515 
static void send_message2() {
  twai_message_t message;
  message.identifier = 0x0F6;          // ID del mensaje CAN
  message.data_length_code = 4;        // Longitud del mensaje: 4 bytes
  message.extd = 0;                    // Usa formato estándar (11 bits)
  message.rtr = 0;                     // No es una solicitud remota (RTR desactivado)

  // Rellena los datos del mensaje (1, 2, 3, 4)
  for (int i = 0; i < 4; i++) {
    message.data[i] = 4 - i;
  }

  // Imprime los datos que se van a enviar
  Serial.print("Enviando: ");
  for (int i = 0; i < 4; i++) {
    Serial.printf("%02X ", message.data[i]);
  }
  Serial.println();

  // Envía el mensaje con un timeout de 1 segundo
  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.println("✅ ✅Mensaje transmitido");
  } else {
    Serial.println("❌ ❌Error al transmitir mensaje");
  }
}

void loop() {
  // Si el driver no está instalado, espera
  if (!driver_installed) {
    delay(1000);
    return;
  }

  // Revisa las alertas activadas en el controlador CAN
  uint32_t alerts_triggered;
  twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(POLLING_RATE_MS));

  // Obtiene información del estado actual del bus CAN
  twai_status_info_t status;
  twai_get_status_info(&status);

  // Procesa las alertas capturadas
  if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
    Serial.println("⚠️ Controlador en estado de error pasivo.");
  }
  if (alerts_triggered & TWAI_ALERT_BUS_ERROR) {
    Serial.println("⚠️ Error en el bus CAN.");
    Serial.printf("Errores en el bus: %lu\n", status.bus_error_count);
  }
  if (alerts_triggered & TWAI_ALERT_TX_FAILED) {
    Serial.println("❌ Falló la transmisión.");
    Serial.printf("TX en cola: %lu | TX errores: %lu | TX fallidas: %lu\n",
                  status.msgs_to_tx, status.tx_error_counter, status.tx_failed_count);
  }
  if (alerts_triggered & TWAI_ALERT_TX_SUCCESS) {
    Serial.println("✅ Transmisión exitosa.");
  }

  // Envía un nuevo mensaje cada TRANSMIT_RATE_MS milisegundos
  if (millis() - previousMillis >= TRANSMIT_RATE_MS) {
    previousMillis = millis();
    send_message1();
    send_message2();
  }
}
