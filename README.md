# ESP8266-SOLAR-MONITOR

Ein modular aufgebauter Solar-Monitor für Pool-Solaranlagen auf Basis eines ESP8266 (Wemos D1 mini).

## Features

* 🌡️ 3× DS18B20 Temperatursensoren
* 📊 Live-Webdashboard
* 📈 Temperaturverlauf (History)
* 📡 MQTT-Anbindung
* ⚙️ WLAN-Konfiguration über Webinterface
* 🔄 Automatischer Access-Point beim Erststart
* 🧠 SolarLogic für die spätere Pumpensteuerung
* 🏗️ Modulare Softwarearchitektur

## Hardware

* Wemos D1 mini (ESP8266)
* 3× DS18B20
* 4,7 kΩ Pull-up-Widerstand
* Relaismodul (optional)

## Projektstatus

Aktuelle Version: **v0.1.0**

### Bereits umgesetzt

* WLAN-Setup
* Webserver
* Dashboard
* MQTT
* History
* ConfigManager
* SolarLogic (Grundstruktur)

### Roadmap

* [ ] Sensorzuordnung über DS18B20-ROM-ID
* [ ] Pumpensteuerung
* [ ] Dashboard 2.0
* [ ] OTA über Webinterface
* [ ] Home Assistant Discovery
* [ ] Datenexport (CSV)

## Lizenz

MIT License
