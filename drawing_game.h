enum DrawingModes {DRAW, CURSOR, ERASER};
DrawingModes drawingMode = DRAW;
bool drawSetupDone = false;
int drawXCursor = 240;
int drawYCursor = 160;

void draw_reset() {
  drawSetupDone = false;
  drawXCursor = 240;
  drawYCursor = 160;
  drawingMode = DRAW;
}

void draw_gridlines() {
  lcd.setColor(20, 20, 20);
  for (int y = 27; y < 299; y += 40) {
    lcd.drawLine(0, y, 480, y);
  }
  for (int x = 0; x <= 480; x += 40) {
    lcd.drawLine(x, 27, x, 299);
  }
}

void drawing_drawUI() {
  lcd.setColor(236, 140, 140);
  lcd.drawLine(0, 0, 0, 26);
  lcd.drawLine(0, 0, 480, 0);
  lcd.drawLine(479, 0, 479, 26);
  lcd.drawLine(0, 26, 480, 26);
  lcd.setFont(BigFont);
  lcd.print("Drawing: Creativity App", CENTER, 5);
  lcd.drawLine(0, 299, 480, 299);
  draw_gridlines();
  lcd.setColor(236, 140, 140);
  lcd.setFont(SmallFont);
  lcd.print(" MODE: DRAW", LEFT, 305);
  lcd.print(String(drawXCursor/2) + "," + String(drawYCursor/2 - 15) + " ", RIGHT, 305);
}

void draw_setup() {
  drawing_drawUI();
  lcd.setColor(255, 255, 255);
  drawSetupDone = true;
}

void draw_loop() {
  if (!drawSetupDone) {
    draw_setup();
  } else {
    lcd.setColor(236, 140, 140);
    if (digitalRead(ButtonH) == LOW && digitalRead(ButtonR) == LOW) {
      draw_reset();
      gameRunning = MENU;
      menu_reset();
    } else if (digitalRead(ButtonH) == LOW) {
      if (drawingMode != CURSOR) {
        drawingMode = CURSOR;
        lcd.print(" MODE: CURSOR", LEFT, 305);
      }
    } else if (digitalRead(ButtonR) == LOW) {
      if (drawingMode != ERASER) {
        drawingMode = ERASER;
        lcd.print(" MODE: ERASER", LEFT, 305);
      }
    } else {
      if (drawingMode != DRAW) {
        drawingMode = DRAW;
        lcd.print(" MODE: DRAW  ", LEFT, 305);
      }
    }
    if (move != JoystickMove::None) {
      if (move == JoystickMove::Left && drawXCursor - 2 >= 2) {
        drawXCursor -= 2;
      } else if (move == JoystickMove::Right && drawXCursor + 2 < 479) {
        drawXCursor += 2;
      } else if (move == JoystickMove::Down && drawYCursor + 2 < 297) {
        drawYCursor += 2;
      } else if (move == JoystickMove::Up && drawYCursor - 2 > 28) {
        drawYCursor -= 2;
      } else if (move == JoystickMove::UpLeft) {
        if ((drawXCursor - 2) >= 2) {
          drawXCursor -= 2;
        }
        if ((drawYCursor - 2) > 28) {
          drawYCursor -= 2;
        }
      } else if (move == JoystickMove::UpRight) {
        if ((drawXCursor + 2) < 479) {
          drawXCursor += 2;
        }
        if ((drawYCursor - 2) > 28) {
          drawYCursor -= 2;
        }
      } else if (move == JoystickMove::DownLeft) {
        if ((drawXCursor - 2) >= 2) {
          drawXCursor -= 2;
        }
        if ((drawYCursor + 2) < 297) {
          drawYCursor += 2;
        }
      } else if (move == JoystickMove::DownRight) {
        if ((drawXCursor + 2) < 479) {
          drawXCursor += 2;
        }
        if ((drawYCursor + 2) < 297) {
          drawYCursor += 2;
        }
      }
      lcd.print("   " + String(drawXCursor/2 - 1) + "," + String(drawYCursor/2 - 15) + " ", RIGHT, 305);
      if (drawingMode == ERASER) {
        lcd.setColor(0, 0, 0);
        lcd.fillRect(drawXCursor - 2, drawYCursor - 2, drawXCursor + 2, drawYCursor + 2);
        draw_gridlines();
      } else if (drawingMode == DRAW) {      
        lcd.setColor(255, 255, 255);
        lcd.fillRect(drawXCursor - 0.5, drawYCursor - 0.5, drawXCursor + 0.5, drawYCursor + 0.5);
      }
    }
  }
}
