const int snakeDelay = 80;
bool snakeSetupDone = false;
bool snakeGameOver = false;
int snakeBoardMatrix[25][20];
int snakeHeadX = 9; int snakeHeadY = 12;
int snakeTailX = 11; int snakeTailY = 12;
int snakeDir = -1;
int snakeLength = 3;

void snake_reset() {
  snakeSetupDone = false;
  snakeGameOver = false;
  snakeHeadX = 9; snakeHeadY = 12;
  snakeTailX = 11; snakeTailY = 12;
  snakeDir = -1;
  snakeLength = 3;
}

void snake_setCheckerBoardColor(int i, int j) {
  if (i % 2 == 0) {
    if (j % 2 == 0) {lcd.setColor(94,120,44);} else {lcd.setColor(118,150,56);}
  } else {
    if (j % 2 == 1) {lcd.setColor(94,120,44);} else {lcd.setColor(118,150,56);}
  }
}

void snake_drawBoard() {
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 20; j++) {
      snake_setCheckerBoardColor(i, j);
      snakeBoardMatrix[i][j] = 0;
      lcd.fillRect(50 + i * 16, 320 - j * 16, 50 + i * 16 + 16, 320 - j * 16 - 16);
    }
  }
}

void snake_random_apple() {
  int randomX, randomY;
  do {
    randomX = random(0, 25);
    randomY = random(0, 20);
  } while (snakeBoardMatrix[randomX][randomY] != 0);
  snakeBoardMatrix[randomX][randomY] = -1;
  lcd.setColor(VGA_RED);
  lcd.fillCircle(50 + randomX * 16 + 8, 320 - randomY * 16 - 8, 6);
}

void snake_drawUI() {
  lcd.setColor(60, 96, 37);
  lcd.setBackColor(60, 96, 37);
  lcd.fillRect(0, 0, 50, 320);
  lcd.fillRect(450, 0, 480, 320);
  lcd.setFont(BigFont);
  lcd.setColor(255, 255, 255);
  lcd.print("SNAKE", 12, 200, -90);
  lcd.setFont(SmallFont);
  lcd.print("The game of apples", 30, 232, -90);
  lcd.print(String(snakeLength - 3), 459, 298, -90);
  lcd.print("HIGH SCORE: " + String(EEPROM.read("SNAKE_HIGHSCORE")), 459, ("HIGH SCORE: " + String(EEPROM.read("SNAKE_HIGHSCORE"))).length() * 8 + 6, -90);
  snake_drawBoard();
  lcd.setColor(77, 124, 238);
  snakeBoardMatrix[12][9] = 1;
  snakeBoardMatrix[12][10] = 2;
  snakeBoardMatrix[12][11] = 3;
  lcd.fillRect(50 + 12 * 16, 320 - 9 * 16, 50 + 12 * 16 + 16, 320 - 9 * 16 - 16);
  lcd.fillRect(50 + 12 * 16, 320 - 10 * 16, 50 + 12 * 16 + 16, 320 - 10 * 16 - 16);
  lcd.fillRect(50 + 12 * 16, 320 - 11 * 16, 50 + 12 * 16 + 16, 320 - 11 * 16 - 16);
  snake_random_apple();
  lcd.fillCircle(465, 308, 6);
}

void snake_setup() {
  snake_drawUI();
  snakeSetupDone = true;
}

void snake_gameOver() {
  snakeGameOver = true;
  snakeDir = -1;
  lcd.setColor(VGA_RED);
  lcd.setFont(BigFont);
  lcd.print("GAME OVER", 232, 232, -90);
  lcd.print("CLICK TO RESTART", 254, 288, -90);
}

