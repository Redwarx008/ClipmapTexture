#include "core/io/image.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/object/ref_counted.h"
#include "core/variant/array.h"
#include "core/variant/variant.h"
#include "scene/resources/mesh.h"
namespace terrain
{
class MeshProcessor
{
  public:
    /// The position of godot's PlaneMesh is at the midpoint of the geometry,
    /// and the position of this PlaneMesh is at (0,0,0) in its model space.
    static Ref<Mesh> CreatePlaneMesh(int dimensionX, int dimensionY, float sizeX, float sizeY)
    {
        float strideX = sizeX / dimensionX;
        float strideY = sizeY / dimensionY;

        int verticesCountX = dimensionX + 1;
        int verticesCountY = dimensionY + 1;

        Vector<Vector3> vertices;
        vertices.resize(verticesCountX * verticesCountY);
        Vector<int> indices;
        indices.resize((verticesCountX - 1) * (verticesCountY - 1) * 6);
        Vector<Vector2> uvs;
        uvs.resize(verticesCountX * verticesCountY);
        Vector<Vector3> normals;
        normals.resize(verticesCountX * verticesCountY);

        int vertexCount = 0;
        int indexCount = 0;

        for (int y = 0; y < verticesCountY; ++y)
        {
            for (int x = 0; x < verticesCountX; ++x)
            {
                vertices.write[vertexCount] = Vector3(x * strideX, 0, y * strideY);
                uvs.write[vertexCount] = Vector2(x * strideX / sizeX, y * strideY / sizeY);
                normals.write[vertexCount] = Vector3(0, 1, 0);
                ++vertexCount;
            }
        }
        for (int y = 0; y < verticesCountY - 1; ++y)
        {
            for (int x = 0; x < verticesCountX - 1; ++x)
            {
                int baseIndex = y * verticesCountX + x;
                indices.write[indexCount++] = baseIndex + 1;
                indices.write[indexCount++] = baseIndex + 1 + verticesCountX;
                indices.write[indexCount++] = baseIndex + verticesCountX;
                indices.write[indexCount++] = baseIndex + 1;
                indices.write[indexCount++] = baseIndex + verticesCountX;
                indices.write[indexCount++] = baseIndex;
            }
        }

        Array meshData;
        meshData.resize(Mesh::ARRAY_MAX);
        meshData[Mesh::ARRAY_VERTEX] = vertices;
        meshData[Mesh::ARRAY_INDEX] = indices;
        meshData[Mesh::ARRAY_TEX_UV] = uvs;
        meshData[Mesh::ARRAY_NORMAL] = normals;
        
        Ref<ArrayMesh> mesh;
        mesh.instantiate();
        mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, meshData);

        return mesh;
    }
};
} // namespace terrain