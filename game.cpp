#include "game.hpp"

#include "ball.hpp"
#include "racket.hpp"
#include "frame.hpp"

#include <string>

Game::Game(vector2d<int> screen_size, vector3df map_size, int ball_number)
{
    m_screen_size = screen_size;
    m_hmap_size = map_size;
    m_ball_number = ball_number;

} // Game

// ----------------------------------------------------------------------------
/** The Racket's color is changed by vertex manipulation 
*   It's not the best way to do it, but I wanted to see how vertices work
*/
void Game::changeColorInMeshBuffer(IMeshBuffer* mb, const SColor& color)
{
    unsigned int vertex_count = mb->getVertexCount();
    S3DVertex* vertices = (S3DVertex*)mb->getVertices();

    for (int i = 0; i < vertex_count; i++)
        vertices[i].Color = color;
}

// ----------------------------------------------------------------------------
void Game::initPlayerRacket()
{
    m_player_racket = new Racket(m_racket_size);

    m_player_racket_node = m_scene_manager->addCubeSceneNode(1.0);
    m_player_racket_node->setScale(vector3df(m_racket_size.X, m_racket_size.Y, 0.1));
    m_player_racket_node->setMaterialFlag(video::EMF_LIGHTING, false);
    
    m_player_racket_node->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);


    changeColorInMeshBuffer(m_player_racket_node->getMesh()->getMeshBuffer(0),
                            SColor(127.5, 0, 255, 0));
}


// ----------------------------------------------------------------------------
void Game::initAiRacket()
{
    m_ai_racket = new Racket(m_racket_size);

    m_ai_racket_node = m_scene_manager->addCubeSceneNode(1.0);
    m_ai_racket_node->setScale(vector3df(7, 4, 0.1));
    m_ai_racket_node->setMaterialFlag(video::EMF_LIGHTING, false);

    changeColorInMeshBuffer(m_ai_racket_node->getMesh()->getMeshBuffer(0),
                            SColor(0, 255, 0, 0));
}


// ----------------------------------------------------------------------------
bool Game::init()
{
    m_device = createDevice(video::EDT_OPENGL, 
        dimension2d<u32>(m_screen_size.X, m_screen_size.Y), 16,
        false, false,  true, this);

    if (!m_device) return false;

    m_video_driver  = m_device->getVideoDriver();
    m_gui_env       = m_device->getGUIEnvironment();
    m_scene_manager = m_device->getSceneManager();
    m_scene_manager->addCameraSceneNode(0, 
            vector3df(0, 0, - m_hmap_size.Z - 16.0), 
            vector3df(0, 0, m_hmap_size.Z)
        );

    m_cursor = m_device->getCursorControl();
	m_cursor->setVisible(false);

    //frame
    m_frame = new Frame(m_scene_manager->getRootSceneNode(),
                        m_scene_manager, 12, m_hmap_size);


    // game is paused at the begining
    m_run = false;

    // ball
    m_ball = new Ball(vector3df(0,0,0),1.2,m_hmap_size);
    m_ball_node = m_scene_manager->addSphereSceneNode(1.2, 6, 0, -4);

    if (!m_ball_node) return false;
    m_ball_node->setMaterialFlag(video::EMF_WIREFRAME, true);
    //m_ball_node->setMaterialFlag(video::EMF_LIGHTING, false);
    
    m_racket_size = vector2df(6, 3);    
    initPlayerRacket();
    initAiRacket();

    m_timer_text =
        m_gui_env->addStaticText(L"0 : 0", rect<s32>(10, 10, 50, 20));

} // init

// ----------------------------------------------------------------------------
void Game::render()
{
    m_video_driver->beginScene(true, true, SColor(255, 80, 0, 170));
   
    m_scene_manager->drawAll();
    m_gui_env->drawAll();
    
    m_video_driver->endScene();
} // render

// ----------------------------------------------------------------------------
void Game::playerRacketControl()
{
    double x = m_cursor->getPosition().X;
    double y = m_cursor->getPosition().Y;

    x =  (x / m_screen_size.X - 1.0 / 2.0) * m_hmap_size.X * 2;
    y = -(y / m_screen_size.Y - 1.0 / 2.0) * m_hmap_size.Y * 2;

	if (x >  m_hmap_size.X - m_racket_size.X / 2.0) 
		x = m_hmap_size.X - m_racket_size.X / 2.0;
	if (x < -m_hmap_size.X + m_racket_size.X / 2.0)
		x = -m_hmap_size.X + m_racket_size.X / 2.0;
	if (y >  m_hmap_size.Y - m_racket_size.Y / 2.0)
		y =  m_hmap_size.Y - m_racket_size.Y / 2.0;
	if (y < -m_hmap_size.Y + m_racket_size.Y / 2.0)
		y = -m_hmap_size.Y + m_racket_size.Y / 2.0;

    m_player_racket->setTarget(vector2df(x,y));

    vector2df pos = m_player_racket->getPosition();
    m_player_racket_node->setPosition(vector3df(pos.X,pos.Y, -m_hmap_size.Z));

} // playerRacketControl

// ----------------------------------------------------------------------------
void Game::aiRacketControl()
{
    m_ai_racket->setTarget(m_ball->calculatePath(m_player_racket, m_ai_racket));

    vector2df pos = m_ai_racket->getPosition();
    m_ai_racket_node->setPosition(vector3df(pos.X, pos.Y, m_hmap_size.Z));

} // aiRacketControl

// ----------------------------------------------------------------------------
void Game::animate(int dt)
{
    if (m_run)
    {
        m_timer += dt;

        std::string  sec = std::to_string((m_timer / 1000) % 60);
        std::string  min = std::to_string((m_timer / 1000) / 60);
        std::string s = min + " : " + sec;
        stringw sw(s.c_str());

        m_timer_text->setText(sw.c_str());
    }

    m_player_racket->animate(dt);
    m_ai_racket->animate(dt);

    if (!m_ball->animate(dt, m_player_racket, m_ai_racket))
    {
        m_ball->setPosition(vector3df(0,0,0));
        m_ball->setVelocity(vector3df(0, 0, 0));
        m_run = false;
    }

    m_ball_node->setPosition(m_ball->getPosition());
    m_frame->updateBallPos(m_ball->getPosition().Z);
    

} // animate

// ----------------------------------------------------------------------------
bool Game::OnEvent(const SEvent& event)
{
    if (!m_run && event.EventType == irr::EET_MOUSE_INPUT_EVENT
        && event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
    {
        m_run = true;
        m_ball->setVelocity(vector3df(0, 0, -0.05));
        m_timer = 0;
    }
    return true;
}

// ----------------------------------------------------------------------------
Game* Game::createGame(vector2d<int> screen_size, 
                       vector3df map_size, int ball_number)
{
    Game* game = new Game(screen_size,map_size, ball_number);
    
    if (!game->init())
    {
        delete game;
        return NULL;
    }

    return game;
} // createGame


// ----------------------------------------------------------------------------
void Game::play()
{
    u32 current_time;
    u32 last_time = m_device->getTimer()->getTime();
    while (m_device->run())
    {
        render();
        current_time = m_device->getTimer()->getTime();
        
        playerRacketControl();
        aiRacketControl();
        
        animate((current_time - last_time));
        last_time = current_time;
    }
} // play

// ----------------------------------------------------------------------------
Game::~Game()
{
    m_frame->drop();
    m_device->drop();
} // ~Game
