#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DS3231 rtc;

void setup() {
  Wire.begin();
  lcd.begin(16, 2);
}

void loop() {
  bool century = false;
  bool h12Flag = false;
  bool pmFlag = false;

  // Line 1: Date
  lcd.setCursor(0, 0);
  lcd.print("20");
  lcd.print(rtc.getYear(), DEC);
  lcd.print("-");
  if (rtc.getMonth(century) < 10) lcd.print("0");
  lcd.print(rtc.getMonth(century), DEC);
  lcd.print("-");
  if (rtc.getDate() < 10) lcd.print("0");
  lcd.print(rtc.getDate(), DEC);

  // Line 2: Time
  lcd.setCursor(0, 1);
  if (rtc.getHour(h12Flag, pmFlag) < 10) lcd.print("0");
  lcd.print(rtc.getHour(h12Flag, pmFlag), DEC);
  lcd.print(":");
  if (rtc.getMinute() < 10) lcd.print("0");
  lcd.print(rtc.getMinute(), DEC);
  lcd.print(":");
  if (rtc.getSecond() < 10) lcd.print("0");
  lcd.print(rtc.getSecond(), DEC);

  delay(1000);
}