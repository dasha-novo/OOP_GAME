#ifndef GAME_CELL_H_
#define GAME_CELL_H_

#include <random>
#include <vector>

#include "status_type.h"

class Robot;

struct CellStatus {
    StatusType type;
    int amount;
};

class Cell {
  public:
    Cell(bool passable, int slowdown = 0);
    bool IsPassable() const;
    bool IsOccupied() const;
    Robot* GetRobot() const;
    bool AddRobot(Robot& robot);
    void RemoveRobot();
    int GetSlowdown() const;
    void AddStatus(StatusType type, int amount);
    bool TakeRandomStatus(std::mt19937& rng, CellStatus& result);
    bool HasStatuses() const;

  private:
    bool is_passable_;
    int slowdown_;
    Robot* current_robot_;
    std::vector<CellStatus> statuses_;

};

#endif