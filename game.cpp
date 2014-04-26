#include "game.hpp"

using namespace video;

/** The constructor sets map parameters and creates resources */
Game::Game(vector3df map_size, int ball_number)
{
    m_map_size    = map_size;
    m_ball_number = ball_number;  

} // Game


// ----------------------------------------------------------------------------
bool Game::init()
{
    m_device = createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
        false, false, false, 0);

    if (!m_device) return false;

    m_video_driver  = m_device->getVideoDriver();
    m_scene_manager = m_device->getSceneManager();
    m_scene_manager->addCameraSceneNode(0, 
            vector3df(0, 0, -m_map_size.Z - 7), 
            vector3df(0, 0, m_map_size.Z)
        );

    // TODO!
    // create rackets && ball

} // init

// ----------------------------------------------------------------------------
void Game::drawFrame()
{
    SMaterial m;
    m.Thickness = 0.3;
    m.Lighting = false;
    m_video_driver->setMaterial(m);
    m_video_driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
    
    for (int z = -m_map_size.Z; z < m_map_size.Z; z += 5)
    {

        for (int i = -1; i < 2; i += 2)
            m_video_driver->draw3DLine(
            vector3df(i * m_map_size.X / 2.0, m_map_size.Y / 2.0, z),
            vector3df(i * m_map_size.X / 2.0, -m_map_size.Y / 2.0, z),
            SColor(255, 0, 255, 0)
            );

        for (int i = -1; i < 2; i += 2)
            m_video_driver->draw3DLine(
            vector3df(-m_map_size.X / 2.0, i * m_map_size.Y / 2.0, z),
            vector3df( m_map_size.X / 2.0, i * m_map_size.Y / 2.0, z),
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
Game* Game::createGame(vector3df map_size, int ball_number)
{
    Game* game = new Game(map_size, ball_number);
    
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
    while (m_device->run())
    {
        render();
    }
} // play

// ----------------------------------------------------------------------------
Game::~Game()
{
    m_device->drop();
} // ~Game