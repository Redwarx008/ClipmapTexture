#include "core/math/vector2i.h"
#include "core/object/ref_counted.h"
#include "core/templates/rid.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"
#include "scene/resources/world_3d.h"
namespace terrain
{
class MeshBlock
{
  public:
    MeshBlock(Ref<World3D> world, Ref<Mesh> mesh, Ref<Material> material, Vector2i pos);
    void SetVisible(bool visible);
    void SetMaterial(Ref<Material> material);
    ~MeshBlock();
  private:
    Ref<Mesh> _mesh;
    RID _instance;
};
} // namespace terrain