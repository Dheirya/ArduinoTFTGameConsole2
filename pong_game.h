const String pongCxBx[] = {"1.0-1.0", "1.0-1.1", "1.0-1.2", "1.0-1.3", "1.3-1.1", "1.3-1.2", "1.3-1.3", "1.3-1.4", "1.6-1.3", "1.6-1.4", "1.6-1.5", "1.6-1.6", "1.9-1.6"};
const float pongtiredC = 0.005;
const int ppY = 426;
const int ppW = 56;
const int pbS = 12;
const int pcY = 66;
const int ppH = 8;
bool pongSetupDone = false;
bool pongGameStarted = false;
bool pongGameOver = false;
float pbX = ppY - pbS;
float pbVx = -1.0;
float pbVy = -1.0;
float pcVx = 0.9;
float pbY = 156;
float ppX = 132;
float pcX = 132;
float ppVx = 2;
int pongCxBxIndex = -1;
int pongScore = 0;
int pongLives = 3;

void pong_reset() {
  pongSetupDone = false;
  pongGameStarted = false;
  pongGameOver = false;
  pbVx = -1.0;
  pbVy = -1.0;
  pcVx = 0.9;
  pbY = 156;
  ppX = 132;
  pcX = 132;
  ppVx = 2;
  pongCxBxIndex = -1;
  pbX = ppY - pbS;
  pongScore = 0;
  pongLives = 3;
}

void pong_moveComputerPaddle() {
  if (pcX + ppW / 2 < pbY + pbS / 2) {
        if (pcX + ppW + 1 <= 320) {
          pcX += pcVx;
        }
    } else if (pcX + ppW / 2 > pbY + pbS / 2) {
        if (pcX - 1 >= 0) {
          pcX -= pcVx;
        }
    }
}

void pong_addScore() {
  pongScore++;
  if (pongScore > EEPROM.read("PONG_HIGHSCORE")) {EEPROM.update("PONG_HIGHSCORE", pongScore);}
  lcd.setColor(255, 255, 255);
  lcd.print(String(pongScore), 459, 256, -90);
}

void pong_drawDottedLine() {
  int segmentLength = 8;
  bool isBlack = false;
  for (int y = 0; y <= 320; y += segmentLength) {
      if (isBlack) {
          lcd.setColor(0, 0, 0);
      } else {
          lcd.setColor(255, 255, 255);
      }
      lcd.drawLine(250, y, 250, min(y + segmentLength - 1, 320));
      isBlack = !isBlack;
  }
}

void pong_drawUI() {
  lcd.setFont(BigFont);
  lcd.setColor(0, 0, 24);
  lcd.setBackColor(0, 0, 24);
  lcd.fillRect(0, 0, 50, 320);
  lcd.fillRect(450, 0, 480, 320);
  lcd.setColor(255, 255, 255);
  lcd.print("PONG", 12, 192, -90);
  lcd.setFont(SmallFont);
  lcd.print("The game of angles", 30, 232, -90);
  lcd.print("SCORE: " + String(pongScore), 459, 312, -90);
  lcd.fillRect(ppY, ppX, ppY + ppH, ppX + ppW);
  lcd.fillRect(pcY, pcX, pcY + ppH, pcX + ppW);
  lcd.drawBitmap(pbX, pbY, pbS, pbS, ball);
  lcd.drawRect(50, 0, 450, 319);
  pong_drawDottedLine();
  lcd.setColor(VGA_YELLOW);
  lcd.fillCircle(465, 12, 6);
  lcd.fillCircle(465, 27, 6);
  lcd.fillCircle(465, 42, 6);
}

void pong_newRound(bool updateLevel) {
  if (updateLevel) {
    if (pongCxBxIndex + 1 < 13) {
      pongCxBxIndex++;
    }
  }
  pongGameStarted = false;
  delay(250);
  lcd.setColor(0, 0, 0);
  lcd.fillRect(pbX, pbY, pbX + pbS, pbY + pbS);
  lcd.fillRect(pcY, 1, pcY + ppH, 318);
  lcd.fillRect(ppY, 1, ppY + ppH, 318);
  pbX = ppY - pbS;
  pbY = 156;
  ppX = 132;
  pcX = 132;
  if (pongCxBxIndex == -1) {
    pcVx = 0.9;
  } else {
    pcVx = pongCxBx[pongCxBxIndex].substring(4).toFloat();
  }
  if (updateLevel) {
    pbVx = -pongCxBx[pongCxBxIndex].substring(0, 3).toFloat();
    pbVy = -pongCxBx[pongCxBxIndex].substring(0, 3).toFloat();
  } else {
    pbVx = -abs(pbVx);
    pbVy = pbVx;
  }
  lcd.setColor(255, 255, 255);
  lcd.drawRect(50, 0, 450, 319);
  lcd.fillRect(ppY, ppX, ppY + ppH, ppX + ppW);
  lcd.fillRect(pcY, pcX, pcY + ppH, pcX + ppW);
}

