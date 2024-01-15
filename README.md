<p align="right"><img src="https://github.com/Tigermatze/Alarmanlage/assets/45244783/a70fd7b6-153b-412b-a6fb-e56f49879feb" width="212" height="73"></p>
    
# ESP8266 + RFID-RC522 + MQTT
Ziel ist ein kleines Einsteigerprojekt um mit Hile eines RFID Tags am Eingang eines Zimmers einige Smarthomeelemte wie z.B. Lichter anzusteuern.

## Einleitung
### ESP8266 D1 mini
Ich habe mich für dieses Board entschieden, weil es sehr klein ist und ich es noch liegen hatte. Außerdem hat es die notwendigen Pins, einen seriellen Programmieradapter und Wifi, also alles was ich benötige.

### Systemübersicht
<img alt="Systemuebersicht" src="https://github.com/Tigermatze/ESP8266_RFID_MQTT/assets/45244783/43571cf6-51ae-4cbd-9f35-620c5cc53840" width="40%"/>

Als Basis dient mein Server, auf dem die notwendigen Services (MQTT Broker, openhab, zigbee2mqtt) laufen. Openhab ist mein Hausautomatisierungs-System. \
Der ESP8266 soll keinerlei Entscheidungen treffen, sondern die Informationen via MQTT Protokoll an openhab senden. In openhab erstelle ich die Regeln welche Lichter geschaltet werden sollen. Das hat den Vorteil, dass ich Änderungen flexibel vornehmen kann ohne jedesmal den ESP neu flashen zu müssen. \
Allgemein werden einige Lichter aktiviert solange ein RFID-Tag auf dem Leser liegt, wird das Tag entfernt werden auch die Lichter deaktiviert.

### RFID-RC522
Dieses Modul sowie zwei RFID-Tags waren Bestandteil eines Arduino Einsteigerkastens.
Wenn ein Tag vor das Lesegerät gehalten wird, dann wird der Code des Tags ausgelesen und kann verarbeitet werden.
<img alt="RFID TAG" src="https://github.com/Tigermatze/ESP8266_RFID_MQTT/assets/45244783/e28cbd86-0240-4321-9d39-74f1495bb726" width="40%"/>

<img alt="RFID Pins" src="https://github.com/Tigermatze/ESP8266_RFID_MQTT/assets/45244783/3d77ac0f-b2cb-4c08-be4e-fba1fa5af0ef" />

### MQTT
Da ich viele Devices via MQTT ansteuern kann bzw. dessen Status empfange, das Protkoll sehr einfach ist, werden wir es auch hier verwenden.
Die Daten der Karte und dessen Status wird im JSON Format übertragen.

## Anschluss & IDE
### Anschluss
Hier ist der Anschluss der Board dargestellt.\
<img alt="RFID_Wemos_D1_Mini" src="https://github.com/Tigermatze/ESP8266_RFID_MQTT/assets/45244783/e3f1fe85-8f4e-4fd8-bb20-a9bbad68554c" />

|RC522 | ESP |
|-----------|-------|
| SDA | D8 |
| SCK | D5 |
| MOSI | D7 |
| MISO | D6 |
| IRQ | - |
| GND | GND |
| RST | D3 |
| 3.3V | 3.3V |

### IDE
Als IDE verwende ich Visual Studio Code (VSC) und die Erweiterung PlatformIO.

#### Ein Projekt erstellen
Zuerst erstellen wir ein neues Projekt. Dazu klicken wir im PlatformIO Home im Quick Access auf „New Project“, oder navigieren zum Reiter „Projects“ und klicken da auf „Create New Project“.

##### Neues PlatformIO Projekt in VS Code erstellen
Nach dem Klick öffnet sich der „Project Wizard“. In dem geben wir dem Projekt einen Namen und wählen die Plattform espressif8266, das Board "WeMos D1 R2 and mini (d1_mini)" und das Framework "arduino" aus.

##### PlatformIO Projekt nach dem Erstellen
Wie ihr seht hat PlatformIO für uns eine Ordnerstruktur angelegt und auch gleich eine .gitignore erstellt. Zusätzlich gibt es die Datei platformio.ini in der wir später noch Einstellungen für das Projekt vornehmen können.

##### Bibliothek installieren
Um Bibliotheken zu installieren einfach in PIO Home auf Libraries gehen, die Bibliothek suchen und dem Projekt hinzufügen. Dies wird dann der Projektdatei "platform.ini" hinzugefügt.

Folgende Bibliotheken müssen hinzugefügt werden:
* miguelbalboa/MFRC522@^1.4.11
* knolleary/PubSubClient@^2.8
* bblanchon/ArduinoJson@^6.21.5



# Quellen & Verweise
[Zugriff auf Transponder](https://elektro.turanis.de/html/prj102/index.html#ZugriffaufdenTransponder) \
[Alles über ESP](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)