void snake_move() {
  if (snakeBoardMatrix[snakeHeadY][snakeHeadX] > 0) {
    snake_gameOver();
  }
  bool isGrow = (snakeBoardMatrix[snakeHeadY][snakeHeadX] == -1) ? true : false;  
  lcd.setColor(77, 124, 238);
  lcd.fillRect(50 + snakeHeadY * 16, 320 - snakeHeadX * 16, 50 + snakeHeadY * 16 + 16, 320 - snakeHeadX * 16 - 16);
  if (isGrow) {
    snakeLength++;
    lcd.setColor(255, 255, 255);
    lcd.setFont(SmallFont);
    if (snakeLength - 3 > EEPROM.read("SNAKE_HIGHSCORE")) {
      EEPROM.update("SNAKE_HIGHSCORE", snakeLength - 3);
      lcd.print("HIGH SCORE: " + String(EEPROM.read("SNAKE_HIGHSCORE")), 459, ("HIGH SCORE: " + String(EEPROM.read("SNAKE_HIGHSCORE"))).length() * 8 + 6, -90);
    }
    lcd.print(String(snakeLength - 3), 459, 298, -90);
  } else {
    snake_setCheckerBoardColor(snakeTailX, snakeTailY);
    lcd.fillRect(50 + snakeTailY * 16, 320 - snakeTailX * 16, 50 + snakeTailY * 16 + 16, 320 - snakeTailX * 16 - 16);
  }
  for (int i = 0; i < 25; i++) {
    for (int j = 0; j < 20; j++) {
      if (snakeBoardMatrix[i][j] > 0) {
        snakeBoardMatrix[i][j]++;
        if (snakeBoardMatrix[i][j] == snakeLength) {
          snakeTailX = j; snakeTailY = i;
        } else if (snakeBoardMatrix[i][j] > snakeLength) {
          snakeBoardMatrix[i][j] = 0;
        }
      }
    }
  }
  snakeBoardMatrix[snakeHeadY][snakeHeadX] = 1;
  if (isGrow) {
    snake_random_apple();
  }
}

void snake_loop() {
  if (!snakeSetupDone) {
    snake_setup();
  } else {
    if (!snakeGameOver) {
      if (move == JoystickMove::Down) {
        if ((snakeDir == -1) || (snakeDir != 1)) {
          snakeDir = 0;
        }
      } else if (move == JoystickMove::Up) {
        if ((snakeDir == -1) || (snakeDir != 0)) {      
          if (snakeDir == -1) {
            snakeBoardMatrix[12][9] = 3;
            snakeBoardMatrix[12][10] = 2;
            snakeBoardMatrix[12][11] = 1;
            snakeHeadX = 11; snakeHeadY = 12;
            snakeTailX = 9; snakeTailY = 12;
          }
          snakeDir = 1;
        }
      } else if (move == JoystickMove::Left) {
        if ((snakeDir == -1) || (snakeDir != 3)) {
          snakeDir = 2;
        }
      } else if (move == JoystickMove::Right) {
        if ((snakeDir == -1) || (snakeDir != 2)) {
          snakeDir = 3;
        }
      }
      if (snakeDir != -1) {
        if (snakeDir == 0) {
          if (snakeHeadX > 0) {
            snakeHeadX--;
          } else {
            snake_gameOver();
          }
        } else if (snakeDir == 1) {
          if (snakeHeadX < 19) {
            snakeHeadX++;
          } else {
            snake_gameOver();
          }
        } else if (snakeDir == 2) {
          if (snakeHeadY > 0) {
            snakeHeadY--;
          } else {
            snake_gameOver();
          }
        } else if (snakeDir == 3) {
          if (snakeHeadY < 24) {
            snakeHeadY++;
          } else {
            snake_gameOver();
          }
        }
        if (snakeDir != -1) {
          snake_move();
          delay(snakeDelay);
        }
      }
    }
    if (digitalRead(ButtonH) == LOW) {
      lcd.setColor(0, 0, 0);
      lcd.fillRect(50, 0, 450, 320);
      snake_reset();
    }
    if (digitalRead(ButtonR) == LOW) {
      snake_reset();
      gameRunning = MENU;
      menu_reset();
    }
  }
}
