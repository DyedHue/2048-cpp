#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

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

// print the board
void board()
{
    printf("_________________________\n");
    printf("|     |     |     |     |\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (j == 0)
            {
                printf("|");
            }
            if (n[i][j] == 0)
            {
                printf("     |");
            }
            else if (n[i][j] < 10)
            {
                printf("  %d  |", n[i][j]);
            }
            else if (n[i][j] < 100)
            {
                printf("  %d |", n[i][j]);
            }
            else if (n[i][j] < 1000)
            {
                printf(" %d |", n[i][j]);
            }
            else if (n[i][j] < 10000)
            {
                printf(" %d|", n[i][j]);
            }
            else if (n[i][j] < 100000)
            {
                printf("%d|", n[i][j]);
            }
        }
        if (i == 0)
        {
            printf("  u  -  undo");
        }
        else if (i == 1)
        {
            printf("       w");
        }
        else if (i == 2)
        {
            printf("  a   [+]   d");
        }
        else if (i == 3)
        {
            printf("       s");
        }
        printf("\n|_____|_____|_____|_____|");
        if (i != 3)
        {
            printf("\n|     |     |     |     |\n");
        }
    }
    printf("\n\n");
    printf("Score = %d  |  Highest tile = %d\n", score, ht);
}


//check the equality between array n and m
//check if there is any space left on the board
int comp(string s = "equal")
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((s == "equal" && m[i][j] != n[i][j]) || (s == "space" && n[i][j] == 0))
            {
                return 1;
                break;
            }
        }
    }
    return 0;
}
// pass the values of array n to m
void pass()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = n[i][j];
        }
    }
}


// check if its game over!
int gameover()
{
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (n[i][j] == n[i + 1][j] || n[i][j] == n[i][j + 1])
            {
                return 1;
            }
        }
    }

    for (int j = 0; j < 3; j++)
    {
        if (n[3][j] == n[3][j + 1])
        {
            return 1;
            break;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (n[i][3] == n[i + 1][3])
        {
            return 1;
            break;
        }
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
            if (c=='u' && n[i][j] == n[i + 1][j])
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
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            n[i][j] = m[i][j];
        }
    }
}

// spawn a random number (2/4) in the blank spaces
void spawn(int a)
{
    int b = 0;
    int c = 0;
    int x = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (n[i][j] == 0)
            {
                x++;
            }
        }
    }

    uniform_int_distribution<int> distribution(1, x);
    b = distribution(rng_engine);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
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
    }
}

// main function starts here
int main()
{
    random_device rd;
    rng_engine.seed(rd());
    // screen messege
    printf("     w     |\n");
    printf("           |  Welcome to 2048!\n");
    printf(" a  [+]  d |  < How to play\n");
    printf("           |\n");
    printf("     s     |\n");
    printf("___________|_____\n");
    printf("Use the characters mentioned above for moving the numbers up, down, right or left.\n");
    printf("\nYou can also UNDO one move by entering 'u'\n");
    printf("\nYou might want to decrease the font size of your compiler's shell if you cannot view the board properly\n\n");
    printf("Input 'i' for more info about the game 2048 or-\n");
    printf("         Press any button to Start!\n");

    // take input to proceed
    arrow = _getch();
    if (arrow == 'i')
    {
        system("cls");
        printf("How to play 2048:\n__________________________\n\n");
        printf("A random number between 2 and 4 will keep spawning after each move.\n\nYou can move the board up, down, right or left to move all the numbers in the board to that direction");
        printf("\n\n[PRESS A BUTTON]");
        arrow = _getch();

        system("cls");
        printf("As two identical numbers meet, they will sum together, increasing the number of the tile as well as your score.\n\n");
        printf("When there is no more space in the board and no more possible way to move, it's game over. You can still undo however\n");
        printf("\nKeep increasing the number of the tiles this way and try to reach the tile of 2048, good luck!\n\n    Press any button to start\n");
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

                    printf("\nGame Over!\n\nEnter 'g' to give up\nEnter any other character to undo\n");
                    arrow = _getch();

                    if (arrow == 103)
                    {
                        printf("\n\n           You gave up!\n\n");
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
            system("cls");
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