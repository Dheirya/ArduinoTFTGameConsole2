const int fpTop = 26;
const int fpBottom = 282;
const int fpPillar_sZ = 50;
const int fpGap = 80;
const int fp_xP = 82;
const float fpGravity = 0.0092;
bool flappySetupDone = false;
bool flappyGameStart = false;
bool flappyDied = false;
bool flappyGameOverPrinted = false;
int fpPillar_xp = 430;
int fpPillar_tH = 98;
float fp_yP = 132;
float fp_yVelocity = 0;
byte fpScore = 0;
String fpGameMessage = "hitting a pipe";

void flappy_reset() {
  flappySetupDone = false;
  flappySetupDone = false;
  flappyGameStart = false;
  flappyDied = false;
  flappyGameOverPrinted = false;
  fpPillar_xp = 430;
  fpPillar_tH = 98;
  fpScore = 0;
  fp_yP = 132;
  fp_yVelocity = 0;
  fpGameMessage = "hitting a pipe";
}

void flappy_drawUI() {
  lcd.setColor(39, 176, 214);
  lcd.fillRect(0, 0, 480, 320);
  lcd.setColor(0, 0, 0);
  lcd.fillRect(0, 300, 480, 320);
  lcd.setColor(47, 175, 68);
  lcd.fillRect(0, 282, 480, 300);
  lcd.setColor(0, 0, 0);
  lcd.fillRect(0, 0, 480, 25);
  lcd.setFont(BigFont);
  lcd.setColor(255, 255, 255);
  lcd.drawLine(0, 300, 480, 300);
  lcd.drawRect(0, 0, 479, 25);
  lcd.print("Flappy: The Game of Flight", CENTER, 5);
  lcd.setFont(SmallFont);
  lcd.print("CLICK TO START", CENTER, 305);
  lcd.drawBitmap(fp_xP, fp_yP, 20, 20, bird);
}

void flappy_setup() {
  flappy_drawUI();
  flappySetupDone = true;
}

void flappy_loop() {
  if (!flappySetupDone && !flappyDied) {
    flappy_setup();
  } else {
    if (!flappyDied) {
      if (digitalRead(ButtonH) == LOW) {
        if (!flappyGameStart) {
          lcd.setColor(0, 0, 0);
          lcd.fillRect(0, 301, 480, 320);
          lcd.setColor(255, 255, 255);
          lcd.print("SCORE: 0", CENTER, 305);
          flappyGameStart = true;
        } else {
          fp_yVelocity = -0.61;
        }
      }
      if (flappyGameStart) {
        fp_yVelocity += fpGravity;
        fp_yP += fp_yVelocity;
        if ((fp_yP > fpBottom - 15) || (fp_yP < fpTop) || ((32 < fpPillar_xp && fpPillar_xp < 102) && !((26 + fpPillar_tH) < fp_yP && fp_yP < (106 + fpPillar_tH)))) {
          fp_yVelocity = 0;
          flappyGameStart = false;
          flappyDied = true;
          if (fp_yP > fpBottom - 15) {
            fpGameMessage = "hitting the ground";
          } else if (fp_yP < fpTop) {
            fpGameMessage = "hitting the sky";
          } else if ((32 < fpPillar_xp && fpPillar_xp < 102) && !((26 + fpPillar_tH) < fp_yP && fp_yP < (106 + fpPillar_tH))) {
            fpGameMessage = "hitting a pipe";
          }
        }
        if (fpPillar_xp == 32) {
          fpScore++;
          lcd.setColor(255, 255, 255);
          lcd.print("Score: " + String(fpScore), CENTER, 305);
          if (fpScore > EEPROM.read("FLAPPY_HIGHSCORE")) {EEPROM.update("FLAPPY_HIGHSCORE", fpScore);}
        }
        lcd.drawBitmap(fp_xP, fp_yP, 20, 20, bird);
        fpPillar_xp -= 2;
      }
      if (fpPillar_xp <= -25) {
        lcd.setColor(39, 176, 214);
        lcd.fillRect(0, fpTop, 25, fpBottom);
        fpPillar_xp = 480;
        fpPillar_tH = random(24, fpBottom - fpTop - fpGap - 24);
      } else if (fpPillar_xp == 430) {
        lcd.setColor(0, 200, 20);
        lcd.fillRect(fpPillar_xp, fpTop, fpPillar_xp + fpPillar_sZ, fpTop + fpPillar_tH);
        lcd.fillRect(fpPillar_xp, fpTop + fpPillar_tH + fpGap, fpPillar_xp + fpPillar_sZ, fpBottom);
      } else {
        lcd.setColor(0, 200, 20);
        lcd.fillRect(fpPillar_xp, fpTop, fpPillar_xp + 1, fpTop + fpPillar_tH);
        lcd.fillRect(fpPillar_xp, fpTop + fpPillar_tH + fpGap, fpPillar_xp + 1, fpBottom);
        lcd.setColor(39, 176, 214);
        lcd.fillRect(fpPillar_xp + fpPillar_sZ, fpTop, fpPillar_xp + fpPillar_sZ + 1, fpBottom);
      }
    } else {
      if (!flappyGameOverPrinted) {
        delay(650);
        lcd.clrScr();
        lcd.drawBitmap(150, 63, 30, 19, game_over, 6);
        lcd.setFont(BigFont);
        lcd.setColor(255, 0, 0);
        lcd.print("You died from", CENTER, 190);
        lcd.print(fpGameMessage, CENTER, 210);
        lcd.setColor(150, 75, 0);
        lcd.print("YOUR SCORE: " + String(fpScore), CENTER, 235);
        lcd.print("HIGHSCORE: " + String(EEPROM.read("FLAPPY_HIGHSCORE")), CENTER, 255);
        flappyGameOverPrinted = true;
      }
      if (digitalRead(ButtonR) == LOW) {
        flappy_reset();
        gameRunning = MENU;
        menu_reset();
      }
      if (digitalRead(ButtonH) == LOW) {
        flappy_reset();
        lcd.clrScr();
      }
    }
  }
}
