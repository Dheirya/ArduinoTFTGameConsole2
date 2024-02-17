#define C2 0xffff
#define C4 0xf800
#define C8 0xfbe0
#define C16 0xff20
#define C32 0xa772
#define C64 0x25e9
#define C128 0x2497
#define C256 0x21f7
#define C512 0xddd9
#define C1024 0xb8f0
#define C2048 0xa145
#define C4096 0x8a22
#define C8192 0x8410
#define C16384 0x5a06

bool x2048SetupDone = false;
int x2048score = 0;
int x2048highestNum = 2;
int x2048highestType = 1;
int x2048board[][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
JoystickMove x2048lastMove = JoystickMove::None;

bool x2048_checkLoss() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      if (x2048board[i][j] == 0 || x2048board[i][j] == x2048board[i][j + 1]) {
        return false;
      }
      if (x2048board[j][i] == 0 || x2048board[j][i] == x2048board[j + 1][i]) {
        return false;
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    if (x2048board[3][i] == 0) {
      return false;
    }
  }
  return true;
}

void x2048_reset() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      x2048board[i][j] = 0;
    }
  }
  x2048score = 0;
  x2048highestNum = 2;
  x2048highestType = 1;
  x2048SetupDone = false;
  x2048lastMove = JoystickMove::None;
}

int x2048_center_text(int x1, int x2, int length, int size) {
  return ((x1 + x2) / 2) + ((size * length) / 2);
}

void x2048_updateScore() {
  lcd.setFont(SmallFont);
  lcd.setColor(0, 0, 0);
  lcd.setBackColor(228, 188, 97);
  lcd.print("SCORE: " + String(x2048score), 416, x2048_center_text(0, 320, ("SCORE: " + String(x2048score)).length(), 8), -90);
}

void x2048_updateHighType() {
  lcd.setFont(SmallFont);
  lcd.setColor(0, 0, 0);
  lcd.setBackColor(228, 188, 97);
  int score = (int)pow(2, x2048highestType);
  if (score != 2) {
    score++;
  }
  lcd.print("HIGHEST TYPE: " + String(score), 434, x2048_center_text(0, 320, ("HIGHEST TYPE: " + String(score)).length(), 8), -90);
  if (x2048highestType > EEPROM.read("X2048_HIGHSCORE")) {EEPROM.update("X2048_HIGHSCORE", x2048highestType);}
}

void x2048_printBoard() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int x1 = 104 + (i * 80);
      int y1 = 296 - (j * 80);
      int x2 = x1 + 32;
      int y2 = y1 - 32;
      lcd.setColor(222, 173, 59);
      lcd.fillRect(x1, y1, x2, y2);
      if (x2048board[i][j] != 0) {
        int x = 114 + (i * 80);
        int y = 280 - (j * 80) + (String(x2048board[i][j]).length() * 4);
        int colorIndex = x2048board[i][j];
        uint16_t color;
        switch (colorIndex) {
          case 2: color = C2; break;
          case 4: color = C4; break;
          case 8: color = C8; break;
          case 16: color = C16; break;
          case 32: color = C32; break;
          case 64: color = C64; break;
          case 128: color = C128; break;
          case 256: color = C256; break;
          case 512: color = C512; break;
          case 1024: color = C1024; break;
          case 2048: color = C2048; break;
          case 4096: color = C4096; break;
          case 8192: color = C8192; break;
          case 16384: color = C16384; break;
          default: color = VGA_BLACK; break;
        }
        lcd.setColor(color);
        lcd.fillRect(x1, y1, x2, y2);
      }
    }
  }
}

void x2048_randomAdd() {
  int emptyCells[16][2];
  int numEmptyCells = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (x2048board[i][j] == 0) {
        emptyCells[numEmptyCells][0] = i;
        emptyCells[numEmptyCells][1] = j;
        numEmptyCells++;
      }
    }
  }
  if (numEmptyCells > 0) {
    int randomIndex = random(numEmptyCells);
    int row = emptyCells[randomIndex][0];
    int col = emptyCells[randomIndex][1];
    int randomNumber = random(10) < 9 ? 2 : 4;
    x2048board[row][col] = randomNumber;
    if (randomNumber > x2048highestNum) {
      x2048highestNum = randomNumber;
      x2048highestType++;
      x2048_updateHighType();
    }
  }
}

void x2048_slideRow(int row[]) {
  int writeIndex = 0;
  int zeroCount = 0;
  for (int i = 0; i < 4; ++i) {
    if (row[i] != 0) {
      row[writeIndex++] = row[i];
    } else {
      zeroCount++;
    }
  }
  for (int i = writeIndex; i < 4; ++i) {
    row[i] = 0;
  }
}

