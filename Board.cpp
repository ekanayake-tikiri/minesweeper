#include "Board.h"
using namespace std;

Board::Board(float cols_, float rows_) {
    cols = cols_;
    rows = rows_;
    won = false;
    chillin = true;
    paused = false;
    flags = 0;

    happy_t.loadFromFile("files/images/face_happy.png");
    happy.setTexture(happy_t);
    happy.setPosition((cols_*16) - 32, 32*(rows_ + 0.5f));

    win_t.loadFromFile("files/images/face_win.png");
    win.setTexture(win_t);
    win.setPosition((cols_*16) - 32, 32*(rows_ + 0.5f));

    lose_t.loadFromFile("files/images/face_lose.png");
    lose.setTexture(lose_t);
    lose.setPosition((cols_*16) - 32, 32*(rows_ + 0.5f));

    debug_t.loadFromFile("files/images/debug.png");
    debug.setTexture(debug_t);
    debug.setPosition((cols_*32) - 304, 32*(rows_ + 0.5f));

    pause_t.loadFromFile("files/images/pause.png");
    pause.setTexture(pause_t);
    pause.setPosition((cols_*32) - 240, 32*(rows_ + 0.5f));

    play_t.loadFromFile("files/images/play.png");
    play.setTexture(play_t);
    play.setPosition((cols_*32) - 240, 32*(rows_ + 0.5f));

    LB_t.loadFromFile("files/images/leaderboard.png");
    LB.setTexture(LB_t);
    LB.setPosition((cols_*32) - 176, 32*(rows_ + 0.5f));

    digits_t.loadFromFile("files/images/digits.png");
}

void Board::clear(vector<vector<Tile>>& tiles) {
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[i].size(); j++) {
            tiles[i][j].mines = 0;
            tiles[i][j].flagged = false;
            tiles[i][j].revealed = false;
            tiles[i][j].show_mine = false;
            tiles[i][j].neighbors.clear();
        }
    }
    flags = 0;
}

sf::Sprite Board::makeZero(float x) const {
    sf::Sprite zero;
    zero.setTexture(digits_t);
    zero.setTextureRect(sf::IntRect(0, 0, 21, 32));
    zero.setPosition(x, (32 * (rows + 0.5f)) + 16);
    return zero;
}

vector<sf::Sprite> Board::counter(int num) const {
    string meh = to_string(num);
    vector<sf::Sprite> da_vector;
    int i = 0;
    bool two0 = false;
    bool one0 = false;
    if (num < 0) {
        sf::Sprite sign;
        sign.setTexture(digits_t);
        sign.setTextureRect(sf::IntRect(210, 0, 21, 32));
        sign.setPosition(12, (32 * (rows + 0.5f)) + 16);
        da_vector.push_back(sign);
        i++;
    }

    if (10 <= abs(num)) {
        da_vector.push_back(makeZero(33));
        one0 = true;
    }
    else {
        da_vector.push_back(makeZero(33));
        da_vector.push_back(makeZero(54));
        two0 = true;
    }

    for (; i < meh.length(); i++) {
        int yer = meh[i] - '0';
        int width = 0;
        for (int j = 0; j < yer; j++) { width += 21; }
        sf::Sprite temp;
        temp.setTexture(digits_t);
        temp.setTextureRect(sf::IntRect(width, 0, 21, 32));

        if (two0) { temp.setPosition(75, (32 * (rows + 0.5f)) + 16); }
        else if (one0) {
            int k;
            if (num < 0) { k = i - 1; }
            else { k = i; }
            temp.setPosition(54 + (21*k), (32 * (rows + 0.5f)) + 16);
        }
        else { temp.setPosition(33 + (21*i), (32 * (rows + 0.5f)) + 16); }
        da_vector.push_back(temp);
    }
    return da_vector;
}

vector<sf::Sprite> Board::timer(int seconds) {
    vector<sf::Sprite> da_vector;
    int sec = seconds % 60;
    int min = seconds/60;
    string sec_s = to_string(sec);
    string min_s = to_string(min);
    if (sec_s.size() == 1) { sec_s = '0' + sec_s; }
    if (min_s.size() == 1) { min_s = '0' + min_s; }
    string time = min_s + sec_s;

    float x = (cols*32) - 97;
    float x2 = (cols*32) - 54;
    for (int i = 0; i < 4; i++) {
        sf::Sprite temp;

        if (i < 2) {
            if (time[i] == '0') { temp = makeZero(x); }
            else {
                temp.setTexture(digits_t);
                int yer = time[i] - '0';
                int width = 0;
                for (int j = 0; j < yer; j++) { width += 21; }
                temp.setTextureRect(sf::IntRect(width, 0, 21, 32));
                temp.setPosition(x, (32 * (rows + 0.5f)) + 16);
            }
            x += 21;
        }
        else {
            if (time[i] == '0') { temp = makeZero(x2); }
            else {
                temp.setTexture(digits_t);
                int yer = time[i] - '0';
                int width = 0;
                for (int j = 0; j < yer; j++) { width += 21; }
                temp.setTextureRect(sf::IntRect(width, 0, 21, 32));
                temp.setPosition(x2, (32 * (rows + 0.5f)) + 16);
            }
            x2 += 21;
        }
        da_vector.push_back(temp);
    }
    return da_vector;
}

bool Board::slaycheck(vector<vector<Tile>>& tiles, int mine_count) {
    int tile_count = 0;
    int checker = 0;
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[i].size(); j++) {
           if (tiles[i][j].revealed && tiles[i][j].mines != 9) {
               checker++;
           }
           tile_count++;
        }
    }
    if (tile_count - checker == mine_count) { return true; }
    else { return false; }
}

void Board::draw(sf::RenderWindow& window) {
    if (chillin) { window.draw(happy); }
    else if (won) { window.draw(win); }
    else { window.draw(lose); }

    if (paused) { window.draw(play); }
    else { window.draw(pause); }

    window.draw(LB);
    window.draw(debug);

    vector<sf::Sprite> thing = counter(mines - flags);
    for (int i = 0; i < thing.size(); i++) {
        window.draw(thing[i]);
    }

    vector<sf::Sprite> timevec = timer(time);
    for (int i = 0; i < 4; i++) {
        window.draw(timevec[i]);
    }

}