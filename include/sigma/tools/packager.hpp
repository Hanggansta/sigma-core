#ifndef SIGMA_TOOLS_PACKAGER_HPP
#define SIGMA_TOOLS_PACKAGER_HPP

#include <sigma/context.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/json_conversion.hpp>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/operations.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

namespace sigma {
namespace tools {

    struct package_settings {
        BOOST_HANA_DEFINE_STRUCT(
            package_settings,
            (boost::filesystem::path, build_directory),
            (std::vector<boost::filesystem::path>, source_directories));
    };

    template <class PackageSettings, class ContextType>
    class packager;

    using complex_resource_id = std::vector<boost::filesystem::path>;

    std::size_t resource_id_for(const complex_resource_id& cid)
    {
        std::size_t hash_code = 0;
        for (const auto& id : cid)
            boost::hash_combine(hash_code, id);
        return hash_code;
    }

    template <class PackageSettings, class ContextType>
    class resource_loader {
    public:
        virtual ~resource_loader() = default;

        virtual bool supports_filetype(const std::string& ext) const = 0;

        virtual void load(const PackageSettings& package_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file) = 0;
    };

    template <class PackageSettings, class... Resources, class... Settings>
    class packager<PackageSettings, context<type_set<Resources...>, type_set<Settings...>>> {
    public:
        using package_settings_type = PackageSettings;
        using resource_set_type = resource_set<Resources...>;
        using settings_set_type = settings_set<Settings...>;
        using context_type = context<resource_set_type, settings_set_type>;

        packager(context_type& ctx)
            : context_(ctx)
        {
        }

        template <class Loader>
        void add_loader()
        {
            this->loaders_.push_back(std::move(std::make_unique<Loader>(context_)));
        }

        void scan()
        {
            load_package_settings();

            for (const auto& loader : loaders_) {
                for (const auto& source_directory : settings_.source_directories) {
                    boost::filesystem::recursive_directory_iterator it{ source_directory };
                    boost::filesystem::recursive_directory_iterator end;
                    for (; it != end; ++it) {
                        auto path = it->path();
                        if (sigma::filesystem::is_hidden(path)) {
                            if (boost::filesystem::is_directory(path))
                                it.no_push();
                            continue;
                        }

                        auto ext = boost::algorithm::to_lower_copy(path.extension().string());
                        if (boost::filesystem::is_regular_file(path) && loader->supports_filetype(ext)) {
                            try {
                                loader->load(settings_, source_directory, ext, path);
                            } catch (const std::runtime_error& e) {
                                std::cerr << "error: " << e.what() << '\n';
                            }
                        }
                    }
                }
            }

            auto graphics_directory = context_.get_cache_path() / "graphics";
            if (!boost::filesystem::exists(graphics_directory))
                boost::filesystem::create_directories(graphics_directory);

            auto& effect_cache = context_.template get_cache<graphics::post_process_effect>();
            graphics::renderer::settings gsettings;
            gsettings.image_based_light_effect = effect_cache.handle_for(resource_id_for({ "post_process_effect/pbr/deffered/lights/image_based" }));
            gsettings.point_light_effect = effect_cache.handle_for(resource_id_for({ "post_process_effect/pbr/deffered/lights/point" }));
            gsettings.directional_light_effect = effect_cache.handle_for(resource_id_for({ "post_process_effect/pbr/deffered/lights/directional" }));
            gsettings.spot_light_effect = effect_cache.handle_for(resource_id_for({ "post_process_effect/pbr/deffered/lights/spot" }));
            gsettings.texture_blit_effect = effect_cache.handle_for(resource_id_for({ "post_process_effect/pbr/deffered/texture_blit" }));
            gsettings.vignette_effect = effect_cache.handle_for(resource_id_for({ "post_process_effect/vignette" }));
            gsettings.gamma_conversion = effect_cache.handle_for(resource_id_for({ "post_process_effect/pbr/deffered/gamma_conversion" }));
            gsettings.shadow_technique = context_.template get_cache<graphics::technique>().handle_for(resource_id_for({ "vertex/default", "fragment/shadow" }));
            std::ofstream stream{ (graphics_directory / "settings").string(), std::ios::binary };
            boost::archive::binary_oarchive oa(stream);
            oa << gsettings;

            (context_.template get_cache<Resources>().save(), ...);
        }

    private:
        context_type& context_;
        package_settings_type settings_;
        std::vector<std::unique_ptr<resource_loader<package_settings_type, context_type>>> loaders_;

        void load_package_settings()
        {
            Json::Value json_settings;
            auto settings_path = context_.get_cache_path() / "settings.json";
            std::ifstream file{ settings_path.c_str() };
            file >> json_settings;

            json::from_json(json_settings["package"], settings_);
        }
    };
}
}

#endif // SIGMA_TOOLS_PACKAGER_HPP
