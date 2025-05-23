#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "graphics.h"
#include "logic.h"


using namespace std;


void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //full screen
    //window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit( "SDL_image error:", IMG_GetError());

    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy trong máy ảo (ví dụ phòng máy ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}

SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                     "Loading %s", filename);

	SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
	if (texture == NULL) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                       "Load texture %s", IMG_GetError());
      }

	return texture;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

void drawSomething(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // white
    SDL_RenderDrawPoint(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   // red
    SDL_RenderDrawLine(renderer, 200, 300, 500, 700);
    SDL_Rect filled_rect;
    filled_rect.x = SCREEN_WIDTH - 800;
    filled_rect.y = SCREEN_HEIGHT - 350;
    filled_rect.w = 300;
    filled_rect.h = 200;
    SDL_SetRenderDrawColor(renderer, 50, 69, 0, 86); // green
    SDL_RenderFillRect(renderer, &filled_rect);
}

void renderTexture(SDL_Texture *texture, int x, int y, SDL_Renderer* renderer)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void processClick(int x, int y, Tictactoe& game) {
    int clickedCol = 0;
    int clickedRow = 0;
    game.move(clickedRow, clickedCol);
}

void processClickAt(int x, int y, Tictactoe& game) {
    int clickedCol = (x - BOARD_X);
    int clickedRow = (y - BOARD_Y);
    if(clickedCol<0 || clickedRow<0) return;
    if(clickedCol % CELL_SIZE==0 || clickedRow % CELL_SIZE==0) return;
    clickedCol /= CELL_SIZE;
    clickedRow /= CELL_SIZE;
    game.move(clickedRow, clickedCol);
}


int main(int argc, char* argv[])
{
    Graphics graphics;
    graphics.init();
    Tictactoe game;
    game.init();
    graphics.render_home();

    int x, y;
    int home=1;
    SDL_Event event;
    bool quit = false;
    while (! quit) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                 quit = true;
                 break;
            case SDL_MOUSEBUTTONDOWN:
                 SDL_GetMouseState(&x, &y);
                 if(home)
                 {
                     if(x>SCREEN_WIDTH/2-100 && x<SCREEN_WIDTH/2+100 && y>SCREEN_HEIGHT*0.5 && y<SCREEN_HEIGHT*0.5+200)
                     {
                         home=0;
                         graphics.render(game);
                     }
                 }
                 else{
                     if(x>520 && x<560 && y>5 && y<40) {
                         game.reset();
                         graphics.render(game);
                         continue;
                     }
                     if(x>560 && y>5 && y<40)
                     {
                         game.reset();
                         graphics.render_home();
                         home=1;
                         continue;
                     }
                     if(!game.End)
                     {
                         processClickAt(x, y, game);
                         if(game.win_check('x') || game.win_check('o') || game.turn==BOARD_SIZE*BOARD_SIZE+1)
                         {
                             game.End=1;
                             game.turn--;
                         }
                         graphics.render(game);
                     }
                 }
                 break;
        }
        SDL_Delay(10);
    }

    graphics.quit();
    return 0;
}
