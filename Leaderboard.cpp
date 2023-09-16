#include "Leaderboard.h"
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;

Leaderboard::Leaderboard(float width_, float height_, int cols_, int rows_) {
    ifstream file("files/leaderboard.txt", ios_base::binary);
    string name, time, line, temp;
    width = width_;
    height = height_;
    cols = cols_;
    rows = rows_;

    for (int i = 0; i < 5; i++) {
        getline(file, line);
        stringstream X(line);

        getline(X, time, ',');
        getline(X, name, ',');
        names.push_back(name);

        stringstream time_s(time);
        getline(time_s, temp, ':');
        int seconds = stoi(temp) * 60;

        getline(time_s, temp, ':');
        seconds += stoi(temp);

        times.push_back(seconds);
        stars.push_back(false);
    }

    font.loadFromFile("files/font.ttf");
    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(20);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::FloatRect titlebounds = title.getLocalBounds();
    title.setOrigin(titlebounds.left + titlebounds.width/2.0f, titlebounds.top  + titlebounds.height/2.0f);
    title.setPosition(width/2.0f, (height/2.0f) - 120);
}

void Leaderboard::add(const string& name, int time) {
    vector<int> temp;
    vector<string> name_temp;
    vector<bool> star_temp;
    bool blah = true;

    for (int i = 0; i < times.size(); i++) {
        if (times[i] > time && blah) {      // less than time at that index
            temp.push_back(time);
            name_temp.push_back(name);
            star_temp.push_back(true);

            temp.push_back(times[i]);
            name_temp.push_back(names[i]);
            star_temp.push_back(false);
            blah = false;
        }
        else {
            temp.push_back(times[i]);
            name_temp.push_back(names[i]);
            star_temp.push_back(false);
        }
    }
    for (int i = 0; i < 5; i++) {
        times[i] = temp[i];
        names[i] = name_temp[i];
        stars[i] = star_temp[i];
    }

    ofstream file("files/leaderboard.txt");
    for (int i = 0; i < 5; i++) {
        int seconds = times[i] % 60;
        int minutes = floor(times[i]/60);
        string sec = to_string(seconds);
        string min = to_string(minutes);
        if (sec.size() == 1) { sec = '0' + sec; }
        if (min.size() == 1) { min = '0' + min; }

        string final_time = min + ":" + sec;
        string final = final_time + ',' + names[i];

        if (i != 4) { final += '\n'; }
        file << final;
    }
}

void Leaderboard::makeText() {
    big_one = "";
    for (int i = 0; i < 5; i++) {
        int seconds = times[i] % 60;
        int minutes = times[i]/60;
        string sec = to_string(seconds);
        string min = to_string(minutes);

        if (sec.size() == 1) { sec = '0' + sec; }
        if (min.size() == 1) { min = '0' + min; }
        string final_time = min + ":" + sec;

        string num = to_string(i + 1);
        big_one += num + ".\t" + final_time + "\t" + names[i];
        if (stars[i]) { big_one += "*"; }
        if (i != 4) { big_one += "\n\n"; }
    }

    lbtext.setFont(font);
    lbtext.setString(big_one);
    lbtext.setCharacterSize(18);
    lbtext.setStyle(sf::Text::Bold);

    sf::FloatRect lbbounds = lbtext.getLocalBounds();
    lbtext.setOrigin(lbbounds.left + lbbounds.width/2.0f, lbbounds.top  + lbbounds.height/2.0f);
    lbtext.setPosition(width/2.0f, (height/2.0f) +20);

    for (int i = 0; i < 5; i++) { stars[i] = false; }
}

void Leaderboard::draw() {
    makeText();
    sf::RenderWindow da_window(sf::VideoMode(16 * cols, (rows * 16) + 50), "Leaderboard", sf::Style::Close);

    while (da_window.isOpen()) {
        sf::Event lbevent{};
        while (da_window.pollEvent(lbevent)) {
            if (lbevent.type == sf::Event::Closed) { da_window.close(); }
        }
        da_window.clear(sf::Color::Blue);
        da_window.draw(title);
        da_window.draw(lbtext);
        da_window.display();
    }
}