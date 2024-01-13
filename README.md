<p align="right"><img src="https://github.com/Tigermatze/Alarmanlage/assets/45244783/a70fd7b6-153b-412b-a6fb-e56f49879feb" width="212" height="73"></p>
    
# ESP8266 + RFID-RC522 + MQTT
Ziel ist ein kleines Einsteigerprojekt um mit Hile eines RFID Tags am Eingang eines Zimmers einige Smarthomeelemte wie z.B. Lichter anzusteuern.

## Einleitung
### ESP8266 D1 mini
Ich habe mich für dieses Board entschieden, weil es sehr klein ist und ich es noch liegen hatte. Außerdem hat es die notwendigen Pins, einen seriellen Programmieradapter und Wifi, also alles was ich benötige.

### RFID-RC522
Dieses Modul sowie zwei RFID-Tags waren Bestandteil eines Arduino Einsteigerkastens.
Wenn ein Tag vor das Lesegerät gehalten wird, dann wird der Code des Tags ausgelesen und kann verarbeitet werden.
![RFID_Tags](https://github.com/Tigermatze/ESP8266_RFID_MQTT/assets/45244783/e28cbd86-0240-4321-9d39-74f1495bb726)
<img RFID_Pins-q7ndmwl6f40dbbfnvzeblqxq1v43y6eehg3gw261ho.webp>

### MQTT
Als Smarthome Zentrale habe ich openhab laufen und alle Elemnte sind auch mit MQTT Kommandos ansteuerbar. Da dies einfach umzusetzen ist, werden wir das Protokoll auch hier verwenden.

## Anschluss & IDE
### Anschluss
Hier ist der Anschluss der Board dargestellt.
<img RFID_Wemos_D1_Mini.webp>
<img RFID_Anschluss_Wemos_D1.webp>

### IDE
Als IDE verwende ich Visual Studio Code (VSC) und die Erweiterung PlatformIO.

#### Ein Projekt erstellen
Zuerst erstellen wir ein neues Projekt. Dazu klicken wir im PlatformIO Home im Quick Access auf „New Project“, oder navigieren zum Reiter „Projects“ und klicken da auf „Create New Project“.

##### Neues PlatformIO Projekt in VS Code erstellen
Nach dem Klick öffnet sich der „Project Wizard“. In dem geben wir dem Projekt einen Namen und wählen die Plattform espressif8266, das Board "WeMos D1 R2 and mini (d1_mini)" und das Framework "arduino" aus.

##### PlatformIO Projekt nach dem Erstellen
Wie ihr seht hat PlatformIO für uns eine Ordnerstruktur angelegt und auch gleich eine .gitignore erstellt. Zusätzlich gibt es die Datei platformio.ini in der wir später noch Einstellungen für das Projekt vornehmen können.

##### Bibliothek installieren
Zum nutzen des RFID Moduls installieren wir die Bibliothek MFRC522 ("miguelbalboa/MFRC522@^1.4.11"). Dazu in PIO Home auf Libraries gehen, die Bibliothek suchen und dem Projekt hinzufügen.

