#include "core/object/class_db.h"
#include "core/object/object.h"
#include "scene/resources/material.h"

#include "terrain.hpp"

namespace terrain
{

Terrain::Terrain()
{
}

void Terrain::_notification(int p_what)
{
    switch (p_what)
    {
    NOTIFICATION_READY:
        set_process(true);
        break;
    NOTIFICATION_PROCESS:
        process();
        break;
    }
}
void Terrain::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &Terrain::set_mesh);
    ClassDB::bind_method(D_METHOD("get_mesh"), &Terrain::get_mesh);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh"), "set_mesh", "get_mesh");
}

} // namespace terrain