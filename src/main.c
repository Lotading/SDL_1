#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// default values
static const char *title = "SDL1";

// consts
#define WIDTH 1200
#define HEIGHT 800

#define FPS 60
#define FRAMETIME_TARGET (1000 / FPS)

// PTR
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Rect player_rect;

// no PTR
SDL_Event event;
bool quit;
int last_frame_time = 0;

typedef struct player {
  int h;
  int w;
  int x;
  int y;
} Player;

Player player;

void setup() {
  player.x = 0;
  player.y = 0;
  player.h = 15;
  player.w = 15;

  player_rect.x = player.x;
  player_rect.y = player.y;
  player_rect.h = player.h;
  player_rect.w = player.w;
}

void process_input() {
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      quit = SDL_TRUE;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        quit = SDL_TRUE;
        break;
      case SDLK_w:
        player.y -= 10;
        break;

      case SDLK_a:
        player.x -= 10;
        break;

      case SDLK_s:
        player.y += 10;
        break;

      case SDLK_d:
        player.x += 10;
        break;
      }
      break;
    }
  }
}

void update_loop() {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAMETIME_TARGET))
    ;
  last_frame_time = SDL_GetTicks();

  player_rect.x = player.x;
  player_rect.y = player.y;
  player_rect.h = player.h;
  player_rect.w = player.w;
}

void render_loop() {
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 120);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255,
                         255); 
  SDL_RenderFillRect(renderer, &player_rect);

  SDL_RenderPresent(renderer);
}

static void init_window() {
  if (SDL_Init(SDL_INIT_VIDEO) > 0)
    printf("SDL couldn not be initialized, ERR: %s\n", SDL_GetError());

  window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL)
    printf("window did not get created, ERR: %s\n", SDL_GetError());

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (renderer == NULL)
    printf("Renderer failed >:3 err: %s\n", SDL_GetError());
}

static void main_loop() {
  while (!quit) {
    process_input();
    update_loop();
    render_loop();
  }
  printf("window not yet gone?");
}

void destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(void) {
  init_window();
  setup();
  main_loop();
  destroy();
}
