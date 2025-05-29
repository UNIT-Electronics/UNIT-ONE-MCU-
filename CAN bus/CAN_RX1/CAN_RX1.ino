/* 
  Documentacion adicional
   https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html
 
*/

// Ignorar advertencias por campos no inicializados
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "driver/twai.h"

// Definir pines del ESP32 para RX y TX del transceptor CAN
// Estos pines pertenecen al ESP32 integrado en la UNIT DUAL MCU ONE e internamente estan conectados al tranceptor CAN integrado en la placa
// para mas detalles revise la pagina 3 del esquematico que puede encontrar en el siguiente enlace  https://uelectronics.com/wp-content/uploads/2025/01/AR4352-UNIT-DualMCU-ONE-Esquematico.pdf
#define RX_PIN 26
#define TX_PIN 25

// Intervalo de sondeo para leer alertas del bus (en milisegundos)
#define POLLING_RATE_MS 1000

// Variable para saber si el driver se instaló correctamente
static bool driver_installed = false;

void setup() {
  Serial.begin(115200);  // Inicia comunicación serie

  // Configuración general del controlador TWAI (CAN)
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_PIN, (gpio_num_t)RX_PIN, TWAI_MODE_NORMAL);

  // Configuración de velocidad del bus: 500 kbps
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();

  // Aceptar todos los mensajes sin filtrar
// Filtro para aceptar solo el ID 0x0F6 (estándar, 11 bits)
twai_filter_config_t f_config = {
  .acceptance_code = (0x0F6 << 21),  // ID en bits más significativos
  .acceptance_mask = ~(0x7FF << 21), // Solo comparar los 11 bits del ID estándar
  .single_filter = true
};


  // Instalar el driver TWAI
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("Driver instalado");
  } else {
    Serial.println("Error al instalar driver");
    return;
  }

  // Iniciar el controlador CAN
  if (twai_start() == ESP_OK) {
    Serial.println("Driver iniciado");
  } else {
    Serial.println("Error al iniciar driver");
    return;
  }

  // Configurar alertas para eventos importantes del bus
  uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR | TWAI_ALERT_RX_QUEUE_FULL;
  if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
    Serial.println("Alertas CAN configuradas");
  } else {
    Serial.println("Error al configurar alertas");
    return;
  }

  // Marcar que el driver está instalado correctamente
  driver_installed = true;
}

// Función que imprime por serie la información de un mensaje recibido
static void handle_rx_message(twai_message_t &message) {
  if (message.extd) {
    Serial.println("Mensaje en formato extendido");
  } else {
    Serial.println("Mensaje en formato estándar");
  }

  // Imprimir el ID del mensaje
  Serial.printf("ID: %lx\nBytes:", message.identifier);

  // Si no es un mensaje de solicitud remota (RTR), imprimir datos
  if (!message.rtr) {
    for (int i = 0; i < message.data_length_code; i++) {
      Serial.printf(" %d = %02x,", i, message.data[i]);
    }
    Serial.println("");
  }
}

void loop() {
  // Si el driver no está instalado, esperar
  if (!driver_installed) {
    delay(1000);
    return;
  }

  // Leer alertas activadas
  uint32_t alerts_triggered;
  twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(POLLING_RATE_MS));

  // Obtener estado del controlador TWAI
  twai_status_info_t twaistatus;
  twai_get_status_info(&twaistatus);

  // Manejar diferentes alertas posibles
  if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
    Serial.println("Alerta: controlador en estado pasivo por errores.");
  }
  if (alerts_triggered & TWAI_ALERT_BUS_ERROR) {
    Serial.println("Alerta: error en el bus CAN.");
    Serial.printf("Contador de errores en bus: %lu\n", twaistatus.bus_error_count);
  }
  if (alerts_triggered & TWAI_ALERT_RX_QUEUE_FULL) {
    Serial.println("Alerta: cola de recepción llena, mensaje perdido.");
    Serial.printf("RX buffer: %lu\t", twaistatus.msgs_to_rx);
    Serial.printf("RX perdidos: %lu\t", twaistatus.rx_missed_count);
    Serial.printf("RX overrun: %lu\n", twaistatus.rx_overrun_count);
  }

  // Si hay datos disponibles, procesar todos los mensajes recibidos
  if (alerts_triggered & TWAI_ALERT_RX_DATA) {
    twai_message_t message;
    while (twai_receive(&message, 0) == ESP_OK) {
      handle_rx_message(message);
    }
  }
}
