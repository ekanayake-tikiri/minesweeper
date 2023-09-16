#include "Tile.h"
using namespace std;

Tile::Tile(int x, int y, int boardx, int boardy) {
    row = y/32;
    col = x/32;
    board_rows = boardx - 1;
    board_cols = boardy - 1;

    texture.loadFromFile("files/images/tile_hidden.png");
    da_tile.setTexture(texture);
    da_tile.setPosition(x,y);

    mine_t.loadFromFile("files/images/mine.png");
    mine.setTexture(mine_t);
    mine.setPosition(x,y);

    revealed_t.loadFromFile("files/images/tile_revealed.png");
    reveal.setTexture(revealed_t);
    reveal.setPosition(x,y);

    flag_t.loadFromFile("files/images/flag.png");
    flag.setTexture(flag_t);
    flag.setPosition(x,y);

    revealed = false;
    show_mine = false;
    flagged = false;
    mines = 0;
}

bool Tile::clicking(sf::Event& event, sf::Mouse& mouse, vector<vector<Tile>>& tiles) {
    if(event.mouseButton.button == mouse.Right && !revealed) {
        flagged = !flagged;
    }
    if (event.mouseButton.button == mouse.Left) {
        if (!flagged && !revealed) {
            if (mines == 9) { return true; }
            else { unleash(tiles); }
        }
    }
    return false;
}

void Tile::unleash(vector<vector<Tile>>& tiles) {
    revealed = true;
    if (mines != 0) {
        number_t.loadFromFile("files/images/number_" + to_string(mines) + ".png");
        number.setTexture(number_t);
        number.setPosition(col*32, row*32);
    }
    else { recursive(tiles); }
}

void Tile::recursive(vector<vector<Tile>>& tiles) {
    // upper left
    if (exists(row- 1, col-1) && !tiles[row - 1][col - 1].revealed && !tiles[row - 1][col - 1].flagged) {
        tiles[row-1][col-1].unleash(tiles);
    }
    // above
    if (exists(row-1, col) && !tiles[row - 1][col].revealed && !tiles[row - 1][col].flagged) {
        tiles[row-1][col].unleash(tiles);
    }
    // upper right
    if (exists(row-1, col+1) && !tiles[row - 1][col + 1].revealed && !tiles[row - 1][col + 1].flagged) {
        tiles[row-1][col+1].unleash(tiles);
    }

    // left
    if (exists(row, col-1) && !tiles[row][col - 1].revealed && !tiles[row][col - 1].flagged) {
        tiles[row][col-1].unleash(tiles);
    }
    // right
    if (exists(row, col+1) && !tiles[row][col + 1].revealed && !tiles[row][col + 1].flagged) {
        tiles[row][col+1].unleash(tiles);
    }

    // lower left
    if (exists(row+1, col-1) && !tiles[row + 1][col - 1].revealed && !tiles[row + 1][col - 1].flagged) {
        tiles[row+1][col-1].unleash(tiles);
    }
    // below
    if (exists(row+1, col) && !tiles[row + 1][col].revealed && !tiles[row + 1][col].flagged) {
        tiles[row+1][col].unleash(tiles);
    }
    // lower right
    if (exists(row+1, col+1) && !tiles[row + 1][col + 1].revealed && !tiles[row + 1][col + 1].flagged) {
        tiles[row+1][col+1].unleash(tiles);
    }
}

bool Tile::exists(int row_, int col_){
    if (row_ >= 0 && row_ <= board_rows && col_ >= 0 && col_ <= board_cols) { return true; }
    return false;
}

void Tile::peepin_tom(vector<vector<Tile>>& tiles){
    // upper left
    if (exists(row- 1, col-1) && tiles[row-1][col-1].mines == 9) {
        Tile* temp = &tiles[row-1][col-1];
        neighbors.push_back(temp);
    }
    // above
    if (exists(row-1, col) && tiles[row-1][col].mines == 9) {
        Tile* temp = &tiles[row-1][col];
        neighbors.push_back(temp);
    }
    // upper right
    if (exists(row-1, col+1) && tiles[row-1][col+1].mines == 9) {
        Tile* temp = &tiles[row-1][col+1];
        neighbors.push_back(temp);
    }

    // left
    if (exists(row, col-1) && tiles[row][col-1].mines == 9) {
        Tile* temp = &tiles[row][col-1];
        neighbors.push_back(temp);
    }
    // right
    if (exists(row, col+1) && tiles[row][col+1].mines == 9) {
        Tile* temp = &tiles[row][col+1];
        neighbors.push_back(temp);
    }

    // lower left
    if (exists(row+1, col-1) && tiles[row+1][col-1].mines == 9) {
        Tile* temp = &tiles[row+1][col-1];
        neighbors.push_back(temp);
    }
    // below
    if (exists(row+1, col) && tiles[row+1][col].mines == 9) {
        Tile* temp = &tiles[row+1][col];
        neighbors.push_back(temp);
    }
    // lower right
    if (exists(row+1, col+1) && tiles[row+1][col+1].mines == 9) {
        Tile* temp = &tiles[row+1][col+1];
        neighbors.push_back(temp);
    }

    if (neighbors.empty()) { mines = 0; }
    else { mines = neighbors.size(); }
}

void Tile::drawTile(sf::RenderWindow& window, bool paused) {
    if (paused) { window.draw(reveal); }
    else {
        if (revealed) {
            window.draw(reveal);
            if (mines == 0) {}
            else { window.draw(number); }
        }
        else { window.draw(da_tile); }

        if (flagged) { window.draw(flag); }

        if (show_mine && mines == 9) {
            window.draw(reveal);
            if (flagged) { window.draw(flag); }
            window.draw(mine);
        }
    }
}