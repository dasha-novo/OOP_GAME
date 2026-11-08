#include "renderer.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "robot.h"
#include "game_field.h"
#include "game_window.h"  

Renderer::Renderer(int cell_size) : cell_size_(cell_size), font_loaded_(false),
                                    status_textures_loaded_(false) {
            font_loaded_ = font_.loadFromFile("assets/GoodNewsSHA.otf");

            if (!font_loaded_) {
              std::cerr << "Font loading error.\n";
            }

            bool player_loaded = player_texture_.loadFromFile("assets/player_robot.png");
            bool enemy_loaded = enemy_texture_.loadFromFile("assets/enemy_robot.png");
            bool healer_loaded = healer_texture_.loadFromFile("assets/healer_robot.png");
            bool slowdown_status_loaded = slowdown_status_texture_.loadFromFile("assets/slowdown_status.png");
            bool overload_status_loaded = overload_status_texture_.loadFromFile("assets/overload_status.png");
            bool burn_status_loaded = burn_status_texture_.loadFromFile("assets/burn_status.png");
            bool shield_status_loaded = shield_status_texture_.loadFromFile("assets/shield_status.png");
            status_textures_loaded_ = slowdown_status_loaded && overload_status_loaded && burn_status_loaded && shield_status_loaded;

            if (!status_textures_loaded_) {
                std::cerr << "Status texture loading error.\n";
            }

            longrange_effect_loaded_ = longrange_effect_texture_.loadFromFile("assets/longrange_attack.png");
            area_effect_loaded_ = area_effect_texture_.loadFromFile("assets/area_attack.png");

            if (!area_effect_loaded_) {
                std::cerr << "Area effect texture loading error.\n";
            }

            if (!longrange_effect_loaded_) {
                std::cerr << "Longrange effect texture loading error.\n";
            }

            textures_loaded_ = player_loaded && enemy_loaded && healer_loaded;
          
            if (!textures_loaded_) {
                std::cerr << "Texture loading error.\n";
            }

            backlight_loaded_ = backlight_texture_.loadFromFile("assets/backlight.png");

            if (!backlight_loaded_) {
                std::cerr << "Backlight texture loading error.\n";
            }
        }

