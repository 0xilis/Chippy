/*
 * Copyright (C) 2023 Snoolie K / 0xilis. All rights reserved.
 *
 * This document is the property of Snoolie K / 0xilis.
 * It is considered confidential and proprietary.
 *
 * This document may not be reproduced or transmitted in any form,
 * in whole or in part, without the express written permission of
 * Snoolie K / 0xilis.
*/

#ifndef RESOURCE_MANAGEMENT_H
#define RESOURCE_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *resourcesPath;

/* Resource Finder */

char *find_resource_path(char* runPath);
char *find_resource(char* resource);

#endif /* RESOURCE_MANAGEMENT_H */