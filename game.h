#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <vector>
#include <random>

#include "game_window.h"
#include "game_field.h"
#include "renderer.h"
#include "enemy_robot.h"
#include "player_robot.h"
#include "healer_robot.h"
#include "game_state.h"

class Game {
  public:
    Game();
    void Run();

    static constexpr int kEnergyPerCell = 5;

  private:
    GameWindow window_;
    GameField field_;
    Renderer renderer_;
    PlayerRobot player_robot_;
    std::vector<EnemyRobot> enemy_robots_;
    std::vector<HealerRobot> healer_robots_;
    bool is_initialized_;
    std::mt19937 rng_;
    GameState game_state_;

    bool InitializeRobots();
    void HandleEvents();
    void Update();
    void Render();
    void ProcessTurn();
    void MoveComputerRobots();
    void MoveRobotRandomly(Robot& robot);
    void CheckGameState();
    bool AreAllEnemiesDefeated() const;

};

#endif