#include <sigmafive/game/scene.hpp>
#include <sigmafive/system/window.hpp>
#include <sigmafive/game/entity_world.hpp>
#include <sigmafive/game/static_mesh_component_system.hpp>

int main(int argc, char const *argv[]) {
    sigmafive::system::context_attributes context_attributes;
    context_attributes.red = 8;
    context_attributes.green = 8;
    context_attributes.blue = 8;
    context_attributes.alpha = 8;
    context_attributes.depth = 24;
    context_attributes.stencil = 8;
    context_attributes.samples = 0;
    context_attributes.major = 4;
    context_attributes.minor = 4;
    context_attributes.double_buffer = true;
    context_attributes.core_profile = false;
    context_attributes.vsync = false;
    sigmafive::system::window window("sigma five sample", int2(800, 600), context_attributes);

    sigmafive::game::scene scene;
    sigmafive::game::entity_world world;

    world.component_registry().register_component(sigmafive::game::transform_component::CLASS_ID,
                                std::unique_ptr<sigmafive::game::transform_component_pool_factory>(new sigmafive::game::transform_component_pool_factory{}));
    world.component_registry().register_component(sigmafive::game::static_mesh_component::CLASS_ID,
                                std::unique_ptr<sigmafive::game::static_mesh_component_pool_factory>(new sigmafive::game::static_mesh_component_pool_factory{}));


    auto system = world.add_component_system<sigmafive::game::static_mesh_component_system>();
    system->init(world,scene);

    auto e = world.create();
    world.add_component<sigmafive::game::transform_component>(e);
    world.add_component<sigmafive::game::static_mesh_component>(e);

    while(window.good()) {
    }

    return 0;
}
