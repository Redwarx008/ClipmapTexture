#ifndef TERRAIN_H
#define TERRAIN_H

#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "scene/resources/material.h"
#include <scene/3d/mesh_instance_3d.h>
#include <scene/3d/node_3d.h>

namespace terrain {

class Terrain : public Node3D
{
  GDCLASS(Terrain, Node3D)
public:
  void set_mesh(Ref<MeshInstance3D> mesh);
  Ref<MeshInstance3D> get_mesh();

  Terrain();
  // ~Terrain();
protected:
  static void _bind_methods();

private:
  Ref<MeshInstance3D> _mesh;
  Ref<Material> _material;

  const String dataPath{ "heightmap" };
};

}
#endif