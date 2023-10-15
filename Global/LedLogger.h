#pragma once
// Logger to UNO R4 WiFi Led Matrix:
// https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix
// For eliminate all logger stuff use: #define LEDLOGGER_DISSABLE

#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

class LedLogger {
#ifndef LEDLOGGER_DISSABLE
  ArduinoLEDMatrix m_matrix;
  bool isEnable = false;
#endif

public:
	void init() {
#ifndef LEDLOGGER_DISSABLE
    m_matrix.begin();

    isEnable = true;
#endif
	}

  // enable logger in runtime
  void enable() {
#ifndef LEDLOGGER_DISSABLE
    isEnable = true;
#endif
  }

  // disable logger in runtime
  void disable() {
#ifndef LEDLOGGER_DISSABLE
    isEnable = false;
#endif
  }

  // Show static message
  void show(const String& msg) {
#ifndef LEDLOGGER_DISSABLE
    if(!isEnable) {
      return;
    }

    m_matrix.clear();
    m_matrix.beginDraw();
    m_matrix.stroke(0xFFFFFFFF);
    m_matrix.textFont(Font_4x6);
    m_matrix.beginText(0, 1, 0xFFFFFF);
    m_matrix.println(msg);
    m_matrix.endText();
    m_matrix.endDraw();
#endif
  }

  // Show scrolling message
  // Important: this method freeze main thread until animation complete.
  // isFreeze: block execution, used for whow errors, etc.
  void show_scroll(const String& msg, bool isFreeze=true) {
#ifndef LEDLOGGER_DISSABLE
    if(!isEnable) {
      return;
    }

    String sm = String("   ") + msg + String("   ");

    // Make it scroll!
    m_matrix.clear();

    do {
      m_matrix.beginDraw();
      m_matrix.stroke(0xFFFFFFFF);
      m_matrix.textScrollSpeed(70);
      m_matrix.textFont(Font_4x6);
      m_matrix.beginText(0, 1, 0xFFFFFF);
      m_matrix.println(sm);
      m_matrix.endText(SCROLL_LEFT);
      m_matrix.endDraw();
    }
    while(isFreeze);

#endif
  }

/*
void drawCPM(signed int cpm) {
  char text_buffer[60];
  String(cpm).toCharArray(text_buffer, 60);

  matrix.clear();
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(cpm);
  matrix.endText();
  matrix.endDraw();
}

// Draw int with animation
// BUT this animation freeze main thread
void drawAnimationCPM(signed int cpm) {
  char text_buffer[60];
  String(String("   ") + String(cpm) + String("   ")).toCharArray(text_buffer, 60);

  // put your main code here, to run repeatedly:
  // Make it scroll!
  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(70);

  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text_buffer);
  //matrix.print(String("   142   ").c_str());
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
}
*/

};
