#include "core/math/vector3.h"
#include "scene/3d/camera_3d.h"
#include "scene/resources/material.h"
#include "terrain/Constants/TerrainConstants.hpp"
#include <array>
#include <stdint.h>
#include <vector>
namespace terrain
{

class MinMaxMap
{
  public:
    MinMaxMap(std::vector<uint8_t> data)
    {

    }
    bool GetTileMinMax(int tileX, int tileY, int level, float* min, float* max) const;
  private:
    std::vector<std::vector<float>> _data;

};

class TerrainQuadTree
{
  public:
    struct CreateDesc
    {
        int LodLevelCount;
        int RenderGridResolutionMult;
        float MaxVisibilityDistance;
        int LeafRenderNodeSize;
        MinMaxMap MinMaxMap;
        std::vector<Ref<ShaderMaterial>> MaterialsPerLevel;
        std::vector<float> VisibleRangePerLevel;
    };
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
    TerrainQuadTree(const CreateDesc& desc);
    void Select(Vector3 cameraPos, NodeSelections<NODE_SELECTION_MAX> &selectedNodes);
};
} // namespace terrain