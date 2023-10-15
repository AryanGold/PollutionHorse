#pragma once
// Logger to Serial
// For eliminate all logger stuff use: #define LOGGER_DISSABLE

class Logger {
#ifndef LOGGER_DISSABLE
  bool isEnable = false;
#endif

public:
	void init() {
#ifndef LOGGER_DISSABLE
    Serial.begin(9600);

    // wait for serial port to connect. Needed for native USB port only
    while (!Serial) {
      ; 
    }
    delay(1000);

    isEnable = true;
#endif
	}

  // enable logger in runtime
  void enable() {
#ifndef LOGGER_DISSABLE
    isEnable = true;
#endif
  }

  // disable logger in runtime
  void disable() {
#ifndef LOGGER_DISSABLE
    isEnable = false;
#endif
  }

  void show(const String& msg) const {
#ifndef LOGGER_DISSABLE
      if(!isEnable) {
        return;
      }

      Serial.println(msg);
#endif
  }
};
