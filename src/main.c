#include <SDL2/SDL.h>
#include <SDL_log.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_shape.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// default values
static const char *title = "SDL1";

// Screen size :3
#define WIDTH 1200
#define HEIGHT 800

// FPS targets
static float_t FPS = 60;
#define FRAMETIME_TARGET (1000 / FPS)

// Player speed
#define SPEED 10

// PTR
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// no PTR
SDL_Event event;
bool quit;
int last_frame_time = 0;
int start_time = 0;
bool active = false;

// Hitbox alias T?T
typedef SDL_Rect Hitbox;

typedef struct ground {
  int h;
  int w;
  int x;
  int y;
  Hitbox hitbox;
} Ground;
Ground ground;

// player config
typedef struct player {
  double angle;
  int h, w;
  int x, y;
  double dx, dy;
  Hitbox hitbox;
} Player;
Player player;

SDL_Rect setup_hb_obj(int h, int w, int x, int y, Hitbox hb) {
  hb.x = x;
  hb.y = y;
  hb.h = h;
  hb.w = w;
  return hb;
}

void setup() {
  player.angle = 1;
  player.x = WIDTH / 2;
  player.y = HEIGHT / 2;
  player.h = 15;
  player.w = 15;

  player.dx = 1;
  player.dy = 1;

  player.hitbox =
      setup_hb_obj(player.h, player.w, player.x, player.y, player.hitbox);

  ground.x = 600;
  ground.y = 600;
  ground.w = WIDTH;
  ground.h = 200;

  ground.hitbox =
      setup_hb_obj(ground.h, ground.w, ground.x, ground.y, ground.hitbox);
}

bool collision_checker(SDL_Rect OBJ, SDL_Rect OBJ2) {
  return SDL_HasIntersection(&OBJ, &OBJ2);
}

void draw_gameObj(SDL_Renderer *renderer, SDL_Rect obj) {
  SDL_SetRenderDrawColor(renderer, 233, 230, 0, 255);
  SDL_RenderFillRect(renderer, &obj);
}

void process_input() {
  SDL_PumpEvents();

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_ESCAPE]) {
    quit = SDL_TRUE;
    printf("Process ended\n");
  }

  active = false;
  int dx = 0, dy = 0;

  if (state[SDL_SCANCODE_W]) {
    dy -= 1;
    active = true;
  }
  if (state[SDL_SCANCODE_A]) {
    dx -= 1;
    active = true;
  }
  if (state[SDL_SCANCODE_S]) {
    dy += 1;
    active = true;
  }
  if (state[SDL_SCANCODE_D]) {
    dx += 1;
    active = true;
  }

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit = SDL_TRUE;
    }
  }
}

void update_loop() {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAMETIME_TARGET))
    ;
  last_frame_time = SDL_GetTicks();

  if (active) {
    double angleRad = player.angle * M_PI / 180.0;

    double moveSpeed = (fabs(cos(angleRad)) + fabs(sin(angleRad)) > 1.0)
                           ? SPEED / sqrt(2)
                           : SPEED;

    player.dx = cos(angleRad) * moveSpeed;
    player.dy = sin(angleRad) * moveSpeed;

    player.x += (int)player.dx;
    player.y += (int)player.dy;
  }

  player.hitbox =
      setup_hb_obj(player.h, player.w, player.x, player.y, player.hitbox);

  if (player.hitbox.x < 0)
    player.hitbox.x = WIDTH - player.hitbox.w;
  if (player.hitbox.x + player.hitbox.w > WIDTH)
    player.hitbox.x = 0;

  if (player.hitbox.y < 0)
    player.hitbox.y = HEIGHT - player.hitbox.h;
  if (player.hitbox.y + player.hitbox.h > HEIGHT)
    player.hitbox.y = 0;

  ground.hitbox =
      setup_hb_obj(ground.h, ground.w, ground.x, ground.y, ground.hitbox);
}

void render_loop() {
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 120);
  SDL_RenderClear(renderer);

  // TODO: draw player from hitbox location
  // maybe this worky???
  // this worky, yay :D
  draw_gameObj(renderer, player.hitbox);
  //  draw_gameObj(renderer, ground.hitbox);

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
