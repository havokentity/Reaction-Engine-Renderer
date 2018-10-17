#include "Scene.h"

CoreEngine* createEngine(void)
{
#ifdef COMPILE_TEST_APP
		return new CTestApp;
#endif

#ifdef COMPILE_KARTHIK_APP 
		return new CKarthikApp;
#endif

#ifdef COMPILE_MARIA_APP 
		return new CMariaPaneer;
#endif
}