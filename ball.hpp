#ifndef BALL_HPP
#define BALL_HPP

#include <irrlicht.h>

using namespace irr;
using namespace core;


class Racket;

class Ball
{
private:
    double          m_radius;
    vector3df       m_position;
    vector3df       m_velocity;

    vector3df       m_hmap_size;

    bool            handleRacketCollision(Racket* racket);
    bool            handleCollision (Racket* backRacket, Racket* frontRacket);

public:
    Ball(vector3df pos, double radius, vector3df hmap_size);

    void setPosition(const vector3df& pos)             { m_position = pos;  }
    void setVelocity(const vector3df& vel)             { m_velocity = vel;  }
    const vector3df& getPosition() const               { return m_position; }

    vector2df calculatePath(Racket* backRacket,
                            Racket* frontRacket,
                            int dir = 1);

    bool animate(int dt, Racket* backRacket, Racket* frontRacket);
};

#endif