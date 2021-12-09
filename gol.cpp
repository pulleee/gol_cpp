#include <iostream>
#include <csignal>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <windows.h>
#include <vector>

using namespace std;

int HEIGHT = 30;
int WIDTH = 120; 
bool RUNNING = true;

void setCursor(int x, int y){
    HANDLE handle;
    COORD coordinates;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition (handle ,coordinates);
}

void adjustCell(bool isAlive, int x, int y){
    setCursor(x, y);
    isAlive ? cout << '#' : cout << ' ';
}

void populateCells(vector<vector<bool>> &cells, int density){
        for(int i = 0; i < HEIGHT; i++){
            vector<bool> row;
            cells.push_back(row);
            for(int j = 0; j < WIDTH; j++){
                int rnd = rand() % 100;
                bool isAlive = (rnd < density);
                cells[i].push_back(isAlive);
                if(isAlive){
                    //cells[i][j] = isAlive;
                    adjustCell(true, j, i);
                }      
        }
    }
}

void fillVector(vector<vector<bool>> &v){
    for (int i = 0; i < HEIGHT; i++){
        vector<bool> row;
        v.push_back(row); 
        for (int j = 0; j < WIDTH; j++)
        {
            v[i].push_back(false);
        }   
    }
}

void copyVector(vector<vector<bool>> &src, vector<vector<bool>> &dst){
    dst = src;
}

int countNeighbors(vector<vector<bool>> &cells, int x, int y){
    int countNeighbor = 0;
    if(x > 0){
        if(cells[y][x - 1]) countNeighbor++;
        if(y > 0)
            if(cells[y - 1][x - 1]) countNeighbor++;
    }
    if(y > 0){
        if(cells[y - 1][x]) countNeighbor++;
        if(x < WIDTH - 1)
            if(cells[y - 1][x + 1]) countNeighbor++;     
    }
    if(x < WIDTH - 1){
        if(cells[y][x + 1]) countNeighbor++;
        if(y < HEIGHT - 1)
            if(cells[y + 1][x + 1]) countNeighbor++;
    }
    if(y < HEIGHT - 1){
        if(cells[y + 1][x]) countNeighbor++;
        if(x > 0)
            if(cells[y + 1][x - 1]) countNeighbor++;
    }
    return countNeighbor;
}

void moveNext(vector<vector<bool>> &cells, vector<vector<bool>> &cellsNext, int x, int y){
    int neighborCount = countNeighbors(cells, x, y);
    if(cells[y][x]){
        cellsNext[y][x] = neighborCount == 2 || neighborCount == 3; // underpopulation/overpopulation
    }else{
        cellsNext[y][x] = neighborCount == 3;
    }
    if(cells[y][x] != cellsNext[y][x]){
        adjustCell(cellsNext[y][x], x, y);
    }
}

void advance(vector<vector<bool>> &cells, vector<vector<bool>> &cellsNext) {
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            moveNext(cells, cellsNext, j, i);
        }
    }
}

void handler(int nr){
    cout << "EXIT" << endl;
    RUNNING = false;
}

int main(){
    system("cls"); 
    signal(SIGINT, handler);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    vector<vector<bool>> cells;
    vector<vector<bool>> cellsNext;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    WIDTH = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int density; 
    cout << "****************\n* Game of Life *\n****************" << endl;
    do {
        cout << "Density: " << endl;
        cin >> density;
    }while(!cin);
    system("cls"); 

    populateCells(cells, density); 
    fillVector(cellsNext);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while(RUNNING){
        advance(cells, cellsNext);
        copyVector(cellsNext, cells);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cells.clear();
    cellsNext.clear();
    
    return 1;
}
