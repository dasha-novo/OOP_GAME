#include "game.h"

#include <iostream>

Game::Game()
    : window_(800, 800, "My sweet game^^"),
      field_(20, 20),
      renderer_(40),
      player_robot_(100, 25, 100, 100),
      enemy_robots_(),
      healer_robots_(),
      is_initialized_(false),
      rng_(std::random_device{}()),
      game_state_(GameState::kPlaying) {
        enemy_robots_.push_back(EnemyRobot(100, 25, 100, 100));
        enemy_robots_.push_back(EnemyRobot(80, 20, 80, 120));
        healer_robots_.push_back(HealerRobot(70, 10, 120, 100));

        is_initialized_ = InitializeRobots();
}

void Game::Run() {
  if (!is_initialized_) return;

  while(window_.IsOpen()) {
    HandleEvents();
    Update();
    Render();
    }
}

void Game::MoveRobotRandomly(Robot& robot) {
  int moved_cells = 0;
  std::uniform_int_distribution<> dist(0, 3);
  int random_step = dist(rng_);

  switch (random_step) {
    case 0:
      field_.MoveRobot(robot, -1, 0, moved_cells);
      break;

    case 1:
      field_.MoveRobot(robot, 1, 0, moved_cells);
      break;

    case 2:
      field_.MoveRobot(robot, 0, -1, moved_cells);
      break;

    case 3:
      field_.MoveRobot(robot, 0, 1, moved_cells);
      break;
  }

  if (moved_cells > 0 && robot.IsAlive()) {
        robot.RestoreEnergy(moved_cells * kEnergyPerCell);
    }
}

void Game::MoveComputerRobots() {
  for (EnemyRobot& enemy_robot : enemy_robots_) {
    MoveRobotRandomly(enemy_robot);
  }

  for (HealerRobot& healer_robot : healer_robots_) {
    MoveRobotRandomly(healer_robot);
  }
}

void Game::ProcessTurn() {
  CheckGameState();
  if (game_state_ != GameState::kPlaying) return;
  MoveComputerRobots();
  CheckGameState();
}

void Game::HandleEvents() {
  sf::Event event;

  while (window_.PollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window_.Close();
    }

    if (game_state_ == GameState::kPlaying && event.type == sf::Event::KeyPressed) {
      MoveResult player_action_result = MoveResult::kFailed;

      int moved_cells = 0;

      switch (event.key.code) {
        case sf::Keyboard::Left:
          player_action_result = field_.MoveRobot(player_robot_, -1, 0, moved_cells);
          break;

        case sf::Keyboard::Right:
          player_action_result = field_.MoveRobot(player_robot_, 1, 0, moved_cells);
          break;

        case sf::Keyboard::Up:
          player_action_result = field_.MoveRobot(player_robot_, 0, -1, moved_cells);
          break;

        case sf::Keyboard::Down:
          player_action_result = field_.MoveRobot(player_robot_, 0, 1,  moved_cells);
          break;
        default:
          break;
      }

      if (moved_cells > 0 && player_robot_.IsAlive()) {
        player_robot_.RestoreEnergy(moved_cells * kEnergyPerCell);
      }

      if (player_action_result != MoveResult::kFailed) {
        ProcessTurn();
      }
    }
  }
}

void Game::Update() {}

void Game::Render() {
  window_.Clear();
  renderer_.Render(field_, window_, game_state_);
  window_.Display();
}


bool Game::InitializeRobots() {
  if (!field_.PlaceRobotOnFreeCell(player_robot_)) {
    return false;
  }

  for (EnemyRobot& enemy : enemy_robots_) {
    if (!field_.PlaceRobotOnFreeCell(enemy)) {
      return false;
    }
  }

  for (HealerRobot& healer : healer_robots_) {
    if (!field_.PlaceRobotOnFreeCell(healer)) {
      return false;
    }
  }

  return true;
}

bool Game::AreAllEnemiesDefeated() const {
  for (const EnemyRobot& enemy_robot : enemy_robots_) {
    if (enemy_robot.IsAlive()) return false;
  }
  return true;
}

void Game::CheckGameState() {
  if (!player_robot_.IsAlive()) {
    game_state_ = GameState::kGameOver;
  } else if (AreAllEnemiesDefeated()) {
    game_state_ = GameState::kVictory;
  }
}