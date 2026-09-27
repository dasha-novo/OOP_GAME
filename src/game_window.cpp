#include "game_window.h"

GameWindow::GameWindow(int width, int height, const std::string& title)
    : window_(sf::VideoMode(width, height), title) {}

bool GameWindow::IsOpen() const {
  return window_.isOpen();
}

void GameWindow::Close() {
  window_.close();
}

bool GameWindow::PollEvent(sf::Event& event) {
  return window_.pollEvent(event);
}

void GameWindow::Clear() {
  window_.clear();
}

void GameWindow::Display() {
  window_.display();
}

void GameWindow::Draw(const sf::Drawable& object) {
  window_.draw(object);
}

