#include <SPI.h>
// Previamente debes descargar el .zip e instalar en tu Arduino IDE la siguiente librereia, misma que te deje en el repositorio :3
#include <mcp_can.h>

// Pines del MCP2515 (ajusta si es necesario)
#define CAN_CS 5        // Pin CS (Chip Select) del MCP2515
#define CAN_INT 4       // Pin de interrupción del MCP2515

// ID que se quiere escuchar (puedes cambiarlo)
#define MY_CAN_ID 0x123

MCP_CAN CAN(CAN_CS);    // Instancia del objeto CAN

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ)) {
    Serial.println("Inicializando MCP2515 falló. Reintentando...");
    delay(1000);
  }
  Serial.println("MCP2515 Inicializado correctamente!");

  // Configura modo normal
  CAN.setMode(MCP_NORMAL);

  // Aplica filtro para solo recibir mensajes con ID MY_CAN_ID
  // Filtro 0, máscara 0 -> Se filtra exactamente ese ID (11 bits estándar)
  CAN.init_Mask(0, 0, 0x7FF);        // Máscara: todos los bits relevantes
  CAN.init_Filt(0, 0, MY_CAN_ID);    // Filtro 0 acepta solo MY_CAN_ID

  Serial.printf("Escuchando mensajes con ID 0x%03X\n", MY_CAN_ID);
}

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    long unsigned int rxId;
    unsigned char len = 0;
    unsigned char rxBuf[8];

    CAN.readMsgBuf(&rxId, &len, rxBuf);

    if (rxId == MY_CAN_ID) {
      Serial.print("Mensaje recibido con ID 0x");
      Serial.print(rxId, HEX);
      Serial.print(": ");

      for (int i = 0; i < len; i++) {
        Serial.printf("%02X ", rxBuf[i]);
      }
      Serial.println();
    }
  }
}
