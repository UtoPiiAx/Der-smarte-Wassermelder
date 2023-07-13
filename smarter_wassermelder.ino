#include "Arduino.h"
#include <WiFiManager.h> // Bibliothek einbinden, um die Übergabe der WiFi Credentials über einen Access Point zu ermöglichen
#include <ESP_Mail_Client.h> // Bibliothek einbinden, um E-Mails versenden zu können

const int waterAnalogPin = A0; // Pin, an dem der Wasserstandsensor angeschlossen ist
const int waterSensorPowerPin = 4; // Pin für die Stromversorgung

// Deklaration und Initialisierung der Kalibrationswerte
const int waterLevelLow = 50; // Niedriger Wasserstand (minimaler Wert = 0)
const int waterLevelMiddle = 500; // Mittlerer Wasserstand (mittlerer Wert = 512)
const int waterLevelHigh = 900; // Hoher Wasserstand (maximaler Wert = 1023)

// Definition der Server Credentials für E-Mail (SMTP Protokoll)
#define SMTP_server "smtp.gmail.com" // SMTP-Server-Adresse
#define SMTP_Port 465 // SMTP-Server-Port
#define sender_email "smarterwassermelder@gmail.com" // Absender-E-Mail-Adresse
#define sender_password "App-Passwort hier eintragen" // Generiertes App-Passwort als Authentifizierungsalternative
#define Recipient_email "sven.graupner@uni-oldenburg.de" // Empfänger-E-Mail-Adresse

// Deklaration der globalen WiFi und E-Mail Objekte
WiFiManager wifiManager; // WiFi-Manager-Objekt zum Verwalten der WiFi-Verbindung
SMTPSession smtp; // SMTP-Sitzungsobjekt zum Versenden von E-Mails
SMTP_Message message; // E-Mail-Nachrichtenobjekt
ESP_Mail_Session session; // ESP-Mail-Sitzungsobjekt

void setup() {
  
  Serial.begin(115200);
  // Festlegen der definierten Pins für Input und Output
  pinMode(waterAnalogPin, INPUT);
  pinMode(waterSensorPowerPin, OUTPUT);
   
  // Falls es Probleme mit dem WiFi-Manager-Portal gibt, kann man dieses auch über die IP-Adresse 192.168.4.1 erreichen
  Serial.println("Verbindung zu der SSID 'Smarter Wassermelder' wird aufgebaut");
  wifiManager.autoConnect("Smarter Wassermelder"); // Verbindung zum WiFi-Manager-Hotspot herstellen
  delay(2000);

  // Initialisierung der Session mit den zuvor definierten Server Credentials
  session.server.host_name = SMTP_server;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "";

  message.sender.name = "Smarter Wassermelder"; // Absendername für die E-Mail
  message.sender.email = sender_email; // Absender-E-Mail-Adresse
  message.subject = "Aktueller Status des Wasserstands"; // Betreff der E-Mail
  message.addRecipient("Sven Graupner", Recipient_email); // Name und E-Mail-Adresse des Empfängers

  // Programm beenden, wenn keine Verbindung zum SMTP-Server hergestellt werden kann
  if (!smtp.connect(&session))
    Serial.println("Es konnte keine Verbindung zum SMTP-Server hergestellt werden");
    return;
}

void loop() {
  digitalWrite(waterSensorPowerPin, HIGH); // Schaltet den Strom für den Sensor ein
  delay(100);
  int waterAnalog = analogRead(waterAnalogPin); // Liest den analogen Wert des Wasserstandssensors
  String textMsg = "";
  Serial.print("Wasser Wert: ");
  Serial.print(waterAnalog);

  if (waterAnalog <= waterLevelLow) {
    Serial.println(" -> Kein Wasser");
    textMsg = "Wasserstand: " + String(waterAnalog) + " -> Der Wasserstand ist leer, daher muss jetzt Wasser nachgefüllt werden.";

  } else if (waterAnalog > waterLevelLow && waterAnalog <= waterLevelMiddle) {
      Serial.println(" -> Der Wasserstand ist tief");
      textMsg = "Wasserstand: " + String(waterAnalog) + " -> Der Wasserstand ist bereits tief, daher muss bald Wasser nachgefüllt werden.";
      
  } else if (waterAnalog > waterLevelMiddle && waterAnalog <= waterLevelHigh) {
      Serial.println(" -> Der Wasserstand ist mittel");
      textMsg = "Wasserstand: " + String(waterAnalog) + " -> Der Wasserstand ist mittel, daher muss aktuell noch kein Wasser nachgefüllt werden.";

  } else if (waterAnalog > waterLevelHigh) {
      Serial.println(" -> Der Wasserstand ist hoch");
      textMsg = "Wasserstand: " + String(waterAnalog) + " -> Der Wasserstand ist hoch, daher muss kein Wasser nachgefüllt werden.";
  }
  message.text.content = textMsg.c_str(); // Inhalt der E-Mail-Nachricht
  message.text.charSet = "utf-8"; // Zeichensatz der E-Mail-Nachricht
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit; // Übertragungsencoding der E-Mail-Nachricht

  if (!MailClient.sendMail(&smtp, &message, false)) { // E-Mail senden
      Serial.println("Die E-Mail konnte nicht versendet werden, der Grund dafür lautet: " + smtp.errorReason());
  }
  else {
      Serial.println("Die E-Mail wurde erfolgreich versendet");
  }
  digitalWrite(waterSensorPowerPin, LOW); // Schaltet den Strom für den Sensor aus
  delay(3600000); // Wartezeit zwischen den Messungen (1 Stunde = 3600000 Millisekunden)
}
