#ifndef RACKET_HPP
#define RACKET_HPP

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;

class Racket
{
private:

    ISceneNode*     m_node;

    double          m_speed;
    vector2df       m_size;
    vector2df       m_position;
    vector2df       m_velocity;
    vector2df       m_target;

    int             m_life;
    void step();

public:
    Racket(vector2df size = vector2df(2,1), 
           vector2df pos = vector2df(0, 0),
           int life = 3);

    void setPosition(const vector2df& pos)              { m_position = pos;  }
    void fail()                                         { m_life -= 1;       }
    const vector2df& getSize() const                    { return m_size;     }
    const vector2df& getPosition() const                { return m_position; }
    const vector2df& getVelocity() const                { return m_velocity; }
    const int getLife()                                 { return m_life;     }

    void setTarget(const vector2df& target);
    void animate(int dt);
};

#endif