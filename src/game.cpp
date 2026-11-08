#include "game.h"

#include <iostream>

Game::Game()
    : window_(800, 900, "My sweet game^^"),
      field_(20, 20),
      renderer_(kCellSize),
      player_robot_(100, 25, 100, 100),
      enemy_robots_(),
      healer_robots_(),
      is_initialized_(false),
      rng_(std::random_device{}()),
      waiting_for_target_(false),
      pending_ability_(AbilityType::kNone),
      ability_message_(),
      defeated_enemies_count_(0),
      player_move_pending_(false),
      game_state_(GameState::kChoosingAbility) {
        enemy_robots_.push_back(EnemyRobot(100, 25, 100, 100));
        enemy_robots_.push_back(EnemyRobot(100, 25, 100, 100));
        enemy_robots_.push_back(EnemyRobot(100, 25, 100, 100));
        healer_robots_.push_back(HealerRobot(70, 10, 120, 100));
}

void Game::Run() {
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
        MakeComputerRobotTurn(enemy_robot);
    }

    for (HealerRobot& healer_robot : healer_robots_) {
        MakeComputerRobotTurn(healer_robot);
    }
}

void Game::MakeComputerRobotTurn(Robot& robot) {
    if (!robot.IsAlive()) return;
    robot.UpdateStatuses();
    if (!robot.IsAlive()) return;
    AbilityType possible_abilities[] = {AbilityType::kHeal, AbilityType::kTeleport,
                                              AbilityType::kAreaAttack, AbilityType::kLongrangeAttack};
    std::vector<AbilityType> available_abilities;

    for (AbilityType type: possible_abilities) {
        if (robot.HasAbility(type)) {
            available_abilities.push_back(type);
        }
    }

    if (available_abilities.empty()) {
        MoveRobotRandomly(robot);
        return;
    }

    std::uniform_int_distribution<int> action_distribution(0, 1);

    bool use_ability = action_distribution(rng_) == 1;

    if (!use_ability) {
        MoveRobotRandomly(robot);
        return;
    }

    std::uniform_int_distribution<int> ability_distribution(0, static_cast<int>(available_abilities.size()) - 1);
    AbilityType selected_ability = available_abilities[ability_distribution(rng_)];

    std::uniform_int_distribution<int> x_distribution(0, field_.GetWidth() - 1);
    std::uniform_int_distribution<int> y_distribution(0, field_.GetHeight() - 1);
    int goal_x = x_distribution(rng_);
    int goal_y = y_distribution(rng_);

    robot.UseAbility(selected_ability,field_,goal_x,goal_y);
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

    if (player_move_pending_) continue;

    if (game_state_ == GameState::kChoosingAbility) {
      if (event.type == sf::Event::KeyPressed) {
          ChooseInitialAbility(event.key.code);
      }
      continue;
    }

    if (game_state_ == GameState::kChoosingDisplayMode) {
      if (event.type == sf::Event::KeyPressed) {
          ChooseDisplayMode(event.key.code);
      }
      continue;
    }

    if (game_state_ == GameState::kChoosingRankAbility) {
      if (event.type == sf::Event::KeyPressed) {
          ChooseRankAbility(event.key.code);
      }
      continue;
    }

    if (game_state_ != GameState::kPlaying) continue;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
      HandleTargetClick(event.mouseButton.x, event.mouseButton.y);
      continue;
    }

    if (event.type != sf::Event::KeyPressed) continue;

    if (waiting_for_target_) continue;

    MoveResult player_action_result = MoveResult::kFailed;
    int moved_cells = 0;
    bool ability_used = false;

    int move_start_x = -1;
    int move_start_y = -1;

    field_.GetRobotPosition(player_robot_, move_start_x, move_start_y);

    int move_delta_x = 0;
    int move_delta_y = 0;

    switch (event.key.code) {
      case sf::Keyboard::Left:
        player_robot_.UpdateStatuses();
        if (player_robot_.IsAlive()) {
          move_delta_x = -1;
          move_delta_y = 0;
          player_action_result = field_.MoveRobot(player_robot_, -1, 0, moved_cells);
        }
        break;

      case sf::Keyboard::Right:
        player_robot_.UpdateStatuses();
        if (player_robot_.IsAlive()) {
          move_delta_x = 1;
          move_delta_y = 0;
          player_action_result = field_.MoveRobot(player_robot_, 1, 0, moved_cells);
        }
        break;

      case sf::Keyboard::Up:
        player_robot_.UpdateStatuses();
        if (player_robot_.IsAlive()) {
          move_delta_x = 0;
          move_delta_y = -1;
          player_action_result = field_.MoveRobot(player_robot_, 0, -1, moved_cells);
        }
        break;

      case sf::Keyboard::Down:
        player_robot_.UpdateStatuses();
        if (player_robot_.IsAlive()) {
          move_delta_x = 0;
          move_delta_y = 1;
          player_action_result =  field_.MoveRobot(player_robot_, 0, 1, moved_cells);
        }
        break;

      case sf::Keyboard::H:
        player_robot_.UpdateStatuses();
        if (player_robot_.IsAlive()) {
          ability_used = UseImmediateAbility(AbilityType::kHeal);
        }
        break;

      case sf::Keyboard::A:
        player_robot_.UpdateStatuses();
        if (player_robot_.IsAlive()) {
          ability_used = UseImmediateAbility(AbilityType::kAreaAttack);
          if (ability_used) {
            int player_x{-1};
            int player_y{-1};

            if (field_.GetRobotPosition(player_robot_, player_x, player_y)) {
                renderer_.ShowAreaEffect(player_x, player_y, 1);
            }
          }
        }
        break;

      case sf::Keyboard::T:
        StartTargetAbility(AbilityType::kTeleport);
        break;

      case sf::Keyboard::R:
        StartTargetAbility(AbilityType::kLongrangeAttack);
        break;

      case sf::Keyboard::L:
        game_state_ = GameState::kChoosingDisplayMode;
        break;

      default:
        break;
    }

    if (!player_robot_.IsAlive()) {
      CheckGameState();
      continue;
    }

    if (moved_cells > 0 && player_robot_.IsAlive()) {
      player_robot_.RestoreEnergy(moved_cells * kEnergyPerCell);
    }

    if (player_action_result != MoveResult::kFailed) {
      ability_message_.clear();

      if (moved_cells > 0 && player_robot_.IsAlive() && move_start_x >= 0) {
          renderer_.ShowPlayerMovement(move_start_x, move_start_y, move_delta_x, move_delta_y, moved_cells);
          player_move_pending_ = true;
      } else {
          ProcessTurn();
      }
    } else if (ability_used) {
      ability_message_.clear();
      CheckGameState();
    }
  }
}

