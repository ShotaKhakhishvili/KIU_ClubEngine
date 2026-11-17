#include <Core/CommandLine.h>

namespace CE::CommandLine
{

namespace { TArray<std::string> args; }

void Set(const int argc, char** argv)
{
    args.assign(argv + 1, argv + argc);
}

const TArray<std::string>& GetArgs() { return args; }

}
