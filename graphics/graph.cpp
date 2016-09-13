//
// graph.cpp for  in /home/rimokh_y/projects/cpp_tracker/graphics
//
// Made by Yohan Rimokh
// Login   <rimokh_y@epitech.net>
//
// Started on  Thu Jul 21 22:35:43 2016 Yohan Rimokh
// Last update Thu Jul 21 22:37:51 2016 Yohan Rimokh
//

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
      std::cerr << "no font\n";
    sf::Text text;

    text.setFont(font);
    text.setString("Hello world");
    text.setCharacterSize(24);
    text.setColor(sf::Color::Black);
    while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
        {
          // évènement "fermeture demandée" : on ferme la fenêtre
          if (event.type == sf::Event::Closed)
            window.close();
        }
      window.clear(sf::Color::White);
      window.draw(text);
      window.display();
    }
    return 0;
}
