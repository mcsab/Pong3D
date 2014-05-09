#ifndef FRAME_HPP
#define FRAME_HPP

#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

class Frame :public ISceneNode
{
private:
    S3DVertex*      m_vertices;
    u16*            m_indices;
    unsigned int    m_vertex_count;
    SMaterial       m_material;
public:
    Frame(ISceneNode* parent, ISceneManager* mgr, s32 id, vector3df ms);
    ~Frame();

    virtual void OnRegisterSceneNode();
    virtual void render();

    //BoundingBox is not used, as automaticculling is disablead
    virtual const aabbox3d<f32>& getBoundingBox() const
        {return aabbox3d<f32>();}

    virtual u32 getMaterialCount() const
        {return 1;}

    virtual SMaterial& getMaterial(u32 i)
        {return m_material;}

};

#endif