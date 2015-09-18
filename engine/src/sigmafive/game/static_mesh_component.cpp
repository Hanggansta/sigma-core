#include <sigmafive/game/static_mesh_component.hpp>

namespace sigmafive {
    namespace game {
        void static_mesh_component::set_static_mesh(std::shared_ptr<graphics::static_mesh> static_mesh) {
            static_mesh_ = std::move(static_mesh);
        }

        std::weak_ptr<graphics::static_mesh> static_mesh_component::static_mesh() {
            return static_mesh_;
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::game::static_mesh_component)