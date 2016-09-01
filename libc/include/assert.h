#ifndef _ASSERT_H
#define _ASSERT_H

#include <stdlib.h>

#ifndef NDEBUG
#define assert(x) { if(!(x)){ abort(__FILE__, __LINE__); } }
#else
#define assert(ignore)((void) 0)
#endif

#define ASSERT assert

#endif
