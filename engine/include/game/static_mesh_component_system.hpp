#ifndef SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP
#define SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP

#include <game/component_system.hpp>
#include <game/transform_component.hpp>
#include <game/static_mesh_component.hpp>

namespace sigmafive {
	namespace game {
		class static_mesh_component_system : public component_system {
			SIGMAFIVE_COMPONENT_SYSTEM();
		public:
			static_mesh_component_system();

			virtual ~static_mesh_component_system();

			virtual bool is_intrested(entity e) const override;

			virtual void entity_added(entity e) override;

			virtual void entity_removed(entity e) override;

			virtual void process(entity_system &entity_system) override;
		private:
		};
	}
}

#endif //SIGMAFIVE_GAME_STATIC_MESH_COMPONENT_SYSTEM_HPP
