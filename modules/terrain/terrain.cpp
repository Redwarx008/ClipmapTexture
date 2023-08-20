#include "terrain.h"
#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/variant/array.h"
#include "scene/resources/material.h"

namespace terrain {

Terrain::Terrain() {
    _material = Ref<Material>(new ShaderMaterial());
    
}


void Terrain::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &Terrain::set_mesh);
    ClassDB::bind_method(D_METHOD("get_mesh"), &Terrain::get_mesh);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh"), "set_mesh", "get_mesh");
}

}