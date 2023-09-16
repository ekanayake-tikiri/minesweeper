#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#pragma once
using namespace std;

struct Board {
    float cols;
    float rows;
    bool won;
    bool chillin;
    bool paused;
    int flags;
    int mines;
    float time;

    sf::Texture happy_t;
    sf::Sprite happy;
    sf::Texture win_t;
    sf::Sprite win;
    sf::Texture lose_t;
    sf::Sprite lose;

    sf::Texture debug_t;
    sf::Sprite debug;
    sf::Texture pause_t;
    sf::Sprite pause;
    sf::Texture play_t;
    sf::Sprite play;
    sf::Texture LB_t;
    sf::Sprite LB;

    sf::Texture digits_t;
    sf::Sprite digits;

    Board(float cols_, float rows_);
    void clear(vector<vector<Tile>>& tiles);
    sf::Sprite makeZero(float x) const;
    vector<sf::Sprite> counter(int num) const;
    vector<sf::Sprite> timer(int seconds);
    bool slaycheck(vector<vector<Tile>>& tiles, int mine_count);
    void draw(sf::RenderWindow& window);
};