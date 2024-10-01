#pragma once

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace core
{
    class startup_config
    {
    public:
        static constexpr float DefaultVolume = 0.5f;

        startup_config()
        {
            load();
        }

        [[nodiscard]] bool fullscreen() const;
        [[nodiscard]] std::uint32_t height() const;
        [[nodiscard]] std::uint32_t width() const;
        [[nodiscard]] float fov() const;
        [[nodiscard]] bool backface_culling() const;
        [[nodiscard]] bool free_mouse() const;
        [[nodiscard]] std::string window_title() const;

        template <typename T>
        void set(const std::string& option, T value)
        {
            config_json[option] = value;
            save();
        }

    private:
        json config_json;

        [[nodiscard]] static json make_default();

        void load();
        void save() const;
    };
} // namespace core
