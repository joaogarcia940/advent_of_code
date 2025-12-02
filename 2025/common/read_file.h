#include <ranges>
#include <fstream>
#include <string>
#include <vector>

using Text = std::vector<std::string>;

inline Text ReadFile(const std::string& path)
{
    std::ifstream file{path};
    if (!file.good() && !file.eof())
    {
        throw std::runtime_error("Error reading file: " + path);
    }

    const auto begin = std::istreambuf_iterator<char>(file);
    const auto end = std::istreambuf_iterator<char>();

    std::string all_content{begin, end};

    return all_content | std::views::split('\n') |
           std::views::transform([](auto&& str_range) { return std::string(str_range.begin(), str_range.end()); }) |
           std::ranges::to<std::vector>();
}
