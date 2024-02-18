#include <UTFT.h>
#include <EEPROM.h>
#include <CPUVolt.h>
#define ButtonR 12
#define ButtonH 13

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

UTFT lcd(ILI9486, 38, 39, 40, 41);
void(*reset_func)(void) = 0;
int readPercent() {return int((readVcc()) * 100.0 / 5000);}

void screen_setup() {
  lcd.InitLCD();
  lcd.clrScr();
  pinMode(ButtonR, INPUT_PULLUP);
  pinMode(ButtonH, INPUT_PULLUP);
  if (EEPROM.read("SIMON_HIGHSCORE") == 255) {EEPROM.update("SIMON_HIGHSCORE", 0);}
  if (EEPROM.read("FLAPPY_HIGHSCORE") == 255) {EEPROM.update("FLAPPY_HIGHSCORE", 0);}
  if (EEPROM.read("DINO_HIGHSCORE") == 255) {EEPROM.update("DINO_HIGHSCORE", 0);}
  if (EEPROM.read("X2048_HIGHSCORE") == 255) {EEPROM.update("X2048_HIGHSCORE", 0);}
  if (EEPROM.read("PONG_HIGHSCORE") == 255) {EEPROM.update("PONG_HIGHSCORE", 0);}
  if (EEPROM.read("SNAKE_HIGHSCORE") == 255) {EEPROM.update("SNAKE_HIGHSCORE", 0);}
}
