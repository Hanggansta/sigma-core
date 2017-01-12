#ifndef SIGMA_SHADER_TECHNIQUE_COMPILER_HPP
#define SIGMA_SHADER_TECHNIQUE_COMPILER_HPP

#include <sigma/graphics/shader_technique.hpp>

#include <json/json-forwards.h>

namespace sigma {
template <class T>
void compile_shader_technique(T& technique, const Json::Value& technique_data)
{
    for (auto it = technique_data.begin(); it != technique_data.end(); ++it) {
        const auto& value = *it;
        if (it.key() == "vertex") {
            technique.shaders[sigma::graphics::shader_type::vertex] = sigma::resource::identifier{ "vertex", value.asString() }; // TODO warn if tring to set shader more that once
        } else if (it.key() == "fragment") {
            technique.shaders[sigma::graphics::shader_type::fragment] = sigma::resource::identifier{ "fragment", value.asString() }; // TODO warn if tring to set shader more that once
        } else if (it.key() == "geometry") {
            technique.shaders[sigma::graphics::shader_type::geometry] = sigma::resource::identifier{ "geometry", value.asString() }; // TODO warn if tring to set shader more that once
        } else if (it.key() == "textures") {
            const auto& texture_object = *it;
            for (auto it2 = texture_object.begin(); it2 != texture_object.end(); ++it2)
                technique.textures[it2.key().asString()] = sigma::resource::identifier{ "texture", (*it2).asString() }; // TODO warn if tring to set texture more than once
        } else if (it.key() == "cubemaps") {
            const auto& cubemap_object = *it;
            for (auto it2 = cubemap_object.begin(); it2 != cubemap_object.end(); ++it2)
                technique.cubemaps[it2.key().asString()] = sigma::resource::identifier{ "cubemap", (*it2).asString() }; // TODO warn if tring to set cubemap more than once
        }
    }

    // TODO check for errors like no vertex or fragment shader
}
}

#endif // SIGMA_SHADER_TECHNIQUE_COMPILER_HPP