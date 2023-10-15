#pragma once

#define STATION_NAME "PollutionHorse Station #1"; // Name of current device station, used in Email as sender name.

// WiFi
#define SECRET_WIFI_SSID ""; // SSID must be visible
#define SECRET_WIFI_PASS "";  // your network password

// Mail sender info
#define SECRET_EMAIL ""
#define SECRET_EMAIL_PASS ""
#define SECRET_EMAIL_SMTP_HOST "smtp.office365.com"; // for outlook.com
#define SECRET_EMAIL_SMTP_PORT 587; // for TLS with STARTTLS or 25 (Plain/TLS with STARTTLS) or 465 (SSL)

// Mail recepients
String email_receivers[2] = {"user@gmail.com", "user2@gmail.com"};