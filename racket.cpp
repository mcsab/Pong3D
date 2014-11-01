#include "racket.hpp"

void Racket::step(u32 dt)
{
    double dist = (m_target - m_position).getLength();
    if (dist > 0.01)
    {
        m_position += m_velocity * dt;
        float dist_new = (m_target - m_position).getLength();
        if (dist_new > dist)
        {
            m_position = m_target;
            //m_velocity = vector2df(0, 0);
        }
    }
}

Racket::Racket(vector2df size, vector2df pos, int life)
{
    m_speed    = 0.04f;
    m_position = pos;
    m_size     = size;
    m_life     = life;
    m_velocity = vector2df(0, 0);
}

void Racket::setTarget(const vector2df& target)
{
    m_target = target;
    
    m_velocity = (m_target - m_position).normalize();
    m_velocity *= m_speed;
}

void Racket::animate(u32 dt)
{
    step(dt);
   // m_position = m_target;
}