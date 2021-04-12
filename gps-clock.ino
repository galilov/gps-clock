// 2xMAX7119 based indicators
#define LED_DIN     8
#define LED_CS      7
#define LED_CLK     6
// Iskra Nano Pro from Amperka is based on ATmega328PB and provided 2 serial interfaces.
// You can use Serial (pin RX0) instead of Serial1 (pin D12)
#define GPS_PORT    Serial1

#include "MicroNMEA.h"
#include "led.h"

Led _led(LED_DIN, LED_CLK, LED_CS);
char _gpsBuf[100];
char _txtBuf[20];
MicroNMEA _nmea(_gpsBuf, sizeof(_gpsBuf));

void setup() {
  // put your setup code here, to run once:
  GPS_PORT.begin(9600);
  //Serial.begin(115200);
  _nmea.clear();
  _led.setup();
}

uint8_t _prevSecond = 60;
uint8_t _prevDay = 0;

void loop() {
  while (GPS_PORT.available()) {
    char c = GPS_PORT.read();
    if (_nmea.process(c)) {
      char navSystem = _nmea.getNavSystem();
      if (navSystem == '\0') {
        _led.print(0, "--------");
        _led.print(1, "--------");
      } else {
        uint8_t hour = _nmea.getHour();
        uint8_t minute = _nmea.getMinute();
        uint8_t second = _nmea.getSecond();
        uint16_t year = _nmea.getYear();
        uint8_t month = _nmea.getMonth();
        uint8_t day = _nmea.getDay();
        if (_prevSecond != second) {
          _prevSecond = second;
          sprintf(_txtBuf, "%02u-%02u-%02u", hour, minute, second);
          _led.print(0, _txtBuf);
        }
        if (_prevDay != day) {
          _prevDay = day;
          sprintf(_txtBuf, "%04u.%02u.%02u", year, month, day);
          _led.print(1, _txtBuf);          
        }
      }
    }
  }
}
