#include <Joystick.h>

enum class JoystickMove {None, Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight};
JoystickMove move = JoystickMove::None;
Joystick joystick(A0, A1, 5);

void joystick_setup() {
  joystick.initialize();
  joystick.setSensivity(0);
}

void joystick_moveUpd() {
  if (joystick.isUpperLeft() || joystick.isUpperRight() || joystick.isLowerLeft() || joystick.isLowerRight() || joystick.isLeft() || joystick.isRight() || joystick.isUp() || joystick.isDown()) {
    if (joystick.isUpperLeft() || joystick.isUpperRight() || joystick.isLowerLeft() || joystick.isLowerRight()) {
      if (joystick.isUpperLeft()) {move = JoystickMove::DownRight;}
      if (joystick.isUpperRight()) {move = JoystickMove::DownLeft;}
      if (joystick.isLowerLeft()) {move = JoystickMove::UpRight;}
      if (joystick.isLowerRight()) {move = JoystickMove::UpLeft;}
    } else {
      if (joystick.isLeft()) {move = JoystickMove::Right;}
      if (joystick.isRight()) {move = JoystickMove::Left;}
      if (joystick.isUp()) {move = JoystickMove::Down;}
      if (joystick.isDown()) {move = JoystickMove::Up;}
    }
  } else {
    move = JoystickMove::None;
  }
}
