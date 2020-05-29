#include <windows.h>

#include "Print.h"
#include "text.h"


SDL_Window* g_window = NULL;
SDL_Renderer* g_screen = NULL;
SDL_Event g_event;

TTF_Font* font = NULL;
text move_game;
text min_move_game;

// Âm thanh

Mix_Chunk* g_sound_grab;
Mix_Chunk* g_sound_drop;
Mix_Chunk* g_sound_win;
Mix_Music* music;

//Hằng

const int SCREEN_WIDTH = 776; //chiều rộng cửa sổ
const int SCREEN_HEIGHT =545; // chiều cao cửa sổ
const int SCREEN_BPP = 32; //tỉ lệ pixel

const int disk_width = 258; //độ dài ảnh đĩa
const int disk_height = 45; //độ cao ảnh đĩa

int n_disk; //Mức độ game
vector< vector<int> > n_pillar(3, vector<int>(0, 0)); //Khởi tạo vector 2 chiều chỉ 3 cột có 0 đĩa
int click = 0; //số lần click chuột trong 1 lượt
int p_pillar; // Cột click chuột trước đó
int pillar; // Cột vừa mới click chuột
int n_move = 0; // Số lần di chuyển cột
int min_move; // Số lần di chuyển đĩa ít nhất

//Các biến hình ảnh
Print g_background;
Print disk[7];
Print g_start;
Print g_win;
Print g_select;

// Kiểu dữ liệu mới
enum status
{
    menu, playing, complt, quit
};
status stt;

//Nguyên mẫu hàm
int level(SDL_Event event); // Mức độ trò chơi

bool initdata(); // Khởi tạo SDL, SDL_image, cửa sổ, âm thanh...

void set_menu(); // Khởi tạo menu

bool LoadBackground(); //Khởi tạo background

void set_win();

void close(); // xóa hết tất cả những gì đã load lên màn hình

void loaddisk(); // Load hình ảnh các đĩa lên màn hình

void clickmouse(SDL_Event event); // quản lí sự kiện bấm con trỏ chuột

void initgame(); //Khỏi tạo game

void move_disk(); //Cập nhật vị trí các đĩa sau sự kiện bấm chuột

bool win(); //kiểm tra game đã hoàn thành chưa


//Định nghĩa hàm
int main(int argc, char* argv[])
{
    if(n_disk > 7) {return -1;}
    if(initdata() == false)
        return -1;

    if(LoadBackground() == false)
        return -1;

    bool exit = false;
    stt = menu;

    while (!exit)
    {
        if(stt == menu)
            set_menu();
        if(stt == quit)
            exit = true;
        if(stt == complt)
            set_win();
        if(stt == playing) {
            bool is_quit = false;
            while (!is_quit) {
                while(SDL_PollEvent(&g_event) != 0)
                {
                    if (g_event.type == SDL_QUIT)
                    {
                        is_quit = true;
                        exit = true;
                    }
                    clickmouse(g_event);
                }
                if(win()) {
                    stt = complt;
                    is_quit = true;
                    Mix_PlayChannel(-1, g_sound_win, 0);
                }
                if (!Mix_PlayingMusic())
                    Mix_PlayMusic(music, 0);

                SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
                SDL_RenderClear(g_screen);

                g_background.Render(g_screen, NULL);
                for(int i=0; i<n_disk; i++) {
                    disk[i].Render(g_screen,NULL);
                }
                g_select.Render(g_screen, NULL);
                string val1 = to_string(n_move);
                string val2 = to_string(min_move);
                move_game.settext(val1);
                move_game.loadfromrender(font, g_screen);
                move_game.rendertext(g_screen,490,40);
                min_move_game.settext(val2);
                min_move_game.loadfromrender(font, g_screen);
                min_move_game.rendertext(g_screen,490, 13);

                SDL_RenderPresent(g_screen);
            }
        }
    }
    close();
    return 0;
}

bool initdata()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret<0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Tower of Hanoi",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);

    if(g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
    }
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)==-1) {
        success = false;
    }
    g_sound_grab = Mix_LoadWAV("sound//grab.wav");
    g_sound_drop = Mix_LoadWAV("sound//drop.wav");
    g_sound_win = Mix_LoadWAV("sound//win.wav");
    music = Mix_LoadMUS("sound//music.mp3");

     if(g_sound_grab == NULL || g_sound_drop == NULL || g_sound_win==NULL || music == NULL)
    {
        success = false;
    }

    if(TTF_Init() == -1)
        success = false;
    font = TTF_OpenFont("font.ttf", 25);

    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//backgroundtest.png" , g_screen),
        ret2 = g_start.LoadImg("img//start.png",g_screen),
        ret3 = g_win.LoadImg("img//win.png", g_screen);
    if (ret == false || ret2 == false || ret3 == false)
    {
        return false;
    }
    return true;
}

