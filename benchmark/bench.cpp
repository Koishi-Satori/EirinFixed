#include "bench.hpp"
#include <fstream>
#include <unordered_map>
#include <vector>

eirin::fixed64 f64_identity(eirin::fixed64 val)
{
    return val;
}

eirin::fixed32 f32_identity(eirin::fixed32 val)
{
    return val;
}

double db_identity(double val)
{
    return val;
}

static std::vector<std::string> cached_inputs;
static std::unordered_map<std::string, std::string> cached_values;
static const auto VALUE_DEFAULT = "0";

std::string get_input(std::string input, std::string key)
{
    auto it = std::find(cached_inputs.begin(), cached_inputs.end(), input);
    if(it == cached_inputs.end())
    {
        cached_inputs.push_back(input);
        std::ifstream file("benchmark_input/" + input + ".in");
        if(!file.is_open())
        {
            return VALUE_DEFAULT;
        }
        std::string line;
        while(std::getline(file, line))
        {
            auto sep_pos = line.find('=');
            if(sep_pos == std::string::npos)
            {
                continue;
            }
            auto k = line.substr(0, sep_pos);
            auto v = line.substr(sep_pos + 1);
            cached_values[k] = v;
        }
        file.close();
    }
    auto vit = cached_values.find(key);
    if(vit != cached_values.end())
    {
        return vit->second;
    }
    return VALUE_DEFAULT;
}
