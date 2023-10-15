#pragma once
// https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples
// https://github.com/mobizt/ESP-Mail-Client

#include "arduino_secrets.h"

// Include WiFi library
#include "WiFiS3.h"

////////////
// Include ESP Mail Client library
// https://github.com/mobizt/ESP-Mail-Client
// This library very long compiling, you may eliminate it for quick testing other functional.
// For eliminate ESP_Mail_Client stuff use: #define ESPMAILCLIENT_DISSABLE
//#define ESPMAILCLIENT_DISSABLE
#ifndef ESPMAILCLIENT_DISSABLE
#include <ESP_Mail_Client.h>

// Callback function to get the Email sending status
void smtpCallback(SMTP_Status status);

#endif
////////////

class MAILSender {
public:
  static constexpr char ssid[] = SECRET_WIFI_SSID; // SSID must be visible
  static constexpr char pass[] = SECRET_WIFI_PASS;  // your network password

  static constexpr char email_sender[] = SECRET_EMAIL_SENDER;
  static constexpr char email_pass[] = SECRET_EMAIL_PASS;

  int status = WL_IDLE_STATUS;     // the Wifi radio's status

private:
  Logger* m_logger;
  LedLogger* m_ledLogger;

  ////////
  // Mail stuff
#ifndef ESPMAILCLIENT_DISSABLE
  // Declare the global used SMTPSession object for SMTP transport
  SMTPSession smtp;

  // Declare the global used Session_Config for user defined session credentials
  Session_Config config;
#endif
  ///////

public:
	bool init(Logger* logger, LedLogger* ledLogger) {
    m_logger = logger;
    m_ledLogger = ledLogger;

		// check for the presence of the shield:
		if(WiFi.status() == WL_NO_MODULE) 
		{
      m_logger->show("Error[MAILSender->init()] Communication with WiFi module failed!");
      m_ledLogger->show_scroll("Error: no WiFi module!");
      return false;
		}

		String fv = WiFi.firmwareVersion();
		m_logger->show(String("WiFi Firmware v") + fv);

    // attempt to connect to Wifi network:
    int attempt = 0;
    while (status != WL_CONNECTED) {      
      m_logger->show(String("WiFi Attempting to connect to WPA SSID: ") + ssid);
      m_ledLogger->show(String("wf") + String(attempt));

      // Connect to WPA/WPA2 network:
      status = WiFi.begin(ssid, pass);

      m_logger->show(String("WiFi Connection status: ") + String(status));

      attempt++;
      if(attempt > 5) {
        m_logger->show("Error[MAILSender->init()] Unable to connect to WiFi.");
        m_ledLogger->show_scroll("Error: WiFi no connection");
        return false;
      }

      // wait n seconds for connection:
      delay(3000);
    }

    m_logger->show(String("WiFi You're connected to the network, local IP: ") + WiFi.localIP().toString());
    m_ledLogger->show("");

    return true;
	}

  void send_mail(const String receivers[], const size_t receivers_numb, const String& subject, const String& msg) {
#ifndef ESPMAILCLIENT_DISSABLE
    // Set the session config
    config.server.host_name = SECRET_EMAIL_SMTP_HOST; // for outlook.com
    config.server.port = SECRET_EMAIL_SMTP_PORT; // for TLS with STARTTLS or 25 (Plain/TLS with STARTTLS) or 465 (SSL)
    config.login.email = email_sender; // set to empty for no SMTP Authentication
    config.login.password = email_pass; // set to empty for no SMTP Authentication

    // For client identity, assign invalid string can cause server rejection
    // client domain or public ip
    config.login.user_domain = "whitemail.outlook.com"; 

    /*
    Set the NTP config time
    For times east of the Prime Meridian use 0-12
    For times west of the Prime Meridian add 12 to the offset.
    Ex. American/Denver GMT would be -6. 6 + 12 = 18
    See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
    */
    /*
    config.time.ntp_server = "pool.ntp.org,time.nist.gov";
    config.time.gmt_offset = 3;
    config.time.day_light_offset = 0;
    */

    // Declare the SMTP_Message class variable to handle to message being transport
    SMTP_Message message;

    // Set the message headers
    message.sender.name = STATION_NAME;
    message.sender.email = email_sender;
    message.subject = subject;

    for(int i=0; i < receivers_numb; i++) {
      //String user = String("User_") + String(i);
      message.addRecipient("", receivers[i]);
    }

    // Set the message content
    message.text.content = msg;

    // Set the callback function to get the sending results
    // We not use it because we not manage mails errors/success.
    ///smtp.callback(smtpCallback);

    // Connect to the server
    smtp.connect(&config);

    // Start sending Email and close the session
    if (!MailClient.sendMail(&smtp, &message)) {
      m_logger->show(String("Error[MAILSender->send_mail()] sending Email: ") + smtp.errorReason());
      m_ledLogger->show("sm");
    }
#endif
  }
};

#ifndef ESPMAILCLIENT_DISSABLE
void smtpCallback(SMTP_Status status)
{
  // Show mail sending info
  Serial.println(status.info());

  if (status.success())
  {
  }
}
#endif