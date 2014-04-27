#include "racket.hpp"

void Racket::step()
{
    double dist = (m_target - m_position).getLength();
    if (dist > 0.01)
    {
        m_position += m_velocity;
        if ((m_target - m_position).getLength() < dist)
        {
            m_position = m_target;
            m_velocity = vector2df(0, 0);
        }
    }
}

Racket::Racket(vector2df pos, vector2df size, vector3df map_size)
{
    m_speed = 1;
    m_position = pos;
    m_size     = size;
    map_size   = map_size;
}

void Racket::setTarget(const vector2df& target)
{
    m_target = target;
    
    double i = fabs(m_position.X - m_target.X);
    double j = fabs(m_position.Y - m_target.Y);

    m_velocity.Y = m_speed / sqrt(i*i / j*j + 1);
    m_velocity.X = i / j * m_velocity.Y;
}

void Racket::animate(int dt)
{
    for (int i = 0; i < dt; i++)
    {
        step();
    }
}