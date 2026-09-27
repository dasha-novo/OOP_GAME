#include "renderer.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "robot.h"
#include "game_field.h"
#include "game_window.h"  

Renderer::Renderer(int cell_size) : cell_size_(cell_size), font_loaded_(false) {
            font_loaded_ = font_.loadFromFile("assets/GoodNewsSHA.otf");

            if (!font_loaded_) {
              std::cerr << "Font loading error.\n";
            }

            bool player_loaded = player_texture_.loadFromFile("assets/player_robot.png");
            bool enemy_loaded = enemy_texture_.loadFromFile("assets/enemy_robot.png");
            bool healer_loaded = healer_texture_.loadFromFile("assets/healer_robot.png");
          
            textures_loaded_ = player_loaded && enemy_loaded && healer_loaded;
          
            if (!textures_loaded_) std::cerr << "Texture loading error.\n";
          }

void Renderer::Render(const GameField& field, GameWindow& window, GameState game_state) const {
    int width_field = field.GetWidth();
    int height_field = field.GetHeight();

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
        
          const Robot* current_robot = field.GetRobotPtr(x, y);

          if (current_robot != nullptr && textures_loaded_) {
            if (current_robot->GetType() == RobotType::kPlayer) {
                player_robot = current_robot;
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

            sprite.setPosition(
                x * cell_size_,
                y * cell_size_
            );

            window.Draw(sprite);
          }
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
        status_text.setPosition(120.0f, 10.0f);
        window.Draw(status_text);
    }
    if (game_state == GameState::kPlaying || !font_loaded_) return;

    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(field.GetWidth() * cell_size_, field.GetHeight() * cell_size_));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.Draw(overlay);

    sf::Text message;

    message.setFont(font_);
    message.setCharacterSize(50);

    if (game_state == GameState::kVictory) {
        message.setFillColor(sf::Color::Green);
        message.setString("Victory!");
    } else {
        message.setFillColor(sf::Color::Red);
        message.setString("Game Over!");
    }

    message.setPosition(270.0f, 350.0f);
    window.Draw(message);
}