#include "game.hpp"

#include "ball.hpp"
#include "racket.hpp"


Game::Game(vector2d<int> screen_size, vector3df map_size, int ball_number)
{
    m_screen_size = screen_size;
    m_hmap_size = map_size;
    m_ball_number = ball_number;

} // Game


// ----------------------------------------------------------------------------
bool Game::init()
{
    m_device = createDevice(video::EDT_OPENGL, 
        dimension2d<u32>(m_screen_size.X, m_screen_size.Y), 16,
        false, false,  true, this);

    if (!m_device) return false;

    m_video_driver  = m_device->getVideoDriver();
    m_scene_manager = m_device->getSceneManager();
    m_scene_manager->addCameraSceneNode(0, 
            vector3df(0, 0, - m_hmap_size.Z - 20.0), 
            vector3df(0, 0, m_hmap_size.Z)
        );

    m_cursor = m_device->getCursorControl();

    m_ball = new Ball(vector3df(0,0,0),1.2,m_hmap_size);
    m_ball_node = m_scene_manager->addSphereSceneNode(1.2, 6, 0, -4);

    if (!m_ball_node) return false;
    m_ball_node->setMaterialFlag(video::EMF_WIREFRAME, true);
    m_ball_node->setMaterialFlag(video::EMF_LIGHTING, false);

    m_player_racket_node = m_scene_manager->addCubeSceneNode(1.0);
    m_player_racket_node->setScale(vector3df(7,4,0.1));
    m_player_racket_node->setMaterialFlag(video::EMF_WIREFRAME, true);
    m_player_racket_node->setMaterialFlag(video::EMF_LIGHTING, false);

    m_player_racket = new Racket();

    m_run = false;

} // init

// ----------------------------------------------------------------------------
void Game::drawFrame()
{
    SMaterial m;
    m.Thickness = 0.3;
    m.Lighting = false;
    m_video_driver->setMaterial(m);
    m_video_driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
    
    for (int z = -m_hmap_size.Z; z < m_hmap_size.Z; z += 5)
    {

        for (int i = -1; i < 2; i += 2)
            m_video_driver->draw3DLine(
            vector3df(i * m_hmap_size.X, m_hmap_size.Y, z),
            vector3df(i * m_hmap_size.X , -m_hmap_size.Y, z),
            SColor(255, 0, 255, 0)
            );

        for (int i = -1; i < 2; i += 2)
            m_video_driver->draw3DLine(
            vector3df(-m_hmap_size.X, i * m_hmap_size.Y, z),
            vector3df( m_hmap_size.X, i * m_hmap_size.Y, z),
            SColor(255, 0, 255, 0)
            );
    }

} // drawFrame

// ----------------------------------------------------------------------------
void Game::render()
{

    m_video_driver->beginScene(true, true, SColor(255, 80, 0, 170));
    
    m_scene_manager->drawAll();

    drawFrame();
    m_video_driver->endScene();
} // render

// ----------------------------------------------------------------------------
void Game::racketControl()
{
    double x = m_cursor->getPosition().X;
    double y = m_cursor->getPosition().Y;

    x =  (x / m_screen_size.X - 1.0 / 2.0) * m_hmap_size.X * 2;
    y = -(y / m_screen_size.Y - 1.0 / 2.0) * m_hmap_size.Y * 2;

    if (x > m_hmap_size.X) x = m_hmap_size.X;
    if (x < -m_hmap_size.X) x = -m_hmap_size.X;
    if (y > m_hmap_size.Y) y = m_hmap_size.Y;
    if (y < -m_hmap_size.Y) y = -m_hmap_size.Y;

    m_player_racket->setTarget(vector2df(x,y));
    
    vector2df pos = m_player_racket->getPosition();

    m_player_racket_node->setPosition(vector3df(pos.X,pos.Y, -m_hmap_size.Z));

} // racketControl

// ----------------------------------------------------------------------------
void Game::animate(int dt)
{
    
    m_player_racket->animate(dt);

    if (!m_ball->animate(dt, m_player_racket, m_ai_racket))
    {
        m_ball->setPosition(vector3df(0,0,0));
        m_ball->setVelocity(vector3df(0, 0, 0));
        m_run = false;
    }

    m_ball_node->setPosition(m_ball->getPosition());

    

} // animate

// ----------------------------------------------------------------------------
bool Game::OnEvent(const SEvent& event)
{
    if (!m_run && event.EventType == irr::EET_MOUSE_INPUT_EVENT
        && event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
    {
        m_run = true;
        m_ball->setVelocity(vector3df(0, 0, 0.05));
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
    u32 currentTime;
    u32 lastTime = m_device->getTimer()->getTime();
    while (m_device->run())
    {
        render();
        currentTime = m_device->getTimer()->getTime();
        racketControl();
        
        animate((currentTime - lastTime));
        lastTime = currentTime;
    }
} // play

// ----------------------------------------------------------------------------
Game::~Game()
{
    m_device->drop();
} // ~Game