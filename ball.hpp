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
    bool            handleCollision (Racket* back_racket, Racket* front_racket);

public:
    Ball(vector3df pos, double radius, vector3df hmap_size);

    void setPosition(const vector3df& pos)             { m_position = pos;  }
    void setVelocity(const vector3df& vel)             { m_velocity = vel;  }
    const vector3df& getPosition() const               { return m_position; }

    vector2df calculatePath(Racket* back_racket,
                            Racket* front_racket,
                            int dir = 1);

    bool animate(int dt, Racket* back_racket, Racket* front_racket);
};

#endif