void Renderer::Render(const GameField& field, GameWindow& window, GameState game_state, const Robot& player_for_panel, const std::string& ability_message) {
    int width_field = field.GetWidth();
    int height_field = field.GetHeight();

    float player_move_progress = 0.0f;

    if (show_player_move_) {
        const float elapsed_time = player_move_time_.getElapsedTime().asSeconds();
        const float total_time = kPlayerCellMoveTime * player_move_cells_;

        if (elapsed_time >= total_time) {
            show_player_move_ = false;
        } else {
            player_move_progress = elapsed_time / kPlayerCellMoveTime;
        }
    }

    const Robot* player_robot = nullptr;

    for (int x = 0; x < width_field; ++x) {
        for (int y = 0; y < height_field; ++y) {
          sf::RectangleShape cell;

          cell.setSize(sf::Vector2f(cell_size_, cell_size_));
          cell.setPosition(x * cell_size_, y * cell_size_);

          if (!field.IsPassable(x, y)) {
            cell.setFillColor(sf::Color(190, 196, 202));
          } else {
            int slowdown = field.GetSlowdown(x, y);

            switch (slowdown) {
                case 0:
                cell.setFillColor(sf::Color(183, 215, 168));
                break;

                case 1:
                cell.setFillColor(sf::Color(165, 195, 150));
                break;

                case 2:
                cell.setFillColor(sf::Color(135, 170, 120));
                break;

                case 3:
                cell.setFillColor(sf::Color(245, 220, 145));
                break;

                default:
                cell.setFillColor(sf::Color(183, 215, 168));
                break;
            }
          }

          cell.setOutlineThickness(1.0f);
          cell.setOutlineColor(sf::Color::Black);

          window.Draw(cell);

          if (backlight_loaded_ && IsCellHighlighted(field, x, y)) {
            sf::Sprite backlight;
            backlight.setTexture(backlight_texture_);
            sf::FloatRect bounds = backlight.getLocalBounds();

            if (bounds.width > 0.0f && bounds.height > 0.0f) {
                float scale_x = static_cast<float>(cell_size_) / bounds.width;
                float scale_y = static_cast<float>(cell_size_) / bounds.height;
                backlight.setScale(scale_x, scale_y);
            }

            backlight.setPosition(x * cell_size_, y * cell_size_);
            window.Draw(backlight);
          }
        
          const Robot* current_robot = field.GetRobotPtr(x, y);

          if (current_robot != nullptr && textures_loaded_) {
            if (current_robot->GetType() == RobotType::kPlayer) {
              player_robot = current_robot;
              if (show_player_move_) {
                    continue;
                }
            }

            sf::Sprite sprite;

            switch (current_robot->GetType()) {
              case RobotType::kPlayer:
              sprite.setTexture(player_texture_);
              break;

              case RobotType::kHealer:
                  sprite.setTexture(healer_texture_);
                  break;

              case RobotType::kEnemy:
                  sprite.setTexture(enemy_texture_);
                  break;
            }

            sf::FloatRect image_bounds = sprite.getLocalBounds();

            float scale_x = static_cast<float>(cell_size_) / image_bounds.width;
            float scale_y = static_cast<float>(cell_size_) / image_bounds.height;
            sprite.setScale(scale_x, scale_y);

            sprite.setPosition(x * cell_size_, y * cell_size_);
            window.Draw(sprite);
          }
      }

      if (show_player_move_ && player_robot != nullptr && textures_loaded_) {
          sf::Sprite moving_player;
          moving_player.setTexture(player_texture_);

          sf::FloatRect image_bounds =
          moving_player.getLocalBounds();

          if (image_bounds.width > 0 && image_bounds.height > 0) {
              float scale_x =  static_cast<float>(cell_size_) / image_bounds.width;
              float scale_y = static_cast<float>(cell_size_) / image_bounds.height;
              moving_player.setScale(scale_x, scale_y);
            }

            float player_x = player_move_start_x_ + player_move_delta_x_ * player_move_progress;
            float player_y = player_move_start_y_ + player_move_delta_y_ * player_move_progress;

            moving_player.setPosition(player_x * cell_size_, player_y * cell_size_);
            window.Draw(moving_player);
        }
    }

    if (player_robot != nullptr && font_loaded_) {
        sf::Text status_text;
        status_text.setFont(font_);
        status_text.setCharacterSize(20);

        status_text.setFillColor(sf::Color::White);
        status_text.setOutlineThickness(2.0f);
        status_text.setOutlineColor(sf::Color::Black);

        std::string text = "Health: " + std::to_string(player_robot->GetHealth()) + " / " + std::to_string(player_robot->GetMaxHealth()) +
            "    Energy: " + std::to_string(player_robot->GetEnergy()) + " / " + std::to_string(player_robot->GetMaxEnergy()) +
            "    Speed: " + std::to_string(player_robot->GetSpeed());

        status_text.setString(text);
        status_text.setPosition(10.0f, 10.0f);
        window.Draw(status_text);
    }

    if (show_longrange_effect_) {
        float elapsed_time = longrange_effect_time_.getElapsedTime().asSeconds();

        if (elapsed_time >= kLongrangeEffectTime) {
            show_longrange_effect_ = false;
        } else if (longrange_effect_loaded_) {
            sf::Sprite effect_sprite;
            effect_sprite.setTexture(longrange_effect_texture_);

            sf::FloatRect effect_bounds = effect_sprite.getLocalBounds();

            if (effect_bounds.width > 0 && effect_bounds.height > 0) {
                float scale_x = static_cast<float>(cell_size_) / effect_bounds.width;
                float scale_y = static_cast<float>(cell_size_) / effect_bounds.height;

                effect_sprite.setScale(scale_x, scale_y);
            }

            effect_sprite.setPosition(longrange_effect_x_ * cell_size_, longrange_effect_y_ * cell_size_);

            window.Draw(effect_sprite);
        }
    }

    if (show_area_effect_) {
        float elapsed_time = area_effect_time_.getElapsedTime().asSeconds();

        if (elapsed_time >= kAreaEffectTime) {
            show_area_effect_ = false;
        } else if (area_effect_loaded_) {
            for (int x = area_effect_x_ - area_effect_radius_; x <= area_effect_x_ + area_effect_radius_; ++x) {
                for (int y = area_effect_y_ - area_effect_radius_; y <= area_effect_y_ + area_effect_radius_; ++y) {

                    if (x == area_effect_x_ && y == area_effect_y_) {
                        continue;
                    }

                    if (x < 0 || x >= field.GetWidth() || y < 0 || y >= field.GetHeight()) {
                        continue;
                    }

                    sf::Sprite effect_sprite;
                    effect_sprite.setTexture(area_effect_texture_);

                    sf::FloatRect effect_bounds = effect_sprite.getLocalBounds();

                    if (effect_bounds.width > 0 && effect_bounds.height > 0) {
                        float scale_x = static_cast<float>(cell_size_) / effect_bounds.width;
                        float scale_y = static_cast<float>(cell_size_) / effect_bounds.height;

                        effect_sprite.setScale(scale_x, scale_y);
                    }

                    effect_sprite.setPosition(x * cell_size_, y * cell_size_);

                    window.Draw(effect_sprite);
                }
            }
        }
    }

    DrawPlayerStatuses(field, player_for_panel, window);

    if (font_loaded_) {
        const float panel_height = 100.0f;
        const float panel_y = field.GetHeight() * cell_size_;
        const float panel_width = field.GetWidth() * cell_size_;

        sf::RectangleShape ability_panel;
        ability_panel.setSize(sf::Vector2f(panel_width, panel_height));
        ability_panel.setPosition(0.0f, panel_y);
        ability_panel.setFillColor(sf::Color(190, 196, 202));

        window.Draw(ability_panel);

        sf::Text ability_text;

        ability_text.setFont(font_);
        ability_text.setCharacterSize(14);
        ability_text.setFillColor(sf::Color::White);
        ability_text.setOutlineThickness(1.0f);
        ability_text.setOutlineColor(sf::Color::Black);
        ability_text.setPosition(8.0f, panel_y + 4.0f);

        std::string ability_text_value;

        if (game_state == GameState::kChoosingAbility) {
            ability_text_value = "Choose starting ability:\n" "[H] Heal   " "[T] Teleport   "
                                    "[A] Area attack   " "[R] Long-range attack";
        } else if (game_state == GameState::kChoosingDisplayMode) {
            ability_text_value = "Display mode:\n" "[1] Standard   [2] Enemies[3]   Impassable cells   [4] Status cells";
        } else {
            ability_text_value = "Abilities:\n";

            if (player_for_panel.HasAbility(AbilityType::kHeal)) {
                ability_text_value += "Heal [H]   ";
            }

            if (player_for_panel.HasAbility(AbilityType::kTeleport)) {
                ability_text_value += "Teleport [T]   ";
            }

            if (player_for_panel.HasAbility(AbilityType::kAreaAttack)) {
                ability_text_value += "Area attack [A]   ";
            }

            if (player_for_panel.HasAbility(AbilityType::kLongrangeAttack)) {
                ability_text_value += "Long-range attack [R]";
            }
        }

        ability_text.setString(ability_text_value);
        window.Draw(ability_text);

        sf::Text rank_text;
        rank_text.setFont(font_);
        rank_text.setCharacterSize(12);
        rank_text.setFillColor(sf::Color::White);
        rank_text.setOutlineThickness(1.0f);
        rank_text.setOutlineColor(sf::Color::Black);

        rank_text.setString("Rank: " + std::to_string(player_for_panel.GetRank()) +
            "    Experience: " + std::to_string(player_for_panel.GetExperience()) +
            " / " + std::to_string(player_for_panel.GetExperienceToRankUp()));

        rank_text.setPosition(8.0f, panel_y + 58.0f);
        window.Draw(rank_text);

        sf::Text display_mode_hint;
        display_mode_hint.setFont(font_);
        display_mode_hint.setCharacterSize(12);
        display_mode_hint.setFillColor(sf::Color::White);
        display_mode_hint.setOutlineThickness(1.0f);
        display_mode_hint.setOutlineColor(sf::Color::Black);
        display_mode_hint.setString("[L] Display mode");
        sf::FloatRect hint_bounds = display_mode_hint.getLocalBounds();
        const float hint_gap = 8.0f;
        display_mode_hint.setPosition(panel_width - hint_bounds.width - hint_gap, panel_y + 58.0f);
        window.Draw(display_mode_hint);

        if (!ability_message.empty()) {
            sf::Text error_text;

            error_text.setFont(font_);
            error_text.setCharacterSize(12);
            error_text.setFillColor(sf::Color::Red);
            error_text.setOutlineThickness(1.0f);
            error_text.setOutlineColor(sf::Color::Black);

            error_text.setString(ability_message);
            error_text.setPosition(8.0f, panel_y + 40.0f);

            window.Draw(error_text);
        }

        int experience_to_rank = player_for_panel.GetExperienceToRankUp();

        float experience_progress = 0.0f;

        if (experience_to_rank > 0) {
            experience_progress = static_cast<float>(player_for_panel.GetExperience()) / experience_to_rank;
        }

        if (experience_progress < 0.0f) experience_progress = 0.0f;
        if (experience_progress > 1.0f) experience_progress = 1.0f;

        const float experience_bar_y = panel_y + 80.0f;
        const float experience_bar_height = 15.0f;

        sf::RectangleShape experience_background;

        experience_background.setSize(sf::Vector2f(panel_width, experience_bar_height));
        experience_background.setPosition( 0.0f,experience_bar_y);
        experience_background.setFillColor(sf::Color(70, 70, 70));

        window.Draw(experience_background);

        sf::RectangleShape experience_fill;
        experience_fill.setSize(sf::Vector2f(panel_width * experience_progress,experience_bar_height));
        experience_fill.setPosition(0.0f,experience_bar_y);
        experience_fill.setFillColor(sf::Color(255, 238, 140));

        window.Draw(experience_fill);
    }

    if (game_state == GameState::kChoosingRankAbility && font_loaded_) {
        const float field_width = field.GetWidth() * cell_size_;
        const float field_height = field.GetHeight() * cell_size_;

        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(field_width, field_height));
        overlay.setPosition(0.0f, 0.0f);
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.Draw(overlay);

        const float panel_width = 620.0f;
        const float panel_height = 250.0f;
        const float panel_x = (field_width - panel_width) / 2.0f;
        const float panel_y = (field_height - panel_height) / 2.0f;

        sf::RectangleShape rank_panel;
        rank_panel.setSize(sf::Vector2f(panel_width, panel_height));
        rank_panel.setPosition(panel_x, panel_y);
        rank_panel.setFillColor(sf::Color(35, 35, 50, 245));
        rank_panel.setOutlineThickness(3.0f);
        rank_panel.setOutlineColor(sf::Color(255, 238, 140));
        window.Draw(rank_panel);

        sf::Text rank_title;
        rank_title.setFont(font_);
        rank_title.setCharacterSize(28);
        rank_title.setFillColor(sf::Color(240, 200, 40));
        rank_title.setOutlineThickness(1.0f);
        rank_title.setOutlineColor(sf::Color::Black);

        rank_title.setString("Rank increased!");

        rank_title.setPosition(panel_x + 175.0f,panel_y + 22.0f);

        window.Draw(rank_title);
        sf::Text rank_description;
        rank_description.setFont(font_);
        rank_description.setCharacterSize(17);
        rank_description.setFillColor(sf::Color::White);
        rank_description.setOutlineThickness(1.0f);
        rank_description.setOutlineColor(sf::Color::Black);

        rank_description.setString("You can improve an existing ability\n" "or add a new one.");
        rank_description.setPosition(panel_x + 125.0f, panel_y + 78.0f);
        window.Draw(rank_description);

        sf::Text ability_choices;
        ability_choices.setFont(font_);
        ability_choices.setCharacterSize(16);
        ability_choices.setFillColor(sf::Color::White);
        ability_choices.setOutlineThickness(1.0f);
        ability_choices.setOutlineColor(sf::Color::Black);

        ability_choices.setString("[H] Heal       [T] Teleport\n"
                                  "[A] Area attack       " "[R] Long-range attack");

        ability_choices.setPosition(panel_x + 75.0f, panel_y + 145.0f);
        window.Draw(ability_choices);

        sf::Text rank_hint;
        rank_hint.setFont(font_);
        rank_hint.setCharacterSize(14);
        rank_hint.setFillColor(sf::Color(200, 200, 200));
        rank_hint.setString("Press a key to choose");
        rank_hint.setPosition(panel_x + 215.0f,panel_y + 210.0f);
        window.Draw(rank_hint);

        return;
    }

    if (game_state == GameState::kPlaying || game_state == GameState::kChoosingAbility 
        || game_state == GameState::kChoosingDisplayMode || !font_loaded_) {
        return;
    }

    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(field.GetWidth() * cell_size_, field.GetHeight() * cell_size_));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.Draw(overlay);

    sf::Text message;
    message.setFont(font_);
    message.setCharacterSize(50);

    if (game_state == GameState::kVictory) {
        message.setFillColor(sf::Color(152, 251, 152));
        message.setOutlineThickness(1.0f);
        message.setOutlineColor(sf::Color::Black);
        message.setString("Victory!");
    } else {
        message.setFillColor(sf::Color(178, 34, 34));
        message.setOutlineThickness(1.0f);
        message.setOutlineColor(sf::Color::Black);
        message.setString("Game Over!");
    }

    message.setPosition(265.0f, 350.0f);
    window.Draw(message);
}

