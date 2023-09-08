#include "core/math/vector3.h"
#include "scene/3d/camera_3d.h"
#include "scene/resources/material.h"
#include "terrain/Constants/TerrainConstants.hpp"
#include <array>
#include <stdint.h>
#include <vector>
namespace terrain
{
class TerrainQuadTree
{
  public:
    struct Node
    {
        int level;
        void SetVisible(bool visible);
        void SetMaterial(Ref<Material> material);
    };
    template <size_t n> struct NodeSelections
    {
        std::array<TerrainQuadTree::Node *, n> nodes;
        int count;
    };
    TerrainQuadTree();
    void Select(Vector3 cameraPos, NodeSelections<NODE_SELECTION_MAX> &selectedNodes);
};
} // namespace terrain