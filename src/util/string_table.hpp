#pragma once

#include <string>
#include <unordered_map>


namespace util
{
    class string_table
    {
    public:
        size_t hash_and_store(const std::string &str)
        {
            const auto hash = std::hash<std::string>{}(str);
            _hash_to_string[hash] = str;
            return hash;
        }

        std::string& operator[](const size_t key)
        {
            return _hash_to_string[key];
        }

        static size_t hash_for(const std::string &str)
        {
            return std::hash<std::string>{}(str);
        }

    private:
        std::unordered_map<std::size_t, std::string> _hash_to_string;
    };
}
