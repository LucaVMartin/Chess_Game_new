#include <SFML/Graphics.hpp>

#include "EventHandler.h"
#include "Game.h"
#include "UI.h"

int main() {
  // create window
  sf::RenderWindow window(sf::VideoMode(800, 800), "My Window");
  window.setFramerateLimit(30);

  Game game{};
  UI ui{&window, game};
  EventHandler e{game, ui, window};

  // run Program as long as window is open
  while (window.isOpen()) {
    e.handleEvents();
    window.clear();
    ui.drawBoard();
    window.display();
  }
  return 0;
}
