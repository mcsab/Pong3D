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
    // z=const rectangles
    S3DVertex*      m_z_vertices;
    u16*            m_z_indices;
    unsigned int    m_vertex_count;

    //highlight for ball position
    S3DVertex       m_hl_vertices[4];
    u16             m_hl_indices[4];

    SMaterial       m_material;

    vector3df       m_hms;
public:
    Frame(ISceneNode* parent, ISceneManager* mgr, s32 id, vector3df ms);
    void updateBallPos(double z);
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