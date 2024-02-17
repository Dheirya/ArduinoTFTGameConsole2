#define sRNP 0xf800
#define sGNP 0x07e0
#define sBNP 0x001f
#define sYNP 0xffe0
#define sRP 0xb000
#define sGP 0x0580
#define sBP 0x0016
#define sYP 0xb580
#define sMAX_SCORE 255
uint8_t simonComputerSequence[sMAX_SCORE];
bool simonSetupDone = false;
int simonScore = 0;

void simon_reset_array(uint8_t* array) {
    for (uint8_t i = 0; i < sMAX_SCORE; i++) {
        array[i] = 0;
    }
}

void simon_reset() {
  simonSetupDone = false;
  simonScore = 0;
  simon_reset_array(simonComputerSequence);
}

void simonBoxChangeColor(int a, bool pressed) {
  if (a == 1) {
    if (pressed) {lcd.setColor(sGP);} else {lcd.setColor(sGNP);}
    lcd.fillRect(60, 165, 225, 310);
  } else if (a == 2) {
    if (pressed) {lcd.setColor(sRP);} else {lcd.setColor(sRNP);}
    lcd.fillRect(60, 10, 225, 155);
  } else if (a == 3) {
    if (pressed) {lcd.setColor(sYP);} else {lcd.setColor(sYNP);}
    lcd.fillRect(235, 165, 400, 310);
  } else if (a == 4) {
    if (pressed) {lcd.setColor(sBP);} else {lcd.setColor(sBNP);}
    lcd.fillRect(235, 10, 400, 155);
  }
}

void simon_drawUI() {
  lcd.setColor(2, 30, 64);
  lcd.setBackColor(2, 30, 64);
  lcd.fillRect(0, 0, 480, 320);
  lcd.setColor(255, 255, 255);
  lcd.drawRect(50, 0, 450, 319);
  lcd.setFont(BigFont);
  lcd.print("SIMON", 12, 200, -90);
  lcd.print("CLICK TO", 408, 224, -90);
  lcd.print("START!", 428, 208, -90);
  lcd.setFont(SmallFont);
  lcd.print("The game of mind", 30, 224, -90);
  lcd.print("SCORE: " + String(simonScore), 459, 312, -90);
  lcd.print("HIGH SCORE: " + String(EEPROM.read("SIMON_HIGHSCORE")), 459, ("HIGH SCORE: " + String(EEPROM.read("SIMON_HIGHSCORE"))).length() * 8 + 6, -90);
  simonBoxChangeColor(1, false);
  simonBoxChangeColor(2, false);
  simonBoxChangeColor(3, false);
  simonBoxChangeColor(4, false);
}

void simon_randomAdd() {
  int randomPicked = random(1, 5);
  simonComputerSequence[simonScore] = randomPicked;
  simonScore++;
}

void simon_updateScore() {
  lcd.setFont(SmallFont);
  lcd.setColor(255, 255, 255);
  lcd.print("SCORE: " + String(simonScore), 459, 312, -90);
  if (simonScore > EEPROM.read("SIMON_HIGHSCORE")) {
    EEPROM.update("SIMON_HIGHSCORE", simonScore);
    lcd.print("HIGH SCORE: " + String(EEPROM.read("SIMON_HIGHSCORE")), 459, ("HIGH SCORE: " + String(EEPROM.read("SIMON_HIGHSCORE"))).length() * 8 + 6, -90);
  }
}

void clearTopText() {
  lcd.setColor(2, 30, 64);
  lcd.fillRect(408, 240, 424, 80);
}

void simon_printComputerSequence() {
  clearTopText();
  lcd.setFont(BigFont);
  lcd.setColor(255, 255, 255);
  lcd.print("COMPUTER", 408, 224, -90);
  lcd.print("PICKING!", 428, 224, -90);
  for (int i = 0; i < simonScore; i++) {
    simonBoxChangeColor(simonComputerSequence[i], true);
    delay(300);
    simonBoxChangeColor(simonComputerSequence[i], false);
    delay(350);
  }
}

void clearBottomText() {
  lcd.setColor(2, 30, 64);
  lcd.fillRect(428, 224, 444, 96);
}

void simonPrintSelected(int i) {
  clearBottomText();
  lcd.setColor(255, 255, 255);
  lcd.setFont(BigFont);
  String printOut;
  if (i == 1) {printOut = "Green?";} else if (i == 2) {printOut = "Red?";} else if (i == 3) {printOut = "Yellow?";} else if (i == 4) {printOut = "Blue?";}
  lcd.print(printOut, 428, 160 + (printOut.length() * 8), -90);
}

void simon_setup() {
  simon_drawUI();
  simonSetupDone = true;
}

void simon_loop() {
  if (!simonSetupDone) {
    simon_setup();
  } else {
    bool pickedRight = true;
    int numPicked = 0;
    while ((numPicked != simonScore) && pickedRight) {
      bool selectedAnything = false;
      bool selectedHighlighted = false;
      int selected = 1;
      lcd.setColor(255, 255, 255);
      lcd.print("CHOICE " + String(numPicked + 1), 408, 224, -90);
      while (!selectedAnything) {
        joystick_moveUpd();
        if (move == JoystickMove::None) {
          if (!selectedHighlighted) {
            simonBoxChangeColor(selected, true);
            selectedHighlighted = true;
            simonPrintSelected(selected);
          }
          if (digitalRead(ButtonH) == LOW) {
            selectedAnything = true;
          }
        } else {
          if (selectedHighlighted) {
            simonBoxChangeColor(selected, false);
            if (move == JoystickMove::Up) {
              selected++;
              if (selected > 4) {
                selected = 1;
              }
            } else if (move == JoystickMove::Down) {
              selected--;
              if (selected < 1) {
                selected = 4;
              }
            } else if (move == JoystickMove::Left) {
              selected -= 2;
              if (selected < 1) {
                selected += 4;
              }
            } else if (move == JoystickMove::Right) {
              selected += 2;
              if (selected > 4) {
                selected -= 4;
              }
            }
            selectedHighlighted = false;
          }
        }
      }
      if (selected != simonComputerSequence[numPicked]) {
        pickedRight = false;
        break;
      } else {
        simonBoxChangeColor(selected, false);
        numPicked++;
      }
    }
    if (pickedRight) {
      if (digitalRead(ButtonH) == LOW) {
        simon_updateScore();
        simon_randomAdd();
        simon_printComputerSequence();
      }
      if (digitalRead(ButtonR) == LOW) {
        simon_reset();
        gameRunning = MENU;
        menu_reset();
      }
    } else {
      delay(100);
      lcd.clrScr();
      lcd.setBackColor(0, 0, 0);
      lcd.drawBitmap(150, 63, 30, 19, game_over, 6);
      lcd.setFont(BigFont);
      lcd.setColor(255, 0, 0);
      lcd.print("You lost!", CENTER, 190);
      lcd.print("Better luck next time?", CENTER, 210);
      lcd.setColor(150, 75, 0);
      lcd.print("YOUR SCORE: " + String(simonScore - 1), CENTER, 235);
      lcd.print("RESTARTING NOW!", CENTER, 255);
      delay(5000);
      simon_reset();
    }
  }
}
