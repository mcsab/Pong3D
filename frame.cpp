#include "frame.hpp"

Frame::Frame(ISceneNode* parent, ISceneManager* mgr, s32 id, vector3df ms)
    :ISceneNode(parent, mgr, id)
{
    m_material.Wireframe = true;
    m_material.Lighting = false;
    m_hms = ms;

    // z=const rectangles 
    m_vertex_count = (ms.Z * 2 / 5.0 +1)*4;
    m_z_indices = new u16[m_vertex_count];
    for (int i = 0; i < m_vertex_count; i++) m_z_indices[i] = i;
    m_z_vertices = new S3DVertex[m_vertex_count];
    unsigned int ix = 0;
    for (int i = -ms.Z; i <= ms.Z; i += 5)
    {
        m_z_vertices[ix].Pos     = vector3df( ms.X,  ms.Y, i);
        m_z_vertices[ix + 1].Pos = vector3df( ms.X, -ms.Y, i);
        m_z_vertices[ix + 2].Pos = vector3df(-ms.X, -ms.Y, i);
        m_z_vertices[ix + 3].Pos = vector3df(-ms.X,  ms.Y, i);
        for (int j = 0; j < 4; j++)
            m_z_vertices[ix + j].Color = SColor(0, 255, 0, 0);

        ix += 4;
    }

    // ball highlight init
    for (int i = 0; i < 4; i++)
    {
        m_hl_indices[i] = i;
        m_hl_vertices[i].Color = SColor(0, 0, 200, 200);
    }
    m_hl_vertices[0].Pos = vector3df( ms.X,  ms.Y, 0);
    m_hl_vertices[1].Pos = vector3df( ms.X, -ms.Y, 0);
    m_hl_vertices[2].Pos = vector3df(-ms.X, -ms.Y, 0);
    m_hl_vertices[3].Pos = vector3df(-ms.X,  ms.Y, 0);

    ISceneNode::setAutomaticCulling(EAC_OFF);
}

void Frame::updateBallPos(double z)
{
    for (int i = 0; i < 4; i++)
        m_hl_vertices[i].Pos.Z = z;
}

Frame::~Frame()
{
   if (m_z_indices != NULL)  delete m_z_indices;
   if (m_z_vertices != NULL) delete m_z_vertices;
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
    
    driver->drawVertexPrimitiveList(&m_z_vertices[0], m_vertex_count,
                                    &m_z_indices[0], m_vertex_count / 4.0, 
                                    video::EVT_STANDARD, EPT_QUADS,
                                    video::EIT_16BIT);
    
    for (int j = -1; j < 2; j += 2)
        for (int i = -1; i < 2; i += 2)
            driver->draw3DLine
            (
                vector3df(i*m_hms.X, j*m_hms.Y,  m_hms.Z),
                vector3df(i*m_hms.X, j*m_hms.Y, -m_hms.Z),
                SColor(0,255,0,0)
            );

    driver->drawVertexPrimitiveList(&m_hl_vertices[0], 4,
        &m_hl_indices[0], 1,
        video::EVT_STANDARD, EPT_QUADS,
        video::EIT_16BIT);
}