void Renderer::ShowLongrangeEffect(int goal_x, int goal_y) {
    longrange_effect_x_ = goal_x;
    longrange_effect_y_ = goal_y;

    show_longrange_effect_ = true;

    longrange_effect_time_.restart();
}

void Renderer::ShowAreaEffect(int center_x, int center_y, int radius) {
    area_effect_x_ = center_x;
    area_effect_y_ = center_y;
    area_effect_radius_ = radius;

    show_area_effect_ = true;
    area_effect_time_.restart();
}

void Renderer::ShowPlayerMovement(int start_x, int start_y, int delta_x, int delta_y, int moved_cells) {
    if (moved_cells <= 0) {
        return;
    }

    player_move_start_x_ = start_x;
    player_move_start_y_ = start_y;
    player_move_delta_x_ = delta_x;
    player_move_delta_y_ = delta_y;
    player_move_cells_ = moved_cells;

    show_player_move_ = true;
    player_move_time_.restart();
}

bool Renderer::IsPlayerMovementActive() const {
    if (!show_player_move_) {
        return false;
    }

    const float total_time =
        kPlayerCellMoveTime * player_move_cells_;

    return player_move_time_.getElapsedTime().asSeconds() < total_time;
}

void Renderer::DrawPlayerStatuses(const GameField& field, const Robot& player, GameWindow& window) const {
    const StatusType status_types[] = {StatusType::kSlowdown, StatusType::kOverload,
                                        StatusType::kBurn, StatusType::kShield};

    const float icon_x = static_cast<float>(field.GetWidth() * cell_size_) - kStatusIconSize - 12.0f;
    float icon_y = 12.0f;

    for (StatusType type : status_types) {
        if (!player.HasStatus(type)) continue;

        DrawStatusIcon(type, player.GetStatusValue(type), icon_x, icon_y, window);
        icon_y += kStatusIconSize + kStatusIconGap;
    }
}

