#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "logic.h"
struct Graphics {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *cellEmpty, *cellX, *cellO, *replay, *background, *play, *home, *name, *t1, *t2;

    void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                       "%s: %s", msg, error);
        SDL_Quit();
    }
    TTF_Font* loadFont(const char* path, int size)
    {
        TTF_Font* gFont = TTF_OpenFont( path, size );
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Load font %s", TTF_GetError());
        }
    }

    SDL_Texture* renderText(const char* text,
                            TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface =
                TTF_RenderText_Solid( font, text, textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture =
                SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Create texture from text %s", SDL_GetError());
        }
        SDL_FreeSurface( textSurface );
        return texture;
    }

    void initSDL()
    {

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                   SDL_WINDOW_SHOWN);
        if (window == nullptr)
                   logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1,
                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        //renderer =
                  SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

        if (renderer == nullptr)
             logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                             TTF_GetError());
        }
    }
    void init() {
        initSDL();
        cellEmpty = loadTexture("img//cell_empty.png");
        cellX = loadTexture("img//cell_x.png");
        cellO = loadTexture("img//cell_o.png");
        replay = loadTexture("img//replay.png");
        background=loadTexture("img//background.png");
        play=loadTexture("img//play.png");
        home=loadTexture("img//home.png");
        TTF_Font* f1 = loadFont("assets/Purisa-BoldOblique.ttf", 150);
        TTF_Font* f2 = loadFont("assets/Purisa-BoldOblique.ttf", 30);
        SDL_Color color = {0, 0, 0, 0};
        name = renderText("CARO", f1, color);
        t1 = renderText("PLAYER" ,f2, color);
        t2 = renderText("WIN!!!!!!", f2, color);
    }

    void prepareScene(SDL_Texture * background)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy( renderer, background, NULL, NULL);
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                       "Loading %s", filename);
        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                  SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }




    void quit()
    {

        TTF_Quit();
        SDL_DestroyTexture(cellEmpty);
        cellEmpty = nullptr;
        SDL_DestroyTexture(cellX);
        cellX = nullptr;
        SDL_DestroyTexture(cellO);
        cellO = nullptr;
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void render_home()
    {
        renderTexture(background,0,0);
        renderTexture(name, 50,50);
        renderTexture(play,SCREEN_WIDTH/2-100,SCREEN_HEIGHT*0.5);
        presentScene();
    }

    void render(Tictactoe& game) {
        // prepareScene: có thể xóa màn hình hoặc vẽ hình nền, hoặc không gì cả, tùy
        renderTexture(background,0,0);

        // vẽ game board
        renderTexture(replay, 520, 5);
        renderTexture(home, 560 ,5);
        renderTexture(t1, 5,5);
        if(game.turn%2) renderTexture(cellX,150,12);
        else renderTexture(cellO, 150, 12);
        if(game.End)
        {
            renderTexture(t2,200,5);
        }
        for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++) {
            int x = BOARD_X + j * CELL_SIZE;
            int y = BOARD_Y + i * CELL_SIZE;
            switch (game.board[i][j]) {
                case EMPTY_CELL: renderTexture(cellEmpty, x, y); break;
                case X_CELL: renderTexture(cellX, x, y); break;
                case O_CELL: renderTexture(cellO, x, y); break;
            };
        }
        presentScene();
    }

};

#endif

