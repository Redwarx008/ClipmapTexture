#ifndef TERRAIN_H
#define TERRAIN_H

#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "scene/3d/camera_3d.h"
#include "scene/resources/material.h"
#include <scene/3d/mesh_instance_3d.h>
#include <scene/3d/node_3d.h>

#include "TerrainQuadTree.hpp"
#include "terrain/Constants/TerrainConstants.hpp"

namespace terrain
{

class Terrain : public Node3D
{
    GDCLASS(Terrain, Node3D)
  public:
    Terrain();
    // ~Terrain();
  protected:
    void _notification(int p_what);
    static void _bind_methods();

  private:
    void ready();
    void process();

    TerrainQuadTree::NodeSelections<NODE_SELECTION_MAX> _lastSelectedNodes;

    TerrainQuadTree _quadTree;

    Camera3D* _camera;
};

} // namespace terrain
#endif