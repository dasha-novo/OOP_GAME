#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <vector>
#include <random>
#include <string>

#include "game_window.h"
#include "game_field.h"
#include "renderer.h"
#include "enemy_robot.h"
#include "player_robot.h"
#include "healer_robot.h"
#include "game_state.h"
#include "ability_type.h"

class Game {
  public:
    Game();
    void Run();

    static constexpr int kEnergyPerCell = 5;
    static constexpr int kCellSize = 40;
    static constexpr int kExperienceForEnemy = 50;

  private:
    GameWindow window_;
    GameField field_;
    Renderer renderer_;
    PlayerRobot player_robot_;
    std::vector<EnemyRobot> enemy_robots_;
    std::vector<HealerRobot> healer_robots_;
    bool is_initialized_;
    std::mt19937 rng_;
    bool waiting_for_target_;
    AbilityType pending_ability_;
    std::string ability_message_;
    int defeated_enemies_count_;
    bool player_move_pending_;
    GameState game_state_;
    
    bool InitializeRobots();
    void HandleEvents();
    void Update();
    void Render();
    void ProcessTurn();
    void MoveComputerRobots();
    void MakeComputerRobotTurn(Robot& robot);
    void MoveRobotRandomly(Robot& robot);
    void CheckGameState();
    bool AreAllEnemiesDefeated() const;
    bool UseImmediateAbility(AbilityType type);
    void StartTargetAbility(AbilityType type);
    void HandleTargetClick(int mouse_x, int mouse_y);
    void ShowMissingAbilityError();
    bool ChooseInitialAbility(sf::Keyboard::Key key);
    void UpdatePlayerExperience();
    bool ChooseRankAbility(sf::Keyboard::Key key);

};

#endif