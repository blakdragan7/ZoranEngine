#pragma once

#include "Core/PlatformTypes.h"
#include "Core/ZoranEngine.h"
#include "Utils/HighPrecisionClock.h"

// count code lines in powershell from sln directory
// (Get-ChildItem -Recurse -include *.cpp,*.h,*.hpp | ?{ $_.fullname -notmatch "\\ThirdParty\\?" } | select-string .).Count