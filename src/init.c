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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "resource_management.h"
#include "emu.h"
#include "defs.h"

#define OPTSTR "i:hv"

extern char *optarg;

void show_help(void) {
  printf("Usage: chippy <options>\n\n");
  printf(" -i: (required) path to the ROM\n");
  /* printf(" -v: (optional) verbose/show debug\n"); */
  printf(" -h: show usage\n");
  printf("The Chippy emulator and the Peppermint \"frontend\" powered by it are works of Snoolie K / 0xilis.\n");
}

int main(int argc, char* *argv) {
  /* find resources folder */
  if (argv) {
    char *executablePath = argv[0];
    if (executablePath) {
      resourcesPath = find_resource_path(executablePath);
    } else {
      PMError("argv[0] returned NULL\n");
    }
  } else {
    PMError("no argv present?\n");
  }
  PMDLog("resourcesPath: %s\n", resourcesPath);

  /* Check for jumpstart / bootstrap ROM */
  char *romPath;
  char *resource = find_resource("boot.ch8");
  if (access(resource, F_OK) == 0) {
    /* TODO: Memory leak issue since resource will not be freed */
    romPath = resource;
    goto jumpstart;
  }
  free(resource);
  resource = NULL;

  /* Parse args */
  int opt;
  while ((opt = getopt(argc, argv, OPTSTR)) != EOF) {
    if (opt == 'i') {
      romPath = optarg;
    } else if (opt == 'h') {
      /* Show help */
      show_help();
      return 0;
    }
  }

  if (!romPath) {
    /* Show help */
    show_help();
    return 0;
  }

  jumpstart:
  PMDLog("ROM path: %s\n", romPath);
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    PMError("error with SDL: %s\n",SDL_GetError());
    return 1;
  }
  /* Support 128x64 later */
  int SCREEN_WIDTH = 64;
  int SCREEN_HEIGHT = 64;
  int SCREEN_SCALE = 8;
  SDL_Window *win = SDL_CreateWindow("Chippy",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_RESIZABLE);
  if (!win) {
    PMError("error creating window: %s\n",SDL_GetError());
    SDL_Quit();
    return 1;
  }
  emulator(win, (const char *)romPath);

  /* Close */
  free(resourcesPath);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}