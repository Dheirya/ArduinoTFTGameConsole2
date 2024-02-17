const float dnGravity = 0.39;
const float dnJumpSpeed = -6;
float dnCactiSpeed = 3;
bool dinoSetupDone = false;
bool dinoGameStart = false;
bool dinoGameOverPrinted = false;
bool dinoDied = false;
float dinoPosY = 252;
float dinoVelocity = 0;
int dinoRounds = 0;
class Cactus {
  public:
    int posX;
    bool isShown;
    bool hideAtEnd;
    Cactus(int initialPosX, bool shown, bool hidden) : posX(initialPosX), isShown(shown), hideAtEnd(hidden) {}
    void move() {
      posX -= dnCactiSpeed;
      if (posX <= -3) {
        posX = 480;
        dinoRounds++;
        lcd.setColor(127, 127, 127);
        lcd.print("SCORE: " + String(dinoRounds), CENTER, 305);
        if (dinoRounds > EEPROM.read("DINO_HIGHSCORE")) {EEPROM.update("DINO_HIGHSCORE", dinoRounds);}
        if (dnCactiSpeed < 5.9) {dnCactiSpeed += 0.1;}
        if (hideAtEnd) {isShown = false;}
      }
    }
    void draw() {
      if (isShown) {lcd.drawBitmap(posX, 260, 16, 15, cacti, 2);}
    }
};

Cactus cacti1(480, true, false);
Cactus cacti2(480, false, true);

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

void dino_reset() {
  dnCactiSpeed = 3;
  dinoSetupDone = false;
  dinoGameStart = false;
  dinoGameOverPrinted = false;
  dinoDied = false;
  dinoPosY = 252;
  dinoVelocity = 0;
  dinoRounds = 0;
  cacti1.posX = 480;
  cacti1.isShown = true;
  cacti1.hideAtEnd = false;
  cacti2.posX = 480;
  cacti2.isShown = false;
  cacti2.hideAtEnd = true;
}

void dino_drawUI() {
  lcd.setColor(204, 204, 204);
  lcd.fillRect(0, 0, 480, 320);
  lcd.setColor(0, 0, 0);
  lcd.fillRect(0, 300, 480, 320);
  lcd.setColor(0, 0, 0);
  lcd.fillRect(0, 0, 480, 25);
  lcd.setFont(BigFont);
  lcd.setColor(127, 127, 127);
  lcd.drawLine(0, 300, 480, 300);
  lcd.drawRect(0, 0, 479, 25);
  lcd.print("Dino: The Game of Jump", CENTER, 5);
  lcd.setFont(SmallFont);
  lcd.print("CLICK TO START", CENTER, 305);
  for (int i = 0; i < 480; i += 15) {
    lcd.drawBitmap(i, 290, 15, 10, ground);
  }
  lcd.drawBitmap(50, 40, 15, 9, cloud, 2);
  lcd.drawBitmap(130, 90, 15, 9, cloud, 2);
  lcd.drawBitmap(220, 60, 15, 9, cloud, 2);
  lcd.drawBitmap(320, 80, 15, 9, cloud, 2);
  lcd.drawBitmap(400, 110, 15, 9, cloud, 2);
  lcd.drawBitmap(430, 26, 10, 10, sun, 5);
  lcd.drawBitmap(0, 260, 16, 15, cacti, 2);
  lcd.drawBitmap(40, dinoPosY, 15, 19, dino, 2);
}

void dino_setup() {
  dino_drawUI();
  dinoSetupDone = true;
}

void dino_loop() {
  if (!dinoSetupDone && !dinoDied) {
    dino_setup();
  } else {
    if (!dinoDied) {
      if (digitalRead(ButtonH) == LOW) {
        if (!dinoGameStart) {
          lcd.setColor(0, 0, 0);
          lcd.fillRect(0, 301, 480, 320);
          lcd.setColor(127, 127, 127);
          lcd.print("SCORE: 0", CENTER, 305);
          dinoGameStart = true;
        } else {
          if (dinoPosY == 252) {
            dinoVelocity = dnJumpSpeed;
          }
        }
      }
      if (dinoGameStart) {
        dinoVelocity += dnGravity;
        if (dinoPosY + dinoVelocity <= 252) {
          dinoPosY += dinoVelocity;
        } else {
          dinoVelocity = 0;
          dinoPosY = 252;
        }
        if (!cacti2.isShown) {
          if (dinoRounds % 3 == 0 && dinoRounds != 0) {          
            if (130 < cacti1.posX && cacti1.posX < 350) {
              int tRandom = random(100);
              if (tRandom % 33 == 0) {
                cacti2.posX = 480;
                cacti2.isShown = true;
              }
            }
          }
        }
        cacti1.move();
        cacti1.draw();
        if (cacti2.isShown) {
          cacti2.move();
          cacti2.draw();
        }
        if ((cacti1.isShown && checkCollision(40, dinoPosY, 15, 17, cacti1.posX, 260, 16, 15)) || (cacti2.isShown && checkCollision(40, dinoPosY, 15, 17, cacti2.posX, 260, 16, 15))) {
          dinoDied = true;
        }
        lcd.drawBitmap(40, dinoPosY, 15, 19, dino, 2);    
      }      
    } else {
      if (!dinoGameOverPrinted) {
        delay(650);
        lcd.clrScr();
        lcd.drawBitmap(150, 63, 30, 19, game_over, 6);
        lcd.setFont(BigFont);
        lcd.setColor(255, 0, 0);
        lcd.print("You died from", CENTER, 190);
        lcd.print("hitting a cactus", CENTER, 210);
        lcd.setColor(150, 75, 0);
        lcd.print("YOUR SCORE: " + String(dinoRounds), CENTER, 235);
        lcd.print("HIGHSCORE: " + String(EEPROM.read("DINO_HIGHSCORE")), CENTER, 255);
        dinoGameOverPrinted = true;
      }
      if (digitalRead(ButtonR) == LOW) {
        dino_reset();
        gameRunning = MENU;
        menu_reset();
      }
      if (digitalRead(ButtonH) == LOW) {
        dino_reset();
        lcd.clrScr();
      }
    }
  }
}