void close()
{
    g_background.Free();
    g_win.Free();
    for(int i=0; i<n_disk; i++) {
        disk[i].Free();
    }
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}
void loaddisk()
{
    disk[0].LoadImg("img//1.png",g_screen);
    disk[1].LoadImg("img//2.png",g_screen);
    disk[2].LoadImg("img//3.png",g_screen);
    disk[3].LoadImg("img//4.png",g_screen);
    disk[4].LoadImg("img//5.png",g_screen);
    disk[5].LoadImg("img//6.png",g_screen);
    disk[6].LoadImg("img//7.png",g_screen);

}

void initgame()
{
    n_move = 0;
    min_move = pow(2, n_disk)-1;
    for(int i = n_disk; i>0; i--) {
        n_pillar.at(0).push_back(i);
    }
    for (int i=0; i<=n_disk; i++) {
        disk[n_disk - i].b = (12-i) * disk_height;
        disk[n_disk - i].a = 0;
    }
    loaddisk();
}

void clickmouse(const SDL_Event event)
{   if (event.type != SDL_MOUSEBUTTONDOWN) {
        return;
    }
    SDL_MouseButtonEvent mouse = event.button;
    if(mouse.x >= 0 && mouse.x <= 77 && mouse.y >= 0 && mouse.y <= 56)
    {
        for(int i=0; i<3; i++) {
            n_pillar.at(i).clear();
        }
        initgame();
    }
    else {
        pillar = mouse.x / disk_width;
        click++;
        if(click == 2) {
            click=0;
            move_disk();
            g_select.Free();
        }
        else {
            if(n_pillar.at(pillar).size() != 0) {
                p_pillar = pillar;
                Mix_PlayChannel(-1, g_sound_grab, 0);
                g_select.a = pillar * disk_width;
                g_select.b = 0;
                bool ret = g_select.LoadImg("img//select.png", g_screen);
                if(ret == NULL)
                    return;
            }
            else {click = 0;}
        }
    }
}

void move_disk()
{
    if(n_pillar.at(pillar).size() != 0) {
        if (n_pillar.at(pillar).back() > n_pillar.at(p_pillar).back() && n_pillar.at(p_pillar).size()!=0) {
            n_pillar.at(pillar).push_back(n_pillar.at(p_pillar).back());
            n_pillar.at(p_pillar).pop_back();
            disk[n_pillar.at(pillar).back()-1].a += (pillar - p_pillar)*disk_width;
            disk[n_pillar.at(pillar).back()-1].b = (12 - n_pillar.at(pillar).size())*disk_height;
            n_move++;
            Mix_PlayChannel(-1, g_sound_drop, 0);
        }
    }
    else if(n_pillar.at(p_pillar).size() !=0) {
        n_pillar.at(pillar).push_back(n_pillar.at(p_pillar).back());
        n_pillar.at(p_pillar).pop_back();
        disk[n_pillar.at(pillar).back()-1].a += (pillar - p_pillar)*disk_width;
        disk[n_pillar.at(pillar).back()-1].b = (12 - n_pillar.at(pillar).size())*disk_height;
        n_move++;
        Mix_PlayChannel(-1, g_sound_drop, 0);
    }
}
bool win()
{
    if(n_pillar.at(2).size() == n_disk) {
        return 1;
    }
    return 0;
}

int level(SDL_Event event)
{
    if(event.key.keysym.sym == SDLK_1)
        return 1;
    if(event.key.keysym.sym == SDLK_2)
        return 2;
    if(event.key.keysym.sym == SDLK_3)
        return 3;
    if(event.key.keysym.sym == SDLK_4)
        return 4;
    if(event.key.keysym.sym == SDLK_5)
        return 5;
    if(event.key.keysym.sym == SDLK_6)
        return 6;
    if(event.key.keysym.sym == SDLK_7)
        return 7;
}

void set_menu()
{
        g_start.Render(g_screen);
        SDL_RenderPresent(g_screen);
        SDL_Event e1;
        while (SDL_PollEvent(&e1)) {
            if(e1.type == SDL_KEYDOWN)
            {
                n_disk = level(e1);
                stt = playing;
                initgame();
            }
        }
}

void set_win()
{

    for(int i=0; i<3; i++) {
        n_pillar.at(i).clear();
    }
    g_win.Render(g_screen, NULL);
    SDL_RenderPresent(g_screen);
    SDL_Event e2;
    while (SDL_PollEvent(&e2)) {
        if(e2.type==SDL_MOUSEBUTTONDOWN) {
            SDL_MouseButtonEvent mouse = e2.button;
            int xm = mouse.x;
            int ym = mouse.y;
            if(xm >= 101 && xm <= 330 && ym >= 311 && ym <= 379)
            {
                stt = menu;
                SDL_RenderClear(g_screen);
            }
            if(xm >= 439 && xm <= 668 && ym >= 311 && ym <= 379)
                stt = quit;
        }
    }
}