void Game::Update() {
  if (player_move_pending_ && !renderer_.IsPlayerMovementActive()) {
        player_move_pending_ = false;
        ProcessTurn();
    }
}

void Game::Render() {
  window_.Clear();
  renderer_.Render(field_, window_, game_state_, player_robot_, ability_message_);
  window_.Display();
}


bool Game::InitializeRobots() {
  if (!field_.PlaceRobotOnFreeCell(player_robot_)) {
    return false;
  }

  std::uniform_int_distribution<> dist(0, 1);
  for (EnemyRobot& enemy : enemy_robots_) {
    int random_ability = dist(rng_);

    if (random_ability == 0) {
      enemy.AddAbility(AbilityType::kHeal);
    } else {
      enemy.AddAbility(AbilityType::kTeleport);
    }

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
  UpdatePlayerExperience();
  
  if (!player_robot_.IsAlive()) {
    game_state_ = GameState::kGameOver;
  } else if (game_state_ == GameState::kChoosingRankAbility) {
    return;
  } else if (AreAllEnemiesDefeated()) {
    game_state_ = GameState::kVictory;
  }
}

void Game::ShowMissingAbilityError() {
    ability_message_ = "Error! Ability is missing.";
}

bool Game::UseImmediateAbility(AbilityType type) {
    ability_message_.clear();
    if (!player_robot_.HasAbility(type)) {
        ShowMissingAbilityError();
        return false;
    }
    return player_robot_.UseAbility(type, field_, 0, 0);
}

void Game::StartTargetAbility(AbilityType type) {
    ability_message_.clear();
    if (!player_robot_.HasAbility(type)) {
        waiting_for_target_ = false;
        pending_ability_ = AbilityType::kNone;
        ShowMissingAbilityError();
        return;
    }

    pending_ability_ = type;
    waiting_for_target_ = true;
}

void Game::HandleTargetClick(int mouse_x, int mouse_y) {
    if (!waiting_for_target_) {
        return;
    }

    int field_pixel_width = field_.GetWidth() * kCellSize;
    int field_pixel_height = field_.GetHeight() * kCellSize;

    if (mouse_x < 0 || mouse_x >= field_pixel_width ||
        mouse_y < 0 || mouse_y >= field_pixel_height) {
        waiting_for_target_ = false;
        pending_ability_ = AbilityType::kNone;
        return;
    }

    int goal_x = mouse_x / kCellSize;
    int goal_y = mouse_y / kCellSize;

    player_robot_.UpdateStatuses();
    if (!player_robot_.IsAlive()) {
      waiting_for_target_ = false;
      pending_ability_ = AbilityType::kNone;
      CheckGameState();
      return;
    }

    AbilityType used_ability = pending_ability_;
    int slowdowns_before = player_robot_.GetStatusValue(StatusType::kSlowdown);
    bool ability_used = player_robot_.UseAbility(pending_ability_, field_, goal_x, goal_y);
    int slowdowns_after = player_robot_.GetStatusValue(StatusType::kSlowdown);

    bool is_blocked_teleport = !ability_used && used_ability == AbilityType::kTeleport 
        && slowdowns_after < slowdowns_before;

    waiting_for_target_ = false;
    pending_ability_ = AbilityType::kNone;

    if (ability_used) {
      if (used_ability == AbilityType::kLongrangeAttack) {
          renderer_.ShowLongrangeEffect(goal_x, goal_y);
      }

      ability_message_.clear();
      CheckGameState();
    }

    if (is_blocked_teleport) {
      ability_message_.clear();
      ProcessTurn();
    }
}

bool Game::ChooseInitialAbility(sf::Keyboard::Key key) {
    AbilityType selected_ability = AbilityType::kNone;

    switch (key) {
        case sf::Keyboard::H:
            selected_ability = AbilityType::kHeal;
            break;

        case sf::Keyboard::T:
            selected_ability = AbilityType::kTeleport;
            break;

        case sf::Keyboard::A:
            selected_ability = AbilityType::kAreaAttack;
            break;

        case sf::Keyboard::R:
            selected_ability = AbilityType::kLongrangeAttack;
            break;

        default:
            return false;
    }

    player_robot_.AddAbility(selected_ability);

    is_initialized_ = InitializeRobots();

    if (is_initialized_) {
        game_state_ = GameState::kPlaying;
    } else {
        game_state_ = GameState::kGameOver;
    }

    return true;
}

void Game::UpdatePlayerExperience() {
    int defeated_enemies = 0;

    for (const EnemyRobot& enemy : enemy_robots_) {
        if (!enemy.IsAlive()) defeated_enemies++;
    }

    int old_rank = player_robot_.GetRank();

    while (defeated_enemies_count_ < defeated_enemies) {
        player_robot_.GainExperience(kExperienceForEnemy);
        defeated_enemies_count_++;

        if (player_robot_.GetRank() > old_rank) {
          game_state_ = GameState::kChoosingRankAbility;
        }
    }
}

bool Game::ChooseRankAbility(sf::Keyboard::Key key) {
    AbilityType selected_ability = AbilityType::kNone;

    switch (key) {
        case sf::Keyboard::H:
            selected_ability = AbilityType::kHeal;
            break;

        case sf::Keyboard::T:
            selected_ability = AbilityType::kTeleport;
            break;

        case sf::Keyboard::A:
            selected_ability = AbilityType::kAreaAttack;
            break;

        case sf::Keyboard::R:
            selected_ability = AbilityType::kLongrangeAttack;
            break;

        default:
            return false;
    }

    if (player_robot_.HasAbility(selected_ability)) {
        if (!player_robot_.UpgradeAbility(selected_ability)) {
            return false;
        }
    } else {
        player_robot_.AddAbility(selected_ability);

        if (!player_robot_.HasAbility(selected_ability)) {
            return false;
        }
    }

    ability_message_.clear();
    game_state_ = GameState::kPlaying;

    return true;
}

void Game::ChooseDisplayMode(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Num1:
            renderer_.SetDisplayMode(DisplayMode::kStandard);
            break;

        case sf::Keyboard::Num2:
            renderer_.SetDisplayMode(DisplayMode::kEnemies);
            break;

        case sf::Keyboard::Num3:
            renderer_.SetDisplayMode(DisplayMode::kImpassableCells);
            break;

        case sf::Keyboard::Num4:
            renderer_.SetDisplayMode(DisplayMode::kStatusCells);
            break;

        case sf::Keyboard::Escape:
            game_state_ = GameState::kPlaying;
            return;

        default:
            return;
    }

    game_state_ = GameState::kPlaying;
}