void x2048_mergeRow(int row[]) {
  x2048_slideRow(row);
  bool merged = false;
  for (int i = 0; i < 3; i++) {
    if (row[i] == row[i + 1] && row[i] != 0) {
      row[i] *= 2;
      row[i + 1] = 0;
      x2048score += row[i];
      if (row[i] > x2048highestNum) {
        x2048highestNum = row[i];
        x2048highestType++;        
      }
      merged = true;
    }
  }
  if (merged) {
    x2048_slideRow(row);
  }
}

void x2048_reverseRow(int row[]) {
  for (int i = 0; i < 2; i++) {
    int temp = row[i];
    row[i] = row[3 - i];
    row[3 - i] = temp;
  }
}

void x2048_transpose() {
  int tempBoard[4][4];
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      tempBoard[i][j] = x2048board[j][i];
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      x2048board[i][j] = tempBoard[i][j];
    }
  }
}

void x2048_mergeLeft() {
  for (int i = 0; i < 4; i++) {
    x2048_mergeRow(x2048board[i]);
  }
}

void x2048_mergeRight() {
  for (int i = 0; i < 4; i++) {
    x2048_reverseRow(x2048board[i]);
    x2048_mergeRow(x2048board[i]);
    x2048_reverseRow(x2048board[i]);
  }
}

void x2048_mergeUp() {
  x2048_transpose();
  x2048_mergeLeft();
  x2048_transpose();
}

void x2048_mergeDown() {
  x2048_transpose();
  x2048_mergeRight();
  x2048_transpose();
}

void x2048_drawUI() {
  lcd.setColor(222, 173, 59);
  lcd.fillRect(80, 0, 400, 320);
  lcd.setColor(228, 188, 97);
  lcd.setBackColor(228, 188, 97);
  lcd.fillRect(0, 0, 80, 320);
  lcd.fillRect(400, 0, 480, 320);
  lcd.setColor(75, 75, 75);
  lcd.drawRect(0, 0, 80, 320);
  lcd.drawLine(400, 0, 400, 320);
  x2048_updateScore();
  x2048_updateHighType();
  lcd.print("PLEASE ROTATE CONSOLE!", 452, x2048_center_text(0, 320, 22, 8), -90);
  lcd.setFont(SevenSegNumFont);
  lcd.print("2048", 15, 160 + 64, -90);
  lcd.setColor(75, 75, 75);
  lcd.drawRect(80, 0, 400, 320);
  lcd.drawLine(160, 0, 160, 320);
  lcd.drawLine(240, 0, 240, 320);
  lcd.drawLine(320, 0, 320, 320);
  lcd.drawLine(80, 80, 400, 80);
  lcd.drawLine(80, 160, 400, 160);
  lcd.drawLine(80, 240, 400, 240);
  x2048_randomAdd();
  x2048_randomAdd();
  x2048_printBoard();
}

void x2048_setup() {
  x2048_drawUI();
  x2048SetupDone = true;
}

void x2048_loop() {
  if (!x2048SetupDone) {
    x2048_setup();
  } else {    
    if (move != x2048lastMove && move == JoystickMove::None) {
      int previousBoard[4][4];
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          previousBoard[i][j] = x2048board[i][j];
        }
      }
      if (x2048lastMove == JoystickMove::Down) {
        x2048_mergeLeft();
      } else if (x2048lastMove == JoystickMove::Up) {
        x2048_mergeRight();
      } else if (x2048lastMove == JoystickMove::Left) {
        x2048_mergeUp();
      } else if (x2048lastMove == JoystickMove::Right) {
        x2048_mergeDown();
      }
      bool boardChanged = false;
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (previousBoard[i][j] != x2048board[i][j]) {
            boardChanged = true;
            break;
          }
        }
        if (boardChanged) {
          break;
        }
      }
      if (boardChanged && (x2048lastMove == JoystickMove::Up || x2048lastMove == JoystickMove::Left || x2048lastMove == JoystickMove::Right || x2048lastMove == JoystickMove::Down)) {
        x2048_randomAdd();
        x2048_printBoard();
        x2048_updateScore();
        x2048_updateHighType();
      }
    }
    x2048lastMove = move;
    if (x2048_checkLoss()) {
      lcd.setColor(255, 0, 0);
      lcd.print("YOU HAVE LOST! TRY AGAIN?", 452, x2048_center_text(0, 320, 25, 8), -90);
    }
    if (digitalRead(ButtonR) == LOW) {
      x2048_reset();
      gameRunning = MENU;
      menu_reset();
    }
    if (digitalRead(ButtonH) == LOW) {
      x2048_reset();
      lcd.clrScr();
    }
  }
}
