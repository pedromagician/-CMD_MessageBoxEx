#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
	#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define DBG_NEW new
#endif

class Debug {
public:
	Debug();
	~Debug();
};
