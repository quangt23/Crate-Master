// Copyright 2023
// Quang Tran

#include <iostream>
#include <fstream>
#include "Sokoban.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main(int argc, char* argv[]) {
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    bool hasPlayedVictorySound = false;

    Sokoban game;
    inputFile >> game;

    sf::Texture victoryTexture;
    if (!victoryTexture.loadFromFile("victory.png")) {
        std::cout << "Failed to load victory" << std::endl;
    }

    sf::Sprite victorySprite(victoryTexture);

    sf::RenderWindow window(sf::VideoMode(game.width() * 64, game.height() * 64), "Sokoban");

    victorySprite.setPosition((window.getSize().x - victorySprite.getLocalBounds().width) / 2,
                            (window.getSize().y - victorySprite.getLocalBounds().height) / 2);

    window.setPosition(sf::Vector2i(1000, 100));
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) {
                    if (!game.isWon()) {
                        game.movePlayer(Up);
                    }
                }
                if (event.key.code == sf::Keyboard::A) {
                    if (!game.isWon()) {
                        game.movePlayer(Left);
                    }
                }
                if (event.key.code == sf::Keyboard::S) {
                    if (!game.isWon()) {
                        game.movePlayer(Down);
                    }
                }
                if (event.key.code == sf::Keyboard::D) {
                    if (!game.isWon()) {
                        game.movePlayer(Right);
                    }
                }
                if (event.key.code == sf::Keyboard::R) {
                    game.reset();
                    hasPlayedVictorySound = false;
                }
            }
            if (game.isWon() && !hasPlayedVictorySound) {
                game.playVictorySound();
                hasPlayedVictorySound = true;
            }
        }

        window.clear(sf::Color::Black);
        window.draw(game);
        if (game.isWon()) {
            window.draw(victorySprite);
        }
        window.display();
    }

    return 0;
}
