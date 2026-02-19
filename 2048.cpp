#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <windows.h>
using namespace std;

#define loopall() \
    for (int i = 0; i < 4; i++) \
        for (int j = 0; j < 4; j++)

vector<int> validInputsmove = { 119, 97, 115, 100, 117 };
mt19937 rng_engine;

// main array
int n[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0} };

// additional array for comparision, undo
int m[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 1} };

int a = 0, score = 0, store = 0, ht = 0;
int arrow;
bool moved = 0;

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
int comp(string s = "equal")
{
    loopall()
        if ((s == "equal" && m[i][j] != n[i][j]) || (s == "space" && n[i][j] == 0)) return 1;

    return 0;
}
// pass the values of array n to m
void pass()
{
    loopall() m[i][j] = n[i][j];
}


// check if its game over!
int gameover()
{
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (n[i][j] == n[i + 1][j] || n[i][j] == n[i][j + 1])  return 1;
        }
    }

    for (int j = 0; j < 3; j++)
    {
        if (n[3][j] == n[3][j + 1]) return 1;
    }

    for (int i = 0; i < 3; i++)
    {
        if (n[i][3] == n[i + 1][3]) return 1;
    }
    return 0;
}

void move(char c)
{
    int t = 3;
    while (t--)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int i = 1; i < 4; i++)
            {
                if (c == 'u' && n[i][j] != 0 && n[i - 1][j] == 0)
                {
                    moved = 1;
                    n[i - 1][j] = n[i][j];
                    n[i][j] = 0;
                }
                else if (c == 'd' && n[4 - i][j] == 0 && n[4 - i - 1][j] != 0)
                {
                    moved = 1;
                    n[4 - i][j] = n[4 - i - 1][j];
                    n[4 - i - 1][j] = 0;
                }
                else if (c == 'l' && n[j][3 - (i - 1)] != 0 && n[j][3 - (i - 1) - 1] == 0)
                {
                    moved = 1;
                    n[j][3 - (i - 1) - 1] = n[j][3 - (i - 1)];
                    n[j][3 - (i - 1)] = 0;
                }
                else if (c == 'r' && n[j][i - 1] != 0 && n[j][i] == 0)
                {
                    moved = 1;
                    n[j][i] = n[j][i - 1];
                    n[j][i - 1] = 0;
                }
            }
        }
    }
}
void merge(char c)
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (c == 'u' && n[i][j] == n[i + 1][j])
            {
                moved = 1;
                n[i][j] += n[i + 1][j];
                score += n[i][j];
                store += n[i][j];
                if (n[i][j] > ht)
                {
                    ht = n[i][j];
                }
                n[i + 1][j] = 0;
            }
            else if(c == 'd' && n[4 - (i + 1)][j] == n[4 - (i + 1) - 1][j])
            {
                moved = 1;
                n[4 - (i + 1)][j] += n[4 - (i + 1) - 1][j];
                score += n[4 - (i + 1)][j];
                store += n[4 - (i + 1)][j];
                if (n[4 - (i + 1)][j] > ht)
                {
                    ht = n[4 - (i + 1)][j];
                }
                n[4 - (i + 1) - 1][j] = 0;
            }
            else if (c == 'l' && n[j][i] == n[j][i + 1])
            {
                moved = 1;
                n[j][i] += n[j][i + 1];
                score += n[j][i];
                store += n[j][i];
                if (n[j][i] > ht)
                {
                    ht = n[j][i];
                }
                n[j][i + 1] = 0;
            }
            else if (c == 'r' && n[j][3 - i] == n[j][3 - i - 1])
            {
                moved = 1;
                n[j][3 - i] += n[j][3 - i - 1];
                score += n[j][3 - i];
                store += n[j][3 - i];
                if (n[j][3 - i] > ht)
                {
                    ht = n[j][3 - i];
                }
                n[j][3 - i - 1] = 0;
            }
        }
    }
}

// undo the elements and the score
void undo()
{
    score -= store;
    loopall() n[i][j] = m[i][j];
}

// spawn a random number (2/4) in the blank spaces
void spawn(int a)
{
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
                n[i][j] = a;
                break;
            }
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
    int turn = 1;

    while (1)
    {
        // gameplay loop

        if (comp() == 1)
        {
            a = 2;
            uniform_int_distribution<int> distribution(1, 10);
            if (distribution(rng_engine) == 1)
                a += 2;

            spawn(a); // spawn the number
            if (comp("space") == 0)
            {
                // check if its game over when there is no space
                if (gameover() == 0)
                {
                    // undo or give up
                    system("cls");
                    board();

                    cout <<"\nGame Over!\n\nEnter 'g' to give up\nEnter any other character to undo\n";
                    arrow = _getch();

                    if (arrow == 103)
                    {
                        cout <<"\n\n           You gave up!\n\n";
                        break;
                    }
                    else
                    {
                        undo();
                    }
                }
            }
        }

        // main gameplay

        while (1)
        {
			setCursorPosition(0, 0);
            board();
            arrow = _getch();

            if (find(validInputsmove.begin(), validInputsmove.end(), arrow) != validInputsmove.end())
            {
                break;
            }
        }

        if (arrow != 'u')
        {
            pass();
            store = 0;
        }

        // continue moving the board
        if (arrow == 'w')
        {
            move('u');
            merge('u');
            move('u');
        }
        else if (arrow == 's')
        {
            move('d');
            merge('d');
            move('d');
        }
        else if (arrow == 'a')
        {
            move('l');
            merge('l');
            move('l');
        }
        else if (arrow == 'd')
        {
            move('r');
			merge('r');
            move('r');
        }
        else if (arrow == 'u')
        {
            // undo
            if (turn != 1)
                undo();
            else
                pass();
        }
        turn = 0;
        moved = 0;
    }
}