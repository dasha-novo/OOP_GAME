#ifndef GAME_PLAYER_ROBOT_H_
#define GAME_PLAYER_ROBOT_H_

#include "robot.h"

class PlayerRobot : public Robot {
  public:
  using Robot::Robot;

  bool IsEnemy() const override;
  RobotType GetType() const override;
};

#endif