#include <IRremote.hpp>

const int LED_PIN = 9;
int brightness = 128;
bool ledOn = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  IrReceiver.begin(11, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    int code = IrReceiver.decodedIRData.command;

    if (code == 0x45) {
      ledOn = !ledOn;
      analogWrite(LED_PIN, ledOn ? brightness : 0);
    }
    else if (code == 0x46 && ledOn) {
      brightness = min(255, brightness + 30);
      analogWrite(LED_PIN, brightness);
    }
    else if (code == 0x15 && ledOn) {
      brightness = max(10, brightness - 30);
      analogWrite(LED_PIN, brightness);
    }

    IrReceiver.resume();
  }
}