void pong_loseLive() {
  pongLives--;
  if (pongLives >= 0) {
    if (pongLives == 2) {
      lcd.setColor(0, 0, 0);
      lcd.fillCircle(465, 42, 6);
    } else if (pongLives == 1) {
      lcd.setColor(0, 0, 0);
      lcd.fillCircle(465, 27, 6);
    } else if (pongLives == 0) {
      lcd.setColor(0, 0, 0);
      lcd.fillCircle(465, 12, 6);
    }
    pong_newRound(false);
  } else {
    pongGameOver = true;
  }
}

void pong_setup() {
  pong_drawUI();
  pongSetupDone = true;
}

void pong_loop() {
  if (!pongSetupDone) {
    pong_setup();
  } else {  
    if (!pongGameOver) {
      if (pongGameStarted) {
        lcd.setColor(0, 0, 0);
        lcd.fillRect(ppY, ppX, ppY + ppH, ppX - 1);
        lcd.fillRect(ppY, ppX + ppW, ppY + ppH, ppX + ppW + 1);
        lcd.fillRect(pcY, pcX, pcY + ppH, pcX - 1);
        lcd.fillRect(pcY, pcX + ppW, pcY + ppH, pcX + ppW + 1);
        if (128 <= ppX && ppX <= 134) {
          lcd.fillRect(ppY, ppX, ppY + ppH, ppX - 2);
          lcd.fillRect(ppY, ppX + ppW, ppY + ppH, ppX + ppW + 2);
        }
      }
      if ((move == JoystickMove::Down || move == JoystickMove::DownLeft || move == JoystickMove::DownRight) && (ppX + ppW + 1 <= 320)) {
        ppX += ppVx;
        if (!pongGameStarted) {pongGameStarted = true;}
      } else if ((move == JoystickMove::Up || move == JoystickMove::UpLeft || move == JoystickMove::UpRight) && (ppX - 1 >= 0)) {
        ppX -= ppVx;
        if (!pongGameStarted) {pongGameStarted = true;}
      }
      if (pongGameStarted) {
        pong_moveComputerPaddle();
        lcd.setColor(255, 255, 255);
        if ((ppX < 5) || (ppX + ppW > 315)) {
          lcd.drawLine(ppY, 0, ppY + ppH, 0);
          lcd.drawLine(ppY, 319, ppY + ppH, 319);
        }
        if ((pcX < 5) || (pcX + ppW > 315)) {
          lcd.drawLine(pcY, 0, pcY + ppH, 0);
          lcd.drawLine(pcY, 319, pcY + ppH, 319);
        }
        if (abs(250 - pbX) > 12 && abs(250 - pbX) <= 14) {
          pong_drawDottedLine();
        }
        lcd.fillRect(ppY, ppX, ppY + ppH, ppX + ppW);
        lcd.fillRect(pcY, pcX, pcY + ppH, pcX + ppW);
        pbX += pbVx;
        pbY += pbVy;
        if ((abs(pbX + pbS - 2 - ppY) < 1.0) && (pbVx > 0)) {
          if ((ppX - pbS <= pbY) && (pbY <= ppX + ppW)) {
            int nPoint = pbY - ppX + pbS - 34;
            int cM = abs(pbVx);
            float s = (((cM * 2) - (cM * 0.75)) / 34);
            if (nPoint <= 0) {
              pbVy = s * nPoint - (cM * 0.75);
            } else {
              pbVy = s * nPoint + (cM * 0.75);
            }
            pbVx = -pbVx;
          }
        }
        if ((abs(pbX - pcY - ppH) < 1.0) && (pbVx < 0)) {
          if ((pcX - pbS <= pbY) && (pbY <= pcX + ppW)) {
            pcVx -= pongtiredC;
            int nPoint = pbY - pcX + pbS - 34;
            int cM = abs(pbVx);
            float s = (((cM * 2) - (cM * 0.75)) / 34);
            if (nPoint <= 0) {
              pbVy = s * nPoint - (cM * 0.75);
            } else {
              pbVy = s * nPoint + (cM * 0.75);
            }
            pbVx = -pbVx;
          }
        }
        if (pbX - 2 < 50) {
          pong_addScore();      
          pong_newRound(true);
        } else if (pbX + pbS >= 448) {
          pong_loseLive();
        }
        if (pbY < 2) {
          pbY = 2;
          pbVy = -pbVy;
        } else if (pbY + pbS >= 318) {
          pbY = 318 - pbS;
          pbVy = -pbVy;
        }
        lcd.drawBitmap(pbX, pbY, pbS, pbS, ball);
      }
    } else {
      lcd.setBackColor(0, 0, 0);
      lcd.setColor(VGA_RED);
      lcd.setFont(BigFont);
      lcd.print("GAME OVER", 232, 232, -90);
      lcd.print("RESTARTING!", 254, 248, -90);
      delay(2500);
      pong_reset();
      lcd.setColor(0, 0, 0);
      lcd.fillRect(50, 0, 450, 320);
    }
    if (digitalRead(ButtonR) == LOW) {
      pong_reset();
      gameRunning = MENU;
      menu_reset();
    }
    if (digitalRead(ButtonH) == LOW) {
      pong_reset();
      lcd.clrScr();
    }
  }
}
