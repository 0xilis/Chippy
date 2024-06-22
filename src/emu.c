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
#include <inttypes.h>
#include "resource_management.h"
#include "defs.h"

/* Global variables */
SDL_Renderer* rend;
char *vram;
int running = 1;
uint8_t *emuRAM;
uint16_t pc = 0x200;
uint8_t registers[16];
uint16_t ri; /* The 16-bit register I */
uint16_t stack[16];

int draw_pixel(int x, int y) {
  /* Support 128x64 later */
  int SCREEN_WIDTH = 64;
  int SCREEN_HEIGHT = 64;
  /* Like a CRT, each line from left to right then another line */
  int pixelId = (y * SCREEN_WIDTH) + x;
  char pixel = vram[pixelId];
  int pixelDrew;
  if (pixel) {
    /* Pixel set on, turn it off! */
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    pixelDrew = 0;
  } else {
    /* Pixel set off, turn it on! */
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    pixelDrew = 1;
  }
  vram[pixelId] = pixelDrew;
  SDL_RenderDrawPoint(rend, x, y);
  return pixelDrew;
}

void cls(void) {
  /* Support 128x64 later */
  int SCREEN_WIDTH = 64;
  int SCREEN_HEIGHT = 64;
  memset(vram,0, SCREEN_WIDTH * SCREEN_HEIGHT);
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);
}

void render(void) {
  SDL_RenderPresent(rend);
}

void handle_events(void) {
  SDL_Event event;
  SDL_PollEvent(&event);
  /* This should probably be a switch case... */
  if (event.type == SDL_QUIT) {
    running = 0;
  }

  /* execute instruction */
  uint16_t instr = emuRAM[pc] << 8 | emuRAM[pc+1];
  pc += 2;
  uint8_t x = (instr & 0x0F00) >> 8;
  uint8_t y = (instr & 0x00F0) >> 4;
  uint16_t opcode = instr & 0xF000;
  uint8_t lowByte = instr & 0xFF;
  /* printf("executing %02x\n",instr); */
  if (!opcode) {
    if (instr == 0x00E0) {
      /* Clear screen */
      cls();
      return;
    }
    if (instr == 0x00EE) {
      /* return (pop from stack, store in program counter */
      printf("RET (IMPLEMENT LATER)\n");
      return;
    }
  }
  if (opcode == 0x1000) {
    /* JP addr */
    pc = instr & 0xFFF;
  } else if (opcode == 0x3000) {
    if (registers[x] == lowByte) {
      pc += 2;
    }
  } else if (opcode == 0x4000) {
    if (registers[x] != lowByte) {
      pc += 2;
    }
  } else if (opcode == 0x5000) {
    if (registers[x] == registers[y]) {
      pc += 2;
    }
  } else if (opcode == 0x6000) {
    /* LD Vx, byte */
    registers[x] = instr & 0xFF;
  } else if (opcode == 0x7000) {
    /* ADD Vx, byte */
    registers[x] = (registers[x]+lowByte) & 0xFF;
  } else if (opcode == 0xA000) {
    ri = instr & 0xFFF;
  } else if (opcode == 0xD000) {
    /* DRW Vx, Vy, nibble */
    uint8_t nibble = instr & 0xF;
    registers[15] = 0;
    for (int row = 0; row < nibble; row++) {
      uint8_t sprite = emuRAM[ri+row];
      for (int col = 0; col < nibble; col++) {
        if ((sprite & 0x80) > 0) {
          if (draw_pixel(registers[x]+col, registers[y]+row) == 0) {
            registers[15] = 1;
          }
        }
        sprite <<= 1;
      }
    }
    render();
  }
}

void emulator(SDL_Window *win, const char *romPath) {
  printf("starting emulator...\n");
  /*
   * According to https://nullprogram.com/blog/2023/01/08/
   * SDL2 already tries to create an accelerated renderer
   * and not specifying allows for a software renderer fallback
   */
  Uint32 render_flags = SDL_RENDERER_PRESENTVSYNC;
  rend = SDL_CreateRenderer(win, -1, render_flags);
  if (!rend) {
    PMError("error creating renderer: %s\n",SDL_GetError());
    return;
  }
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
  SDL_RenderClear(rend);
  /* Support 128x64 later */
  int SCREEN_WIDTH = 64;
  int SCREEN_HEIGHT = 64;
  vram = malloc(SCREEN_WIDTH * SCREEN_HEIGHT);
  if (!vram) {
    PMError("unable to allocate the 8KB VRAM\n");
    SDL_DestroyRenderer(rend);
    return;
  }
  /* initialize the initial vram to be all black at first. */
  memset(vram, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
  
  /* Load ROM into 4KB memory */
  emuRAM = malloc(4096);
  if (!emuRAM) {
    PMError("unable to allocate the 4KB emuRAM\n");
    free(vram);
    SDL_DestroyRenderer(rend);
    return;
  }
  memset(emuRAM,0,4096);
  FILE *fp = fopen(romPath, "r");
  if (!fp) {
    PMError("unable to open file input\n");
    free(vram);
    SDL_DestroyRenderer(rend);
    return;
  }
  fseek(fp, 0, SEEK_END);
  size_t binarySize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  /* int fd = fp->_file;
  ssize_t bytesRead = read(fd, emuRAM + 0x200, binarySize); */
  size_t bytesRead = fread(emuRAM + 0x200, 1, binarySize, fp);
  fclose(fp);
  if (bytesRead < binarySize) {
    PMError("failed to read entire file, read %zd, expected %zu\n",bytesRead,binarySize);
    goto cleanup;
  }
  /* Main emu loop */
  while (running) {
    /* Handle events */
    handle_events();

    /* Render game state */
    /* render(); */

    SDL_Delay(100);
  }

  /* Cleanup */
  cleanup:
  free(emuRAM);
  free(vram);
  SDL_DestroyRenderer(rend);
  printf("ended emulation.\n");
}