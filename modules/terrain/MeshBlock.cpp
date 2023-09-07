#include "MeshBlock.hpp"
#include "core/math/basis.h"
#include "core/math/transform_3d.h"
#include "core/math/vector3.h"
#include "scene/resources/material.h"
#include "servers/rendering_server.h"

namespace terrain
{
MeshBlock::MeshBlock(Ref<World3D> world, Ref<Mesh> mesh, Ref<Material> material, Vector2i pos)
{
    RenderingServer &rs = *RenderingServer::get_singleton();
    _instance = rs.instance_create();
    rs.instance_set_scenario(_instance, world->get_scenario());

    rs.instance_set_base(_instance, mesh->get_rid());
    _mesh = mesh;

    Transform3D transform{Basis(), Vector3(pos.x, 0, pos.y)};
    rs.instance_set_transform(_instance, transform);

    rs.instance_geometry_set_material_override(_instance, material->get_rid());

    rs.instance_set_visible(_instance, false);
}

void MeshBlock::SetVisible(bool visible)
{
    RenderingServer &rs = *RenderingServer::get_singleton();
    rs.instance_set_visible(_instance, visible);
}

void MeshBlock::SetMaterial(Ref<Material> material)
{
    RenderingServer &rs = *RenderingServer::get_singleton();
    rs.instance_geometry_set_material_override(_instance, material->get_rid());
}

MeshBlock::~MeshBlock()
{
    RenderingServer &rs = *RenderingServer::get_singleton();
    rs.free(_instance);
}
} // namespace terrain