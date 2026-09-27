#ifndef GAME_HEALER_ROBOT_H_
#define GAME_HEALER_ROBOT_H_

#include "robot.h"

class HealerRobot : public Robot {
  public:
  using Robot::Robot;

  bool IsEnemy() const override;
  RobotType GetType() const override;
};

#endif