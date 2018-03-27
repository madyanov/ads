#ifndef RK_H
#define RK_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "vec.h"

void rk_search(size_t **occs, const char *text, const char *patt);

#endif
