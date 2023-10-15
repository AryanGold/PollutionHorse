### PollutionHorse

Arduino based project for outdoor radiation monitoring with Email notification.<br>
Algorythm for compute CPM optimized for quick detection with simple peak elimination filter.

Hardware:
- Arduino UNO R4 WiFi<br>
- RadiationD-v1.1 (or similar)<br>

#### Usage

1
`git clone https://github.com/AryanGold/PollutionHorse.git && cd PollutionHorse`

2
Set WiFi and Email SMTP options in the file "arduino_secrets.h":<br>
`cp arduino_secrets_default.h arduino_secrets.h`

3
Set alarm threshold in CPM (Counts Per Minute)<br> "PollutionHorse.ino -> alarm_radiation_cpm_threshold" (default 100 CPM = 0.57 uSv/hr).


#### Extra
- For boards without Led Matrix just remove class "LedLogger".


#### Docs
- [Arduino UNO R4 WiFi Cheat Sheet](https://docs.arduino.cc/tutorials/uno-r4-wifi/cheat-sheet)<br>
- [UNO R4 WiFi Network Examples](https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples)<br>
- [ESP-Mail-Client](https://github.com/mobizt/ESP-Mail-Client)<br>
- [Geiger-Counter-RadiationD-v1.1-CAJOE](https://github.com/SensorsIot/Geiger-Counter-RadiationD-v1.1-CAJOE-)<br>

#### Similar projects
- [DIY Geiger Counter with SensorOcean](https://www.hackster.io/ruslan-olkhovsky/diy-geiger-counter-with-sensorocean-a76d1a)<br>
- [https://github.com/bmellink/GeigerCounter](https://github.com/bmellink/GeigerCounter)<br>
