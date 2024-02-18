enum GameState {MENU, SIMON, DRAWING, JOKE, FLAPPY, DINO, X2048, PONG, SNAKE};
GameState gameRunning = MENU;
#include "start_setup.h"
#include "graphics.h"
#include "joystick_movement.h"
#include "game_menu.h"
#include "simon_game.h"
#include "drawing_game.h"
#include "joking_app.h"
#include "flappy_game.h"
#include "dino_game.h"
#include "x2048_game.h"
#include "pong_game.h"
#include "snake_game.h"

bool main_displayedLogo = false;
const byte main_multiplierSize = 3;

void setup() {
  randomSeed(analogRead(7));
  joystick_setup();
  screen_setup();
}

void loop() {
  if (!main_displayedLogo) {
    main_draw_startup();
    delay(3000);
    main_displayedLogo = true;
  } else {
    joystick_moveUpd();
    if (gameRunning == MENU) {
      menu_loop();
    } else if (gameRunning == SIMON) {
      simon_loop();
    } else if (gameRunning == DRAWING) {
      draw_loop();
    } else if (gameRunning == JOKE) {
      joke_loop();
    } else if (gameRunning == FLAPPY) {
      flappy_loop();
    } else if (gameRunning == DINO) {
      dino_loop();
    } else if (gameRunning == X2048) {
      x2048_loop();
    } else if (gameRunning == PONG) {
      pong_loop();
    } else if (gameRunning == SNAKE) {
      snake_loop();
    }
  }
}

void main_draw_startup() {
  const float factor = 1.6;
  lcd.setColor(255,0,0);
  for (int i=0; i<=320; i+=5) {
    lcd.drawLine(0, i, static_cast<int>(i * factor), 320);
  }
  for (int i=320; i>=0; i-=5) {
    lcd.drawLine(480, i, static_cast<int>(i * factor), 0);
  }
  lcd.setColor(0,255,255);
  for (int i=320; i>=0; i-=5) {
    lcd.drawLine(0, i, 480-static_cast<int>(i * factor), 0);
  }
  for (int i=0; i<=320; i+=5) {
    lcd.drawLine(480, i, 480-static_cast<int>(i * factor), 320);
  }
  lcd.setColor(0,255,0);
  for (int i=0; i<=155; i+=5) {
    lcd.drawLine(i, 0, i, 320);
    lcd.drawLine(480 - i, 0, 480 - i, 320);
    delay(20);
  }
  lcd.setColor(255,255,0);
  for (int i=0; i<=60; i+=5) {
    lcd.drawLine(0, i, 480, i);
    lcd.drawLine(0, 320 - i, 480, 320 - i);
    delay(20);
  }
  lcd.drawBitmap(240 - ((main_multiplierSize * 54)/2), 160 - ((main_multiplierSize * 64)/2), 54, 64, DELTECHLOGO, main_multiplierSize); 
}
