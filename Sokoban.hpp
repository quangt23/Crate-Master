// Copyright 2023
// Quang Tran
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum Direction {
    Up = sf::Keyboard::Up,
    Down = sf::Keyboard::Down,
    Left = sf::Keyboard::Left,
    Right = sf::Keyboard::Right
};

class Sokoban : public sf::Drawable{
 public:
    Sokoban();

    // Getters
    int width() const { return _width;}
    int height() const { return _height;}
    const std::vector<std::vector<char>>& getGameState() const {
        return game_state;
    }

    void movePlayer(Direction direction);
    void reset();
    bool isWon() const;
    void playVictorySound();
    friend std::istream& operator>>(std::ifstream& file, Sokoban& game);
    friend std::ostream& operator<<(std::ostream& os, Sokoban &game);

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            if (game_state[i][j] == '#') {
                sf::Sprite floorSprite(floor);
                floorSprite.setPosition(j * 64, i * 64);
                target.draw(floorSprite, states);
                sf::Sprite wallSprite(wall);
                wallSprite.setPosition(j * 64, i * 64);
                target.draw(wallSprite, states);
            } else if (game_state[i][j] == '.') {
                sf::Sprite floorSprite(floor);
                floorSprite.setPosition(j * 64, i * 64);
                target.draw(floorSprite, states);
            } else if (game_state[i][j] == 'A') {
                sf::Sprite floorSprite(floor);
                floorSprite.setPosition(j * 64, i * 64);
                target.draw(floorSprite, states);
                sf::Sprite crateSprite(crate);
                crateSprite.setPosition(j * 64, i * 64);
                target.draw(crateSprite, states);
            } else if (game_state[i][j] == 'a') {
                sf::Sprite squareFSprite(squareFloor);
                squareFSprite.setPosition(j * 64, i * 64);
                target.draw(squareFSprite, states);
            } else if (game_state[i][j] == '@') {
                // Draw the player on top
                if (orig_state[i][j] == 'a') {
                    sf::Sprite squareFSprite(squareFloor);
                    squareFSprite.setPosition(j * 64, i * 64);
                    target.draw(squareFSprite, states);
                    if (currDirection == Down) {
                        sf::Sprite playerSprite(playerF);
                        playerSprite.setPosition(j * 64, i * 64);
                        target.draw(playerSprite, states);
                    } else if (currDirection == Left) {
                        sf::Sprite playerSprite(playerL);
                        playerSprite.setPosition(j * 64, i * 64);
                        target.draw(playerSprite, states);
                    } else if (currDirection == Right) {
                        sf::Sprite playerSprite(playerR);
                        playerSprite.setPosition(j * 64, i * 64);
                        target.draw(playerSprite, states);
                    } else if (currDirection == Up) {
                        sf::Sprite playerSprite(playerU);
                        playerSprite.setPosition(j * 64, i * 64);
                        target.draw(playerSprite, states);
                    }
                } else {
                    sf::Sprite floorSprite(floor);
                    floorSprite.setPosition(j * 64, i * 64);
                    target.draw(floorSprite, states);
                    if (currDirection != Up && currDirection != Down &&
                    currDirection != Left && currDirection != Right) {
                        sf::Sprite playerFSprite(playerF);
                        playerFSprite.setPosition(j * 64, i * 64);
                        target.draw(playerFSprite, states);
                    }
                    if (currDirection == Down) {
                        sf::Sprite playerSprite(playerF);
                        playerSprite.setPosition(j * 64, i * 64);
                        target.draw(playerSprite, states);
                    } else if (currDirection == Left) {
                        sf::Sprite playerSprite(playerL);
                        playerSprite.setPosition(j * 64, i * 64);
                        target.draw(playerSprite, states);
                    } else if (currDirection == Right) {
                        sf::Sprite playerSprite(playerR);
                        playerSprite.setPosition(j * 64, i * 64);
                        target.draw(playerSprite, states);
                    } else if (currDirection == Up) {
                        sf::Sprite playerSprite(playerU);
                        playerSprite.setPosition(j * 64, i * 64);
                        target.draw(playerSprite, states);
                    }
                }
            }
        }
    }
}

 private:
    int _width, _height;
    int totalCrates;
    int crateDestinations;
    int correctCrates;
    Direction currDirection;
    sf::Texture wall;
    sf::Texture floor;
    sf::Texture crate;
    sf::Texture squareFloor;
    sf::Texture playerF, playerL, playerR, playerU;
    sf::SoundBuffer victorySoundBuffer;
    sf::Sound victorySound;
    std::vector<std::vector<char>> game_state;
    std::vector<std::vector<char>> orig_state;
    std::vector<std::vector<char>> one_state;
};
