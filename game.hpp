#ifndef GAME_HPP
#define GAME_HPP

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

class Ball;
class Racket;

class Game
{
private:

    vector2d<int>               m_screen_size;

    IrrlichtDevice*             m_device;
    IVideoDriver*               m_video_driver;
    ISceneManager*              m_scene_manager;

    ICursorControl*             m_cursor;

    vector3df                   m_hmap_size;
    int                         m_ball_number;
    
    Ball*                       m_ball;
    Racket*                     m_player_racket;
    Racket*                     m_ai_racket;

    ISceneNode*                 m_ball_node;

    ISceneNode*                 m_player_racket_node;

    Game(vector2d<int> screen_size, vector3df map_size, int ball_number);
    bool init();
    void drawFrame();
    void render();
    void racketControl();
    void animate(int dt);
public:

    static Game* createGame(vector2d<int> screen_size = vector2d<int>(640, 480),
        vector3df map_size = vector3df(15, 10, 20),
        int ball_number = 5);

    void play();

    ~Game();
};

#endif