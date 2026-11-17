#pragma once

#include <Core/Containers/TArray.h>

#include <string>

namespace CE::CommandLine
{

void Set(int argc, char** argv);
const TArray<std::string>& GetArgs();

}