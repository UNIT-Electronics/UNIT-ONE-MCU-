/* // Definición de pines
const int l298n_enable = 16;   // EN del L298N (D12 para la DUAL MCU ONE)
const int l298n_input1 = 19;  // IN1 del L298N (D11 para la DUAL MCU ONE)
const int l298n_input2 = 17; // IN2 del L298N (D10 para la DUAL MCU ONE)

void setup() {
  // Configura los pines como salida
  pinMode(l298n_enable, OUTPUT);
  pinMode(l298n_input1, OUTPUT);
  pinMode(l298n_input2, OUTPUT);
}

void loop() {
  // Habilita el motor
  digitalWrite(l298n_enable, HIGH);

  // Gira en un sentido
  digitalWrite(l298n_input1, HIGH);
  digitalWrite(l298n_input2, LOW);

  delay(2000); // Espera 20 segundos

  // Deshabilita el motor
  digitalWrite(l298n_enable, LOW);
  delay(1000); // Espera 1 segundo

  // Habilita el motor nuevamente
  digitalWrite(l298n_enable, HIGH);

  // Gira en el sentido contrario
  digitalWrite(l298n_input1, LOW);
  digitalWrite(l298n_input2, HIGH);

  delay(1000); // Espera 1 segundo

  // Inhabilita el motor
  digitalWrite(l298n_enable, LOW);
}
*/

// Pines para el L298N y el potenciómetro
const int l298n_enable = 16;   //  EN del L298N  (D12 para la DUAL MCU ONE)
const int l298n_input4 = 19;   //  IN1 del L298N (PWM) (D11 para la DUAL MCU ONE)
const int l298n_input3 = 17;   //  IN2 del L298N (D10 para la DUAL MCU ONE)
const int potPin = 26;         // Pin analógico hacia el potenciómetro (A0 para la DUAL MCU ONE)

void setup() {
  pinMode(l298n_enable, OUTPUT);
  pinMode(l298n_input3, OUTPUT);
  pinMode(l298n_input4, OUTPUT);

  digitalWrite(l298n_enable, HIGH); // Habilita el motor

  Serial.begin(9600);
}

void loop() {
  // Lee el valor del potenciómetro (0 a 1023 en Arduino Uno)
  int potValue = analogRead(potPin);

  // Mapea el valor leído al rango del PWM (0 a 255 en Arduino)
  int pwmValue = map(potValue, 0, 1023, 0, 255);

  // Apaga IN2 para girar el motor en una dirección
  digitalWrite(l298n_input3, LOW);

  // Aplica PWM a IN1
  analogWrite(l298n_input4, pwmValue);

  // Imprime el valor para depuración
  Serial.print("PWM = ");
  Serial.println(pwmValue);

  delay(100); // Espera 100ms antes de la siguiente lectura
}

