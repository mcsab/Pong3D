#include "ball.hpp"

#include "racket.hpp"
#include <math.h>


bool Ball::handleRacketCollision(Racket* racket)
{
    vector2df racketSize = racket->getSize();
    vector2df racketPos = racket->getPosition();

    if ((fabs(racketPos.X - m_position.X) > racketSize.X + m_radius)
        || (fabs(racketPos.Y - m_position.Y) > racketSize.Y + m_radius))
    {
        racket->fail();
        return false;
    }
    vector2df racketVelocity = racket->getVelocity();

    m_velocity.X += racketVelocity.X * 0.01;
    m_velocity.Y += racketVelocity.Y * 0.01;
    m_velocity.Z *= -1;

    return true;
}

// ----------------------------------------------------------------------------
bool Ball::handleCollision(Racket* backRacket, Racket* frontRacket)
{
    // Right Wall
    if (m_position.X + m_radius > m_hmap_size.X)
    {
        m_velocity.X *= -1;
        m_position.X = m_position.X - 2 * (m_position.X + m_radius - m_hmap_size.X);
    }
    else // Left Wall
    if (m_position.X - m_radius < -m_hmap_size.X)
    {
        m_velocity.X *= -1;
        m_position.X = m_position.X + 2 * (fabs(m_position.X) + m_radius - m_hmap_size.X);
    }

    // Top
    if (m_position.Y + m_radius > m_hmap_size.Y)
    {
        m_velocity.Y *= -1;
        m_position.Y = m_position.Y - 2 * (m_position.Y + m_radius - m_hmap_size.Y);
    } 
    else // Floor
    if (m_position.Y - m_radius < -m_hmap_size.Y)
    {
        m_velocity.Y *= -1;
        m_position.Y = m_position.Y + 2 * (fabs(m_position.Y) + m_radius - m_hmap_size.Y);
    }

    // front Wall ~ near lookAt << ai's side >>
    if (m_position.Z + m_radius > m_hmap_size.Z)
    {
        if (!handleRacketCollision(frontRacket))
            return false;
        m_position.Z = m_position.Z - 2 * (m_position.Z + m_radius - m_hmap_size.Z);
    } 
    else  // back Wall ~ near camera << player side >>
    if (m_position.Z - m_radius < -m_hmap_size.Z)
    {
        if (!handleRacketCollision(backRacket))
            return false;
        m_position.Z = m_position.Z + 2 * (fabs(m_position.Z) + m_radius - m_hmap_size.Z);
    }
    return true;
}

// ----------------------------------------------------------------------------
Ball::Ball(vector3df pos, double radius, vector3df hmap_size)
{
    m_position = pos;
    m_radius   = radius;
    m_hmap_size = hmap_size;
    m_velocity = vector3df(0, 0, 0);
}

// ----------------------------------------------------------------------------
bool Ball::animate(int dt, Racket* backRacket, Racket* frontRacket)
{
    for (int i = 0; i < dt; i++)
    {
        m_position += m_velocity;
        if (!handleCollision(backRacket, frontRacket)) return false;
    }
    return true;
}