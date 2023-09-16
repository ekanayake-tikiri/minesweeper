#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#pragma once
using namespace std;

struct Tile {
    sf::Texture texture;
    sf::Sprite da_tile;

    sf::Texture mine_t;
    sf::Sprite mine;

    sf::Texture revealed_t;
    sf::Sprite reveal;

    sf::Texture flag_t;
    sf::Sprite flag;

    sf::Texture number_t;
    sf::Sprite number;

    int col;
    int row;
    int board_rows;
    int board_cols;

    bool revealed;
    bool show_mine;
    bool flagged;
    int mines; // mines around it. 9 = is a  mine, 0 = has no mines around it
    vector<Tile*> neighbors;

    Tile(int x, int y, int boardx, int boardy);
    bool clicking(sf::Event& event, sf::Mouse& mouse, vector<vector<Tile>>& tiles);
    void unleash(vector<vector<Tile>>& tiles);
    void recursive(vector<vector<Tile>>& tiles);
    bool exists(int row_, int col_);
    void peepin_tom(vector<vector<Tile>>& tiles);
    void drawTile(sf::RenderWindow& window, bool paused);
};