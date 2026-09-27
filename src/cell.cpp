#include "cell.h"

Cell::Cell(bool passable, int slowdown) 
  : is_passable_(passable),
    slowdown_(slowdown),
    current_robot_(nullptr) {}

bool Cell::IsPassable() const {
  return is_passable_;
}

bool Cell::IsOccupied() const {
  return current_robot_ != nullptr;
}

Robot* Cell::GetRobot() const {
  return current_robot_;
}

bool Cell::AddRobot(Robot& robot) {
  if (IsOccupied() || !IsPassable()) return false;
  current_robot_ = &robot;
  return true;
}

void Cell::RemoveRobot() {
  current_robot_ = nullptr;
}

int Cell::GetSlowdown() const {
    return slowdown_;
}