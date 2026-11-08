#include <cstdlib>
#include <algorithm>

#include "game_field.h"
#include "robot.h"

int GameField::ControlSize(int size) {
  if (size < kMinFieldSize) return kMinFieldSize;
  if (size > kMaxFieldSize) return kMaxFieldSize;
  return size;
}

GameField::GameField(int width, int height)
    : width_(ControlSize(width)), 
      height_(ControlSize(height)),
      cells_(std::vector<std::vector<Cell>>(height_, std::vector<Cell>(width_, Cell(true)))),
      rng_(std::random_device{}()) { 
    GenerateCells();
}

void GameField::GenerateCells() {
  std::bernoulli_distribution blocked(0.1);

  std::uniform_int_distribution<int> slowdown_random_value(0, 99);
  std::uniform_int_distribution<int> status_count_random_value(0, 99);
  std::uniform_int_distribution<int> status_type_random_value(0, 3);

  for (int y=0; y < height_; y++) {
    for (int x=0; x < width_; x++) {
      if (blocked(rng_)) {
        cells_[y][x] = Cell(false);
        continue;
      }

      int random_value = slowdown_random_value(rng_);
      int slowdown = 0;

      if (random_value < 85) {
        slowdown = 0;
      } else if (random_value < 97) {
        slowdown = 1;
      } else if (random_value < 99) {
        slowdown = 2;
      } else slowdown = 3;

      cells_[y][x] = Cell(true, slowdown);

      random_value = status_count_random_value(rng_);
      int count_status = 0;

      if (random_value > 94) {
        count_status = 2;
      } else if (random_value > 79) {
        count_status = 1;
      }

      for (int i=0; i < count_status; ++i) {
        int random_type = status_type_random_value(rng_);
        if (random_type == 0) {
          cells_[y][x].AddStatus(StatusType::kSlowdown, 1);
        } else if (random_type == 1) {
          cells_[y][x].AddStatus(StatusType::kOverload, 1);
        } else if (random_type == 2) {
          cells_[y][x].AddStatus(StatusType::kBurn, 5);
        } else {
          cells_[y][x].AddStatus(StatusType::kShield, 50);
        }
      }
    }
  }
}

int GameField::GetWidth() const {
  return width_;
}

int GameField::GetHeight() const {
  return height_;
}

bool GameField::IsPassable(int x, int y) const {
  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
    return false;
  }
  return cells_[y][x].IsPassable();
}

bool GameField::PlaceRobot(Robot& robot, int x, int y) {
  if (!IsPassable(x, y)) return false;
  return cells_[y][x].AddRobot(robot);
}

MoveResult GameField::MoveRobotOneCell(Robot& robot, int delta_x, int delta_y) {
  int old_x{-1};
  int old_y{-1};
 
  if (!FindRobotPosition(robot, old_x, old_y)) return MoveResult::kFailed;
  int new_x = old_x + delta_x;
  int new_y = old_y + delta_y;

  if (!IsPassable(new_x, new_y)) return MoveResult::kFailed;

  Robot* other_robot = cells_[new_y][new_x].GetRobot();

  if (other_robot != nullptr) {
    if (robot.Interact(*other_robot)) {
      if (!other_robot->IsAlive()) {
            cells_[new_y][new_x].RemoveRobot();
        }
      return MoveResult::kInteracted;
    }
    return MoveResult::kFailed;
  }

  Cell& new_cell = cells_[new_y][new_x];
  if (new_cell.AddRobot(robot)) {
    cells_[old_y][old_x].RemoveRobot();
    ApplyCellStatus(robot, new_cell);
    return MoveResult::kMoved;
  }

  return MoveResult::kFailed;
}

bool GameField::PlaceRobotOnFreeCell(Robot& robot) {
  std::vector<int> free_cells;

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      if (IsPassable(x, y) && !cells_[y][x].IsOccupied() && cells_[y][x].GetSlowdown() < 3){
        free_cells.push_back(y * width_ + x);
      }
    }
  }

  if (free_cells.empty()) return false;

  std::uniform_int_distribution<int> distribution(0, free_cells.size() - 1);

  int random_position = distribution(rng_);
  int index = free_cells[random_position];

  int x = index % width_;
  int y = index / width_;

  return PlaceRobot(robot, x, y);
}

