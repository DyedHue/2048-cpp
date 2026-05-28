#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <windows.h>
#include <array>
using namespace std;

#define loopall() \
    for (int i = 0; i < 4; i++) \
        for (int j = 0; j < 4; j++)

vector<char> validInputsmove = { 'w', 'a', 's', 'd', 'u' };
mt19937 rng_engine;

array<array<int, 4>, 4> n = {}, m = {};//m is main, n is previous

int score = 0, store = 0, ht = 0, turn = 1;
char arrow;

void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

// print the board
void board()
{
    string frame = "";
    frame.reserve(500);
    frame += "_________________________\n";
    frame += "|     |     |     |     |\n";
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (j == 0) frame += "|";

            if (n[i][j] == 0)frame += "     |";
            else if (n[i][j] < 10)    frame += "  " + to_string(n[i][j]) + "  |";
            else if (n[i][j] < 100)   frame += "  " + to_string(n[i][j]) + " |";
            else if (n[i][j] < 1000)  frame += " " + to_string(n[i][j]) + " |";
            else if (n[i][j] < 10000) frame += " " + to_string(n[i][j]) + "|";
            else if (n[i][j] < 100000)frame += to_string(n[i][j]) + "|";
        }
        frame += "\n|_____|_____|_____|_____|";

        if (i == 0) frame += "  u  -  undo";
        else if (i == 1) frame += "       w";
        else if (i == 2) frame += "  a   [+]   d";
        else if (i == 3) frame += "       s";
        
        if (i != 3) frame += "\n|     |     |     |     |\n";
    }
    frame += "\n\n";
    frame += "Score = " + to_string(score) + " | Highest tile = " + to_string(ht) + "\n";
    cout << frame;
}

//check the equality between array n and m
//check if there is any space left on the board
int isSpace()
{
    loopall()
        if (n[i][j] == 0) return 1;

    return 0;
}

// check if its game over!
int gameover()
{
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (n[i][j] == n[i + 1][j] || n[i][j] == n[i][j + 1])  return 0;
        }
    }

    for (int j = 0; j < 3; j++)
    {
        if (n[3][j] == n[3][j + 1]) return 0;
    }

    for (int i = 0; i < 3; i++)
    {
        if (n[i][3] == n[i + 1][3]) return 0;
    }
    return 1;
}

// spawn a random number (2/4) in the blank spaces
void spawn()
{
    int num = 2;
    uniform_int_distribution<int> distribution1(1, 10);
    if (distribution1(rng_engine) == 1)
        num = 4;
    
    if (turn == 1) ht = num, score = num;

    int b = 0;
    int c = 0;
    int x = 0;

    loopall()
        if (n[i][j] == 0) x++;

    uniform_int_distribution<int> distribution(1, x);
    b = distribution(rng_engine);

    loopall()
        if (n[i][j] == 0)
        {
            c++;
            if (b == c)
            {
                n[i][j] = num;
                break;
            }
        }
}

void undo()
{
    score -= store;
    n = m;
}

void make_move_and_spawn(char c)
{
    store = 0;
    array<array<int, 4>, 4> newN = n;
    array<array<bool, 4>, 4> locked = {};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 4; j++)
        {
            int ii = i, idir = 0, jj = j, jdir = 0;

            if      (c == 'w') jj = i, ii = j,   idir = -1;
            else if (c == 's') jj = i, ii = 3-j, idir = 1;
            else if (c == 'a')           jdir = -1;
            else if (c == 'd') jj = 3-j, jdir = 1;

            for(int cnt = 1; cnt <= j; cnt++, ii += idir, jj += jdir)
            {
                int& cur = newN[ii][jj];
                int& prev = newN[ii + idir][jj + jdir];
                if(cur != 0 && prev == 0)
                {
                    prev = cur;
                    cur = 0;
                }
                else if (cur == prev && cur != 0 && !locked[ii][jj] && !locked[ii + idir][jj + jdir])
                {
                    prev += cur;
                    score += prev;
                    store += prev;
                    cur = 0;
                    locked[ii + idir][jj + jdir] = 1;
                    if (prev > ht)
                    {
                        ht = prev;
                    }
                }
                else break;
            }
        }
    }
    if (newN != n)
    {
        m = n;
        n = newN;
        spawn();
    }
}


// main function starts here
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    random_device rd;
    rng_engine.seed(rd());

    // screen messege
    cout <<"     w     |\n";
    cout <<"           |  Welcome to 2048!\n";
    cout <<" a  [+]  d |  < How to play\n";
    cout <<"           |\n";
    cout <<"     s     |\n";
    cout <<"___________|_____\n";
    cout <<"Use the characters mentioned above for moving the numbers up, down, right or left.\n";
    cout <<"\nYou can also UNDO one move by entering 'u'\n";
    cout <<"\nYou might want to decrease the font size of your compiler's shell if you cannot view the board properly\n\n";
    cout <<"Input 'i' for more info about the game 2048 or-\n";
    cout <<"         Press any button to Start!\n";

    // take input to proceed
    arrow = _getch();
    if (arrow == 'i')
    {
        system("cls");
        cout <<"How to play 2048:\n__________________________\n\n";
        cout <<"A random number between 2 and 4 will keep spawning after each move.\n\nYou can move the board up, down, right or left to move all the numbers in the board to that direction";
        cout <<"\n\n[PRESS A BUTTON]";
        arrow = _getch();

        system("cls");
        cout <<"As two identical numbers meet, they will sum together, increasing the number of the tile as well as your score.\n\n";
        cout <<"When there is no more space in the board and no more possible way to move, it's game over. You can still undo however\n";
        cout <<"\nKeep increasing the number of the tiles this way and try to reach the tile of 2048, good luck!\n\n    Press any button to start\n";
        arrow = _getch();
    }

    system("cls");

    spawn();
    while (1)
    {
        // gameplay loop start
        while (1)
        {
			setCursorPosition(0, 0);
            board();
            arrow = _getch();
            if (turn == 1 && arrow == 'u') continue;
            if (find(validInputsmove.begin(), validInputsmove.end(), arrow) != validInputsmove.end()) break;
        }
        if (arrow != 'u') make_move_and_spawn(arrow);
        else undo();
        turn = 0;

        if (!isSpace() && gameover())
        {
            // check if its game over when there is no space. undo or give up
            system("cls");
            board();
            cout <<"\nGame Over!\n\nEnter 'g' to give up\nEnter any other character to undo\n";
            
            arrow = _getch();
            if (arrow == 'g')
            {
                cout <<"\n\n           You gave up!\n\n";
                break;
            }
            else undo();
            system("cls");
        }
    }
}