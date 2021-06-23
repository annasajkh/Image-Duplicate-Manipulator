
#include <stdio.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600

static size_t image_count_x = 1;
static size_t image_count_y = 1;


int main (int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("TestSDL2", 
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 
                                          SCREEN_SIZE_X, 
                                          SCREEN_SIZE_Y, 
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, 
                                                -1, 
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture *texture = IMG_LoadTexture(renderer,"image.jpg");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(1);

    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool quit = false;

    while (!quit)
    {
        SDL_Event windowEvent;

        bool q_pressed;
        bool w_pressed;

        while (SDL_PollEvent(&windowEvent))
        {
            switch (windowEvent.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                
                case SDL_KEYDOWN:
                    if(windowEvent.key.keysym.sym == SDLK_q)
                    {
                        q_pressed = true;
                    }
                    else if(windowEvent.key.keysym.sym == SDLK_w)
                    {
                        w_pressed = true;
                    }

                    break;

                case SDL_KEYUP:
                    if(windowEvent.key.keysym.sym == SDLK_q)
                    {
                        q_pressed = false;
                    }
                    else if(windowEvent.key.keysym.sym == SDLK_w)
                    {
                        w_pressed = false;
                    }

                    break;
                
                case SDL_MOUSEWHEEL:
                    if(w_pressed)
                        image_count_x -= windowEvent.wheel.y;
                    
                    if(q_pressed)
                        image_count_y -= windowEvent.wheel.y;

                    image_count_x = image_count_x < 1 ? 1 : image_count_x;
                    image_count_y = image_count_y < 1 ? 1 : image_count_y;

                    break;
                
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        size_t x_percent = SCREEN_SIZE_X / image_count_x;
        size_t y_percent = SCREEN_SIZE_Y / image_count_y;

        SDL_Rect* dst = new SDL_Rect;

        for (size_t i = 0; i < image_count_x; i++)
        {
            for (size_t j = 0; j < image_count_y; j++)
            {
                dst->x = i * x_percent;
                dst->y = j * y_percent;
                dst->w = x_percent;
                dst->h = y_percent;

                SDL_RenderCopy(renderer, texture, NULL, dst);
            }
            
        }
        
        SDL_RenderPresent(renderer);

        delete dst;
    }

    //clean up
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
}