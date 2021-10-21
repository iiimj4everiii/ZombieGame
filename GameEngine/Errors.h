#pragma once
#include <string>

namespace GameEngine
{
	//Need the extern here because this is a separate normal file, not a class file.
	extern void fatalError(std::string errorString);
}