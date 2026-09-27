#ifndef GAME_CELL_H_
#define GAME_CELL_H_

class Robot;

class Cell {
  public:
    Cell(bool passable, int slowdown = 0);
    bool IsPassable() const;
    bool IsOccupied() const;
    Robot* GetRobot() const;
    bool AddRobot(Robot& robot);
    void RemoveRobot();
    int GetSlowdown() const;

  private:
    bool is_passable_;
    int slowdown_;
    Robot* current_robot_;

};

#endif