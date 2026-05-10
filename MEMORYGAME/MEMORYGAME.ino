#include <LiquidCrystal.h>

// LCD
LiquidCrystal lcd(8, 6, 11, 12, 9, 2);

// LEDs
#define LED_RED     3
#define LED_GREEN   5
#define LED_BLUE    10
#define LED_YELLOW  13

// Buttons
#define BTN_RED     A0   // EASY
#define BTN_GREEN   A1   // HARD
#define BTN_BLUE    A2   // SUPER HARD
#define BTN_YELLOW  A3   // START (optional)

// Buzzer
#define BUZZER 4

// Colors
#define RED     1
#define GREEN   2
#define BLUE    3
#define YELLOW  4

int sequence[50];
int level = 0;
int highScore = 0;
int speedDelay = 300; // default

// ---------------- SETUP ----------------
void setup() {
  lcd.begin(16, 2);
  lcd.print("Memory Game");
  delay(1500);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(BTN_RED, INPUT_PULLUP);
  pinMode(BTN_GREEN, INPUT_PULLUP);
  pinMode(BTN_BLUE, INPUT_PULLUP);
  pinMode(BTN_YELLOW, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
}

// ---------------- LOOP ----------------
void loop() {
  selectMode();   // 👈 NEW
  startGame();
}

// ---------------- MODE SELECT ----------------
void selectMode() {
  lcd.clear();
  lcd.print("Choose Mode:");
  lcd.setCursor(0,1);
  lcd.print("R=E G=H B=SH");

  while (1) {
    if (digitalRead(BTN_RED) == LOW) {
      speedDelay = 500; // EASY
      showMode("EASY");
      return;
    }
    if (digitalRead(BTN_GREEN) == LOW) {
      speedDelay = 300; // HARD
      showMode("HARD");
      return;
    }
    if (digitalRead(BTN_BLUE) == LOW) {
      speedDelay = 150; // SUPER HARD
      showMode("SUPER HARD");
      return;
    }
  }
}

void showMode(String mode) {
  lcd.clear();
  lcd.print(mode);

  // little LED flash feedback
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_YELLOW, HIGH);

  tone(BUZZER, 800);
  delay(500);
  noTone(BUZZER);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, LOW);

  delay(700);
}

// ---------------- GAME ----------------
void startGame() {
  level = 0;

  lcd.clear();
  lcd.print("Press any btn");

  while(readButton() == 0);

  while (1) {
    addStep();

    lcd.clear();
    lcd.print("Watch...");
    delay(600);

    playSequence();

    lcd.clear();
    lcd.print("Your turn");

    if (!checkInput()) {
      gameOver();
      return;
    }

    lcd.clear();
    lcd.print("Score: ");
    lcd.print(level);
    delay(800);
  }
}

// ---------------- FUNCTIONS ----------------
void addStep() {
  sequence[level] = random(1,5);
  level++;
}

void playSequence() {
  for (int i = 0; i < level; i++) {
    light(sequence[i]);
    delay(speedDelay); // 👈 uses mode speed
  }
}

bool checkInput() {
  for (int i = 0; i < level; i++) {
    int btn = waitButton();
    if (btn != sequence[i]) return false;
  }
  return true;
}

int waitButton() {
  while (1) {
    int b = readButton();
    if (b != 0) {
      light(b);
      return b;
    }
  }
}

int readButton() {
  if (digitalRead(BTN_RED) == LOW) return RED;
  if (digitalRead(BTN_GREEN) == LOW) return GREEN;
  if (digitalRead(BTN_BLUE) == LOW) return BLUE;
  if (digitalRead(BTN_YELLOW) == LOW) return YELLOW;
  return 0;
}

// ---------------- LED + SOUND ----------------
void light(int color) {
  int pin;
  int freq;

  if (color == RED) { pin = LED_RED; freq = 400; }
  if (color == GREEN) { pin = LED_GREEN; freq = 600; }
  if (color == BLUE) { pin = LED_BLUE; freq = 800; }
  if (color == YELLOW) { pin = LED_YELLOW; freq = 1000; }

  digitalWrite(pin, HIGH);
  tone(BUZZER, freq);
  delay(200);
  digitalWrite(pin, LOW);
  noTone(BUZZER);
}

// ---------------- GAME OVER ----------------
void gameOver() {
  lcd.clear();
  lcd.print("GAME OVER");

  if (level > highScore) highScore = level;

  // fail sound
  for (int i = 0; i < 3; i++) {
    tone(BUZZER, 200);
    delay(200);
    noTone(BUZZER);
    delay(100);
  }

  delay(1000);

  lcd.clear();
  lcd.print("High Score:");
  lcd.setCursor(0,1);
  lcd.print(highScore);

  delay(2500);
}