#ifndef GAME_HPP
#define GAME_HPP

#include <irrlicht.h>

using namespace irr;
using namespace core;

class Ball;
class Racket;

class Game
{
private:

    irr::IrrlichtDevice*        m_device;
    irr::video::IVideoDriver*   m_video_driver;
    irr::scene::ISceneManager*  m_scene_manager;

    irr::core::vector3df        m_map_size;
    int                         m_ball_number;
    
    Ball*                       m_ball;
    Racket*                     m_player_racket;
    Racket*                     m_ai_racket;


    Game(vector3df map_size, int ball_number);
    bool init();
    void drawFrame();
    void render();

public:

    static Game* createGame(vector3df map_size = vector3df(10, 10, 20),
        int ball_number = 5);

    void play();

    ~Game();
};

#endif