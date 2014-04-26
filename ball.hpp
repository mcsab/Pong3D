#ifndef BALL_HPP
#define BALL_HPP

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

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

    vector3df       m_position;
    vector3df       m_velocity;

    vector3df       m_map_size;

    GameEvent       handleCollision();

public:
    Ball(vector3df pos, vector3df map_size);

    void hit(vector3df dv)                      { m_velocity += dv; }
    void setPosition(vector3df pos)             { m_position = pos; }

    GameEvent animate(int dt);
};

#endif