#include "scene/3d/camera_3d.h"
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
    };
    template <size_t n> struct NodeSelections
    {
        std::array<TerrainQuadTree::Node *, n> nodes;
        int count;
    };
    TerrainQuadTree();
    void Select(const Camera3D &camera, NodeSelections<NODE_SELECTION_MAX> &selectedNodes);
};
} // namespace terrain