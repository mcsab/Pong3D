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
    m_scene_manager->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));


    // TODO!
    // create rackets && ball

} // init


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
        m_video_driver->beginScene(true, true, SColor(255, 100, 101, 140));
        m_scene_manager->drawAll();
        m_video_driver->endScene();
    }
} // play

// ----------------------------------------------------------------------------
Game::~Game()
{
    m_device->drop();
} // ~Game