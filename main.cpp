#include <iostream>
#include <math.h>
#include "windows.h"
struct pole {
    int info;
    int num_of_ship;
};
struct ship {
    int x;
    int y;
};
int hit = 0; // сколько раз компьютер попал по кораблю; обнуляется, когда он "убивает" корабль
bool dead_ship_of_player [10]; // (true => dead, false => alive)
bool dead_ship_of_ai[10]; // (true => dead, false => alive)
char winner; // определяет победителя (победил: p - игрок, a - компьютер, n - пока что никто не победил)
bool xod_of_player; // определяет кто сейчас стреляет ( true/1 => ходит игрок, false/0 => ходит компьютер )
pole pole_player[10][10]; // 0 ~ неоткрытая клетка без корабля, 1 ~ неоткрытая клетка с кораблём, 2 ~ открытая клетка без корабля, 3 ~ открытая клетка с кораблём, 4 ~ клетка, на которую нельзя ставить корабль (= 0..9)
pole pole_ai[10][10];  // = (0..9)
ship shot_ai[2]; // координаты точек
ship shot; // координаты выстрела
ship ship1[4]; // [номер корабля] (= 0..9)
ship ship2[2][3]; // [номер клетки][номер корабля] (= 0..9)
ship ship3[3][2]; // [номер клетки][номер корабля] (= 0..9)
ship ship4[4]; // [номер клетки]
ship ship1_ai[4]; // [номер клетки] компьютер
ship ship2_ai[2][3]; //[номер клетки][номер корабля] компьютер
ship ship3_ai[3][2]; //[номер клетки][номер корабля] компьютер
ship ship4_ai[4]; //[номер корабля] компьютер
void graphic_develop(){
    int i,j;
    printf("\n\n");
    printf(" компьютер                   игрок\n");
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("%i ", pole_ai[j][i].info);
        }
        printf("    ");
        for (j = 0; j < 10; j++) {
            printf("%i ",pole_player[j][i].info);
        }
        printf("\n");
    }
    printf("\n\n");
}

