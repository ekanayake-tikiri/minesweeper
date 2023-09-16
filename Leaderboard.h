#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#pragma once
using namespace std;

struct Leaderboard {
    vector<string> names;
    vector<int> times;
    vector<bool> stars;
    sf::Font font;
    sf::Text title;
    sf::Text lbtext;
    string big_one;
    vector<sf::Text> positions;

    float width;
    float height;
    int cols;
    int rows;

    Leaderboard(float width_, float height_, int cols_, int rows_);
    void add(const string& name, int time);
    void makeText();
    void draw();
};

