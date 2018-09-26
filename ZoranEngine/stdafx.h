#pragma once

#include "Core/PlatformTypes.h"
#include "Core/ZoranEngine.h"
#include "Utils/HighPrecisionClock.h"

// count code lines in powershell from sln directory
// (gci -include *.cpp,*.h,*.hpp -recurse | select-string .).Count