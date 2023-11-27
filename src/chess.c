/*
 * chess.c
 */

#include <postgres.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils/builtins.h"
#include "libpq/pqformat.h"

#include "smallchesslib.h"
#include "chess.h"

PG_MODULE_MAGIC;

/*****************************************************************************/

