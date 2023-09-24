#ifndef TERRAIN_H
#define TERRAIN_H

#include "core/error/error_macros.h"
#include "core/io/config_file.h"
#include "core/io/file_access.h"
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

    void set_map_config(String &p_file_path);

    TerrainQuadTree::NodeSelections<NODE_SELECTION_MAX> _lastSelectedNodes;

    TerrainQuadTree *_quadTree;

    Camera3D *_camera;

    class MapConfig
    {
      public:
        int LeafQuadTreeNodeSize;
        int LodLevelCount;
        int LODLevelDistanceRatio;
        int RenderGridResolutionMult;
        float MorphStartRatio = 0.667;

      private:
        String _heightmapPath;
        String _normalmapPath;
        String _minmaxmapsPath;

      public:
        MapConfig()
        {
        }
        const String &GetHeightmapPath() const
        {
            return _heightmapPath;
        }
        const String &GetNormalmapPath() const
        {
            return _normalmapPath;
        }
        const String &GetMinmaxmapsPath() const
        {
            return _minmaxmapsPath;
        }
        bool Load(const String &mapConfigPath)
        {
            ConfigFile configFile;
            if (configFile.load(mapConfigPath) != OK)
            {
                ERR_FAIL_V_MSG(false, "Load map_config failed.");
            }

            _heightmapPath = (String)configFile.get_value("SourceData", "HeightmapPath");
            _normalmapPath = (String)configFile.get_value("SourceData", "NormalmapPath");
            _minmaxmapsPath = (String)configFile.get_value("SourceData", "MinmaxmapsPath");
            LODLevelDistanceRatio = (float)configFile.get_value("CDLOD", "LODLevelDistanceRatio");
            RenderGridResolutionMult = (int)configFile.get_value("CDLOD", "RenderGridResolutionMult");

            Ref<FileAccess> f = FileAccess::open(_minmaxmapsPath, FileAccess::READ);
            if (f == nullptr)
            {
                ERR_FAIL_V_MSG(false, "Load MinmaxmapsPath failed.");
            }
            LeafQuadTreeNodeSize = f->get_8();
            LodLevelCount = f->get_8();
            return true;
        }
    };

    MapConfig _mapConfig;
};

} // namespace terrain
#endif