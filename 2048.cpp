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
#include <chrono>
#include <thread>
using namespace std;

#define loopall() \
    for (int i = 0; i < 4; i++) \
        for (int j = 0; j < 4; j++)

vector<char> validInputsmove = { 'w', 'a', 's', 'd', 'u' };
mt19937 rng_engine;

array<array<int, 4>, 4> n = {};
vector<array<array<int, 4>, 4>> prevStates;
vector<int>store;

int score = 0, ht = 0, turn = 1, undoUsage = 0, maxUndoUsage = 1, delayAmt = 30, movecnt = 0;
char arrow;
bool infiniteUndo = 0, animation = 0;

void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

// print the board
void board(array<array<int, 4>, 4> &arr = n)
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

            if (arr[i][j] == 0)frame += "     |";
            else if (arr[i][j] < 10)    frame += "  " + to_string(arr[i][j]) + "  |";
            else if (arr[i][j] < 100)   frame += "  " + to_string(arr[i][j]) + " |";
            else if (arr[i][j] < 1000)  frame += " " + to_string(arr[i][j]) + " |";
            else if (arr[i][j] < 10000) frame += " " + to_string(arr[i][j]) + "|";
            else if (arr[i][j] < 100000)frame += to_string(arr[i][j]) + "|";
        }
        frame += "\n|_____|_____|_____|_____|";

        if (i == 0) frame += "  u  -  undo";
        else if (i == 1) frame += "       w";
        else if (i == 2) frame += "  a   [+]   d";
        else if (i == 3) frame += "       s";
        
        if (i != 3) frame += "\n|     |     |     |     |\n";
    }
    frame += "\n\n";
    frame += "Score : " + to_string(score) + " | Highest tile : " + to_string(ht) + "                   " + "\nUndo usage: " + to_string(undoUsage) + " | Moves: " + to_string(movecnt) + "                   " + "\n";
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
    if(prevStates.empty() || undoUsage == 0) return;

    score -= store.back();
    store.pop_back();

    n = prevStates.back();
    prevStates.pop_back();

    undoUsage--;
    movecnt--;
}

void make_move_and_spawn(char c)
{
    array<array<int, 4>, 4> newN = n;
    array<array<bool, 4>, 4> locked = {};

    store.push_back(0);

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
                    if (animation)
                    {
                        setCursorPosition(0, 0);
                        board(newN);
                        this_thread::sleep_for(chrono::milliseconds(delayAmt));
                    }
                }
                else if (cur == prev && cur != 0 && !locked[ii][jj] && !locked[ii + idir][jj + jdir])
                {
                    prev += cur;
                    score += prev;
                    store.back() += prev;
                    cur = 0;
                    locked[ii + idir][jj + jdir] = 1;
                    if (prev > ht)
                    {
                        ht = prev;
                    }
                    if (animation)
                    {
                        setCursorPosition(0, 0);
                        board(newN);
                        this_thread::sleep_for(chrono::milliseconds(delayAmt));
                    }
                }
                else break;
            }
        }
    }
    if (newN != n)
    {
        prevStates.push_back(n);
        n = newN;
        spawn();
        if(undoUsage < maxUndoUsage || infiniteUndo) undoUsage++;
        movecnt++;
    }
}

void showIntroduction()
{
    cout <<"     w     |\n";
    cout <<"           |  Welcome to 2048!\n";
    cout <<" a  [+]  d |  < How to play\n";
    cout <<"           |\n";
    cout <<"     s     |\n";
    cout <<"___________|_____\n";
    cout <<"Use the characters mentioned above for moving the numbers up, down, right or left.\n";
    cout <<"\nYou can also UNDO by pressing 'u'\n";
    cout <<"\nYou might want to decrease the font size of your terminal if you cannot view the board properly\n\n";
    cout <<"Enter 's' for changing settings\n";
    cout <<"Enter 'i' for more info about the game 2048 or-\n\n";
    cout <<"         Press any other button to Start!\n";
}

void GuideScrn()
{
    system("cls");
    cout << "How to play 2048:\n__________________________\n\n";
    cout << "A random number between 2 and 4 will keep spawning after each move.\n\nYou can move the board up, down, right or left to move all the numbers in the board to that direction";
    cout << "\n\n[PRESS A BUTTON]";
    arrow = _getch();

    system("cls");
    cout << "As two identical numbers meet, they will sum together, increasing the number of the tile as well as your score.\n\n";
    cout << "When there is no more space in the board and no more possible way to move, it's game over. You can still undo however.\n";
    cout << "\nKeep increasing the number of the tiles this way and try to reach the tile of 2048, good luck!\n\n    [PRESS A BUTTON]\n";
    arrow = _getch();
}

void SettingsScrn()
{
    while(1)
    {
        system("cls");
        cout << "Settings:\n_________\n\n";
        cout << "1. Undo limit: " << (infiniteUndo ? "inf" : to_string(maxUndoUsage)) << "\n\n";
        cout << "2. Animation: " << (animation ? "Enabled" : "Disabled") << "\n\n";
        cout << "3. Animation slowness: " << delayAmt << "\n\n\n\n\n";
        cout << "Enter a setting index to modify the setting. Enter anything else to continue:\n";
        arrow = _getch();

        if (arrow == '1')
        {
            while (1)
            {
                system("cls");
                cout << "Enter undo limit (i for no limit): ";
                string inp;
                cin >> inp;
                if (inp == "i")
                {
                    infiniteUndo = 1;
                    break;
                }
                try
                {
                    maxUndoUsage = stoi(inp);
                    break;
                }
                catch (const invalid_argument& e) {}
            }
        }
        else if (arrow == '2')
        {
            animation = !animation;
        }
        else if (arrow == '3')
        {
            while (1)
            {
                system("cls");
                cout << "Enter animation slowness : ";
                try
                {
                    cin >> delayAmt;
                    break;
                }
                catch (const invalid_argument& e) {}
            }
        }
        else break;
    }
}
// main function starts here
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    random_device rd;
    rng_engine.seed(rd());

    while(1)
    {
        system("cls");
        showIntroduction();
        arrow = _getch();
        if (arrow == 'i')
        {
            GuideScrn();
        }
        else if (arrow == 's')
        {
            SettingsScrn();
        }
        else break;
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
            if (find(validInputsmove.begin(), validInputsmove.end(), arrow) != validInputsmove.end()) break;
        }
        if (arrow != 'u') make_move_and_spawn(arrow);
        else undo();
        turn = 0;

        if (!isSpace() && gameover())
        {
            system("cls");
            board();
            cout <<"\nGame Over!\n\nEnter 'g' to give up\nEnter any other character to undo\n";
            
            arrow = _getch();
            if (arrow == 'g')
            {
                cout <<"\n\n           You gave up!\n\n";
                break;
            }
            else
            {
                undo();
            }
            system("cls");
        }
    }
}