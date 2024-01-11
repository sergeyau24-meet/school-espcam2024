#define AHT10_SDA 21
#define AHT10_SCL 22
#define AHT10_I2C_ADDRESS 0x38

void i2cStart() {
  pinMode(AHT10_SDA, OUTPUT);
  digitalWrite(AHT10_SDA, HIGH);
  digitalWrite(AHT10_SCL, HIGH);
  delayMicroseconds(5);
  digitalWrite(AHT10_SDA, LOW);
  delayMicroseconds(5);
  digitalWrite(AHT10_SCL, LOW);
  delayMicroseconds(5);
}

void i2cStop() {
  pinMode(AHT10_SDA, OUTPUT);
  digitalWrite(AHT10_SCL, LOW);
  digitalWrite(AHT10_SDA, LOW);
  delayMicroseconds(5);
  digitalWrite(AHT10_SCL, HIGH);
  delayMicroseconds(5);
  digitalWrite(AHT10_SDA, HIGH);
  delayMicroseconds(5);
}

void i2cWriteByte(uint8_t data) {
  pinMode(AHT10_SDA, OUTPUT);
  for (int i = 0; i < 8; i++) {
    digitalWrite(AHT10_SCL, LOW);
    delayMicroseconds(2);
    digitalWrite(AHT10_SDA, (data & 0x80) ? HIGH : LOW);
    delayMicroseconds(2);
    digitalWrite(AHT10_SCL, HIGH);
    delayMicroseconds(2);
    data <<= 1;
  }
  digitalWrite(AHT10_SCL, LOW);
  delayMicroseconds(2);
  pinMode(AHT10_SDA, INPUT);
  while (digitalRead(AHT10_SDA) == HIGH);
  delayMicroseconds(2);
  digitalWrite(AHT10_SCL, HIGH);
  delayMicroseconds(2);
  digitalWrite(AHT10_SCL, LOW);
}

uint8_t i2cReadByte(bool ack) {
  uint8_t data = 0;
  pinMode(AHT10_SDA, INPUT);
  for (int i = 0; i < 8; i++) {
    digitalWrite(AHT10_SCL, HIGH);
    delayMicroseconds(2);
    data = (data << 1) | digitalRead(AHT10_SDA);
    digitalWrite(AHT10_SCL, LOW);
    delayMicroseconds(2);
  }
  pinMode(AHT10_SDA, OUTPUT);
  digitalWrite(AHT10_SDA, ack ? LOW : HIGH);
  delayMicroseconds(2);
  digitalWrite(AHT10_SCL, HIGH);
  delayMicroseconds(2);
  digitalWrite(AHT10_SCL, LOW);
  digitalWrite(AHT10_SDA, HIGH);
  delayMicroseconds(2);
  return data;
}

void aht10Reset() {
  i2cStart();
  i2cWriteByte(0xBA); // AHT10 soft reset command
  i2cStop();
}

bool readAHT10Data(float& temperature, float& humidity) {
  aht10Reset();

  i2cStart();
  i2cWriteByte(0xAC); // AHT10 read command
  i2cStop();

  delay(75); // Wait for sensor to process data (datasheet recommends 75ms)

  i2cStart();
  i2cWriteByte(0x71); // AHT10 read data command
  delayMicroseconds(50); // Wait for sensor to process data
  uint8_t data[6];
  for (int i = 0; i < 6; i++) {
    data[i] = i2cReadByte(i < 5);
  }
  i2cStop();

  // Convert data to temperature and humidity
  int32_t rawTemperature = ((int32_t)data[3] << 16) | ((int32_t)data[4] << 8) | data[5];
  int32_t rawHumidity = ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) | (data[2] >> 4);

  temperature = (float)rawTemperature * 200.0 / 1048576.0 - 50.0;
  humidity = (float)rawHumidity * 100.0 / 1048576.0;

  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(AHT10_SDA, INPUT_PULLUP);
  pinMode(AHT10_SCL, INPUT_PULLUP);

  delay(500); // Give some time for sensor initialization
}

void loop() {
  float temperature, humidity;

  if (readAHT10Data(temperature, humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    delay(2000); // Delay for 2 seconds before reading again
  } else {
    Serial.println("Failed to read data from AHT10");
  }
}
