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

void Cell::AddStatus(StatusType type, int amount) {
    if (amount <= 0) return;
    statuses_.push_back(CellStatus{type, amount});
}

bool Cell::TakeRandomStatus(std::mt19937& rng, CellStatus& result) {
    if (statuses_.empty()) return false;

    std::uniform_int_distribution<int> distribution(0, statuses_.size() - 1);
    int index = distribution(rng);
    result = statuses_[index];
    statuses_.erase(statuses_.begin() + index);

    return true;
}