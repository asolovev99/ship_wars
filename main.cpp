#include <iostream>
struct pole {
    int info;
    int num_of_ship;
};
struct ship {
    int x;
    int y;
};
bool dead_ship_of_player [10]; // (true => dead, false => alive)
bool dead_ship_of_ai[10]; // (true => dead, false => alive)
char winner; // определяет победителя (победил: p - игрок, a - компьютер, n - пока что никто не победил)
bool xod_of_player; // определяет кто сейчас стреляет ( true/1 => ходит игрок, false/0 => ходит компьютер )
pole pole_player[10][10]; // 0 ~ неоткрытая клетка без корабля, 1 ~ неоткрытая клетка с кораблём, 2 ~ открытая клетка без корабля, 3 ~ открытая клетка с кораблём, 4 ~ клетка, на которую нельзя ставить корабль (= 0..9)
pole pole_ai[10][10];  // = (0..9)
bool aim; // проверяет попал ли компьютер по кораблю (1 - попал, 0 - не попал)
ship shot; // координаты выстрела
ship ship1[4]; // [номер корабля] (= 0..9)
ship ship2[2][3]; // [номер клетки][номер корабля] (= 0..9)
ship ship3[3][2]; // [номер клетки][номер корабля] (= 0..9)
ship ship4[4]; // [номер клетки]
ship ship1_ai[4]; // [номер клетки] компьютер
ship ship2_ai[2][3]; //[номер клетки][номер корабля] компьютер
ship ship3_ai[3][2]; //[номер клетки][номер корабля] компьютер
ship ship4_ai[4]; //[номер корабля] компьютер
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
    for (j = 0; j < 3; j++){ // задание двухпалубников

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
                            pole_ai[ship2_ai[i][j].x - 1][ship2_ai[i][j].y].info = 4; // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
                        if (ship2_ai[i][j].x + 1 < 10)
                            pole_ai[ship2_ai[i][j].x + 1][ship2_ai[i][j].y].info = 4; // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли
                    }
                    for (i = 0; i < 3; i++) {
                        if ((x - 1 + i > -1) && (y - 1 > -1) && (x - 1 + i < 10))
                            pole_ai[x - 1 + i][y - 1].info = 4;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                        if ((x - 1 + i > -1) && (y + 2 < 10) && (x - 1 + i < 10))
                            pole_ai[x - 1 + i][y + 2].info = 4;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    }
                }
            }
        }


        else { // корабль стоит горизонтально
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
                            pole_ai[ship2_ai[i][j].x][ship2_ai[i][j].y - 1].info = 4; // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
                        if (ship2_ai[i][j].y + 1 < 10)
                            pole_ai[ship2_ai[i][j].x][ship2_ai[i][j].y + 1].info = 4; // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
                    }
                    for (i = 0; i < 3; i++) {
                        if ((y - 1 + i > -1) && (x - 1 > -1) && (y - 1 + i < 10))
                            pole_ai[x - 1][y - 1 + i].info = 4;  // задание на поле точек, находящихся левее корабля, на которые нельзя ставить корабли
                        if ((y - 1 + i > -1) && (x + 2 < 10) && (y - 1 + i < 10))
                            pole_ai[x + 2][y - 1 + i].info = 4;  // задание на поле точек, находящихся правее корабля, на которые нельзя ставить корабли
                    }
                }
            }



        } // конец задания двухпалубников

        for (j = 0; j < 4;j++) { //
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





}
void point (){ //задание кораблей игрока
    int i;
    int j;
    if (ship4[0].x>ship4[3].x) {  //задание и упорядочивание точек 4-ёх плубника
        ship4[0].x=ship4[0].x+ship4[3].x;
        ship4[3].x=ship4[0].x-ship4[3].x;
        ship4[0].x=ship4[0].x-ship4[3].x; //x0=min(x0,x3) x3=max(x0,x3)
        ship4[1].x= ship4[0].x+1; //x1=x0+1
        ship4[2].x= ship4[0].x+2; //x2=x0+2
        ship4[1].y = ship4[0].y; //y1=y0
        ship4[2].y = ship4[0].y; //y2=y0
    }
    else {
        if (ship4[0].y > ship4[3].y) {
            ship4[0].y = ship4[0].y + ship4[3].y;
            ship4[3].y = ship4[0].y - ship4[3].y;
            ship4[0].y = ship4[0].y - ship4[3].y; //y0=min(y0,y3) y3=max(y0,y3)
            ship4[1].y=ship4[0].y+1; //y1=y0+1
            ship4[2].y=ship4[0].y+2; //y2=y0+2
            ship4[1].x=ship4[0].x; //x1=x0
            ship4[2].x=ship4[0].x; //x2=x0

        }
    }
    for (j=0;j<4;j++)
        pole_player[ship4[j].x][ship4[j].y].info=1; //задание и упорядочивание точек 4-ёх палубника на поле игрока
    for (i=0;i<2;i++) {
        if (ship3[0][i].x > ship3[2][i].x) { //задание точек 3-ёх плубника
            ship3[0][i].x = ship3[0][i].x + ship3[2][i].x;
            ship3[2][i].x = ship3[0][i].x - ship3[2][i].x;
            ship3[0][i].x = ship3[0][i].x - ship3[2][i].x; //x0=min(x0,x2) x3=max(x0,x2)
            ship3[1][i].x = ship3[0][i].x+1; //x1=x0+1
            ship3[1][i].y = ship3[0][i].y; //y1=y0
        }
        else
            if (ship3[0][i].y > ship3[2][i].y) {
            ship3[0][i].y = ship3[0][i].y + ship3[2][i].y;
            ship3[2][i].y = ship3[0][i].y - ship3[2][i].y;
            ship3[0][i].y = ship3[0][i].y - ship3[2][i].y; //y0=min(y0,y3) y3=max(y0,y3)
            ship3[1][i].y = ship3[0][i].y+1; //y1=y0+1
            ship3[1][i].x = ship3[0][i].x; //x1=x0

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
        else if (ship2[0][i].y > ship2[1][i].y) {
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
    int i;
    if (dead_ship_of_player[0] == false && pole_player[ship4[0].x][ship4[0].y].info == 3 &&
        pole_player[ship4[1].x][ship4[1].y].info == 3 && pole_player[ship4[2].x][ship4[2].y].info == 3 &&
        pole_player[ship4[3].x][ship4[3].y].info == 3) {
        dead_ship_of_player[0] == true;
    }
    for (i = 1; i <= 2; i++) {
        if (dead_ship_of_player[i] == false && pole_player[ship3[0][i].x][ship3[0][i].y].info == 3 &&
            pole_player[ship3[1][i].x][ship3[1][i].y].info == 3 &&
            pole_player[ship3[2][i].x][ship3[2][i].y].info == 3) {
            dead_ship_of_player[i] == true;
        }
    }
    for (i = 3; i <= 5; i++) {
        if (dead_ship_of_player[i] == false && pole_player[ship2[0][i].x][ship2[0][i].y].info == 3 &&
            pole_player[ship2[1][i].x][ship2[1][i].y].info == 3) {
            dead_ship_of_player[i] == true;
        }
        for (i = 6; i <= 9; i++) {
            if (dead_ship_of_player[i] == false && pole_player[ship1[i].x][ship1[i].y].info == 3) {
                dead_ship_of_player[i] == true;
            }
        }
    }
}
void dead_of_ships_ai () {  // проверяет живы ли корабли компьютера, если нет то отмечает смерть и отмечает на поле пустые клетки вокруг корабля
    int i;
    if (dead_ship_of_ai[0] == false && pole_ai[ship4_ai[0].x][ship4_ai[0].y].info == 3 &&
        pole_ai[ship4_ai[1].x][ship4_ai[1].y].info == 3 && pole_ai[ship4_ai[2].x][ship4_ai[2].y].info == 3 &&
        pole_ai[ship4_ai[3].x][ship4_ai[3].y].info == 3) {
        dead_ship_of_ai[0] == true;


     /*   if (ship4_ai[j].x - 1 > -1)
            pole_ai[ship4_ai[j].x - 1][ship4_ai[j].y].info = 4; // задание на поле точек, находящихся слева от корабля, на которые нельзя ставить корабли
        if (ship4_ai[j].x + 1 < 10)
            pole_ai[ship4_ai[j].x + 1][ship4_ai[j].y].info = 4; // задание на поле точек, находящихся справа от корабля, на которые нельзя ставить корабли

        for (i = 0; i < 3; i++) {
            if ((x - 1 + i > -1) && (y - 1 > -1) && (x - 1 + i < 10))
                pole_ai[x - 1 + i][y - 1].info = 4;  // задание на поле точек, находящихся выше корабля, на которые нельзя ставить корабли
            if ((x - 1 + i > -1) && (y + 1 < 10) && (x - 1 + i < 10))
                pole_ai[x - 1 + i][y + 1].info = 4;  // задание на поле точек, находящихся ниже корабля, на которые нельзя ставить корабли
       */ // задание на поле пустых точек вокруг убитого корабля тоже самое нужно для игрока

    }
    for (i = 1; i <= 2; i++) {
        if (dead_ship_of_ai[i] == false && pole_ai[ship3_ai[0][i].x][ship3_ai[0][i].y].info == 3 &&
            pole_ai[ship3_ai[1][i].x][ship3_ai[1][i].y].info == 3 &&
            pole_ai[ship3_ai[2][i].x][ship3_ai[2][i].y].info == 3) {
            dead_ship_of_ai[i] == true;
        }
    }
    for (i = 3; i <= 5; i++) {
        if (dead_ship_of_ai[i] == false && pole_ai[ship2_ai[0][i].x][ship2_ai[0][i].y].info == 3 &&
            pole_ai[ship2_ai[1][i].x][ship2_ai[1][i].y].info == 3) {
            dead_ship_of_ai[i] == true;
        }
        for (i = 6; i <= 9; i++) {
            if (dead_ship_of_ai[i] == false && pole_ai[ship1_ai[i].x][ship1_ai[i].y].info == 3) {
                dead_ship_of_ai[i] == true;
            }
        }
    }
}
void win (){ // проверяет победил ли кто-либо; если кто-то победил, то отмечает кто победил в переменной winner
    if (xod_of_player == true && dead_ship_of_ai[0] == true && dead_ship_of_ai[1] == true && dead_ship_of_ai[2] == true && dead_ship_of_ai[3] == true && dead_ship_of_ai[4] == true && dead_ship_of_ai[5] == true && dead_ship_of_ai[6] == true && dead_ship_of_ai[7] == true && dead_ship_of_ai[8] == true && dead_ship_of_ai[9] == true) {
        winner = 'p';
    }
    if (xod_of_player == false && dead_ship_of_player[0] == true && dead_ship_of_player[1] == true && dead_ship_of_player[2] == true && dead_ship_of_player[3] == true && dead_ship_of_player[4] == true && dead_ship_of_player[5] == true && dead_ship_of_player[6] == true && dead_ship_of_player[7] == true && dead_ship_of_player[8] == true && dead_ship_of_player[9] == true) {
        winner = 'a';
    }
}
void player() { // считывает или определяет выстрел
    if (xod_of_player == 1) {
        do {
            printf("введите координаты выстрела");
            scanf("%i%i", &shot.x, &shot.y); //A=65 a=97
            if (pole_ai[shot.x][shot.y].info != 0 && pole_ai[shot.x][shot.y].info != 1){
            printf("вы ошиблись");
            }
    }
        while (pole_ai[shot.x][shot.y].info != 0 && pole_ai[shot.x][shot.y].info != 1);
    }
    else {


    }
}
void shot_of_gamer(){ // работает с координатами выстрела
if (xod_of_player == 1){
    switch(pole_ai[shot.x][shot.y].info){
        case 0: {
            pole_ai[shot.x][shot.y].info = 2;
            xod_of_player = false;
            break;
        }
        case 1: {
            pole_ai[shot.x][shot.y].info = 3;
            break;
        }
    }
}
else {


   }
   win();
}
int main() {
    int i,j;
    winner = 'n';
    for (i=0;i<10;i++) // обнуление клеток полей и корабли не мертвы
        dead_ship_of_ai[i] = false; // все корабли живы
        dead_ship_of_player[i] = false; // все корабли живы
        for (j=0;j<10;j++) {
            pole_player[i][j].info = 0;
            pole_ai[i][j].info = 0;
        }
    //printf();
    scanf("%i%i%i%i",&ship4[0].x,&ship4[0].y,&ship4[3].x,&ship4[3].y); // координаты начала и конца
    //printf();
    scanf("%i%i%i%i%i%i%i%i",&ship3[0][0].x,&ship3[0][0].y,&ship3[2][0].x,&ship3[2][0].y,&ship3[0][1].x,&ship3[0][1].y,&ship3[2][1].x,&ship3[2][1].y);  // координаты начала и конца
    //printf();
    scanf("%i%i%i%i%i%i%i%i%i%i%i%i",&ship2[0][0].x,&ship2[0][0].y,&ship2[1][0].x,&ship2[1][0].y,&ship2[0][1].x,&ship2[0][1].y,&ship2[1][1].x,&ship2[1][1].y,&ship2[0][2].x,&ship2[0][2].y,&ship2[1][2].x,&ship2[1][2].y);  // координаты начала и конца
    //printf();
    scanf("%i%i%i%i%i%i%i%i",&ship1[0].x,&ship1[0].y,&ship1[1].x,&ship1[1].y,&ship1[2].x,&ship1[2].y,&ship1[3].x,&ship1[3].y);  // координаты начала и конца
    point();
    srand ( time(NULL) );
    ship_ai();
    for (i=0;i<10;i++) // обнуление клеток поля компьютера
        for (j=0;j<10;j++) {
            if (pole_ai[i][j].info != 0 && pole_ai[i][j].info != 1 ) {
                pole_ai[i][j].info = 0;
            }
        }
    if (rand()%2==1) xod_of_player = 1; // выбирается, кто ходит первым
    else xod_of_player = 0;
    while (winner == 'n') {
        player();
        shot_of_gamer();
    }
    if (winner == 'a') {
        printf ("YOU LOSE");
    }
    if (winner == 'p'){
        printf("YOU WIN");
    }
    return 0;
}//srand ( time(NULL) ); rand()%10