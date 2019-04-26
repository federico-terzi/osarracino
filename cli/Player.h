//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_PLAYER_H
#define OSARRACINO_PLAYER_H

#include <string>
#include <algorithm>
#include <map>

class Player {
public:
    enum Value : uint8_t
    {
        WHITE,
        BLACK
    };

    Player() = default;
    constexpr Player(Value acolor) : value(acolor) { }

    bool operator==(Player a) const { return value == a.value; }
    bool operator!=(Player a) const { return value != a.value; }

    static Player from_string(const std::string &s) {
        std::string lowercase = s;
        std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);

        if (s == "black") {
            return Player::BLACK;
        }else if (s == "white") {
            return Player::WHITE;
        }

        throw std::invalid_argument(s + " is not a valid player color!");
    }

    int default_port() {
        if (value == BLACK) {
            return 5801;
        }else{
            return 5800;
        }
    }

    friend std::ostream &operator<<(std::ostream &s, const Player &color) {
        if (color.value == BLACK) {
            s << std::string("BLACK");
        }else{
            s << std::string("WHITE");
        }
        return s;
    };

private:
    Value value;
};

#endif //OSARRACINO_PLAYER_H
