#include "ball.hpp"

#include "racket.hpp"
#include <math.h>


bool Ball::handleRacketCollision(Racket* racket)
{
    vector2df racket_size = racket->getSize();
    vector2df racket_pos = racket->getPosition();

    if ((fabs(racket_pos.X - m_position.X) > racket_size.X / 2.0 + m_radius)
        || (fabs(racket_pos.Y - m_position.Y) > racket_size.Y / 2.0 + m_radius))
    {
        racket->fail();
        return false;
    }
    vector2df racket_velocity = racket->getVelocity();

    m_velocity.X += racket_velocity.X * 0.3;
    m_velocity.Y += racket_velocity.Y * 0.3;
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
/** This function simulate the ball's path, until it reaches the plain defined
*   by map_size.Z and dir
*  \return <x,y> coordinates of the ball's centerpoint
*/
vector2df Ball::calculatePath(Racket* backRacket,
    Racket* frontRacket,
    int dir)
{

    if (m_velocity.Z * dir <= 0)
        return vector2df(0, 0);

    Ball* testball = new Ball(m_position, m_radius, m_hmap_size);
    testball->setVelocity(m_velocity);
    
    float z;
    do
    {
        testball->animate(1, backRacket, frontRacket);
        z = fabs((testball->getPosition()).Z) + m_radius * 1.2;
    } while (z < m_hmap_size.Z);

    vector2df result = vector2df(testball->getPosition().X, 
                                  testball->getPosition().Y);
    delete testball;
    
    return result;
} // calculatePath

// ----------------------------------------------------------------------------
bool Ball::animate(int dt, Racket* backRacket, Racket* frontRacket)
{
    m_position += m_velocity * dt;
    if (!handleCollision(backRacket, frontRacket)) return false;
    return true;
}
