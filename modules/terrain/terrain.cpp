#include "core/error/error_macros.h"
#include "core/object/class_db.h"
#include "core/object/object.h"
#include "scene/main/viewport.h"
#include "scene/resources/material.h"

#include "terrain.hpp"

namespace terrain
{

Terrain::Terrain()
{
}

void Terrain::ready()
{
    const Viewport *vp = get_viewport();
    _camera = vp->get_camera_3d();
    ERR_FAIL_COND_MSG(_camera != nullptr, "no active camera!");
}

void Terrain::process()
{
    for (int i = 0; i < _lastSelectedNodes.count; ++i)
    {
        //set visiable
    }

    TerrainQuadTree::NodeSelections<NODE_SELECTION_MAX> selection;
    _quadTree.Select(_camera->get_global_position(), selection);

    for (int i = 0; i < selection.count; ++i)
    {

    }
    _lastSelectedNodes = selection;
}

void Terrain::_notification(int p_what)
{
    switch (p_what)
    {
    case NOTIFICATION_READY:
        ready();
        set_process(true);
        break;
    case NOTIFICATION_PROCESS:
        process();
        break;
    }
}
void Terrain::_bind_methods()
{
}

} // namespace terrain