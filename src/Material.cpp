#include <Material.hpp>


void Material::apply(){
    shader->activate();

    shader->set_float("materialColor", {color.r, color.g, color.b});
    shader->set_float("tiling", {tiling.x, tiling.y});
    shader->set_float("shininess", {shininess});
    shader->set_float("specularStrength", {specularStrength});

    albedo->bind_texture_unit(GL_TEXTURE0);
    shader->set_int("albedoTexture", {0});
}
