#ifndef GAME_GAME_WINDOW_H_
#define GAME_GAME_WINDOW_H_

#include <SFML/Graphics.hpp>
#include <string>

class GameWindow {
    public:
      GameWindow(int width, int height, const std::string& title);

      bool IsOpen() const;
      void Close();
      bool PollEvent(sf::Event& event);
      void Clear();
      void Display();
      void Draw(const sf::Drawable& object);

    private:
      sf::RenderWindow window_;
};

#endif