void ship_ai() { //задание кораблей компьютера
    bool possible; // проверяет можно ли поставить корабли; сначала нельзя (false), чтобы можно было войти в цикл
    int i, j;
    int x, y;
    x = rand() % 10; // задание 4-ёх палубника
    y = rand() % 10;
    if (rand() % 2 == 1) { // корабль стоит вертикально
        if (y > 6)
            y = rand() % 7;
        for (i = 0; i < 4; i++) {
            ship4_ai[i].x = x;  // задание точек корабля в массиве корабля на оси х
            ship4_ai[i].y = y + i;  // задание точек корабля в массиве корабля на оси у
            pole_ai[ship4_ai[i].x][ship4_ai[i].y].info = 1; // задание точек корабля на поле
            pole_ai[ship4_ai[i].x][ship4_ai[i].y].num_of_ship = 1;
            if (ship4_ai[i].x - 1 > -1)
                pole_ai[ship4_ai[i].x - 1][ship4_ai[i].y].info = 4; // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
            if (ship4_ai[i].x + 1 < 10)
                pole_ai[ship4_ai[i].x + 1][ship4_ai[i].y].info = 4; // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли
        }
        for (j = 0; j < 3; j++) {
            if ((x - 1 + j > -1) && (y - 1 > -1) && (x - 1 + j < 10))
                pole_ai[x - 1 + j][y - 1].info = 4;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
            if ((x - 1 + j > -1) && (y + 4 < 10) && (x - 1 + j < 10))
                pole_ai[x - 1 + j][y + 4].info = 4;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
        }
    }
    else {
        if (x > 6)
            x = rand() % 7; // корабль стоит горизонтально
        for (i = 0; i < 4; i++) {
            ship4_ai[i].x = x + i;  // задание точек корабля в массиве корабля на оси х
            ship4_ai[i].y = y;  // задание точек корабля в массиве корабля на оси у
            pole_ai[ship4_ai[i].x][ship4_ai[i].y].info = 1; // задание точек корабля на поле
            pole_ai[ship4_ai[i].x][ship4_ai[i].y].num_of_ship = 1;
            if (ship4_ai[i].y - 1 > -1)
                pole_ai[ship4_ai[i].x][ship4_ai[i].y - 1].info = 4; // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
            if (ship4_ai[i].y + 1 < 10)
                pole_ai[ship4_ai[i].x][ship4_ai[i].y + 1].info = 4; // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
        }
        for (j = 0; j < 3; j++) {
            if ((y - 1 + j > -1) && (x - 1 > -1) && (y - 1 + j < 10))
                pole_ai[x - 1][y - 1 + j].info = 4;  // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
            if ((y - 1 + j > -1) && (x + 4 < 10) && (y - 1 + j < 10))
                pole_ai[x + 4][y - 1 + j].info = 4;  // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли
        }
    } // конец задания четырёхпалубника
    for (j = 0; j < 2; j++) { // задание трёхпалубников
        if (rand() % 2 == 1) { // корабль стоит вертикально
            possible = false;
            while (!possible) {
                x = rand() % 10;
                y = rand() % 8;
                possible = true;
                for (i = 0; i < 3; i++) {
                    if (pole_ai[x][y + i].info != 0) {
                        possible = false;
                    }
                }
                if (possible) {
                    for (i = 0; i < 3; i++) {
                        ship3_ai[i][j].x = x;  // задание точек корабля в массиве корабля на оси х
                        ship3_ai[i][j].y = y + i;  // задание точек корабля в массиве корабля на оси у
                        pole_ai[ship3_ai[i][j].x][ship3_ai[i][j].y].info = 1; // задание точек корабля на поле
                        pole_ai[ship3_ai[i][j].x][ship3_ai[i][j].y].num_of_ship = 2 + j;
                        if (ship3_ai[i][j].x - 1 > -1)
                            pole_ai[ship3_ai[i][j].x - 1][ship3_ai[i][j].y].info = 4; // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
                        if (ship3_ai[i][j].x + 1 < 10)
                            pole_ai[ship3_ai[i][j].x + 1][ship3_ai[i][j].y].info = 4; // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли
                    }
                    for (i = 0; i < 3; i++) {
                        if ((x - 1 + i > -1) && (y - 1 > -1) && (x - 1 + i < 10))
                            pole_ai[x - 1 + i][y - 1].info = 4;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                        if ((x - 1 + i > -1) && (y + 3 < 10) && (x - 1 + i < 10))
                            pole_ai[x - 1 + i][y + 3].info = 4;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    }
                }
            }
        }


        else { // корабль стоит горизонтально
            possible = false;
            while (!possible) {
                y = rand() % 10;
                x = rand() % 8;
                possible = true;
                for (i = 0; i < 3; i++) {
                    if (pole_ai[x + i][y].info != 0) {
                        possible = false;
                    }
                }
                if (possible) {
                    for (i = 0; i < 3; i++) {
                        ship3_ai[i][j].x = x + i;  // задание точек корабля в массиве корабля на оси х
                        ship3_ai[i][j].y = y;  // задание точек корабля в массиве корабля на оси у
                        pole_ai[ship3_ai[i][j].x][ship3_ai[i][j].y].info = 1; // задание точек корабля на поле
                        pole_ai[ship3_ai[i][j].x][ship3_ai[i][j].y].num_of_ship = 2 + j;
                        if (ship3_ai[i][j].y - 1 > -1)
                            pole_ai[ship3_ai[i][j].x][ship3_ai[i][j].y - 1].info = 4; // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                        if (ship3_ai[i][j].y + 1 < 10)
                            pole_ai[ship3_ai[i][j].x][ship3_ai[i][j].y + 1].info = 4; // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    }
                    for (i = 0; i < 3; i++) {
                        if ((y - 1 + i > -1) && (x - 1 > -1) && (y - 1 + i < 10))
                            pole_ai[x - 1][y - 1 + i].info = 4;  // задание на поле точек, находящихся левее корабля, на которые нельзя ставить корабли
                        if ((y - 1 + i > -1) && (x + 3 < 10) && (y - 1 + i < 10))
                            pole_ai[x + 3][y - 1 + i].info = 4;  // задание на поле точек, находящихся правее корабля, на которые нельзя ставить корабли
                    }
                }
            }



        }

    } // конец задания трёхпалубников
    for (j = 0; j < 3; j++) { // задание двухпалубников

        if (rand() % 2 == 1) { // корабль стоит вертикально
            possible = false;
            while (!possible) {
                x = rand() % 10;
                y = rand() % 9;
                possible = true;
                for (i = 0; i < 2; i++) {
                    if (pole_ai[x][y + i].info != 0) {
                        possible = false;
                    }
                }
                if (possible) {
                    for (i = 0; i < 2; i++) {
                        ship2_ai[i][j].x = x;  // задание точек корабля в массиве корабля на оси х
                        ship2_ai[i][j].y = y + i;  // задание точек корабля в массиве корабля на оси у
                        pole_ai[ship2_ai[i][j].x][ship2_ai[i][j].y].info = 1; // задание точек корабля на поле
                        pole_ai[ship2_ai[i][j].x][ship2_ai[i][j].y].num_of_ship = 4 + j;
                        if (ship2_ai[i][j].x - 1 > -1)
                            pole_ai[ship2_ai[i][j].x -
                                    1][ship2_ai[i][j].y].info = 4; // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
                        if (ship2_ai[i][j].x + 1 < 10)
                            pole_ai[ship2_ai[i][j].x +
                                    1][ship2_ai[i][j].y].info = 4; // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли
                    }
                    for (i = 0; i < 3; i++) {
                        if ((x - 1 + i > -1) && (y - 1 > -1) && (x - 1 + i < 10))
                            pole_ai[x - 1 + i][y -
                                               1].info = 4;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                        if ((x - 1 + i > -1) && (y + 2 < 10) && (x - 1 + i < 10))
                            pole_ai[x - 1 + i][y +
                                               2].info = 4;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    }
                }
            }
        } else { // корабль стоит горизонтально
            possible = false;
            while (!possible) {
                y = rand() % 10;
                x = rand() % 9;
                possible = true;
                for (i = 0; i < 2; i++) {
                    if (pole_ai[x + i][y].info != 0) {
                        possible = false;
                    }
                }
                if (possible) {
                    for (i = 0; i < 2; i++) {
                        ship2_ai[i][j].x = x + i;  // задание точек корабля в массиве корабля на оси х
                        ship2_ai[i][j].y = y;  // задание точек корабля в массиве корабля на оси у
                        pole_ai[ship2_ai[i][j].x][ship2_ai[i][j].y].info = 1; // задание точек корабля на поле
                        pole_ai[ship2_ai[i][j].x][ship2_ai[i][j].y].num_of_ship = 4 + j;
                        if (ship2_ai[i][j].y - 1 > -1)
                            pole_ai[ship2_ai[i][j].x][ship2_ai[i][j].y -
                                                      1].info = 4; // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                        if (ship2_ai[i][j].y + 1 < 10)
                            pole_ai[ship2_ai[i][j].x][ship2_ai[i][j].y +
                                                      1].info = 4; // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    }
                    for (i = 0; i < 3; i++) {
                        if ((y - 1 + i > -1) && (x - 1 > -1) && (y - 1 + i < 10))
                            pole_ai[x - 1][y - 1 +
                                           i].info = 4;  // задание на поле точек, находящихся левее корабля, на которые нельзя ставить корабли
                        if ((y - 1 + i > -1) && (x + 2 < 10) && (y - 1 + i < 10))
                            pole_ai[x + 2][y - 1 +
                                           i].info = 4;  // задание на поле точек, находящихся правее корабля, на которые нельзя ставить корабли
                    }
                }
            }


        } // конец задания двухпалубников
    }
        for (j = 0; j < 4; j++) { //
            possible = false;
            while (!possible) {
                x = rand() % 10;
                y = rand() % 10;
                possible = true;
                if (pole_ai[x][y].info != 0) {
                    possible = false;
                }
                if (possible) {
                    ship1_ai[j].x = x;  // задание точек корабля в массиве корабля на оси х
                    ship1_ai[j].y = y;  // задание точек корабля в массиве корабля на оси у
                    pole_ai[ship1_ai[j].x][ship1_ai[j].y].info = 1; // задание точек корабля на поле
                    pole_ai[ship1_ai[j].x][ship1_ai[j].y].num_of_ship = 7 + j;
                    if (ship1_ai[j].x - 1 > -1)
                        pole_ai[ship1_ai[j].x - 1][ship1_ai[j].y].info = 4; // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
                    if (ship1_ai[j].x + 1 < 10)
                        pole_ai[ship1_ai[j].x + 1][ship1_ai[j].y].info = 4; // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли

                    for (i = 0; i < 3; i++) {
                        if ((x - 1 + i > -1) && (y - 1 > -1) && (x - 1 + i < 10))
                            pole_ai[x - 1 + i][y - 1].info = 4;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                        if ((x - 1 + i > -1) && (y + 1 < 10) && (x - 1 + i < 10))
                            pole_ai[x - 1 + i][y + 1].info = 4;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    }
                }
            }
        }


    }
void point (){ //задание кораблей игрока
    int i;
    int j;
    if (ship4[0].x > ship4[3].x) {  //КОРАБЛЬ СТОИТ ГОРИЗОНТАЛЬНО И Х0 > Х3 задание и упорядочивание точек 4-ёх плубника
        ship4[0].x = ship4[0].x+ship4[3].x;
        ship4[3].x = ship4[0].x-ship4[3].x;
        ship4[0].x = ship4[0].x-ship4[3].x; //x0=min(x0,x3) x3=max(x0,x3)

    }
    if (ship4[0].y > ship4[3].y) { // корабль стоит вертикально и у0 > у3
        ship4[0].y = ship4[0].y + ship4[3].y;
        ship4[3].y = ship4[0].y - ship4[3].y;
        ship4[0].y = ship4[0].y - ship4[3].y; //y0=min(y0,y3) y3=max(y0,y3)

    }
    if (ship4[0].x != ship4[3].x) {
        ship4[1].x = ship4[0].x+1; //x1=x0+1
        ship4[2].x = ship4[0].x+2; //x2=x0+2
        ship4[1].y = ship4[0].y; //y1=y0
        ship4[2].y = ship4[0].y; //y2=y0
    }
    else {
        ship4[1].y = ship4[0].y + 1; //y1 = y0 + 1
        ship4[2].y = ship4[0].y + 2; //y2=y0+2
        ship4[1].x = ship4[0].x; //x1=x0
        ship4[2].x = ship4[0].x; //x2=x0
    }
    for (j = 0;j < 4;j++)
        pole_player[ship4[j].x][ship4[j].y].info = 1; //задание и упорядочивание точек 4-ёх палубника на поле игрока
    for (i=0;i<2;i++) {
        if (ship3[0][i].x > ship3[2][i].x) { //задание точек 3-ёх плубника
            ship3[0][i].x = ship3[0][i].x + ship3[2][i].x;
            ship3[2][i].x = ship3[0][i].x - ship3[2][i].x;
            ship3[0][i].x = ship3[0][i].x - ship3[2][i].x; //x0=min(x0,x2) x3=max(x0,x2)

        }
        if (ship3[0][i].y > ship3[2][i].y) {
            ship3[0][i].y = ship3[0][i].y + ship3[2][i].y;
            ship3[2][i].y = ship3[0][i].y - ship3[2][i].y;
            ship3[0][i].y = ship3[0][i].y - ship3[2][i].y; //y0=min(y0,y3) y3=max(y0,y3)


        }
        if (ship3[0][i].x != ship3[2][i].x) {
            ship3[1][i].x = ship3[0][i].x+1; //x1=x0+1
            ship3[1][i].y = ship3[0][i].y; //y1=y0
        }
        else {
            ship3[1][i].y = ship3[0][i].y+1; //x1=x0+1
            ship3[1][i].x = ship3[0][i].x; //y1=y0
        }
        for (j=0;j<3;j++)
            pole_player[ship3[j][i].x][ship3[j][i].y].info = 1; //задание и упорядочивание точек 3-ёх палубника на поле игрока
    }
    for (i=0;i<3;i++) {
        if (ship2[0][i].x > ship2[1][i].x) {  //упорядочивание точек 2-yх палубника
            ship2[0][i].x = ship2[0][i].x + ship2[1][i].x;
            ship2[1][i].x = ship2[0][i].x - ship2[1][i].x;
            ship2[0][i].x = ship2[0][i].x - ship2[1][i].x; //x0=min(x0,x1) x3=max(x0,x1)
        }
        if (ship2[0][i].y > ship2[1][i].y) {
            ship2[0][i].y = ship2[0][i].y + ship2[1][i].y;
            ship2[1][i].y = ship2[0][i].y - ship2[1][i].y;
            ship2[0][i].y = ship2[0][i].y - ship2[1][i].y; //y0=min(y0,y1) y3=max(y0,y1)
        }
        for(j=0;j<2;j++)
            pole_player[ship2[j][i].x][ship2[j][i].y].info = 1;
    }
    for (i=0;i<4;i++)
        pole_player[ship1[i].x][ship1[i].y].info = 1;

}
void dead_of_ships_player () { // проверяет живы ли корабли игрока, если нет то отмечает смерть и отмечает на поле пустые клетки вокруг корабля
    int i,j,k;
    if (dead_ship_of_player[0] == false && pole_player[ship4[0].x][ship4[0].y].info == 3 &&
        pole_player[ship4[1].x][ship4[1].y].info == 3 && pole_player[ship4[2].x][ship4[2].y].info == 3 &&
        pole_player[ship4[3].x][ship4[3].y].info == 3) {
        dead_ship_of_player[0] = true;
        hit = 0;
        for (j = 0; j < 4; j++) {
            if (ship4[j].x - 1 > -1 && pole_player[ship4[j].x - 1][ship4[j].y].info != 3)
                pole_player[ship4[j].x - 1][ship4[j].y].info = 2; // задание на поле точки, находящихся слева от точки корабля, на которые нельзя ставить корабли
            if (ship4[j].x + 1 < 10 && pole_player[ship4[j].x + 1][ship4[j].y].info != 3)
                pole_player[ship4[j].x + 1][ship4[j].y].info = 2; // задание на поле точки, находящихся справа от точки корабля, на которые нельзя ставить корабли
            for (i = 0; i < 3; i++) {
                if ((ship4[j].x - 1 + i > -1) && (ship4[j].y - 1 > -1) && (ship4[j].x - 1 + i < 10) &&
                    (pole_player[ship4[j].x - 1 + i][ship4[j].y - 1].info != 3))
                    pole_player[ship4[j].x - 1 + i][ship4[j].y -
                                                   1].info = 2;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                if ((ship4[j].x - 1 + i > -1) && (ship4[j].y + 1 < 10) && (ship4[j].x - 1 + i < 10) &&
                    (pole_player[ship4[j].x - 1 + i][ship4[j].y + 1].info != 3))
                    pole_player[ship4[j].x - 1 + i][ship4[j].y + 1].info = 2;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
            }   // задание на поле пустых точек вокруг убитого корабля тоже самое нужно для игрока
        }
    }
    for (i = 1; i <= 2; i++) {
        if (dead_ship_of_player[i] == false && pole_player[ship3[0][i - 1].x][ship3[0][i - 1].y].info == 3 &&
            pole_player[ship3[1][i - 1].x][ship3[1][i - 1].y].info == 3 &&
            pole_player[ship3[2][i - 1].x][ship3[2][i - 1].y].info == 3) {
            dead_ship_of_player[i] = true;
            hit = 0;
            for (j = 0; j < 3; j++) {
                if (ship3[j][i - 1].x - 1 > -1 && pole_player[ship3[j][i - 1].x - 1][ship3[j][i - 1].y].info != 3)
                pole_player[ship3[j][i - 1].x - 1][ship3[j][i -
                                                                  1].y].info = 2; // задание на поле точки, находящихся слева от точки корабля, на которые нельзя ставить корабли
                if (ship3[j][i - 1].x + 1 < 10 && pole_player[ship3[j][i - 1].x + 1][ship3[j][i - 1].y].info != 3)
                    pole_player[ship3[j][i - 1].x + 1][ship3[j][i -
                                                                  1].y].info = 2; // задание на поле точки, находящихся справа от точки корабля, на которые нельзя ставить корабли

                for (k = 0; k < 3; k++) {
                    if ((ship3[j][i - 1].x - 1 + k > -1) && (ship3[j][i - 1].y - 1 > -1) &&
                        (ship3[j][i - 1].x - 1 + k < 10) &&
                        (pole_player[ship3[j][i - 1].x - 1 + k][ship3[j][i - 1].y - 1].info != 3))
                        pole_player[ship3[j][i - 1].x - 1 + k][ship3[j][i - 1].y -
                                                              1].info = 2;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                    if ((ship3[j][i - 1].x - 1 + k > -1) && (ship3[j][i - 1].y + 1 < 10) &&
                        (ship3[j][i - 1].x - 1 + k < 10) &&
                        (pole_player[ship3[j][i - 1].x - 1 + k][ship3[j][i - 1].y + 1].info != 3))
                        pole_player[ship3[j][i - 1].x - 1 + k][ship3[j][i - 1].y +
                                                              1].info = 2;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    // задание на поле пустых точек вокруг убитого корабля тоже самое нужно для игрока
                }

            }
        }
    }
    for (i = 3; i <= 5; i++) {
        if (dead_ship_of_player[i] == false && pole_player[ship2[0][i - 3].x][ship2[0][i - 3].y].info == 3 &&
                pole_player[ship2[1][i - 3].x][ship2[1][i - 3].y].info == 3) {
            dead_ship_of_player[i] = true;
            hit = 0;
            for (j = 0; j < 2; j++) {
                if (ship2[j][i - 3].x - 1 > -1 && pole_player[ship2[j][i - 3].x - 1][ship2[j][i - 3].y].info != 3)
                    pole_player[ship2[j][i - 3].x - 1][ship2[j][i -
                                                                  3].y].info = 2; // задание на поле точки, находящихся слева от точки корабля, на которые нельзя ставить корабли
                if (ship2[j][i - 3].x + 1 < 10 && pole_player[ship2[j][i - 3].x + 1][ship2[j][i - 3].y].info != 3)
                    pole_player[ship2[j][i - 3].x + 1][ship2[j][i -
                                                                  3].y].info = 2; // задание на поле точки, находящихся справа от точки корабля, на которые нельзя ставить корабли

                for (k = 0; k < 3; k++) {
                    if ((ship2[j][i - 3].x - 1 + k > -1) && (ship2[j][i - 3].y - 1 > -1) &&
                        (ship2[j][i - 3].x - 1 + k < 10) &&
                        (pole_player[ship2[j][i - 3].x - 1 + k][ship2[j][i - 3].y - 1].info != 3))
                        pole_player[ship2[j][i - 3].x - 1 + k][ship2[j][i - 3].y -
                                                              1].info = 2;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                    if ((ship2[j][i - 3].x - 1 + k > -1) && (ship2[j][i - 3].y + 1 < 10) &&
                        (ship2[j][i - 3].x - 1 + k < 10) &&
                        (pole_player[ship2[j][i - 3].x - 1 + k][ship2[j][i - 3].y + 1].info != 3))
                        pole_player[ship2[j][i - 3].x - 1 + k][ship2[j][i - 3].y +
                                                              1].info = 2;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    // задание на поле пустых точек вокруг убитого корабля тоже самое нужно для игрока
                }

            }
        }
    }
    for (i = 6; i <= 9; i++) {
        if (dead_ship_of_player[i] == false && pole_player[ship1[i-6].x][ship1[i-6].y].info == 3) {
            dead_ship_of_player[i] = true;
            hit = 0;
            if (ship1[i-6].x - 1 > -1)
                pole_player[ship1[i-6].x - 1][ship1[i-6].y].info = 2; // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
            if (ship1[i-6].x + 1 < 10)
                pole_player[ship1[i-6].x + 1][ship1[i-6].y].info = 2; // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли

            for (k = 0; k < 3; k++) {
                if ((ship1[i-6].x - 1 + k > -1) && (ship1[i-6].y - 1 > -1) && (ship1[i-6].x - 1 + k < 10))
                    pole_player[ship1[i-6].x - 1 + k][ship1[i-6].y - 1].info = 2;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                if ((ship1[i-6].x - 1 + k > -1) && (ship1[i-6].y + 1 < 10) && (ship1[i-6].x - 1 + k < 10))
                    pole_player[ship1[i-6].x - 1 + k][ship1[i-6].y + 1].info = 2;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
            }

        }
    }
}
void dead_of_ships_ai () {  // проверяет живы ли корабли компьютера, если нет то отмечает смерть и отмечает на поле пустые клетки вокруг корабля
    int i,j,k;
    if (dead_ship_of_ai[0] == false && pole_ai[ship4_ai[0].x][ship4_ai[0].y].info == 3 &&
        pole_ai[ship4_ai[1].x][ship4_ai[1].y].info == 3 && pole_ai[ship4_ai[2].x][ship4_ai[2].y].info == 3 &&
        pole_ai[ship4_ai[3].x][ship4_ai[3].y].info == 3) {
        dead_ship_of_ai[0] = true;
        for (j = 0; j < 4; j++) {
           if (ship4_ai[j].x - 1 > -1 && pole_ai[ship4_ai[j].x - 1][ship4_ai[j].y].info != 3)
               pole_ai[ship4_ai[j].x - 1][ship4_ai[j].y].info = 2; // задание на поле точки, находящихся слева от точки корабля, на которые нельзя ставить корабли
           if (ship4_ai[j].x + 1 < 10 && pole_ai[ship4_ai[j].x + 1][ship4_ai[j].y].info != 3)
               pole_ai[ship4_ai[j].x + 1][ship4_ai[j].y].info = 2; // задание на поле точки, находящихся справа от точки корабля, на которые нельзя ставить корабли
           for (i = 0; i < 3; i++) {
               if ((ship4_ai[j].x - 1 + i > -1) && (ship4_ai[j].y - 1 > -1) && (ship4_ai[j].x - 1 + i < 10) &&
                   (pole_ai[ship4_ai[j].x - 1 + i][ship4_ai[j].y - 1].info != 3))
                   pole_ai[ship4_ai[j].x - 1 + i][ship4_ai[j].y -
                                                  1].info = 2;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
               if ((ship4_ai[j].x - 1 + i > -1) && (ship4_ai[j].y + 1 < 10) && (ship4_ai[j].x - 1 + i < 10) &&
                   (pole_ai[ship4_ai[j].x - 1 + i][ship4_ai[j].y + 1].info != 3))
                   pole_ai[ship4_ai[j].x - 1 + i][ship4_ai[j].y + 1].info = 2;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
           }   // задание на поле пустых точек вокруг убитого корабля тоже самое нужно для игрока
        }
    }
    for (i = 1; i <= 2; i++) {
        if (dead_ship_of_ai[i] == false && pole_ai[ship3_ai[0][i - 1].x][ship3_ai[0][i - 1].y].info == 3 &&
        pole_ai[ship3_ai[1][i - 1].x][ship3_ai[1][i - 1].y].info == 3 &&
        pole_ai[ship3_ai[2][i - 1].x][ship3_ai[2][i - 1].y].info == 3) {
            dead_ship_of_ai[i] = true;
            for (j = 0; j < 3; j++) {
                if (ship3_ai[j][i - 1].x - 1 > -1 && pole_ai[ship3_ai[j][i - 1].x - 1][ship3_ai[j][i - 1].y].info != 3)
                    pole_ai[ship3_ai[j][i - 1].x - 1][ship3_ai[j][i -
                                                                  1].y].info = 2; // задание на поле точки, находящихся слева от точки корабля, на которые нельзя ставить корабли
                if (ship3_ai[j][i - 1].x + 1 < 10 && pole_ai[ship3_ai[j][i - 1].x + 1][ship3_ai[j][i - 1].y].info != 3)
                    pole_ai[ship3_ai[j][i - 1].x + 1][ship3_ai[j][i -
                                                                  1].y].info = 2; // задание на поле точки, находящихся справа от точки корабля, на которые нельзя ставить корабли

                for (k = 0; k < 3; k++) {
                    if ((ship3_ai[j][i - 1].x - 1 + k > -1) && (ship3_ai[j][i - 1].y - 1 > -1) &&
                        (ship3_ai[j][i - 1].x - 1 + k < 10) &&
                        (pole_ai[ship3_ai[j][i - 1].x - 1 + k][ship3_ai[j][i - 1].y - 1].info != 3))
                        pole_ai[ship3_ai[j][i - 1].x - 1 + k][ship3_ai[j][i - 1].y -
                                                              1].info = 2;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                    if ((ship3_ai[j][i - 1].x - 1 + k > -1) && (ship3_ai[j][i - 1].y + 1 < 10) &&
                        (ship3_ai[j][i - 1].x - 1 + k < 10) &&
                        (pole_ai[ship3_ai[j][i - 1].x - 1 + k][ship3_ai[j][i - 1].y + 1].info != 3))
                        pole_ai[ship3_ai[j][i - 1].x - 1 + k][ship3_ai[j][i - 1].y +
                                                              1].info = 2;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    // задание на поле пустых точек вокруг убитого корабля тоже самое нужно для игрока
                }

            }
        }
    }
    for (i = 3; i <= 5; i++) {
        if (dead_ship_of_ai[i] == false && pole_ai[ship2_ai[0][i - 3].x][ship2_ai[0][i - 3].y].info == 3 &&
            pole_ai[ship2_ai[1][i - 3].x][ship2_ai[1][i - 3].y].info == 3) {
            dead_ship_of_ai[i] = true;
            for (j = 0; j < 2; j++) {
                if (ship2_ai[j][i - 3].x - 1 > -1 && pole_ai[ship2_ai[j][i - 3].x - 1][ship2_ai[j][i - 3].y].info != 3)
                    pole_ai[ship2_ai[j][i - 3].x - 1][ship2_ai[j][i -
                                                                  3].y].info = 2; // задание на поле точки, находящихся слева от точки корабля, на которые нельзя ставить корабли
                if (ship2_ai[j][i - 3].x + 1 < 10 && pole_ai[ship2_ai[j][i - 3].x + 1][ship2_ai[j][i - 3].y].info != 3)
                    pole_ai[ship2_ai[j][i - 3].x + 1][ship2_ai[j][i -
                                                                  3].y].info = 2; // задание на поле точки, находящихся справа от точки корабля, на которые нельзя ставить корабли

                for (k = 0; k < 3; k++) {
                    if ((ship2_ai[j][i - 3].x - 1 + k > -1) && (ship2_ai[j][i - 3].y - 1 > -1) &&
                        (ship2_ai[j][i - 3].x - 1 + k < 10) &&
                        (pole_ai[ship2_ai[j][i - 3].x - 1 + k][ship2_ai[j][i - 3].y - 1].info != 3))
                        pole_ai[ship2_ai[j][i - 3].x - 1 + k][ship2_ai[j][i - 3].y -
                                                              1].info = 2;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                    if ((ship2_ai[j][i - 3].x - 1 + k > -1) && (ship2_ai[j][i - 3].y + 1 < 10) &&
                        (ship2_ai[j][i - 3].x - 1 + k < 10) &&
                        (pole_ai[ship2_ai[j][i - 3].x - 1 + k][ship2_ai[j][i - 3].y + 1].info != 3))
                        pole_ai[ship2_ai[j][i - 3].x - 1 + k][ship2_ai[j][i - 3].y +
                                                              1].info = 2;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    // задание на поле пустых точек вокруг убитого корабля тоже самое нужно для игрока
                }

            }
        }
    }
    for (i = 6; i <= 9; i++) {
            if (dead_ship_of_ai[i] == false && pole_ai[ship1_ai[i-6].x][ship1_ai[i-6].y].info == 3) {
                dead_ship_of_ai[i] = true;

                 if (ship1_ai[i-6].x - 1 > -1)
                     pole_ai[ship1_ai[i-6].x - 1][ship1_ai[i-6].y].info = 2; // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
                 if (ship1_ai[i-6].x + 1 < 10)
                     pole_ai[ship1_ai[i-6].x + 1][ship1_ai[i-6].y].info = 2; // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли

                 for (k = 0; k < 3; k++) {
                        if ((ship1_ai[i-6].x - 1 + k > -1) && (ship1_ai[i-6].y - 1 > -1) && (ship1_ai[i-6].x - 1 + k < 10))
                            pole_ai[ship1_ai[i-6].x - 1 + k][ship1_ai[i-6].y - 1].info = 2;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                        if ((ship1_ai[i-6].x - 1 + k > -1) && (ship1_ai[i-6].y + 1 < 10) && (ship1_ai[i-6].x - 1 + k < 10))
                            pole_ai[ship1_ai[i-6].x - 1 + k][ship1_ai[i-6].y + 1].info = 2;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    }

            }
    }
}
void win() {   // проверяет победил ли кто-либо; если кто-то победил, то отмечает кто победил в переменной winner
    if (xod_of_player == true && dead_ship_of_ai[0] == true && dead_ship_of_ai[1] == true && dead_ship_of_ai[2] == true && dead_ship_of_ai[3] == true && dead_ship_of_ai[4] == true && dead_ship_of_ai[5] == true && dead_ship_of_ai[6] == true && dead_ship_of_ai[7] == true && dead_ship_of_ai[8] == true && dead_ship_of_ai[9] == true) {
        winner = 'p';
    }
    if (xod_of_player == false && dead_ship_of_player[0] == true && dead_ship_of_player[1] == true && dead_ship_of_player[2] == true && dead_ship_of_player[3] == true && dead_ship_of_player[4] == true && dead_ship_of_player[5] == true && dead_ship_of_player[6] == true && dead_ship_of_player[7] == true && dead_ship_of_player[8] == true && dead_ship_of_player[9] == true) {
        winner = 'a';
    }
}

void player() { // считывает или определяет выстрел
    bool aim; // проверяет попал ли компьютер в неоткрытую клетку (1 - попал, 0 - не попал)
    if (xod_of_player == 1) { // ход игрока
        do {
            printf("введите координаты выстрела\n");
            scanf("%i%i", &shot.x, &shot.y); //A=65 a=97 сканирует координаты выстрела
            if (pole_ai[shot.x][shot.y].info != 0 && pole_ai[shot.x][shot.y].info != 1){ // проверка на то, что стрелял игрок в эту точку или нет
            printf("вы ошиблись\n");
            }
    }
        while (pole_ai[shot.x][shot.y].info != 0 && pole_ai[shot.x][shot.y].info != 1);
    }
    else { // ход компьютера
       // xod_of_player == 1;
        switch (hit){ // точка выстрела определяется в зависимости от количества попаданий по 1-му кораблю до его потопления
            case 0 :{ // ни разу не попал => выбираем рандомную точку, в которую можно стрелять
                aim = false; // сначала компьютер никуда не попал
                while (aim == false) {
                    shot.x = rand() % 10;
                    shot.y = rand() % 10;
                    if (pole_player[shot.x][shot.y].info == 0 || pole_player[shot.x][shot.y].info == 1) aim = true;
                }
                break;
            }
            case 1:{ // попал один раз => стреляем рядом с попаданием
                aim = false; // сначала компьютер никуда не попал
                do {
                    switch (rand() % 4) { // выбирает направления выбора следующей точки корабля; 0 - вверх, 1 - влево, 2 - вниз , 3 - вправо
                        case 0: { // вверх
                            if (shot_ai[0].y - 1 >= 0 && (pole_player[shot_ai[0].x][shot_ai[0].y - 1].info == 0 || pole_player[shot_ai[0].x][shot_ai[0].y - 1].info == 1)) { // существует ли такая клетка на поле и можно открыта ли она
                                shot.x = shot_ai[0].x;
                                shot.y = shot_ai[0].y - 1;
                                aim = true;
                            }
                        }
                        case 1: { // влево
                            if (shot_ai[0].x - 1 >= 0 && (pole_player[shot_ai[0].x - 1][shot_ai[0].y].info == 0 || pole_player[shot_ai[0].x - 1][shot_ai[0].y].info == 1)) { // существует ли такая клетка на поле и можно открыта ли она
                                shot.y = shot_ai[0].y;
                                shot.x = shot_ai[0].x - 1;
                                aim = true;
                            }
                        }
                        case 2: { // вниз
                            if (shot_ai[0].y + 1 <= 9 && (pole_player[shot_ai[0].x][shot_ai[0].y + 1].info == 0 || pole_player[shot_ai[0].x][shot_ai[0].y + 1].info == 1)) { // существует ли такая клетка на поле и можно открыта ли она
                                shot.x = shot_ai[0].x;
                                shot.y = shot_ai[0].y + 1;
                                aim = true;
                            }
                        }
                        case 3: { // вправо
                            if (shot_ai[0].x + 1 <= 9 && (pole_player[shot_ai[0].x + 1][shot_ai[0].y].info == 0 || pole_player[shot_ai[0].x + 1][shot_ai[0].y].info == 1)) { // существует ли такая клетка на поле и можно открыта ли она
                                shot.y = shot_ai[0].y;
                                shot.x = shot_ai[0].x + 1;
                                aim = true;
                            }
                        }
                    }
                }
                while (aim == false);
                break;
            }
            default:{ // попал 2 или более раз => стрелять надо рядом с запомненными точками
                if (shot_ai[0].x == shot_ai[1].x) { // корабль стоит вертикально
                    aim = false;
                    while (aim == false) {
                        switch (rand() % 4){ //
                            case 0: { // стреляем в точку, находящуюся выше shot_ai[0].y ( - 1 )
                                if (shot_ai[0].y - 1 >= 0 && pole_player[shot_ai[0].x][shot_ai[0].y - 1].info != 2 && pole_player[shot_ai[0].x][shot_ai[0].y - 1].info != 3) {
                                    shot.x = shot_ai[0].x;
                                    shot.y = shot_ai[0].y - 1;
                                    aim = true;
                                }
                                break;
                            }
                            case 1: { // стреляем в точку, находящуюся ниже shot_ai[0].y
                                if (shot_ai[0].y + 1 <= 9 && pole_player[shot_ai[0].x][shot_ai[0].y + 1].info != 2 && pole_player[shot_ai[0].x][shot_ai[0].y + 1].info != 3) {
                                    shot.x = shot_ai[0].x;
                                    shot.y = shot_ai[0].y + 1;
                                    aim = true;
                                }
                                break;
                            }
                            case 2: { // стреляем в точку, находящуюся выше shot_ai[1].y
                                if (shot_ai[1].y - 1 >= 0 && pole_player[shot_ai[1].x][shot_ai[1].y - 1].info != 2 && pole_player[shot_ai[1].x][shot_ai[1].y - 1].info != 3) {
                                    shot.x = shot_ai[1].x;
                                    shot.y = shot_ai[1].y - 1;
                                    aim = true;
                                }
                                break;
                            }
                            case 3: { // стреляем в точку, находящуюся ниже shot_ai[1].y
                                if (shot_ai[1].y + 1 <= 9 && pole_player[shot_ai[1].x][shot_ai[1].y + 1].info != 2 && pole_player[shot_ai[1].x][shot_ai[1].y + 1].info != 3) {
                                    shot.x = shot_ai[1].x;
                                    shot.y = shot_ai[1].y + 1;
                                    aim = true;
                                }
                                break;
                            }
                        }
                    }
                }
                else { // корабль стоит горизонтально
                    aim = false;
                    while (aim == false) {
                        switch (rand() % 4){ //
                            case 0: { // стреляем в точку, находящуюся левее shot_ai[0].x
                                if (shot_ai[0].x - 1 >= 0 && pole_player[shot_ai[0].x - 1][shot_ai[0].y].info != 2 && pole_player[shot_ai[0].x - 1][shot_ai[0].y].info != 3) {
                                    shot.y = shot_ai[0].y;
                                    shot.x = shot_ai[0].x - 1;
                                    aim = true;
                                }
                                break;
                            }
                            case 1: { // стреляем в точку, находящуюся правее shot_ai[0].x
                                if (shot_ai[0].x + 1 <= 9 && pole_player[shot_ai[0].x + 1][shot_ai[0].y].info != 2 && pole_player[shot_ai[0].x + 1][shot_ai[0].y].info != 3) {
                                    shot.y = shot_ai[0].y;
                                    shot.x = shot_ai[0].x + 1;
                                    aim = true;
                                }
                                break;
                            }
                            case 2: { // стреляем в точку, находящуюся левее shot_ai[1].x
                                if (shot_ai[1].x - 1 >= 0 && pole_player[shot_ai[1].x - 1][shot_ai[1].y].info != 2 && pole_player[shot_ai[1].x - 1][shot_ai[1].y].info != 3) {
                                    shot.y = shot_ai[1].y;
                                    shot.x = shot_ai[1].x - 1;
                                    aim = true;
                                }
                                break;
                            }
                            case 3: { // стреляем в точку, находящуюся правее shot_ai[1].x
                                if (shot_ai[1].x + 1 <= 9 && pole_player[shot_ai[1].x + 1][shot_ai[1].y].info != 2 && pole_player[shot_ai[1].x + 1][shot_ai[1].y].info != 3) {
                                    shot.y = shot_ai[1].y;
                                    shot.x = shot_ai[1].x + 1;
                                    aim = true;
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }

    }
}
void shot_of_gamer(){ // работает с координатами выстрела
    if (xod_of_player == true){ // ход игрока
        switch(pole_ai[shot.x][shot.y].info){
            case 0: { // не попал
                pole_ai[shot.x][shot.y].info = 2; // отметка на поле
                xod_of_player = false; // смена хода
                break;
            }
            case 1: { // попал
                pole_ai[shot.x][shot.y].info = 3; // отметка на поле
                dead_of_ships_ai (); // убит ли корабль?
                break;
            }
        }
    }
    else { // ход компьютера
        switch(pole_player[shot.x][shot.y].info) {
            case 0: { // не попал
                pole_player[shot.x][shot.y].info = 2; // отметка на поле
                xod_of_player = true; // смена хода
                break;
            }
            case 1: { // ПОПАЛ (для этого есть  shot_ai[2].(x or y)) нужно запомнить точку(-и) попадания!!!!!!!!!!!!!!!!!!
                pole_player[shot.x][shot.y].info = 3; // отметка на поле
                switch (hit) { // запоминание точек попадания в зависимоти от количества попаданий до этого момента
                    case 0:{ // до этого момента ни разу не попал
                        shot_ai[0].x = shot.x;
                        shot_ai[0].y = shot.y;
                        break;
                    }
                    case 1:{ // до этого момента попал один раз
                        shot_ai[1].x = shot.x;
                        shot_ai[1].y = shot.y;
                        break;
                    }
                    default:{ // ДО ЭТОГО МОМЕНТА ПОПАЛ 2 ИЛИ БОЛЕЕ РАЗ  shot_ai[0] и shot_ai[1] должны быть крайними открытыми точками корабля
                        if (shot_ai[0].x == shot_ai[1].x) { // корабль стоит вертикально
                            if (abs(shot_ai[1].y - shot.y) < abs(shot_ai[0].y - shot.y)) shot_ai[1].y = shot.y; // минимальное расстояние от точки выстрела
                            else shot_ai[0].y = shot.y;
                        }
                        else {
                            if (abs(shot_ai[1].x - shot.x) < abs(shot_ai[0].x - shot.x)) shot_ai[1].x = shot.x;
                            else shot_ai[0].x = shot.x;
                        }
                        break;
                    }
                }
                hit = hit + 1; // попал на один раз больше по одному кораблю
                dead_of_ships_player (); // убит ли корабль?
                break;
            }
        }
                                                            //xod_of_player = true;
    }
}
void graphic(){
    int x,y;
    printf("\n");
    printf("     компьютер                    игрок\n");
    for (x = 1;x <= 47;x++) printf("-");
    printf("\n");
    for (y = -1; y <= 9; y++) {
        if (y != -1) {
            printf("%i", y); // координаты по оси у
            for (x = 1; x <= 21; x++) {
                if (x % 2 == 1) printf("|"); // граница между клетками по горизонтали
                else {
                    switch (pole_ai[(x - 2) / 2][y].info) { // клетки
                        case 2: { // пустая клетка
                            printf("0");
                            break;
                        }
                        case 3: { // клетка с кораблём
                            printf("1");
                            break;
                        }
                        default: { // неоткрытая клетка
                            printf("&");
                            break;
                        }
                    }
                }
            }



            printf(" | ");
            printf("%i", y); // координаты по оси у
            for (x = 1; x <= 21; x++) {
                if (x % 2 == 1) printf("|"); // граница между клетками по горизонтали
                else {
                    switch (pole_player[(x - 2) / 2][y].info) { // клетки
                        case 0: { // неоткрытая без корабля
                            printf(" ");
                            break;
                        }
                        case 1: { // неоткрытая с кораблём
                            printf("*");
                            break;
                        }
                        case 2: { // открытая без корабля
                            printf("0");
                            break;
                        }
                        case 3: { // открытая с кораблём
                            printf("1");
                            break;
                        }
                    }
                }
            }




            printf("\n");
            for (x = 1; x <= 22; x++) printf("-"); // граница между клетками по вертикали

            printf(" | ");
            for (x = 1; x <= 22; x++) printf("-"); // граница между клетками по вертикали




        }
        else { // координаты по оси х
            printf(" ");
            for (x = 1; x <= 21; x++) {
                if (x % 2 == 1) printf("|"); // границы между клетками по горизонтали
                else {
                    printf("%i",(x-2)/2); // координаты по оси х
                }
            }


            printf(" | ");
            printf(" ");
            for (x = 1; x <= 21; x++) {
                if (x % 2 == 1) printf("|");
                else {
                    printf("%i",(x-2)/2);
                }
            }



            printf("\n");
            for (x = 1; x <= 22; x++) printf("-");

            printf(" | ");
            for (x = 1; x <= 22; x++) printf("-");




        }
        printf("\n");
    }
}
int main() {
    int i,j;
    winner = 'n';
    for (i=0;i<10;i++) { // обнуление клеток полей и корабли не мертвы
        dead_ship_of_ai[i] = false; // все корабли живы
        dead_ship_of_player[i] = false; // все корабли живы
        for (j = 0; j < 10; j++) {
            pole_player[i][j].info = 0;
            pole_ai[i][j].info = 0;
        }
    }
   // printf("Введите координаты начала и конца четырёхпалубника\n");
    scanf("%i%i%i%i",&ship4[0].x,&ship4[0].y,&ship4[3].x,&ship4[3].y); // координаты начала и конца
   // printf("Введите координаты начала и конца первого трёхпалубника, затем координаты начала и конца второго трёхпалубника\n");
    scanf("%i%i%i%i%i%i%i%i",&ship3[0][0].x,&ship3[0][0].y,&ship3[2][0].x,&ship3[2][0].y,&ship3[0][1].x,&ship3[0][1].y,&ship3[2][1].x,&ship3[2][1].y);  // координаты начала и конца
   // printf("Введите координаты начала и конца двухпалубников\n");
    scanf("%i%i%i%i%i%i%i%i%i%i%i%i",&ship2[0][0].x,&ship2[0][0].y,&ship2[1][0].x,&ship2[1][0].y,&ship2[0][1].x,&ship2[0][1].y,&ship2[1][1].x,&ship2[1][1].y,&ship2[0][2].x,&ship2[0][2].y,&ship2[1][2].x,&ship2[1][2].y);  // координаты начала и конца
   // printf("Введите координаты однопалубников\n");
    scanf("%i%i%i%i%i%i%i%i",&ship1[0].x,&ship1[0].y,&ship1[1].x,&ship1[1].y,&ship1[2].x,&ship1[2].y,&ship1[3].x,&ship1[3].y);  // координаты начала и конца
    point();
    srand ( time(NULL) );
    ship_ai();
    for (i = 0;i < 10;i++) // обнуление клеток поля компьютера
        for (j = 0;j < 10;j++) {
            if (pole_ai[i][j].info != 0 && pole_ai[i][j].info != 1 ) {
                pole_ai[i][j].info = 0;
            }
        }
   if (rand()%2==1) xod_of_player = 1; // выбирается, кто ходит первым
   else xod_of_player = 0;
    xod_of_player = 1;
    /* for (i=0;i<10;i++) {
        for (j = 0; j < 10; j++) {
            printf("%i ",pole_ai[i][j].info);
        }
        printf("\n");
    } */
    graphic_develop();
    graphic();
    while (winner == 'n') {
        player();
        shot_of_gamer();
        graphic();
        win();
        graphic_develop();
    }
    if (winner == 'a') {
        printf ("YOU LOSE");
    }
    if (winner == 'p'){
        printf("YOU WIN");
    }
    return 0; //srand ( time(NULL) ); rand()%10
}