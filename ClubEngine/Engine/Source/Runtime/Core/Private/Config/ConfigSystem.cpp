#include <Core/Config/ConfigSystem.h>
#include <Core/Config/IniParser.h>
#include <Core/Log.h>

namespace CE
{

void ConfigSystem::LoadFile(const std::filesystem::path& path)
{
    IniParser::Parse(path, data);
}

void ConfigSystem::AddCommandLine(const TArray<std::string>& commandLine)
{
    for (const std::string& arg : commandLine)
    {
        // Only "-Section.Key=Value" or "--Section.Key=Value", other args are left for other systems or get ignored

        if (arg.size() < 2 || arg[0] != '-')
            continue;

        const size_t start = (arg[1] == '-') ? 2 : 1;
        const std::string body = arg.substr(start);

        const size_t dot = body.find('.');
        if (dot == std::string::npos)
            continue;

        const size_t eq = body.find('=', dot);

        const std::string section = body.substr(0, dot);
        const std::string key     = body.substr(dot + 1, eq == std::string::npos ? std::string::npos : eq - dot - 1);
        const std::string value   = (eq == std::string::npos) ? "true" : body.substr(eq + 1);

        if (section.empty() || key.empty())
        {
            CE_LOG(Warning, "Config: ignoring malformed argument '{}'", arg);
            continue;
        }

        data[section][key] = value;
        CE_LOG(Info, "Config: command line override [{}] {}={}", section, key, value);
    }
}

}