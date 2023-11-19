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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
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
SDL_AudioSpec audioSpec;
Mix_Music *music;
int device = 0;
float dt = 0;
float fps_current = 60;
int fper = 10;

bool keys[322];

float zoom = 1.9999;
float cam_x = 0.0;
float cam_y = 0.0;
float cam_speed = 1000.0;

bool in_cutscene = false;
int frame = 0;
float frame_time = 1.0;
SDL_Surface *current_frame;
string scene = "cutscenes/out/GOOMZONE/";
string scene1 = "cutscenes/out/thejouney/";
string scene2 = "cutscenes/out/uap/";
string scene3 = "cutscenes/out/TELEVISION/";


bool isFullScreen = true;



struct colour{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

colour pixelmap[MAX_HEIGHT][MAX_WIDTH];

void end_scene();

struct player{
    float x;
    float y;
    float r;
    float speed;
} p = {100,100,100,3};
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
            pixelmap[y][x].r = 0;
        }
    }
    for (int x = -p.r; x < p.r; x++){
        for (int y = -p.r; y < p.r; y++){
            int pxx = y + p.y;
            int pxy = x + p.x;
            if (pxx < 0){
                continue;
            }
            if (pxy < 0)
            {
                continue;
            }
            if (pxx > dm.w){
                continue;
            }
            if (pxy >= dm.h)
            {
                continue;
            }
            pixelmap[pxx][pxy].r = 255;
        }
    }

}



void physics(){

}



void update(){
    debug("drawing");
    draw();

    debug("physics");
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

SDL_Texture* cutscene(){
    frame_time += dt/fper;
    if (frame_time >= 1.0/24.0){
        SDL_FreeSurface(current_frame);
        frame_time = 0.0;
        frame += 1;
        string s = to_string(frame);
        s.insert(0, 4 - s.length(), '0');
        string b = "out-";
        string e = ".jpg";
        string out = scene + b + s + e;
        current_frame = IMG_Load(out.c_str());
        if (!current_frame){
            frame_time = 100.0;
            in_cutscene = false;
            frame = 0;
            end_scene();
        }
    }
    return SDL_CreateTextureFromSurface(renderer,current_frame);
}

void set_scene(string next_scene){
    end_scene();
    device = SDL_OpenAudioDevice(nullptr, 0, &audioSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);
    scene = next_scene;
    in_cutscene = true;
    string a = "sample.wav";
    string out = scene + a;
    cout << "ads" << "\n";
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8 *wav_buffer;
    frame = 0;
    SDL_PauseAudioDevice(device,1);
    /* Load the WAV */
    if (SDL_LoadWAV(out.c_str(), &wav_spec, &wav_buffer, &wav_length) == NULL) {
        fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
    } else {
        /* Do stuff with the WAV data, and then... */
        SDL_PauseAudioDevice(device,0);
        int status = SDL_QueueAudio(device, wav_buffer, wav_length);

        SDL_FreeWAV(wav_buffer);
    }
    // SDL_RWops* rw = SDL_RWFromFile(out.c_str(), "r");
    // printf("Mix_LoadMUS: %s\n", Mix_GetError());
    // music = Mix_LoadMUS_RW(rw,1);
    //
    // printf("Mix_LoadMUS: %s\n", Mix_GetError());
    // cout << out << " " << music << "\n";
    // if(music!=nullptr){//victory.mp3
    //     cout << "aasda\n";
    //     //Mix_PlayChannel(1, music, -1 );
    // }
}

void end_scene(){
    SDL_CloseAudioDevice(device);
    frame = -1;
}

int main(int argc, char* argv[]){
    srand(time(nullptr));
    init();
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0){
        cout << "SDL could not be initialized: " <<
                  SDL_GetError();
    }else{
        cout << "SDL video system is ready to go\n";
    }
    for(int i = 0; i < 322; i++) { // init them all to false
        keys[i] = false;
    }
    audioSpec.freq = 48000;

    // Mix_SetSoundFonts("wario.sf2");
    // if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
    //     std::cout << "Audio library not working:" << Mix_GetError() << std::endl;;
    // }
    // else{
    //     cout << "SDL audio system is ready to go\n";
    // }

    printf("Mix_LoadMUS: %s\n", Mix_GetError());
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
    //set_scene(scene);
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
            //cam_y += (cam_speed)/fps_current;
            p.y -= p.speed;

        }
        if (keys[SDLK_s]){
            //cam_y -= (cam_speed)/fps_current;
            p.y += p.speed;
        }
        if (keys[SDLK_a]){
            //cam_x += (cam_speed)/fps_current;
            p.x -= p.speed;
        }
        if (keys[SDLK_d]){
            //cam_x -= (cam_speed)/fps_current;
            p.x += p.speed;
        }
        if (keys[SDLK_f])
        {
            zoom += 10.1/fps_current;
        }
        if (keys[SDLK_r])
        {
            zoom -= 10.1/fps_current;
        }
        if (keys[SDLK_1])
        {
            set_scene(scene1);
        }
        if (keys[SDLK_2])
        {
            set_scene(scene2);
        }
        if (keys[SDLK_3])
        {
            set_scene(scene3);
        }
        if (keys[SDLK_4])
        {
            end_scene();
        }
        if (!in_cutscene){
            rectangle.x = 0;
            rectangle.y = 0;
            rectangle.w = MAX_WIDTH;
            rectangle.h = MAX_HEIGHT;

            update();
            texture = change_texture(&(pixelmap[0][0].r));
        }


        if (in_cutscene){
            texture = cutscene();
            rectangle.x = 0;
            rectangle.y = 0;
            rectangle.w = dm.w;
            rectangle.h = dm.h;

        }
        // SDL_SetRenderDrawColor(renderer,255,2,255,SDL_ALPHA_OPAQUE);
        SDL_RenderCopy(renderer,texture,NULL,&rectangle);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
        if(frames % fper == 0){
            dt = (float)(SDL_GetTicks() - start) / 1000.0;
            fps_current = (float)fper/dt;
            //cout << fps_current << " " <<  (int)SDL_GetTicks() - start << "\n";
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
