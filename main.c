#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "SDL_ttf.h"
#include <time.h>
#include <math.h>
/* //Four traffic lines by 145 px
1. main car algorithm for moving(check)
2. graphics for moving background(probably the opposite cars are going to be part of the background);
3. random opposite cars placement (1/5 check)
4. collision of the main car and the opposite cars
5. game start, restart, users input on that
6. game over screen
7. road boarder collision (check)
8. Add screen boarder collision to the collisionDetection_BOARDERS function. (check)
*/
typedef struct Car{

    float x,x1; // x,y - bottom cube , x1,y1 - top cube
    float y,y1;
}CAR;
//======================================================================
typedef struct Boarder{
    float ox,oy,h,w; //boarder x and y coordinates and height and width

}BOARDER;
typedef struct Traff_car{

    float x, y;

}TRAFF;
//======================================================================
typedef struct Gamestate{
    CAR car;
    TRAFF trafficMove, trafficMove2, trafficMove3, trafficMove4;
    SDL_Texture *traf_car, *traf_car2, *traf_car3, *traf_car4;
    SDL_Texture *m_car;
    BOARDER boarder;

}GAMESTATE;
//=======================================================================
void renderCars(SDL_Renderer *render, GAMESTATE *game5, SDL_Surface *a1, SDL_Surface *a2, SDL_Surface *a3,  SDL_Surface *a4);
void renderBackground(GAMESTATE *game_back, SDL_Renderer *ren_back, SDL_Surface *back_ground);
void start_screen(GAMESTATE *game_screen);
void how_to_play(GAMESTATE *game_how);
int gameOver(GAMESTATE *game_over);
void Seq_1(GAMESTATE *game_s1);
void Seq_2(GAMESTATE *game_s2);
void road_3(GAMESTATE *game7);
void Seq_3(GAMESTATE *game_s3);
void Seq_4(GAMESTATE *game_s4);
int ProcessEvents(SDL_Window *w, GAMESTATE *game1);
void Windowshow(SDL_Renderer *r, GAMESTATE *game2, BOARDER *ledge);
void collisionDetection_BOARDERS(GAMESTATE *game3, BOARDER *led);
int collisionDetection_TrafficCARS(GAMESTATE *game_c);
//=======================================================================
int main(int argc, char *argv[])
{
    int number, num = 0, num2 = 0;
    srand(time(NULL));
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *traf_carSurface = NULL, *mainCar = NULL, *traf_carSurface2 = NULL, *traf_carSurface3 = NULL, *traf_carSurface4 = NULL;
    GAMESTATE games;
    BOARDER *ptr =(BOARDER *)malloc(sizeof(BOARDER) * 2);
//==========================================================
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Initialization failed;"); //initializes the SDL and the checks if nothing went wrong
    }
    //create the window SDL_CreateWindow(name, position_x, position_y, size ,size, flags);
    win = SDL_CreateWindow("CAR Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,700,700,0);
    //crating a renderer that connect this program to the GPU SDL_CreateRenderer(pass the window, index, how GPU is connected to this);
    renderer = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//========================================================
    start_screen(&games);
//========================================================
while(1)//this loop is responsible for restart game option
{
    games.trafficMove.x = 190;
    games.trafficMove.y = 86;
    games.trafficMove2.x = 313;
    games.trafficMove2.y = -300;
    games.trafficMove3.x = 434;
    games.trafficMove3.y = -150;
    games.trafficMove4.x = 434;
    games.trafficMove4.y = -457;
//==========================================================
    games.car.x = 330;
    games.car.y = 633;              //the main cars coordinates
//==========================================================
//==========================================================
    (*ptr).ox = 133;
    (*ptr).oy = 0;                 // the left boarder
    (*ptr).w = 20;
    (*ptr).h = 700;

    (ptr + 1)->ox = 547;
    (ptr + 1)->oy = 0;
    (ptr + 1)->w = 20;            // the right boarder
    (ptr + 1)->h = 700;
//==========================================================
    int done = 0; //the flag amount

    renderCars(renderer, &games, traf_carSurface, traf_carSurface2, traf_carSurface3, traf_carSurface4);

    mainCar = IMG_Load("car2.png");
    if(mainCar == NULL)
    {
        printf("The picture is not found;");
        SDL_Quit();
        return 1;
    }
    games.m_car = SDL_CreateTextureFromSurface(renderer, mainCar);
    SDL_FreeSurface(mainCar);

  // a loop for processing event
    while(!done)
    {
        done = ProcessEvents(win, &games);
        Windowshow(renderer, &games, ptr);
        collisionDetection_BOARDERS(&games, ptr);
        num = collisionDetection_TrafficCARS(&games);
        if(num > 0)
        {
            break;
        }

        games.trafficMove.y = games.trafficMove.y + 3;
        games.trafficMove2.y = games.trafficMove2.y + 3;
        games.trafficMove3.y = games.trafficMove3.y + 3;
        games.trafficMove4.y = games.trafficMove4.y + 3;

        if((games.trafficMove.y > 900 && games.trafficMove2.y > 1350) || (games.trafficMove2.y > 900 && games.trafficMove.y > 1350))
        {
            number = 1 + rand() % 4;
            switch(number)
            {
                case 1:
                    Seq_1(&games);
                    break;

                case 2:
                    Seq_2(&games);
                    break;

                case 3:
                    Seq_3(&games);
                    break;

                case 4:
                    Seq_4(&games);
            }

        }
        if(games.trafficMove3.y > 1000)
        {
            road_3(&games);
        }

    }
    num2 = gameOver(&games);
    if(num2 == 2) // if gameover returns two the the player wants to quit the game if it returns 1 then the player wants to restart the game
    {
        break;
    }
}
    // clean the window and the renderer
    SDL_DestroyWindow(win);
    SDL_DestroyTexture(games.traf_car);
    SDL_DestroyTexture(games.traf_car2);
    SDL_DestroyTexture(games.traf_car3);
    SDL_DestroyTexture(games.traf_car4);
    SDL_DestroyRenderer(renderer);
    //clean the font
    TTF_Quit();
    //clean the screen and window
    SDL_Quit();
    free(ptr);
    return 0;
}
void start_screen(GAMESTATE *game_screen)
{
    TTF_Init(); //open the font system
    SDL_Window *w = NULL, *w2 = NULL;
    SDL_Renderer *R = NULL, *R2 = NULL;
    int screen = 0;
    w = SDL_CreateWindow("Start",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,700,700,0);
    R = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Event scr;
    SDL_Surface *ground;
    ground = IMG_Load("bc.jpg");
    if(ground == NULL)
    {
        printf("The picture is not found;");
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *Gr;
    //===================================================
    //background
    Gr = SDL_CreateTextureFromSurface(R, ground);
    SDL_FreeSurface(ground);
    SDL_Rect G = {0,0,800,700};
    SDL_RenderCopy(R, Gr, NULL, &G);
    //===================================================
    SDL_Color black = {0,0,0};
    TTF_Font *font = TTF_OpenFont("Alex_Bold.ttf", 48);
    SDL_Surface *textSurf, *textSurf2, *textSurf3, *textSurf4, *textSurf5, *textSurf6, *textSurf7;

    // the game title
    textSurf = TTF_RenderText_Solid(font, "The car go brum brum", black);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(R, textSurf);
    SDL_Rect textbox = {90, 70, 500, 70};
    SDL_FreeSurface(textSurf);
    SDL_RenderCopy(R, textTexture, NULL, &textbox);

    // the principal
    textSurf2 = TTF_RenderText_Blended(font, "The old arcade type game, where you try not to crash", black);
    SDL_Texture *textTexture2 = SDL_CreateTextureFromSurface(R, textSurf2);
    SDL_Rect textbox2 = {80, 140, 520, 40};
    SDL_FreeSurface(textSurf2);
    SDL_RenderCopy(R, textTexture2, NULL, &textbox2);

    //to continue
    textSurf3 = TTF_RenderText_Blended(font,"*To continue press any key, on the keyboard", black);
    SDL_Texture *textTexture3 = SDL_CreateTextureFromSurface(R, textSurf3);
    SDL_Rect textbox3 = {90, 480, 500, 40};
    SDL_FreeSurface(textSurf3);
    SDL_RenderCopy(R, textTexture3, NULL, &textbox3);

    SDL_RenderPresent(R);
    while(!screen)
    {
        while(SDL_PollEvent(&scr)) //will loop till there will be no more pending events to be processed
        {
            switch(scr.type)
            {
                case SDL_QUIT:
                ++screen;
                break;
                case SDL_WINDOWEVENT_CLOSE:
                {
                    if(w)
                    {
                        SDL_DestroyWindow(w);
                        w = NULL;             //will close the window if X is pressed on the window
                        ++screen;
                    }
                }
                break;
                case SDL_KEYDOWN:
                {
                    ++screen;

                }
                break;
            }
        }
    }
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(textTexture2);
    SDL_DestroyTexture(textTexture3);
    SDL_DestroyWindow(w);
    SDL_DestroyRenderer(R);
//=====================================================================================================
//how to play screen
    w2 = SDL_CreateWindow("How to play",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,700,700,0);
    R2 = SDL_CreateRenderer(w2,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(R2, 0, 0, 0, 250);
    SDL_RenderClear(R2);

    screen = 0;
    SDL_Color white = {255, 255, 255};

    //Title
    textSurf4 = TTF_RenderText_Blended(font,"*How to play:", white);
    SDL_Texture *textTexture4 = SDL_CreateTextureFromSurface(R2, textSurf4);
    SDL_Rect textbox4 = {270, 70, 160, 50};
    SDL_FreeSurface(textSurf4);
    SDL_RenderCopy(R2, textTexture4, NULL, &textbox4);

    //Game mechanics
    textSurf5 = TTF_RenderText_Blended(font,"*Move the players(main) red car with the arrow keys", white);
    SDL_Texture *textTexture5 = SDL_CreateTextureFromSurface(R2, textSurf5);
    SDL_Rect textbox5 = {80, 180, 520 , 50};
    SDL_FreeSurface(textSurf5);
    SDL_RenderCopy(R2, textTexture5, NULL, &textbox5);

    textSurf6 = TTF_RenderText_Blended(font,"*try not to crash into the oncoming traffic", white);
    SDL_Texture *textTexture6 = SDL_CreateTextureFromSurface(R2, textSurf6);
    SDL_Rect textbox6 = {80, 240, 480 , 50};
    SDL_FreeSurface(textSurf5);
    SDL_RenderCopy(R2, textTexture6, NULL, &textbox6);

    //Get to game window
    textSurf7 = TTF_RenderText_Blended(font,"*To continue press any key, on the keyboard", white);
    SDL_Texture *textTexture7 = SDL_CreateTextureFromSurface(R2, textSurf7);
    SDL_Rect textbox7 = {130, 650, 420, 20};
    SDL_FreeSurface(textSurf7);
    SDL_RenderCopy(R2, textTexture7, NULL, &textbox7);

    SDL_RenderPresent(R2);
    while(!screen)
    {
        while(SDL_PollEvent(&scr)) //will loop till there will be no more pending events to be processed
        {
            switch(scr.type)
            {
                case SDL_QUIT:
                ++screen;
                break;
                case SDL_WINDOWEVENT_CLOSE:
                {
                    if(w2)
                    {
                        SDL_DestroyWindow(w2);
                        w2 = NULL;             //will close the window if X is pressed on the window
                        ++screen;
                    }
                }
                break;
                case SDL_KEYDOWN:
                {
                    ++screen;

                }
                break;
            }
        }
    }
    SDL_DestroyWindow(w2);
    SDL_DestroyRenderer(R2);
    TTF_CloseFont(font);
    SDL_DestroyTexture(textTexture4);
    SDL_DestroyTexture(textTexture5);
    SDL_DestroyTexture(textTexture6);
    SDL_DestroyTexture(textTexture7);
}
int gameOver(GAMESTATE *game_over)
{
    SDL_Window *w3 = NULL;
    SDL_Renderer *R3 = NULL;
    TTF_Font *font = TTF_OpenFont("Alex_Bold.ttf", 48);
    SDL_Surface *text1, *text2, *text3;
    w3 = SDL_CreateWindow("Game over",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,700,700,0);
    R3 = SDL_CreateRenderer(w3,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    int restart = 0;
    SDL_Event res;
    SDL_Color white = {255, 255, 255};

    SDL_SetRenderDrawColor(R3, 0, 0, 0, 255);
    SDL_RenderClear(R3);
    //Game Over text
    text1 = TTF_RenderText_Solid(font, "Game over", white);
    SDL_Texture *texture11 = SDL_CreateTextureFromSurface(R3, text1);
    SDL_Rect textbox = {250, 70, 200, 80};
    SDL_FreeSurface(text1);
    SDL_RenderCopy(R3, texture11, NULL, &textbox);

    text2 = TTF_RenderText_Solid(font, "*To play again press any button on the keyboard", white);
    SDL_Texture *texture22 = SDL_CreateTextureFromSurface(R3, text2);
    SDL_Rect textbox2 = {90, 150, 520, 50};
    SDL_FreeSurface(text2);
    SDL_RenderCopy(R3, texture22, NULL, &textbox2);

    text3 = TTF_RenderText_Solid(font, "*Press an Escape button on the keyboard to quit", white);
    SDL_Texture *texture33 = SDL_CreateTextureFromSurface(R3, text3);
    SDL_Rect textbox3 = {90, 600, 480, 50};
    SDL_FreeSurface(text3);
    SDL_RenderCopy(R3, texture33, NULL, &textbox3);


    SDL_RenderPresent(R3);
    while(!restart)
    {
        while(SDL_PollEvent(&res))
        {
            switch(res.type)
            {
                case SDL_KEYDOWN:
                {
                    ++restart;
                    switch(res.key.keysym.sym)
                    {
                        case SDLK_ESCAPE: // closes if you press the escape button
                        {
                            ++restart;
                        }
                        break;
                    }
                }break;

            }
        }
    }
    SDL_DestroyWindow(w3);
    SDL_DestroyRenderer(R3);
    SDL_DestroyTexture(texture11);
    SDL_DestroyTexture(texture22);
    SDL_DestroyTexture(texture33);
    TTF_CloseFont(font);
    return restart;

}
void renderCars(SDL_Renderer *render, GAMESTATE *game5, SDL_Surface *a1, SDL_Surface *a2, SDL_Surface *a3, SDL_Surface *a4)
{
//=====================================================================
    //create an image  and render it
    a1 = IMG_Load("R2.png");
    //Validation
    if(a1 == NULL)
    {
        printf("The picture is not found;");
        SDL_Quit();
        exit(1);
    }

    //rendering the texture
    game5->traf_car = SDL_CreateTextureFromSurface(render, a1);
    SDL_FreeSurface(a1);
//=====================================================================
    //create an image  and render it
    a2 = IMG_Load("R2.png");
    //Validation
    if(a2 == NULL)
    {
        printf("The picture is not found;");
        SDL_Quit();
        exit(1);
    }

    //rendering the texture
    game5->traf_car2 = SDL_CreateTextureFromSurface(render, a2);
    SDL_FreeSurface(a2);
//=====================================================================
    //create an image  and render it
    a3 = IMG_Load("R2.png");
    //Validation
    if(a3 == NULL)
    {
        printf("The picture is not found;");
        SDL_Quit();
        exit(1);
    }

    //rendering the texture
    game5->traf_car3 = SDL_CreateTextureFromSurface(render, a3);
    SDL_FreeSurface(a3);
//=====================================================================
    a4 = IMG_Load("R2.png");
    //Validation
    if(a4 == NULL)
    {
        printf("The picture is not found;");
        SDL_Quit();
        exit(1);
    }

    //rendering the texture
    game5->traf_car4 = SDL_CreateTextureFromSurface(render, a4);
    SDL_FreeSurface(a4);
}
void Seq_1(GAMESTATE *game_s1)
{
    game_s1->trafficMove.y = -100;
    game_s1->trafficMove2.y = -500;
}
void Seq_2(GAMESTATE *game_s2)
{
    game_s2->trafficMove.y = -500;
    game_s2->trafficMove2.y = -100;
}
void Seq_3(GAMESTATE *game_s3)
{
    game_s3->trafficMove.y = -500;
    game_s3->trafficMove4.y = -670;
    game_s3->trafficMove4.x = 190;
    game_s3->trafficMove2.y = -100;
}
void Seq_4(GAMESTATE *game_s4)
{
    game_s4->trafficMove.y = -100;
    game_s4->trafficMove4.y = -670;
    game_s4->trafficMove4.x = 313;
    game_s4->trafficMove2.y = -500;
}
void road_3(GAMESTATE *game7)
{
    game7->trafficMove3.y = -1000 + rand() % 1012;
}
int collisionDetection_TrafficCARS(GAMESTATE *game_c)
{
    int crash = 0;
//first car
    if(game_c->car.y + 120 > game_c->trafficMove.y && game_c->car.y < game_c->trafficMove.y + 120)
    {
        if(game_c->car.x + 80 > game_c->trafficMove.x + 80 && game_c->car.x < game_c->trafficMove.x + 80)
        {
             ++crash;
        }
    }
    if(game_c->car.x + 80 > game_c->trafficMove.x && game_c->car.x < game_c->trafficMove.x + 80)
    {
        if(game_c->car.y < game_c->trafficMove.y + 120 && game_c->car.y >  game_c->trafficMove.y)
        {
            ++crash;
        }
    }
//===================================================================================================
//second car
    if(game_c->car.y + 120 > game_c->trafficMove2.y && game_c->car.y < game_c->trafficMove2.y + 120)
    {
        if(game_c->car.x + 80 > game_c->trafficMove2.x + 80 && game_c->car.x < game_c->trafficMove2.x + 80)
        {
             ++crash;
        }
        else if(game_c->car.x + 80 > game_c->trafficMove2.x && game_c->car.x < game_c->trafficMove2.x)
        {
            ++crash;
        }
    }
    if(game_c->car.x + 80 > game_c->trafficMove2.x && game_c->car.x < game_c->trafficMove2.x + 80)
    {
        if(game_c->car.y < game_c->trafficMove2.y + 120 && game_c->car.y >  game_c->trafficMove2.y)
        {
            ++crash;
        }
    }
//==================================================================================================
//third car
    if(game_c->car.y + 120 > game_c->trafficMove3.y && game_c->car.y < game_c->trafficMove3.y + 120)
    {
        if(game_c->car.x + 80 > game_c->trafficMove3.x && game_c->car.x < game_c->trafficMove3.x)
        {
            ++crash;
        }
    }
    if(game_c->car.x + 80 > game_c->trafficMove3.x && game_c->car.x < game_c->trafficMove3.x + 80)
    {
        if(game_c->car.y < game_c->trafficMove3.y + 120 && game_c->car.y >  game_c->trafficMove3.y)
        {
            ++crash;
        }
    }
//==================================================================================================
//fourth car
    if(game_c->car.y + 120 > game_c->trafficMove4.y && game_c->car.y < game_c->trafficMove4.y + 120)
    {
        if(game_c->car.x + 80 > game_c->trafficMove4.x && game_c->car.x < game_c->trafficMove4.x)
        {
            ++crash;
        }
        else if(game_c->car.x + 80 > game_c->trafficMove4.x && game_c->car.x < game_c->trafficMove4.x)
        {
            ++crash;
        }
    }
    if(game_c->car.x + 80 > game_c->trafficMove4.x && game_c->car.x < game_c->trafficMove4.x + 80)
    {
        if(game_c->car.y < game_c->trafficMove4.y + 120 && game_c->car.y >  game_c->trafficMove4.y)
        {
            ++crash;
        }
    }

 return crash;
}
int ProcessEvents(SDL_Window *w, GAMESTATE *game1)
{
    int flag = 0;
    SDL_Event event;
    while(SDL_PollEvent(&event)) //will loop till there will be no more pending events to be processed
        {
            switch(event.type)
            {
                case SDL_QUIT:
                flag = 1;
                break;
                case SDL_WINDOWEVENT_CLOSE:
                {
                    if(w)
                    {
                        SDL_DestroyWindow(w);
                        w = NULL;             //will close the window if X is pressed on the window
                        flag = 1;
                    }
                }
                break;
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE: // closes if you press the escape button
                        {
                            flag = 1;
                        }
                        break;
                    }
                }break;
            }
        }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_UP])
    {
         game1->car.y -= 4; //moves the car to the left
         game1->car.y1 -= 4;
    }
    if(state[SDL_SCANCODE_DOWN])
    {
         game1->car.y += 4; //moves the car to the left
         game1->car.y1 += 4;
    }
    if(state[SDL_SCANCODE_LEFT])
    {
         game1->car.x -= 4; //moves the car to the left
         game1->car.x1 -= 4;
    }
    if(state[SDL_SCANCODE_RIGHT])
    {
        game1->car.x += 4;//moves the car to the right
        game1->car.x1 += 4;
    }

    return flag;
}
void Windowshow(SDL_Renderer *r, GAMESTATE *game2, BOARDER *ledge)
{
    // set the color of the window SDL_SetRenderDrawColor(renderer, red, green, blue, opacity); they are bytes
    SDL_SetRenderDrawColor(r,70, 70, 70, 250);
    //clear screen and set to the color mentioned above  SDL_RenderClear(pointer to the renderer);
    SDL_RenderClear(r);

    SDL_Rect main_rect = {game2->car.x,game2->car.y,90,140};
    SDL_RenderCopy(r, game2->m_car, NULL, &main_rect);
    //present on the screen SDL_RenderPresent(rendering pointer);
    SDL_SetRenderDrawColor(r,250,250,250,250);
    //will draw a line
    SDL_Rect lineRec = {(ledge)->ox,(ledge)->oy,(ledge)->w,(ledge)->h};  // lineRec = {Ox, Oy, width, height}
    //will fill the line with a color in the rendering target
    SDL_RenderFillRect(r, &lineRec);
    //will draw a line
    SDL_Rect lineRec2 = {(ledge + 1)->ox,(ledge + 1)->oy,(ledge + 1)->w,(ledge + 1)->h};
     //will fill the line with a color in the rendering target
    SDL_RenderFillRect(r, &lineRec2);
    //1 enemy car
    SDL_Rect traf_rect = {game2->trafficMove.x,game2->trafficMove.y,90,140};
    SDL_RenderCopy(r, game2->traf_car, NULL, &traf_rect);
    //2 enemy car
    SDL_Rect traf_rect2 = {game2->trafficMove2.x,game2->trafficMove2.y,90,140};
    SDL_RenderCopy(r, game2->traf_car2, NULL, &traf_rect2);
    //3 enemy car
    SDL_Rect traf_rect3 = {game2->trafficMove3.x,game2->trafficMove3.y,90,140};
    SDL_RenderCopy(r, game2->traf_car3, NULL, &traf_rect3);
    //4 enemy car
    SDL_Rect traf_rect4 = {game2->trafficMove4.x,game2->trafficMove4.y,90,140};
    SDL_RenderCopy(r, game2->traf_car4, NULL, &traf_rect4);

    SDL_RenderPresent(r);
}
void collisionDetection_BOARDERS(GAMESTATE *game3, BOARDER *led) // +0 left ,+1 right
{
    if((*game3).car.y + 140 > (*led).oy && (*game3).car.y <  (*led).oy + (*led).h)
    {
        //is touching the right side
        if((*game3).car.x + 90 > (*led).ox + (*led).w && (*game3).car.x <  (*led).ox + (*led).w)
        {
            //correct the x
            (*game3).car.x = (*led).ox + (*led).w;
        }
        //is touching the left side
        else if((*game3).car.x + 90 > (*(led + 1)).ox && (*game3).car.x < (*(led + 1)).ox)
        {
             (*game3).car.x = (*(led + 1)).ox - 90;
        }
    }
    if((*game3).car.x + 90 > 0 && (*game3).car.x < 700)
    {
        //touching the top of the screen
        if((*game3).car.y < 0)
        {
            (*game3).car.y = 1;

        }
        //touching the bottom of the screen
        else if((*game3).car.y + 140 > 700 && (*game3).car.y < 700)
        {
            (*game3).car.y = 550;
        }
    }

}





//nice