void Renderer::DrawStatusIcon(StatusType type, int value, float x, float y, GameWindow& window) const {
    const sf::Texture* texture = nullptr;

    switch (type) {
        case StatusType::kSlowdown:
            texture = &slowdown_status_texture_;
            break;

        case StatusType::kOverload:
            texture = &overload_status_texture_;
            break;

        case StatusType::kBurn:
            texture = &burn_status_texture_;
            break;

        case StatusType::kShield:
            texture = &shield_status_texture_;
            break;
    }

    if (texture == nullptr) return;

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sf::FloatRect bounds = sprite.getLocalBounds();

    if (bounds.width <= 0.0f || bounds.height <= 0.0f) return;

    float scale_x = kStatusIconSize / bounds.width;
    float scale_y = kStatusIconSize / bounds.height;
    sprite.setScale(scale_x, scale_y);
    sprite.setPosition(x, y);
    window.Draw(sprite);

    if (!font_loaded_) return;

    sf::Text value_text;
    value_text.setFont(font_);
    value_text.setCharacterSize(18);
    value_text.setFillColor(sf::Color::White);
    value_text.setOutlineThickness(1.0f);
    value_text.setOutlineColor(sf::Color::Black);
    value_text.setString(std::to_string(value));
    value_text.setPosition(x - 40.0f, y + 7.0f);

    window.Draw(value_text);
}

bool Renderer::IsCellHighlighted(const GameField& field, int x, int y) const {
    
    switch (display_mode_) {
        case DisplayMode::kStandard:
            return false;

        case DisplayMode::kEnemies: {
            const Robot* robot = field.GetRobotPtr(x, y);
            return robot != nullptr && robot->GetType() == RobotType::kEnemy;
        }

        case DisplayMode::kImpassableCells:
            return !field.IsPassable(x, y);

        case DisplayMode::kStatusCells:
            return field.HasCellStatuses(x, y);
    }

    return false;
}

void Renderer::SetDisplayMode(DisplayMode mode) {
    display_mode_ = mode;
}