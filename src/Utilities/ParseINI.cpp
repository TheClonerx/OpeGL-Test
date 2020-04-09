#include <Utilities.hpp>
#include <fstream>

std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> tcx::parse_ini(std::istream& is)
{
    std::string line;
    std::string current_section = "";
    std::size_t line_number = 1;

    std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> data;

    while (std::getline(is, line)) {
        std::string_view line_view = strip(line);
        if (line_view.empty())
            continue;
        if (line_view.front() == '[') {
            auto it = std::find(line_view.crbegin(), line_view.crend(), ']');
            if (it == line_view.crend())
                throw std::runtime_error("No encloding ] at line " + std::to_string(line_number));
            current_section = std::string(line_view.cbegin() + 1, it.base() - 1);
        } else {
            auto it = std::find(line_view.cbegin(), line_view.cend(), '=');
            if (it == line_view.cend())
                throw std::runtime_error("No = found at line " + std::to_string(line_number));
            auto key = std::string { line_view.cbegin(), it };
            auto value = std::string { it + 1, line_view.cend() };

            if (auto it = std::find_if(data.begin(), data.end(), [&current_section](const auto& p) { return p.first == current_section; }); it == data.end()) {
                data.push_back({ current_section, {} });
            } else {
                if (auto lt = std::find_if(it->second.begin(), it->second.end(), [&key](const auto& p) { return p.first == key; }); lt == it->second.end()) {
                    it->second.push_back({ std::move(key), std::move(value) });
                } else {
                    lt->second = std::move(value);
                }
            }
        }
        line_number++;
    }
    return data;
}

std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> tcx::parse_ini(std::string_view name)
{
    std::ifstream is;
    is.open(name.data());
    if (!is.is_open())
        throw std::runtime_error("Can't open file " + std::string(name));
    return parse_ini(is);
}
