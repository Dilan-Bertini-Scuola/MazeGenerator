// Dilan Bertini

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <conio.h>

// REF: https://docs.microsoft.com/en-us/windows/console/getconsolewindow?redirectedfrom=MSDN

#define _WIN32_WINNT 0x0500
#include <windows.h>

using namespace std;

const int MAP_SIZE  = 15 * 3; // Il primo numero identifica il numero di celle il secondo adatta il primo alla matricex
int MAP[MAP_SIZE][MAP_SIZE] = {};

bool gameOver = false;
enum ACTION {STOP = 0, UP, DOWN, RIGHT, LEFT};

/// Player Var

ACTION pDir;
int pX = 1, pY = 1;

/// ANTI FLICKERING System

void cls()
{
    // Get the Win32 handle representing standard output.
    // This generally only has to be done once, so we make it static.
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD topLeft = { 0, 0 };

    // std::cout uses a buffer to batch writes to the underlying console.
    // We need to flush that to the console because we're circumventing
    // std::cout entirely; after we clear the console, we don't want
    // stale buffered text to randomly be written out.
    cout.flush();

    // Figure out the current width and height of the console window
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
        // TODO: Handle failure!
        abort();
    }
    DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

    DWORD written;

    // Flood-fill the console with spaces to clear it
    FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

    // Reset the attributes of every character to the default.
    // This clears all background color formatting, if any.
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

    // Move the cursor back to the top left for the next sequence of writes
    SetConsoleCursorPosition(hOut, topLeft);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
    ShowConsoleCursor(false);
}

/// ANTI FLICKERING System

