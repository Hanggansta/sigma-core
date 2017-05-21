#ifndef SIGMA_ENGINE_MATERIAL_HPP
#define SIGMA_ENGINE_MATERIAL_HPP

#include <sigma/graphics/material.hpp>
#include <sigma/graphics/opengl/shader_technique.hpp>

#define MATERIAL_PTR(material_mgr, x) material_mgr.acquire(x)

namespace sigma {
namespace opengl {
    class material : public opengl::shader_technique<graphics::material> {
    public:
        material() = default;

        material(const graphics::material& data);

        material(material&&) = default;

        material& operator=(material&&) = default;

        void link(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, shader_manager& shader_mgr);

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };

    class material_manager {
    public:
        // TODO remove the use of unique_ptr

        material_manager(texture_manager& textures, cubemap_manager& cubemaps, shader_manager& shaders, resource::cache<graphics::material>& material_cache)
            : textures_(textures)
            , cubemaps_(cubemaps)
            , shaders_(shaders)
            , material_cache_(material_cache)
        {
        }

        resource::handle<graphics::material> get(resource::identifier id)
        {
            return material_cache_.get(id);
        }

        opengl::material* acquire(resource::handle<graphics::material> hndl)
        {
            // TODO not thread safe
            if (hndl.index >= materials_.size())
                materials_.resize(hndl.index + 1);

            if (materials_[hndl.index] == nullptr) {
                materials_[hndl.index] = std::make_unique<material>(*material_cache_.acquire(hndl));
                materials_[hndl.index]->link(textures_, cubemaps_, shaders_);
            }

            return materials_.at(hndl.index).get();
        }

    private:
        texture_manager& textures_;
        cubemap_manager& cubemaps_;
        shader_manager& shaders_;
        resource::cache<graphics::material>& material_cache_;
        std::vector<std::unique_ptr<material>> materials_;
    };
}
}

#endif // SIGMA_ENGINE_MATERIAL_HPP
