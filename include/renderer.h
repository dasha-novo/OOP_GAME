#ifndef GAME_RENDERER_H_
#define GAME_RENDERER_H_

#include <SFML/Graphics.hpp>
#include <string>

#include "game_state.h"

class GameField;
class GameWindow;

class Robot;

class Renderer { 
  public:
    Renderer(int cell_size);
    void Render(const GameField& field, GameWindow& window, GameState game_state, 
                const Robot& player_for_panel, const std::string& ability_message);
    void ShowLongrangeEffect(int goal_x, int goal_y);
    void ShowAreaEffect(int center_x, int center_y, int radius);
    void ShowPlayerMovement(int start_x, int start_y, int delta_x, int delta_y, int moved_cells);
    bool IsPlayerMoveActive() const;

bool IsPlayerMovementActive() const;
          
  private:
    int cell_size_;
    sf::Font font_;
    bool font_loaded_;

    sf::Texture player_texture_;
    sf::Texture enemy_texture_;
    sf::Texture healer_texture_;
    bool textures_loaded_;

    sf::Texture longrange_effect_texture_;
    sf::Texture area_effect_texture_;
    bool longrange_effect_loaded_ = false;
    bool area_effect_loaded_ = false;
    bool show_longrange_effect_ = false;
    bool show_area_effect_ = false;
    int longrange_effect_x_ = 0;
    int longrange_effect_y_ = 0;
    int area_effect_x_ = 0;
    int area_effect_y_ = 0;
    int area_effect_radius_ = 1;
    bool show_player_move_ = false;
    int player_move_start_x_ = 0;
    int player_move_start_y_ = 0;
    int player_move_delta_x_ = 0;
    int player_move_delta_y_ = 0;
    int player_move_cells_ = 0;

    sf::Clock player_move_time_;
    sf::Clock longrange_effect_time_;
    sf::Clock area_effect_time_;

    static constexpr float kAreaEffectTime = 0.25f;
    static constexpr float kLongrangeEffectTime = 0.25f;
    static constexpr float kPlayerCellMoveTime = 0.12f;
};

#endif