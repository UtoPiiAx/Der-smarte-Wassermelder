# Der-smarte-Wassermelder

GitHub-Repository des smarten Wassermelders | Semesterprojekt Gruppe 9

Mitglieder:

* Nicklas Wessel
* Christos Bletsogiannis
* Sven Graupner
* Marie Witte
* Mirko Lohmann

Dieses Projekt beinhaltet die Messung des Wasserstandes in einem Gefäß durch einen D1 Mini - ESP8266 mit einem Wasserstandsensor und basierend darauf die Benachrichtigung per E-Mail wie hoch dieser Wasserstand in dem jeweiligen Gefäß ist.

In diesem Respository befinden sich sämtliche zur Installation und Inbetriebnahme des smarten Wassermelders erforderlichen Dateien. 
Die Powerbank ist optional und dient lediglich als Stromzufuhr für den ESP8266 und kann alternativ auch durch 
eine andere Stromquelle ersetzt werden.

## Benötigte Hardware:
* D1 Mini - ESP8266
* Gehäuse für D1 Mini - ESP8266
* Wasserstandsensor
* Halterung für Wasserstandsensor
* Kabelhalterung
* Powerbank mit Solaraufladung
* Wasserdichtes Gefäß

## Benötigte Software:
* smarter_wassermelder.ino

## Verwendete Bibliotheken:
* WiFi Manager by tzapu: https://github.com/tzapu/WiFiManager
* ESP Mail Client by mobizt: https://github.com/mobizt/ESP-Mail-Client
