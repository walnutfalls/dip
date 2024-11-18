#pragma once

#include <string>
#include <optional>

namespace os {
    std::optional<std::string> browse_dialog();
    std::optional<std::string> save_dialog();
}