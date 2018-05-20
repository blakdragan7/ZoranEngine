#pragma once
#include "PlatformTypes.h"

/*
* ShaderProgramBase must be sub classed To implement specific shaders.
* There is not really shared logic in the baseclass, it's only a base class to 
* allow easy abstract passing between the scene object and the render engine
*/

class DRAGENGINE_EXPORT ShaderProgramBase
{

public:
	ShaderProgramBase();
	virtual ~ShaderProgramBase();
};

