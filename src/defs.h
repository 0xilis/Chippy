/*
 * Copyright (C) 2024 Snoolie K / 0xilis. All rights reserved.
 *
 * This document is the property of Snoolie K / 0xilis.
 * It is considered confidential and proprietary.
 *
 * This document may not be reproduced or transmitted in any form,
 * in whole or in part, without the express written permission of
 * Snoolie K / 0xilis.
*/

#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>

#define DEBUGLOG 1
/* Peppermint Errors */
#define PMError(...) \
            do { fprintf(stderr, __VA_ARGS__); exit(1); } while (0)

#if DEBUGLOG
#define PMDLog(fmt, ...) printf("%s: " fmt, __FUNCTION__, __VA_ARGS__);
#else /* DEBUGLOG */
#define PMDLog(fmt, ...)
#endif /* DEBUGLOG */

#endif /* DEFS_H */