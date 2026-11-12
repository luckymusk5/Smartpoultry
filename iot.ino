 #include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Servo.h>

// --- Définition des broches ---
#define RELAY_VENT 7
#define RELAY_LAMP 8
#define TRIG_PIN 4
#define ECHO_PIN 5
#define DHTPIN 6
#define WATER_PIN A0
#define SERVO_PIN 3
#define DHTTYPE DHT11

// --- Initialisation des modules ---
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo monServo;

void setup() {
  // Initialisation LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Systeme Init...");
  
  // Initialisation des modules
  Serial.begin(9600);
  dht.begin();
  monServo.attach(SERVO_PIN);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_VENT, OUTPUT);
  pinMode(RELAY_LAMP, OUTPUT);
  
  // Éteindre relais au départ
  digitalWrite(RELAY_VENT, LOW);
  digitalWrite(RELAY_LAMP, LOW);

  delay(2000);
  lcd.clear();
}

void loop() {
  // --- Lecture température et humidité ---
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C | Hum: ");
  Serial.print(hum);
  Serial.println(" %");

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C H:");
  lcd.print(hum);
  lcd.print("% ");
  
  // --- Lecture niveau d'eau ---
  int waterValue = analogRead(WATER_PIN);
  int volumePourcent = map(waterValue, 0, 1023, 0, 100);

  Serial.print("Niveau d'eau : ");
  Serial.print(volumePourcent);
  Serial.println(" %");

  lcd.setCursor(0, 1);
  lcd.print("Eau:");
  lcd.print(volumePourcent);
  lcd.print("% ");

  delay(1000);

  // --- Mesure distance à l'ultrason ---
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- Mouvement du servo moteur ---
  monServo.write(0);
  delay(1000);
  monServo.write(180);
  delay(1000);

  // --- Commande relais lampe et ventilateur ---
  digitalWrite(RELAY_LAMP, HIGH); // Allume la lampe
  digitalWrite(RELAY_VENT, LOW);  // Éteint le ventilateur
  delay(1000000);

  digitalWrite(RELAY_LAMP, LOW);  // Éteint la lampe
  digitalWrite(RELAY_VENT, HIGH); // Allume le ventilateur
  delay(100000);
}
