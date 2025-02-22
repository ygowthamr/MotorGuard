/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

#define BLYNK_TEMPLATE_ID "TMPL3jTGOrAiC"
#define BLYNK_TEMPLATE_NAME "Relay"
#define BLYNK_AUTH_TOKEN "qyh3mD9HmeJO1Vm6Yzs4ZU5Qto3VGj1_"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <LiquidCrystal.h>
const int rs = D1, en = D2, d4 = D3, d5 = D4, d6 = D5, d7 = D6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define relay_pin D7
#define buzzer_pin D8


// Wi-Fi credentials
char ssid[] = "TEMPMON_5G";
char pass[] = "DUHACKS@12345";

float vref = 3.3;
float resolution = vref / 1023;

BLYNK_WRITE(V7) {
  int switchState = param.asInt();       // Get the switch state (0 or 1)
  digitalWrite(relay_pin, switchState);  // Control the relay/LED
  Serial.print("Switch state on V7: ");
  Serial.println(switchState);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to Blynk server");

  pinMode(relay_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  digitalWrite(buzzer_pin, HIGH);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("TEMP:");
}

// the loop function runs over and over again forever
void loop() {


  float temperature = analogRead(A0);
  temperature = (temperature * resolution);
  temperature = temperature * 100;
  lcd.setCursor(1, 1);
  lcd.print(temperature);
  delay(1000);

  if (temperature > 45) {
    lcd.clear();
    lcd.print("OVER TEMP...");
    digitalWrite(relay_pin, HIGH);
    digitalWrite(buzzer_pin, LOW);
    delay(5000);
    lcd.clear();
    lcd.print("TEMP:");
    digitalWrite(buzzer_pin, HIGH);
  }

  Blynk.run();  // Handle Blynk events
}
