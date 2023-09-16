#include <iostream>
#include <SFML/Graphics.hpp>
#include <cctype>
#include "random.h"
#include "Tile.h"
#include "Board.h"
#include "Leaderboard.h"
#include <fstream>
using namespace std;

int main() {

    // loading config.cfg
    ifstream file("files/board_config.cfg", ios_base::binary);
    string col;
    string row;
    string mine_num;

    getline(file, col);
    float cols = stof(col);
    getline(file, row);
    float rows = stof(row);
    getline(file, mine_num);
    int mines = stoi(mine_num);
    if (cols <= 0 || rows <= 0) { return 0; }

    float width = cols * 32;
    float height = (rows * 32) + 100;
    sf::RenderWindow welcomewin(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);
    sf::RenderWindow game;
    sf::RenderWindow leaderboard;

    sf::Font font;
    font.loadFromFile("files/font.ttf");
    string name_text;

    while (welcomewin.isOpen()) {
        sf::Event event{};
        // welcome text
        sf::Text welctext;
        welctext.setFont(font);
        welctext.setString("WELCOME TO MINESWEEPER!");
        welctext.setCharacterSize(24);
        welctext.setStyle(sf::Text::Bold | sf::Text::Underlined);
        sf::FloatRect welcbounds = welctext.getLocalBounds();
        welctext.setOrigin(welcbounds.left + welcbounds.width/2.0f, welcbounds.top  + welcbounds.height/2.0f);
        welctext.setPosition(sf::Vector2f(width/2.0f,height/2.0f - 150));

        // enter your name
        sf::Text prompt;
        prompt.setFont(font);
        prompt.setString("Enter your name:");
        prompt.setCharacterSize(20);
        prompt.setStyle(sf::Text::Bold);
        sf::FloatRect promptbounds = prompt.getLocalBounds();
        prompt.setOrigin(promptbounds.left + promptbounds.width/2.0f, promptbounds.top  + promptbounds.height/2.0f);
        prompt.setPosition(sf::Vector2f(width/2.0f,height/2.0f - 75));

        // name input
        sf::Text name;
        name.setFont(font);
        name.setCharacterSize(18);
        name.setStyle(sf::Text::Bold);

        while (welcomewin.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { welcomewin.close();}
            else if (event.type == sf::Event::TextEntered) {
                string input;
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 8 && !name_text.empty()) {
                        name_text.erase(name_text.size() - 1,1);
                    }
                    if (isalpha(event.text.unicode) && name_text.size() < 10) {
                        name_text += static_cast<char>(event.text.unicode);
                        name_text[0] = toupper(name_text[0]);
                        for (int i = 1; i < name_text.size(); i++) {
                            name_text[i] = tolower(name_text[i]);
                        }
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !name_text.empty()) {
                    game.create(sf::VideoMode(width, height), "Game Window", sf::Style::Close);
                    welcomewin.close();
                }
            }
        }
        name.setString(name_text + "|");
        sf::FloatRect namebounds = name.getLocalBounds();
        name.setOrigin(namebounds.left + namebounds.width/2.0f, namebounds.top  + namebounds.height/2.0f);
        name.setPosition(sf::Vector2f(width/2.0f,height/2.0f - 45));
        name.setFillColor(sf::Color::Yellow);

        welcomewin.clear(sf::Color::Blue);
        welcomewin.draw(welctext);
        welcomewin.draw(prompt);
        welcomewin.draw(name);
        welcomewin.display();
    }

    // vector of tiles
    vector<vector<Tile>> tiles;
    float y = 0;
    for(int i = 0; i < rows; i++) {
        float x = 0;
        vector<Tile> temp;
        for(int j = 0; j < cols; j++) {
            Tile temp_tile(x, y, rows, cols);
            temp.push_back(temp_tile);
            x += 32;
        }
        tiles.push_back(temp);
        y += 32;
    }

    Board board(cols, rows);
    Leaderboard lb(16*cols, (rows*16)+50, cols, rows);
    bool paused = false;
    bool set_mines = false;
    bool u_ded = false;
    bool done = false;
    bool summer = true;
    float pause_time = 0;
    bool retrieve = false;
    bool stop = true;
    sf::Clock clock;
    sf::Clock pause_clock;

    while (game.isOpen()) {
        sf::Event event{};

        if (!set_mines) {
            int mine_count = 0;
            while (mine_count != mines) {
                int x = Random::Int(0, rows - 1);
                int y = Random::Int(0, cols - 1);
                if (tiles[x][y].mines != 9) {
                    tiles[x][y].mines = 9;
                    mine_count++;
                }
                board.mines = mines;
            }

            for(int i = 0; i < tiles.size(); i++){
                for(int j = 0; j < tiles[i].size(); j++) {
                    if (tiles[i][j].mines != 9) {
                        tiles[i][j].peepin_tom(tiles);
                    }
                }
            }
            set_mines = true;
        }

        while (game.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { game.close(); }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Mouse mouse;
                auto coords = mouse.getPosition(game);

                auto face_bounds = board.happy.getGlobalBounds();
                auto play_bounds = board.play.getGlobalBounds();
                auto lb_bounds = board.LB.getGlobalBounds();
                auto debug_bounds = board.debug.getGlobalBounds();

                if (face_bounds.contains(coords.x, coords.y)) {
                    set_mines = false;
                    board.clear(tiles);
                    done = false;
                    board.chillin = true;
                    board.won = false;
                    u_ded = false;
                    summer = true;
                    clock.restart();
                    pause_time = 0;
                    stop = true;
                }

                if (play_bounds.contains(coords.x, coords.y) && !done && summer) {
                    board.paused = !board.paused;
                    paused = !paused;
                    if(!paused) { retrieve = true; }
                    else { pause_clock.restart(); }
                }

                if (lb_bounds.contains(coords.x, coords.y)) {
                    bool temp = false;
                    if (paused) { temp = true; }
                    if(!done) { paused = true; }
                    game.clear(sf::Color::White);
                    for(int i = 0; i < tiles.size(); i++){
                        for(int j = 0; j < tiles[i].size(); j++) {
                            tiles[i][j].drawTile(game, paused);
                        }
                    }
                    board.draw(game);
                    game.display();
                    lb.draw();
                    if (!temp) { paused = false; }
                }

                if (debug_bounds.contains(coords.x, coords.y) && !done && summer && !paused) {
                    for(int i = 0; i < tiles.size(); i++){
                        for(int j = 0; j < tiles[i].size(); j++) {
                            tiles[i][j].show_mine = !tiles[i][j].show_mine;
                        }
                    }
                }

                for(int i = 0; i < tiles.size(); i++){
                    for(int j = 0; j < tiles[i].size(); j++) {
                        auto bounds = tiles[i][j].da_tile.getGlobalBounds();
                        if (bounds.contains(coords.x, coords.y) && !done && summer) {
                            u_ded = tiles[i][j].clicking(event, mouse, tiles);
                        }
                    }
                }
            }
        }
        int flag_count = 0;
        for(int i = 0; i < tiles.size(); i++){
            for(int j = 0; j < tiles[i].size(); j++) {
                if (tiles[i][j].flagged) {flag_count++;}
            }
        }
        board.flags = flag_count;

        if (retrieve) {
            pause_time += pause_clock.getElapsedTime().asSeconds();
            retrieve = false;
        }
        if (paused) {
            float temp = clock.getElapsedTime().asSeconds() - pause_clock.getElapsedTime().asSeconds() - pause_time;
            board.time = temp;
        }
        else { board.time = clock.getElapsedTime().asSeconds() - pause_time; }

        if (u_ded) {
            int temp;
            if(stop) {
                sf::Time time = clock.getElapsedTime();
                temp = time.asSeconds();
                stop = false;
            }
            board.time = temp - pause_time;

            for(int i = 0; i < tiles.size(); i++){
                for(int j = 0; j < tiles[i].size(); j++) {
                    tiles[i][j].show_mine = true;
                }
            }
            game.clear(sf::Color::White);
            for(int i = 0; i < tiles.size(); i++){
                for(int j = 0; j < tiles[i].size(); j++) {
                    tiles[i][j].drawTile(game, paused);
                }
            }
            board.draw(game);
            game.display();
            board.chillin = false;
            done = true;
        }
        else if (board.slaycheck(tiles, mines)) {
            board.flags = 0;
            board.mines = 0;
            int temp;
            if(stop) {
                sf::Time time = clock.getElapsedTime();
                temp = time.asSeconds();
                stop = false;
            }
            board.time = temp - pause_time;
            board.chillin = false;
            board.won = true;
            done = true;

            for(int i = 0; i < tiles.size(); i++){
                for(int j = 0; j < tiles[i].size(); j++) {
                    if (tiles[i][j].mines == 9) {tiles[i][j].flagged = true;}
                }
            }

            game.clear(sf::Color::White);
            for(int i = 0; i < tiles.size(); i++){
                for(int j = 0; j < tiles[i].size(); j++) {
                    tiles[i][j].drawTile(game, paused);
                }
            }

            board.draw(game);
            game.display();
            if (summer) {
                lb.add(name_text, board.time);
                lb.draw();
                summer = false;
            }
        }
        else {
            game.clear(sf::Color::White);
            for(int i = 0; i < tiles.size(); i++){
                for(int j = 0; j < tiles[i].size(); j++) {
                    tiles[i][j].drawTile(game, paused);
                }
            }
            board.draw(game);
            game.display();
        }
    }
return 0;
}