#include "Global/Logger.h"
#include "Global/LedLogger.h"
#include "MAILSender.h"

Logger logger;
LedLogger ledLogger;
MAILSender mailSender;

////////
// Gcounter
const int CounterPinDigital = 2;  // Digital pin on aurdino

// we use time window for fitering peaks of CPM.
// milliseconds in current time average window
// less value = more quickly reacting but have more diverse CPM
// more value = slowly react on increased radiation level.
const signed int ms_in_window = 5000;   

const signed int timeResolution = 500;  // How frequnetly we read acumulated counter
const signed int DrawTime = 2000 / timeResolution; // We print text on led matrix with this delay, 2s
const signed int windows_size = ms_in_window / timeResolution;
signed int cpw = 0; // Counters per window
signed int time_buffer[windows_size];
signed int prev_time = 0;
signed int prev_counter = 0;
unsigned int delta_index = 0;
signed int CounterVal = 0;

const unsigned int alarm_radiation_cpm_threshold = 100; // CPM threshold for enable alarm
const unsigned int alarm_time_threshold = 3000 / timeResolution; // How long should be hight level radiation before start alarm.
unsigned int alarm_counter = 0;

bool isSilentMode = false; // After send alarm mails we enable silent mode for prevent send many mails.
const unsigned int silentMode_time_threshold = 3600000 / timeResolution; // How long silent mode, 1 hour
unsigned int silentMode_counter = 0;
////////

void setup() {
  logger.init();
  ledLogger.init();

  ////////
  // Gcounter
  pinMode(CounterPinDigital, INPUT);
  attachInterrupt(digitalPinToInterrupt(CounterPinDigital), onCounter, FALLING);
  ////////

  mailSender.init(&logger, &ledLogger);
}

void loop() {
  /////////////////
  signed int curr_time = millis();
  signed int time_delta = curr_time - prev_time;

  // Every 50 days millis overloaded and return 0 again.
  if(time_delta < 0) {
    reset();
    return;
  }

  if (time_delta >= timeResolution) {
    //////////
    // CPM - counter per minute, counter delta everaged mer minute
    signed int cpm_averaged = 0; // Filtered CPM in time window
    signed int counter_delta = CounterVal - prev_counter; // Current counters
    signed int cpm_ex = 60000 * counter_delta / time_delta; // CPM extrapolated to one minute

    int buffer_index = delta_index % windows_size;

    // We use filter for eliminate peaks for CPM
    // We use time seried filter for average CPM per time window
    signed int outdated_value = time_buffer[buffer_index];
    time_buffer[buffer_index] = cpm_ex;
    cpw += cpm_ex;

    if(delta_index < windows_size) 
    {
      cpm_averaged = cpw / (delta_index + 1);
    } 
    else 
    {
      cpw -= outdated_value;
      cpm_averaged = cpw / windows_size;
    }
    //////////

    prev_time = curr_time;
    prev_counter = CounterVal;

    // Debug
    // Serial.print("delta_index[");    Serial.print(delta_index);     Serial.print("] ");
    // Serial.print("counter_delta[");    Serial.print(counter_delta);     Serial.print("] ");
    // Serial.print("cpm_averaged[");    Serial.print(cpm_averaged);     Serial.print("] ");
    // Serial.println("");

    /////////////////
    // Show current CPM in led matrix
    if(delta_index % DrawTime == 0) {
      drawCPM(cpm_averaged);
    }
    /////////////////

    /////////////////
    // if radiation level is hight then we enable alarm.
    // Alarm sends mails to recepitients.
    if(!isSilentMode) {
      if(cpm_averaged > alarm_radiation_cpm_threshold) {
        alarm_counter++;

        if(alarm_counter > alarm_time_threshold) {
          String msg = String("Detected hight radiation level! CPM[") + String(cpm_averaged) + String("]");

          logger.show(msg);

          size_t receivers_numb = sizeof(email_receivers) / sizeof(*email_receivers);
          mailSender.send_mail(email_receivers, receivers_numb, "Alert", msg);

          alarm_counter = 0;
          isSilentMode = true; // After send alarm mails we enable silent mode for prevent send many mails.
        }
      }
      else {
        // Reset counter if in current measuring radiation not hight
        alarm_counter = 0;
      }
    }
    else {
      // Silent mode, is this mode we pause sending mails

      silentMode_counter++;

      // Disable silent mode
      if(silentMode_counter > silentMode_time_threshold) {
        isSilentMode = false;
        silentMode_counter = 0;
      }
    }
    /////////////////

    delta_index++;
  }
  /////////////////
}

// Call every time Gaiger detect ionization
void onCounter() {
  CounterVal++;
}

// Reset global counter, during sanity check for time and integer overflow
void reset() {
  CounterVal = 0;
  prev_time = 0;
  prev_counter = 0;
  cpw = 0;
  delta_index = 0;
  alarm_counter = 0;
  silentMode_counter = 0;
  isSilentMode = false;
  memset(time_buffer, 0, sizeof(time_buffer));
}

void drawCPM(signed int cpm) {
  char text_buffer[60];
  String(cpm).toCharArray(text_buffer, 60);
  ledLogger.show(text_buffer);
}