// Prima scrittura della matrice
void print() {
    cls();
    for(int y = 0; y < MAP_SIZE; y++){
        for(int x = 0; x < MAP_SIZE; x++){
            if(y == pY && x == pX){
                cout << "X";
            }else{
                if(MAP[y][x] == 2){
                    cout << " ";
                }else if(MAP[y][x] == 1){
                    cout << " ";
                }else if(MAP[y][x] == 3){
                    cout << " ";
                }else{
                    cout << "#";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Algoritmo di generazione del labiritno usando l'argomento Depth first search!
// More: https://en.wikipedia.org/wiki/Maze_generation_algorithm#Depth-first_search
void genLabyrinth() {
    bool right = false, left = false, up = false, down = false;

    bool finish = false;
    bool fTime = true;
    int x = 1, y = 1;
    int q, slice = 5;

    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            MAP[x][y] = 0;
        }
    }

    while (!finish){
        pY = y;
        pX = x;
        if (fTime)
        {
            fTime = false;
            MAP[y][x] = 2;
        }
        else
        {
            q = rand()%4;

            if(right && left && up && down){
                if (MAP[y-1][x] == 2 && slice != 0 && MAP[y-1][x] != 1)
                {

                    for(int i = 0; i < 3; i++){
                        y--;
                        MAP[y][x] = 1;
                    }
                    slice = 1;

                }
                else if (MAP[y+1][x] == 2 && slice != 1 && MAP[y+1][x] != 1)
                {
                    for(int i = 0; i < 3; i++){
                        y++;
                        MAP[y][x] = 1;
                    }
                    slice = 0;
                }
                else if (MAP[y][x+1] == 2 && slice != 2 && MAP[y][x+1] != 1)
                {
                    for(int i = 0; i < 3; i++){
                        x++;
                        MAP[y][x] = 1;
                    }
                    slice = 3;
                }
                else if (MAP[y][x-1] == 2 && slice != 3 && MAP[y][x-1] != 1)
                {
                    for(int i = 0; i < 3; i++){
                        x--;
                        MAP[y][x] = 1;
                    }
                    slice = 2;
                }else{
                    if(x == 1 && y == 1){
                        finish = true;
                    }
                }
                right = false;
                left = false;
                up = false;
                down = false;
            }else{
                switch (q)
                {
                case 0: // UP
                    if(!(y-1 <= 0)){
                        if(MAP[y-3][x] != 2 && MAP[y-3][x] != 1){
                            up = false;
                            slice = 5;
                            for(int i = 0; i < 3; i++){
                                y--;
                                MAP[y][x] = 2;
                            }
                        }else{
                            up=true;
                        }
                    }else{
                        up = true;
                    }
                    break;
                case 1: // DOWN
                    if(!(y+1 == MAP_SIZE-1)){
                        if(MAP[y+3][x] != 2 && MAP[y+3][x] != 1){
                            down = false;
                            slice = 5;
                            for(int i = 0; i < 3; i++){
                                y++;
                                MAP[y][x] = 2;
                            }
                        }else{
                            down = true;
                        }
                    }else{
                        down = true;
                    }
                    break;
                case 2: // RIGHT
                    if(!(x+1 == MAP_SIZE-1)){

                        if(MAP[y][x+3] != 2 && MAP[y][x+3] != 1){
                            right = false;
                            slice = 5;
                            for(int i = 0; i < 3; i++){
                                x++;
                                MAP[y][x] = 2;
                            }
                        }else{
                            right = true;
                        }
                    }else{
                        right = true;
                    }
                    break;
                case 3: // LEFT
                    if(!(x-1 <= 0)){

                        if(MAP[y][x-3] != 2 && MAP[y][x-3] != 1){
                            left = false;
                            slice = 5;
                            for(int i = 0; i < 3; i++){
                                x--;
                                MAP[y][x] = 2;
                            }
                        }else{
                            left = true;
                        }
                    }else{
                        left = true;
                    }
                    break;
                }
            }
        }
    }
    MAP[MAP_SIZE-1][MAP_SIZE-2] = 3;
}

// Inizializza/Resetta il gioco per una nuova partita
void SetUp() {
    srand(time(NULL));
    gameOver = false;
    cout << "Loading labyrinth... wait for a few seconds!" << endl;
    genLabyrinth();
    print();
}

// Ottiene in input i tasti dell'utente
void Input(){
    if(_kbhit()){
        switch (_getch())
        {
        case 'w':
            pDir = UP;
            break;
        case 'a':
            pDir = LEFT;
            break;
        case 'd':
            pDir = RIGHT;
            break;
        case 's':
            pDir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }

    }
}

// Qui si svolge tutta la logica del programma nel quale è sono presenti le collisioni e la vincita
void Logic(){
    switch (pDir)
    {
    case UP:
        if(MAP[pY - 1][pX] != 0){
            setCursorPosition(pX, pY);
            cout << " ";
            pY--;
            setCursorPosition(pX, pY);
            cout << "X";
        }
        break;

    case DOWN:
        if(MAP[pY + 1][pX] != 0){
            setCursorPosition(pX, pY);
            cout << " ";
            pY++;
            setCursorPosition(pX, pY);
            cout << "X";

        }
        break;

    case LEFT:
        if(MAP[pY][pX - 1] != 0){
            setCursorPosition(pX, pY);
            cout << " ";
            pX--;
            setCursorPosition(pX, pY);
            cout << "X";

        }
        break;

    case RIGHT:
        if(MAP[pY][pX + 1] != 0){
            setCursorPosition(pX, pY);
            cout << " ";
            pX++;
            setCursorPosition(pX, pY);
            cout << "X";

        }
        break;

    case STOP:
        break;
    }
    pDir = STOP;
    if(MAP[pY][pX] == 3){
        gameOver = true;
    }
}

int main(){
    SetConsoleTitle("RandomMaze");
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_MAXIMIZE);
    while (true){
        SetUp();
        ShowConsoleCursor(false);
        while (!gameOver){
            Input();
            Logic();
        }
        cls();
        cout << endl;
        cout << " /$$     /$$ /$$$$$$  /$$   /$$       /$$      /$$  /$$$$$$  /$$   /$$ /$$" << endl;
        cout << "|  $$   /$$//$$__  $$| $$  | $$      | $$  /$ | $$ /$$__  $$| $$$ | $$| $$" << endl;
        cout << " \\  $$ /$$/| $$  \\ $$| $$  | $$      | $$ /$$$| $$| $$  \\ $$| $$$$| $$| $$" << endl;
        cout << "  \\  $$$$/ | $$  | $$| $$  | $$      | $$/$$ $$ $$| $$  | $$| $$ $$ $$| $$" << endl;
        cout << "   \\  $$/  | $$  | $$| $$  | $$      | $$$$_  $$$$| $$  | $$| $$  $$$$|__/" << endl;
        cout << "    | $$   | $$  | $$| $$  | $$      | $$$/ \\  $$$| $$  | $$| $$\\  $$$    " << endl;
        cout << "    | $$   |  $$$$$$/|  $$$$$$/      | $$/   \\  $$|  $$$$$$/| $$ \\  $$ /$$" << endl;
        cout << "    |__/    \\______/  \\______/       |__/     \\__/ \\______/ |__/  \\__/|__/" << endl << endl << endl;
        cout << "Press ENTER to play again!" << endl;
        system("PAUSE > nul");
    }
}
