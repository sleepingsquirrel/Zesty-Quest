// On linux compile with:
// g++ -std=c++17 main.cpp -o prog -lSDL2 -ldl
// On windows compile with (if using mingw)
// g++ main.cpp -o prog.exe -lmingw32 -lSDL2main -lSDL2
// On Mac compile with:
// clang++ main.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2

// C++ Standard Libraries
#include <iostream>
// Third Party
#include <SDL2/SDL.h> // For Mac, use <SDL.h>
#include <cstdlib>
#include <ctime>
#include <list>
#include <cmath>
#include <map>

#define WIDTH 1000
#define HEIGHT 1000
#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080
#define FPS_INTERVAL 0.2 //seconds.
#define DEBUG false
#define PI 3.1415926
using namespace std;

SDL_Renderer* renderer;
SDL_Window* window;
SDL_DisplayMode dm;

float dt = 0;
float fps_current = 60;


bool keys[322];

float zoom = 1.9999;
float cam_x = 0.0;
float cam_y = 0.0;
float cam_speed = 1000.0;

int frame = 0;
char *scene = "thejouney";

bool isFullScreen = true;



struct colour{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

colour pixelmap[MAX_HEIGHT][MAX_WIDTH];



void debug(string message){
    if (DEBUG){
        cout << message << "\n";
    }
}


void init(){

}


void draw() {
    for(int x = 0; x < dm.w; x++){
        for(int y = 0; y < dm.h; y++){
            pixelmap[y][x].r = rand() % 255;
        }
    }
}



void physics(){

}



void update(){

    draw();


    physics();

}



SDL_Texture* change_texture(uint8_t* data){
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, MAX_WIDTH, MAX_HEIGHT, 8 * 3, MAX_WIDTH * 3, 0x0000FF, 0x00FF00, 0xFF0000,0);
    if (!surface){
        cout << "oopsie\n";
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    if (!texture){
        cout << "oopsie\n";
    }
    return texture;
}

void cutscene(){
    SDL_Surface * IMG_Load(scene);
}


int main(int argc, char* argv[]){
    srand(time(nullptr));
    init();
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        cout << "SDL could not be initialized: " <<
                  SDL_GetError();
    }else{
        cout << "SDL video system is ready to go\n";
    }
    for(int i = 0; i < 322; i++) { // init them all to false
        keys[i] = false;
    }
    //SDL_EnableKeyRepeat(0,0);
    window = SDL_CreateWindow("Bad Chemistry Simulator",20, 20, WIDTH,HEIGHT,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    cout << (int) SDLK_F11 << "\n";


    SDL_Texture* texture = change_texture(&(pixelmap[0][0].r));


    // Create a rectangle
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = MAX_WIDTH;
    rectangle.h = MAX_HEIGHT;


    // Infinite loop for our application
    bool gameIsRunning = true;
    int start;
    int frames = 0;
    int fper = 10;
    start = SDL_GetTicks();
    // Main application loop
    while(gameIsRunning){
        frames++;
        SDL_Event event;
        if (SDL_GetDesktopDisplayMode(0, &dm))
        {
            printf("Error getting desktop display mode\n");
            return -1;
        }
        dm.w = SDL_GetWindowSurface(window)->w;
        dm.h = SDL_GetWindowSurface(window)->h;

        // (1) Handle Input
        // Start our event loop
        while(SDL_PollEvent(&event)){
            // Handle each specific event
            switch( event.type ){
                case SDL_QUIT:
                    gameIsRunning = false;

                case SDL_KEYDOWN:
                    if(event.key.keysym.sym > 322){
                        break;
                    }
                    keys[event.key.keysym.sym] = true;
                    break;

                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_F11){
                        isFullScreen = !isFullScreen;
                        if (isFullScreen){
                            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        }
                        else{
                            SDL_SetWindowFullscreen(window, 0);
                        }
                        break;
                    }

                    if(event.key.keysym.sym > 322){
                        break;
                    }
                    keys[event.key.keysym.sym] = false;

                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_X1) {
                        cout << "banana" << "\n";
                    }

                    break;
                default:
                    break;
            }
        }
        if (keys[SDLK_w]){
            cam_y += (cam_speed)/fps_current;

        }
        if (keys[SDLK_s]){
            cam_y -= (cam_speed)/fps_current;
        }
        if (keys[SDLK_a]){
            cam_x += (cam_speed)/fps_current;
        }
        if (keys[SDLK_d]){
            cam_x -= (cam_speed)/fps_current;
        }
        if (keys[SDLK_f])
        {
            zoom += 10.1/fps_current;
        }
        if (keys[SDLK_r])
        {
            zoom -= 10.1/fps_current;
        }
        update();

        SDL_DestroyTexture(texture);
        texture = change_texture(&(pixelmap[0][0].r));
        // SDL_SetRenderDrawColor(renderer,255,2,255,SDL_ALPHA_OPAQUE);
        SDL_RenderCopy(renderer,texture,NULL,&rectangle);
        SDL_RenderPresent(renderer);

        if(frames % fper == 0){
            dt = (float)(SDL_GetTicks() - start) / 1000.0;
            fps_current = (float)fper/dt;
            cout << fps_current << " " <<  (int)SDL_GetTicks() - start << "\n";
            start = SDL_GetTicks();

        }

    }

    SDL_DestroyTexture(texture);
    // We destroy our window. We are passing in the pointer
    // that points to the memory allocated by the
    // 'SDL_CreateWindow' function. Remember, this is
    // a 'C-style' API, we don't have destructors.
    SDL_DestroyWindow(window);

    // our program.
    SDL_Quit();
    return 0;
}
