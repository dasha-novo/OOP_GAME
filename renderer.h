#ifndef GAME_RENDERER_H_
#define GAME_RENDERER_H_

#include <SFML/Graphics.hpp>

#include "game_state.h"

class GameField;
class GameWindow;

class Renderer { 
  public:
    Renderer(int cell_size);
    void Render(const GameField& field, GameWindow& window, GameState game_state) const;

  private:
    int cell_size_;
    sf::Font font_;
    bool font_loaded_;

    sf::Texture player_texture_;
    sf::Texture enemy_texture_;
    sf::Texture healer_texture_;
    bool textures_loaded_;
};

#endif