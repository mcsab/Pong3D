#ifndef BALL_HPP
#define BALL_HPP

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

class Racket;

enum GameEvent
{
    NOTHING,
    PLAYER_FAILED,
    AI_FAILED
};

class Ball
{
private:
    ISceneNode*     m_node;

    double          m_radius;
    vector3df       m_position;
    vector3df       m_velocity;

    vector3df       m_map_size;

    bool            handleRacketCollision(const Racket& racket);
    GameEvent       handleCollision (const Racket& backRacket, 
                                     const Racket& frontRacket);

public:
    Ball(vector3df pos, double radius, vector3df map_size);

    void hit(const vector3df& dv)                      { m_velocity += dv; }
    void setPosition(const vector3df& pos)             { m_position = pos; }

    GameEvent animate(int dt, const Racket& backRacket, 
                              const Racket& frontRacket);
};

#endif