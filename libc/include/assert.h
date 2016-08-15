#ifndef _ASSERT_H
#define _ASSERT_H

#include <stdlib.h>

#ifndef NDEBUG
#define assert(x) if(!(x)){ abort(); }
#else
#define assert(ignore)((void) 0)
#endif

#define ASSERT assert

#endif
