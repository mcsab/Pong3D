#include "ball.hpp"

GameEvent Ball::handleCollision()
{
    // TODO
    return NOTHING;
}

Ball::Ball(vector3df pos, vector3df map_size)
{
    m_position = pos;
    m_map_size = map_size;
    m_velocity = vector3df(0, 0, 0);
}

// ----------------------------------------------------------------------------
GameEvent Ball::animate(int dt)
{
    for (int i = 0; i < dt; i++)
    {
        m_position += m_velocity;
        switch (handleCollision())
        {
        case PLAYER_FAILED:
            return PLAYER_FAILED;
        case AI_FAILED:
            return AI_FAILED;
        case NOTHING:
            break;
        }
    }
}