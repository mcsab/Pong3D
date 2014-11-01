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
class Frame;

class Game :IEventReceiver 
{
private:

    vector2d<int>               m_screen_size;

    IrrlichtDevice*             m_device;
    IVideoDriver*               m_video_driver;
    ISceneManager*              m_scene_manager;
    IGUIEnvironment*            m_gui_env;

    ICursorControl*             m_cursor;

    vector3df                   m_hmap_size;
    int                         m_ball_number;
    vector2df                   m_racket_size;
    
    Ball*                       m_ball;
    Racket*                     m_player_racket;
    Racket*                     m_ai_racket;

    ISceneNode*                 m_ball_node;
    IMeshSceneNode*             m_player_racket_node;
    IMeshSceneNode*             m_ai_racket_node;
    Frame*                      m_frame;
    
    bool                        m_run;

    u32                         m_timer;
    IGUIStaticText*             m_timer_text;

    Game(vector2d<int> screen_size, vector3df map_size, int ball_number);
    void changeColorInMeshBuffer(IMeshBuffer* mb, const SColor& color);
    void initPlayerRacket();
    void initAiRacket();
    bool init();
    void drawFrameElement(int z, SColor color);
    void drawFrame();
    void render();
    void playerRacketControl();
    void aiRacketControl();
    void animate(int dt);

    bool OnEvent(const SEvent& event);

public:

    static Game* createGame(vector2d<int> screen_size = vector2d<int>(640, 480),
                    vector3df map_size = vector3df(15, 10, 25), int ball_number = 5);

    void play();

    ~Game();
};

#endif