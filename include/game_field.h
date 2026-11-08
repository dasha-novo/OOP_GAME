#ifndef GAME_GAME_FIELD_H_
#define GAME_GAME_FIELD_H_

#include <vector>
#include <random>

#include "cell.h"

enum class MoveResult {
    kMoved,
    kInteracted,
    kFailed,
    kBlockedByStatus
};

class GameField {
  public:
    GameField(int width, int height);

    int GetWidth() const;
    int GetHeight() const;
    bool IsPassable(int x, int y) const;
    bool PlaceRobot(Robot& robot, int x, int y);
    MoveResult MoveRobot(Robot& robot, int delta_x, int delta_y, int& moved_cells);
    bool PlaceRobotOnFreeCell(Robot& robot);
    const Robot* GetRobotPtr(int x, int y) const;
    int GetSlowdown(int x, int y) const;
    bool TeleportRobot(Robot& robot, int goal_x, int goal_y, int max_radius);
    bool GetRobotPosition(const Robot& robot, int& x, int& y) const;
    Robot* GetRobotForChange(int x, int y);
    bool RemoveDeadRobot(int x, int y);
    bool HasCellStatuses(int x, int y) const;

    static constexpr int kMinFieldSize = 3;
    static constexpr int kMaxFieldSize = 100;

  private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> cells_;
    std::mt19937 rng_;

    static int ControlSize(int size);
    void GenerateCells();
    MoveResult MoveRobotOneCell(Robot& robot, int delta_x, int delta_y);
    bool FindRobotPosition(const Robot& robot, int& x, int& y) const;
    void ApplyCellStatus(Robot& robot, Cell& cell);
  };

#endif