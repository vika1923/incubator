#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define RELAY_PIN 3
#define PUMP_PIN 4

const float TEMP_LOW = 35.0;
const float TEMP_HIGH = 39.0;
const float HUM_THRESHOLD = 60.0;

unsigned long pumpLastActivated = 0;
const unsigned long pumpCooldown = 600000;

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(PUMP_PIN, HIGH);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  if (temperature < TEMP_LOW) {
    digitalWrite(RELAY_PIN, LOW);
  } else if (temperature > TEMP_HIGH) {
    digitalWrite(RELAY_PIN, HIGH);
  }

  unsigned long currentMillis = millis();
  
  if (humidity < HUM_THRESHOLD && (currentMillis - pumpLastActivated >= pumpCooldown)) {
    digitalWrite(PUMP_PIN, LOW);
    delay(500);
    digitalWrite(PUMP_PIN, HIGH);
    pumpLastActivated = currentMillis;
  }

  delay(2000);
}