const Robot* GameField::GetRobotPtr(int x, int y) const {
  if (y < 0 || y >= height_ || x < 0 || x >= width_) {
    return nullptr;
  }
  if (!cells_[y][x].IsOccupied()) {
    return nullptr;
  }
  return cells_[y][x].GetRobot();
}

int GameField::GetSlowdown(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return 0;
    }
    return cells_[y][x].GetSlowdown();
}

bool GameField::FindRobotPosition(const Robot& robot, int& x, int& y) const {
    for (int current_y = 0; current_y < height_; ++current_y) {
        for (int current_x = 0; current_x < width_; ++current_x) {
            if (cells_[current_y][current_x].GetRobot() == &robot) {
                x = current_x;
                y = current_y;

                return true;
            }
        }
    }
    return false;
}

MoveResult GameField::MoveRobot(Robot& robot, int delta_x, int delta_y, int& moved_cells) {
    moved_cells = 0;

    if (robot.IsAlive() &&
      robot.TryBlockMovement()) {
      return MoveResult::kBlockedByStatus;
    }

    MoveResult result = MoveResult::kFailed;
    bool is_player = (robot.GetType() == RobotType::kPlayer);
    int max_steps = robot.GetSpeed();

    for (int step = 0; step < max_steps; ++step) {
        MoveResult step_result = MoveRobotOneCell(robot, delta_x, delta_y);

        if (step_result == MoveResult::kMoved) {
            ++moved_cells;
            result = MoveResult::kMoved;

            int current_x{-1};
            int current_y{-1};

            if (FindRobotPosition(robot, current_x, current_y)) {
                if (GetSlowdown(current_x, current_y) == 3 && is_player) {
                    robot.SetSpeed(0);
                    robot.Die();
                    cells_[current_y][current_x].RemoveRobot();
                    return result;
                }
            }
            continue;
        }

        if (step_result == MoveResult::kInteracted) result = MoveResult::kInteracted;
        break;
    }

    if (moved_cells > 0) {
        int current_x{-1};
        int current_y{-1};

        if (FindRobotPosition(robot, current_x, current_y)) {
            int slowdown = GetSlowdown(current_x, current_y);
            int new_speed = robot.GetSpeed() - slowdown;

            if (!is_player && new_speed < 1) new_speed = 1;
            
            robot.SetSpeed(new_speed);

            if (robot.GetSpeed() == 0 && is_player) {
                robot.Die();
                cells_[current_y][current_x].RemoveRobot();
            }
        }
    }
    return result;
}

bool GameField::TeleportRobot(Robot& robot, int goal_x, int goal_y, int max_radius) {
  int current_x{-1};
  int current_y{-1};
  if (!FindRobotPosition(robot, current_x, current_y)) return false;
  
  int distance = std::max(std::abs(goal_x - current_x), std::abs(goal_y -  current_y));
  if (distance > max_radius) return false;

  if (PlaceRobot(robot, goal_x, goal_y)) {
    cells_[current_y][current_x].RemoveRobot();
    ApplyCellStatus(robot, cells_[goal_y][goal_x]);
    return true;
  }
  return false;
}

Robot* GameField::GetRobotForChange(int x, int y) {
  if (y < 0 || y >= height_ || x < 0 || x >= width_) {
    return nullptr;
  }
  if (!cells_[y][x].IsOccupied()) {
    return nullptr;
  }
  return cells_[y][x].GetRobot();
}

bool GameField::GetRobotPosition(const Robot& robot, int& x, int& y) const {
    return FindRobotPosition(robot, x, y);
}

bool GameField::RemoveDeadRobot(int x, int y) {
  Robot* robot = GetRobotForChange(x, y);
  if (robot != nullptr && !robot->IsAlive()) {
    cells_[y][x].RemoveRobot();
    return true;
  }

  return false;
}

void GameField::ApplyCellStatus(Robot& robot, Cell& cell) {
    CellStatus selected_status;

    if (!cell.TakeRandomStatus(rng_, selected_status)) {
        return;
    }

    robot.AddStatus(selected_status.type, selected_status.amount);

    if (!robot.IsAlive()) {
        cell.RemoveRobot();
    }
}

bool GameField::HasCellStatuses(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return false;
    }

    return cells_[y][x].HasStatuses();
}