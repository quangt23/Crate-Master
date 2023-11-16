// Copyright 2023
// Quang Tran
#include "Sokoban.hpp"

Sokoban::Sokoban() {
    correctCrates = 0;
    totalCrates = 0;
    crateDestinations = 0;
    if (!wall.loadFromFile("assets/block_06.png")) {
        std::cout << "Failed to load wall texture." << std::endl;
    }

    if (!floor.loadFromFile("assets/ground_01.png")) {
        std::cout << "Failed to load floor texture." << std::endl;
    }

    if (!crate.loadFromFile("assets/crate_03.png")) {
        std::cout << "Failed to load crate texture." << std::endl;
    }

    if (!squareFloor.loadFromFile("assets/ground_04.png")) {
        std::cout << "Failed to load square floor texture." << std::endl;
    }

    if (!playerF.loadFromFile("assets/player_05.png")) {
        std::cout << "Failed to load player forward texture." << std::endl;
    }

    if (!playerL.loadFromFile("assets/player_20.png")) {
        std::cout << "Failed to load player left texture." << std::endl;
    }

    if (!playerR.loadFromFile("assets/player_17.png")) {
        std::cout << "Failed to load player right texture." << std::endl;
    }

    if (!playerU.loadFromFile("assets/player_08.png")) {
        std::cout << "Failed to load player up texture." << std::endl;
    }

    if (!victorySoundBuffer.loadFromFile("assets/victory.wav")) {
        std::cout << "Failed to load victory sound." << std::endl;
    }

    victorySound.setBuffer(victorySoundBuffer);
}

void Sokoban::playVictorySound() {
    victorySound.play();
}

bool Sokoban::isWon() const {
    if (correctCrates == totalCrates || correctCrates == crateDestinations) {
        return true;
    } else if (crateDestinations == 0) {
        return true;
    }
    return false;
}

void Sokoban::movePlayer(Direction direction) {
    int playerXcord = -1;
    int playerYcord = -1;

    int prevCrateX = -1;
    int prevCrateY = -1;

    currDirection = direction;

    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            if (game_state[i][j] == '@') {
                playerXcord = j;
                playerYcord = i;
                break;
            }
        }
    }

    // Calculate the new position based on the direction
    int newplayerXcord = playerXcord;
    int newplayerYcord = playerYcord;

    if (direction == Up) {
        newplayerYcord -= 1;
    } else if (direction == Down) {
        newplayerYcord += 1;
    } else if (direction == Left) {
        newplayerXcord -= 1;
    } else if (direction == Right) {
        newplayerXcord += 1;
    }

    // Check if the new position is valid
    if (newplayerXcord >= 0 && newplayerXcord < _width
        && newplayerYcord >= 0 && newplayerYcord < _height) {
        char newTile = game_state[newplayerYcord][newplayerXcord];

        // Checks if not a wall or crate
         if (newTile == '.' || newTile == 'a') {
            // If current cord is a player and the original was the destination
            if (orig_state[playerYcord][playerXcord] == 'a') {
                game_state[playerYcord][playerXcord] = 'a';
            } else {
                game_state[playerYcord][playerXcord] = '.';
            }

            game_state[newplayerYcord][newplayerXcord] = '@';

        } else if (newTile == 'A') {
            // Pushing a crate
            int crateX = newplayerXcord;
            int crateY = newplayerYcord;

            prevCrateX = crateX;
            prevCrateY = crateY;

            // Calculate the new position for the crate
            if (direction == Up) {
                crateY -= 1;
            } else if (direction == Down) {
                crateY += 1;
            } else if (direction == Left) {
                crateX -= 1;
            } else if (direction == Right) {
                crateX += 1;
            }

            if (crateX >= 0 && crateX < _width && crateY >= 0 && crateY < _height) {
                char crateTile = game_state[crateY][crateX];
                // Allowed to push if ground sprites
                if (crateTile == '.' || crateTile == 'a') {
                    // Move the player and crate
                    if (game_state[playerYcord][playerXcord] == '@') {
                        // Check if the tile behind the player is 'a'
                        if (orig_state[playerYcord][playerXcord] == 'a') {
                            game_state[playerYcord][playerXcord] = 'a';
                        } else {
                            game_state[playerYcord][playerXcord] = '.';
                        }
                    }

                    game_state[newplayerYcord][newplayerXcord] = '@';

                    if (crateTile == '.') {
                        game_state[crateY][crateX] = 'A';  // Crate itself
                    } else if (crateTile == 'a') {
                        game_state[crateY][crateX] = 'A';  // Crate if at goal
                        if (orig_state[prevCrateY][prevCrateX] != 'a') {
                            correctCrates++;
                        } else if (orig_state[prevCrateY][prevCrateX] == 'a' &&
                                    orig_state[crateY][crateX] != 'a') {
                            correctCrates--;
                        }
                    }
                    // If the box was removed from the destination
                    if (orig_state[prevCrateY][prevCrateX] == 'a' && crateTile != 'a') {
                        correctCrates--;
                    }
                }
            }
        }
    }
}

void Sokoban::reset() {
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            game_state[i][j] = one_state[i][j];
            if (game_state[i][j] == '1') {
                game_state[i][j] = 'A';
            }
        }
    }
    correctCrates = 0;
}

std::istream& operator>>(std::ifstream& file, Sokoban& game) {
    int width, height;

    file >> height;
    file >> width;

    game._width = width;
    game._height = height;

    std::vector<std::vector<char>> gameState(height, std::vector<char>(width));
    // Lambda Requirement here:
    std::for_each(gameState.begin(), gameState.end(), [&game, &file](std::vector<char>& row) {
        std::for_each(row.begin(), row.end(), [&game, &file](char& tile) {
            file >> tile;
            if (tile == 'A') {
                game.totalCrates++;
            } else if (tile == 'a') {
                game.crateDestinations++;
            }
        });
    });

    // Algorithm Requirement here:
    std::copy(gameState.begin(), gameState.end(), std::back_inserter(game.game_state));
    std::copy(gameState.begin(), gameState.end(), std::back_inserter(game.orig_state));
    std::copy(gameState.begin(), gameState.end(), std::back_inserter(game.one_state));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (game.game_state[i][j] == '1') {
                game.game_state[i][j] = 'A';
                game.orig_state[i][j] = 'a';
            }
        }
    }
    return file;
}

std::ostream& operator<<(std::ostream& os, Sokoban& game) {
    for (int i = 0; i < game.height(); i++) {
        for (int j = 0; j < game.width(); j++) {
            os << game.game_state[i][j];
        }
        os << "\n";
    }
    return os;
}

