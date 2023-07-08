#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SCREEN_W 1280
#define SCREEN_H 1024

struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

typedef struct {
    SDL_Rect rect;
    bool is_set;
} Cell;

#define BOARD_W 60
#define BOARD_H 60

int border_size = 20;

int border_y = 20;
int border_x = 20;


Cell board[BOARD_H * BOARD_W];

SDL_Rect play_button;
SDL_Rect reset_button;

bool use_logic = false;
    
        


void init_SDL(){

    int render_flags, window_flags;
    render_flags = SDL_RENDERER_ACCELERATED;
    window_flags = 0;


    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }

    App.window = SDL_CreateWindow("Game of life", 100, 100, SCREEN_W, SCREEN_H, window_flags);

    if(!App.window){
        printf("Error creating SDL window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    App.renderer = SDL_CreateRenderer(App.window, -1, render_flags);

    if(!App.renderer){
        printf("Error creating SDL renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void input(){
    SDL_Event event;
    SDL_Point mouse_position;

   
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                exit(1);
                break;
            
            case SDL_MOUSEBUTTONDOWN:

                SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
            
                for (int i = 0; i < BOARD_H; i++) {
                    for (int j = 0; j < BOARD_W; j++) {  

                        if (SDL_PointInRect(&mouse_position, &board[i * BOARD_W + j].rect)) {
                        
                            board[i*BOARD_W + j].is_set = true;
                        }
                    }
                }

                if(SDL_PointInRect(&mouse_position, &play_button)){
                    use_logic = !use_logic;
                    if(use_logic)
                        printf("PLAYING\n");
                    
                    else
                        printf("STOPPED\n");
                }
            
                if(SDL_PointInRect(&mouse_position, &reset_button)){
                    for(int k=0; k < BOARD_H * BOARD_W; k++)
                        board[k].is_set = false;
                }


                break;
            
            default:
                break;
        }
    }
}

void render_scene(){

    SDL_SetRenderDrawColor(App.renderer, 0, 0, 0, 255);

    for(int i = 0;i < BOARD_H * BOARD_W; i++){
        if(board[i].is_set){
            SDL_SetRenderDrawColor(App.renderer, 100,0,0, 255);
            SDL_RenderFillRect(App.renderer, &board[i].rect);
        }
        else{
            SDL_SetRenderDrawColor(App.renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(App.renderer, &board[i].rect);
        } 

    }

  
    SDL_SetRenderDrawColor(App.renderer, 50, 100, 100, 255);
    SDL_RenderFillRect(App.renderer, &play_button);
    SDL_RenderFillRect(App.renderer, &reset_button);

    SDL_RenderPresent(App.renderer);
}

int check_neighbours(int x, int y){
    int count = 0;
    if(board[(y-1)*BOARD_W + x-1].is_set)
        count++;

    if(board[(y-1)*BOARD_W + x].is_set)
        count++;

    if(board[(y-1)*BOARD_W + x+1].is_set)
        count++;

    if(board[(y)*BOARD_W + x-1].is_set)
        count++;

    if(board[(y)*BOARD_W + x+1].is_set)
        count++;

    if(board[(y+1)*BOARD_W + x-1].is_set)
        count++;

    if(board[(y+1)*BOARD_W + x].is_set)
        count++;

    if(board[(y+1)*BOARD_W + x+1].is_set)
        count++;


    return count;    
}

void GoL_logic(){

    Cell copy_board[BOARD_H * BOARD_W];

    memcpy(copy_board, board, sizeof(board));

    int index = -1;
    for(int y = 0; y < BOARD_H; y++){
        for(int x = 0; x < BOARD_W; x++){
            int set_neighs = check_neighbours(x, y);
            index = y*BOARD_W + x;
            if(board[index].is_set){
                if(set_neighs == 2 || set_neighs == 3){
                    //ok
                }
                else
                    copy_board[index].is_set = false;
            }
            else {
                if(set_neighs == 3)
                    copy_board[index].is_set = true;
            }
        }
    }

    memcpy(board, copy_board, sizeof(board));
}

int main(int argc, char* argv[]){
    
    memset(&App, 0, sizeof(App));

    init_SDL();


    play_button.h = 50;
    play_button.w = 100;
    play_button.y = 200;
    play_button.x = 600;

    reset_button.h = 50;
    reset_button.w = 100;
    reset_button.y = 300;
    reset_button.x = 600;


    for(int i=0;i<BOARD_H;i++)
        for(int j=0;j<BOARD_W;j++){
            board[i * BOARD_W + j].rect = (SDL_Rect){.x=i*25, .y=j*25, .w=25, .h=25};
            board[i * BOARD_W + j].is_set = false;
        }

    while(1){

        SDL_SetRenderDrawColor(App.renderer, 96, 128, 255, 255);
        SDL_RenderClear(App.renderer);

        input();

        render_scene();

        if(use_logic)
            GoL_logic();

        SDL_Delay(50);
    }

    return 0;
}