#ifndef SIGMA_CONTEXT_HPP
#define SIGMA_CONTEXT_HPP

#include <sigma/config.hpp>

#include <boost/filesystem/path.hpp>

#include <memory>
#include <unordered_map>

namespace sigma {
class game;
struct game_class;
namespace graphics {
    class renderer;
    class renderer_class;
    class texture_cache;
    class shader_cache;
    class material_cache;
    class static_mesh_cache;
}
class SIGMA_API context {
public:
    graphics::texture_cache& textures();

    graphics::shader_cache& shaders();

    graphics::material_cache& materials();

    graphics::static_mesh_cache& static_meshes();

    bool load_plugin(boost::filesystem::path path);

    void set_renderer_class(std::string renderer_class);

    void set_game_class(std::string game_class);

    void update(std::chrono::duration<float> dt);

private:
    std::unordered_map<std::string, boost::shared_ptr<game_class> > game_classes;
    std::unordered_map<std::string, boost::shared_ptr<graphics::renderer_class> > renderer_classes;

    std::shared_ptr<graphics::renderer> current_renderer;
    std::shared_ptr<game> current_game;
};
}

#endif // SIGMA_CONTEXT_HPP