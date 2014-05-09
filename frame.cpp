#include "frame.hpp"

Frame::Frame(ISceneNode* parent, ISceneManager* mgr, s32 id, vector3df ms)
    :ISceneNode(parent, mgr, id)
{
    m_material.Wireframe = true;
    m_material.Lighting = false;
    
    m_vertex_count = (ms.Z * 2 / 5.0 +1)*4;
    
    m_indices = new u16[m_vertex_count];
    for (int i = 0; i < m_vertex_count; i++) m_indices[i] = i;

    m_vertices     = new S3DVertex[m_vertex_count];
    unsigned int ix = 0;
    for (int i = -ms.Z; i <= ms.Z; i += 5)
    {
        m_vertices[ix].Pos   = vector3df( ms.X,  ms.Y, i);
        m_vertices[ix+1].Pos = vector3df(ms.X,  -ms.Y, i);
        m_vertices[ix+2].Pos = vector3df(-ms.X, -ms.Y, i);
        m_vertices[ix+3].Pos = vector3df(-ms.X,  ms.Y, i);
        for (int j = 0; j < 4; j++)
            m_vertices[ix+j].Color = SColor(125,255,0,0);

        ix += 4;
    }

    ISceneNode::setAutomaticCulling(EAC_OFF);
}

Frame::~Frame()
{
   if (m_indices != NULL)  delete m_indices;
   if (m_vertices != NULL) delete m_vertices;
}

void Frame::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void Frame::render()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setMaterial(m_material);
    driver->setTransform(video::ETS_WORLD, IdentityMatrix);
    driver->drawVertexPrimitiveList(&m_vertices[0], m_vertex_count,
                                    &m_indices[0], m_vertex_count / 4.0, 
                                    video::EVT_STANDARD, EPT_QUADS,
                                    video::EIT_16BIT);
}