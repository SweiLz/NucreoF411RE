#include "mbed.h"
#include "my8x8RGB.h"
#include "Gamecontrol.h"

int color_select[8][8][4];
int map[80][80][4]={0};
uint8_t i,j,k;
uint8_t state=0;
uint8_t ci=9,cj=18;
uint8_t mi=0,mj=0;
uint8_t now_ci=0,now_cj=0;
uint8_t last_ci=9,last_cj=18;
uint8_t finalfirst=0;
Serial fibo(SERIAL_TX, SERIAL_RX);
int itemtime=0;
void gamestart()
{
    
    onmap();
        color_choose(color_select);
        Showscreen();
     //   if(fibo.readable()) {
      //      state=fibo.getc();
            move();
     //   }
}

void onmap()
{
    mi=0;
    mj=0;
    for(i=last_ci-3; i<=last_ci+4; i++) {
        for(j=last_cj-4; j<=last_cj+3; j++) {
            for(k=0; k<=3; k++) {
                color_select[mi][mj][k]=map[i][j][k];
            }
            mj++;
        }
        mi++;
        mj=0;
    }
    mi=0;
}

void setup_game()
{
    char wb[3]= {100,100,100};
    INITRGB(wb);
    set_wall();
    character_position(ci,cj);
    setitem();
    
}

void setitem()
{
    map[31][69][0]=137;//key
    map[31][69][1]=255;
    map[31][69][2]=0;
    map[31][69][3]=2;


    map[41][57][0]=0;//portal
    map[41][57][1]=44;
    map[41][57][2]=102;
    map[41][57][3]=4;
    
    map[19][9][0]=0;//portal
    map[19][9][1]=44;
    map[19][9][2]=102;
    map[19][9][3]=5;

    map[39][67][0]=255;//door
    map[39][67][1]=0;
    map[39][67][2]=0;
    map[39][67][3]=3;

    map[39][69][0]=142;//finish
    map[39][69][1]=245;
    map[39][69][2]=40;
    map[39][69][3]=9;
    
    map[9][15][0]=123;//plus
    map[9][15][1]=106;
    map[9][15][2]=128;
    map[9][15][3]=6;
    
    map[9][13][0]=123;//de
    map[9][13][1]=106;
    map[9][13][2]=128;
    map[9][13][3]=7;
}

void character_position(int character_x,int character_y)
{
    now_ci=character_x;
    now_cj=character_y;

    map[last_ci][last_cj][0]=0;
    map[last_ci][last_cj][1]=0;
    map[last_ci][last_cj][2]=0;
    map[last_ci][last_cj][3]=0;

    map[character_x][character_y][0]=0;
    map[character_x][character_y][1]=255;
    map[character_x][character_y][2]=0;
    item_check();
    safety_wall();




    last_ci=now_ci,last_cj=now_cj;
    ci=now_ci;
    cj=now_cj;
}

void teleport()
{
    map[47][41][0]=125;
    map[47][41][1]=125;
    map[47][41][2]=125;
    map[ now_ci][ now_cj][0]=0;
    map[ now_ci][ now_cj][1]=44;
    map[ now_ci][ now_cj][2]=102;
    now_ci=47;
    now_cj=41;
}
void teleport1()
{
    map[43][11][0]=125;
    map[43][11][1]=125;
    map[43][11][2]=125;
    map[ now_ci][ now_cj][0]=0;
    map[ now_ci][ now_cj][1]=44;
    map[ now_ci][ now_cj][2]=102;
    now_ci=43;
    now_cj=11;
}
int timeformitem()
{
    return itemtime;
    }
    
void plustime()
{
itemtime+=50;    
}

void decreasetime()
{
itemtime-=50;    
}
void finally()
{

    while(1) {
        if(finalfirst==0) {
            for(i=0; i<=7; i++) {
                for(j=0; j<=1; j++)

                {
                    color_select[i][j][0]=0;
                    color_select[i][j][1]=0;
                    color_select[i][j][2]=255;
                    
                }
                for(j=2; j<=4; j++)

                {
                    color_select[i][j][0]=0;
                    color_select[i][j][1]=255;
                    color_select[i][j][2]=255;
                   
                }
                for(j=5; j<=7; j++)

                {
                    color_select[i][j][0]=0;
                    color_select[i][j][1]=255;
                    color_select[i][j][2]=0;
                    
                }
            }
            color_choose(color_select);
            finalfirst=1;
            
        }
        Showscreen();

    }
}

void wall(int wall_x,int wall_y)
{
    map[wall_x][wall_y][0]=0;
    map[wall_x][wall_y][1]=0;
    map[wall_x][wall_y][2]=100;
    map[wall_x][wall_y][3]=1;
}

void item_check()
{
        if(map[now_ci][now_cj][3]==2) {
        map[39][67][0]=0;
        map[39][67][1]=0;
        map[39][67][2]=0;
        map[39][67][3]=0;
    }

    if(map[now_ci][now_cj][3]==4) {
        teleport();
    }
    if(map[now_ci][now_cj][3]==5) {
    teleport1();}
    if(map[now_ci][now_cj][3]==9) {
        finally();
    }
    if(map[now_ci][now_cj][3]==6) {
        plustime();
    }
    if(map[now_ci][now_cj][3]==7) {
        decreasetime();
    }
}
void safety_wall()
{


    if(map[now_ci][now_cj][3]==1) {
        map[last_ci][last_cj][0]=0;
        map[last_ci][last_cj][1]=255;
        map[last_ci][last_cj][2]=255;
        map[last_ci][last_cj][3]=0;

        map[now_ci][now_cj][0]=255;
        map[now_ci][now_cj][1]=0;
        map[now_ci][now_cj][2]=255;
        map[now_ci][now_cj][3]=1;
        now_ci=last_ci;
        now_cj=last_cj;



    } else if(map[now_ci][now_cj][3]==3) {


        map[last_ci][last_cj][0]=0;
        map[last_ci][last_cj][1]=255;
        map[last_ci][last_cj][2]=255;
        map[last_ci][last_cj][3]=0;

        map[now_ci][now_cj][0]=255;
        map[now_ci][now_cj][1]=0;
        map[now_ci][now_cj][2]=0;
        map[now_ci][now_cj][3]=3;
        now_ci=last_ci;
        now_cj=last_cj;

    }


    else {

        map[now_ci][now_cj][3]=0;
    }
}

void moving(int moving_position)
{
    state=moving_position;
 //   Showscreen();
}

void move()
{



    if(state==97) {
        ci++;

        if(ci==8) {
            ci=7;
        }
        character_position(ci,cj);


        state=0;
    } else if(state==100) {
        ci--;
        if(ci==255) {
            ci=0;
        }

        character_position(ci,cj);


        state=0;

    }


    else if(state==119) {
        cj++;
        if(cj==8) {
            cj=7;
        }

        character_position(ci,cj);


        state=0;
    }


    else if(state==115) {
        cj--;
        if(cj==255) {
            cj=0;
        }

        character_position(ci,cj);


        state=0;
    }
    
}


void set_wall()
{
    for(i=8;i<=20;i++)
    {
        wall(8,i);
        wall(i,8);
    }
    
    for(i=10;i<=70;i++)
    {
        wall(i,10);
        wall(i,70);
        wall(10,i);
        wall(70,i);
    }
    


 wall(9,20);
 wall(20,9);   
 wall(34,11);
wall(44,11);
wall(50,11);
wall(62,11);
wall(64,11);

wall(12,12);
wall(13,12);
wall(14,12);
wall(15,12);
wall(16,12);
wall(17,12);
wall(18,12);
wall(19,12);
wall(20,12);
wall(21,12);
wall(22,12);
wall(23,12);
wall(24,12);
wall(25,12);
wall(26,12);
wall(27,12);
wall(28,12);
wall(29,12);
wall(30,12);
wall(31,12);
wall(32,12);
wall(34,12);
wall(36,12);
wall(37,12);
wall(38,12);
wall(39,12);
wall(40,12);
wall(41,12);
wall(42,12);
wall(44,12);
wall(46,12);
wall(47,12);
wall(48,12);
wall(50,12);
wall(52,12);
wall(53,12);
wall(54,12);
wall(56,12);
wall(57,12);
wall(58,12);
wall(59,12);
wall(60,12);
wall(61,12);
wall(62,12);
wall(64,12);
wall(65,12);
wall(66,12);
wall(67,12);
wall(68,12);

wall(12,13);
wall(18,13);
wall(22,13);
wall(30,13);
wall(32,13);
wall(34,13);
wall(36,13);
wall(42,13);
wall(44,13);
wall(50,13);
wall(54,13);
wall(58,13);
wall(68,13);

wall(12,14);
wall(14,14);
wall(16,14);
wall(17,14);
wall(18,14);
wall(20,14);
wall(21,14);
wall(22,14);
wall(24,14);
wall(25,14);
wall(26,14);
wall(27,14);
wall(28,14);
wall(30,14);
wall(32,14);
wall(34,14);
wall(36,14);
wall(37,14);
wall(38,14);
wall(40,14);
wall(42,14);
wall(44,14);
wall(46,14);
wall(47,14);
wall(48,14);
wall(49,14);
wall(50,14);
wall(51,14);
wall(52,14);
wall(53,14);
wall(54,14);
wall(58,14);
wall(60,14);
wall(62,14);
wall(63,14);
wall(64,14);
wall(65,14);
wall(66,14);
wall(67,14);
wall(68,14);

wall(12,15);
wall(14,15);
wall(18,15);
wall(20,15);
wall(22,15);
wall(24,15);
wall(28,15);
wall(32,15);
wall(34,15);
wall(38,15);
wall(40,15);
wall(42,15);
wall(44,15);
wall(56,15);
wall(60,15);
wall(66,15);

wall(12,16);
wall(13,16);
wall(14,16);
wall(15,16);
wall(16,16);
wall(18,16);
wall(20,16);
wall(22,16);
wall(24,16);
wall(26,16);
wall(27,16);
wall(28,16);
wall(29,16);
wall(30,16);
wall(31,16);
wall(32,16);
wall(34,16);
wall(36,16);
wall(38,16);
wall(40,16);
wall(42,16);
wall(44,16);
wall(46,16);
wall(47,16);
wall(48,16);
wall(49,16);
wall(50,16);
wall(51,16);
wall(52,16);
wall(53,16);
wall(54,16);
wall(56,16);
wall(58,16);
wall(59,16);
wall(60,16);
wall(62,16);
wall(64,16);
wall(66,16);
wall(67,16);
wall(68,16);
wall(69,16);

wall(12,17);
wall(18,17);
wall(22,17);
wall(24,17);
wall(26,17);
wall(36,17);
wall(38,17);
wall(40,17);
wall(42,17);
wall(44,17);
wall(46,17);
wall(54,17);
wall(56,17);
wall(60,17);
wall(62,17);
wall(64,17);
wall(68,17);

wall(12,18);
wall(13,18);
wall(14,18);
wall(16,18);
wall(18,18);
wall(19,18);
wall(20,18);
wall(22,18);
wall(24,18);
wall(26,18);
wall(18,18);
wall(19,18);
wall(20,18);
wall(22,18);
wall(24,18);
wall(26,18);
wall(28,18);
wall(29,18);
wall(30,18);
wall(31,18);
wall(32,18);
wall(33,18);
wall(34,18);
wall(35,18);
wall(36,18);
wall(38,18);
wall(40,18);
wall(42,18);
wall(44,18);
wall(46,18);
wall(48,18);
wall(49,18);
wall(50,18);
wall(51,18);
wall(52,18);
wall(53,18);
wall(54,18);
wall(56,18);
wall(57,18);
wall(58,18);
wall(60,18);
wall(62,18);
wall(64,18);
wall(66,18);

wall(12,19);
wall(16,19);
wall(22,19);
wall(24,19);
wall(26,19);
wall(30,19);
wall(32,19);
wall(36,19);
wall(38,19);
wall(40,19);
wall(42,19);
wall(44,19);
wall(46,19);
wall(52,19);
wall(56,19);
wall(62,19);
wall(68,19);

wall(12,20);
wall(14,20);
wall(15,20);
wall(16,20);
wall(17,20);
wall(18,20);
wall(19,20);
wall(20,20);
wall(22,20);
wall(24,20);
wall(28,20);
wall(32,20);
wall(36,20);
wall(38,20);
wall(40,20);
wall(42,20);
wall(44,20);
wall(46,20);
wall(47,20);
wall(48,20);
wall(49,20);
wall(50,20);
wall(52,20);
wall(54,20);
wall(55,20);
wall(56,20);
wall(58,20);
wall(59,20);
wall(60,20);
wall(61,20);
wall(62,20);
wall(66,20);
wall(67,20);
wall(68,20);

wall(12,21);
wall(14,21);
wall(20,21);
wall(22,21);
wall(24,21);
wall(28,21);
wall(32,21);
wall(34,21);
wall(36,21);
wall(38,21);
wall(40,21);
wall(44,21);
wall(48,21);
wall(52,21);
wall(60,21);
wall(68,21);

wall(12,22);
wall(14,22);
wall(16,22);
wall(17,22);
wall(18,22);
wall(20,22);
wall(22,22);
wall(24,22);
wall(25,22);
wall(26,22);
wall(28,22);
wall(29,22);
wall(30,22);
wall(32,22);
wall(34,22);
wall(36,22);
wall(38,22);
wall(40,22);
wall(41,22);
wall(42,22);
wall(43,22);
wall(44,22);
wall(46,22);
wall(48,22);
wall(50,22);
wall(52,22);
wall(53,22);
wall(54,22);
wall(55,22);
wall(56,22);
wall(58,22);
wall(60,22);
wall(61,22);
wall(62,22);
wall(63,22);
wall(64,22);
wall(66,22);
wall(67,22);
wall(68,22);
wall(69,22);

wall(12,23);
wall(14,23);
wall(18,23);
wall(20,23);
wall(22,23);
wall(28,23);
wall(34,23);
wall(38,23);
wall(40,23);
wall(46,23);
wall(50,23);
wall(52,23);
wall(56,23);
wall(58,23);
wall(62,23);

wall(12,24);
wall(14,24);
wall(15,24);
wall(16,24);
wall(18,24);
wall(20,24);
wall(22,24);
wall(23,24);
wall(24,24);
wall(25,24);
wall(26,24);
wall(28,24);
wall(29,24);
wall(30,24);
wall(31,24);
wall(32,24);
wall(33,24);
wall(34,24);
wall(35,24);
wall(36,24);
wall(37,24);
wall(38,24);
wall(40,24);
wall(41,24);
wall(42,24);
wall(43,24);
wall(44,24);
wall(46,24);
wall(48,24);
wall(49,24);
wall(50,24);
wall(52,24);
wall(53,24);
wall(54,24);
wall(60,24);
wall(62,24);
wall(63,24);
wall(64,24);
wall(65,24);
wall(66,24);
wall(68,24);
wall(69,24);

wall(12,25);
wall(18,25);
wall(28,25);
wall(32,25);
wall(46,25);
wall(48,25);
wall(50,25);
wall(54,25);
wall(60,25);
wall(64,25);
wall(68,25);

wall(12,26);
wall(14,26);
wall(15,26);
wall(16,26);
wall(18,26);
wall(19,26);
wall(20,26);
wall(21,26);
wall(22,26);
wall(24,26);
wall(25,26);
wall(26,26);
wall(28,26);
wall(30,26);
wall(32,26);
wall(34,26);
wall(35,26);
wall(36,26);
wall(37,26);
wall(38,26);
wall(40,26);
wall(41,26);
wall(42,26);
wall(43,26);
wall(44,26);
wall(46,26);
wall(48,26);
wall(50,26);
wall(52,26);
wall(54,26);
wall(56,26);
wall(57,26);
wall(58,26);
wall(60,26);
wall(62,26);
wall(64,26);
wall(66,26);
wall(68,26);

wall(12,27);
wall(16,27);
wall(24,27);
wall(28,27);
wall(30,27);
wall(32,27);
wall(34,27);
wall(38,27);
wall(40,27);
wall(48,27);
wall(52,27);
wall(58,27);
wall(60,27);
wall(62,27);
wall(64,27);
wall(66,27);
wall(68,27);

wall(12,28);
wall(14,28);
wall(15,28);
wall(16,28);
wall(17,28);
wall(18,28);
wall(19,28);
wall(20,28);
wall(22,28);
wall(23,28);
wall(24,28);
wall(26,28);
wall(27,28);
wall(28,28);
wall(30,28);
wall(32,28);
wall(34,28);
wall(36,28);
wall(38,28);
wall(40,28);
wall(41,28);
wall(42,28);
wall(43,28);
wall(44,28);
wall(45,28);
wall(46,28);
wall(47,28);
wall(48,28);
wall(49,28);
wall(50,28);
wall(51,28);
wall(52,28);
wall(53,28);
wall(54,28);
wall(55,28);
wall(56,28);
wall(58,28);
wall(60,28);
wall(61,28);
wall(62,28);
wall(64,28);
wall(66,28);
wall(68,28);

wall(12,29);
wall(14,29);
wall(20,29);
wall(22,29);
wall(30,29);
wall(32,29);
wall(34,29);
wall(36,29);
wall(38,29);
wall(40,29);
wall(42,29);
wall(46,29);
wall(66,29);
wall(68,29);

wall(12,30);
wall(14,30);
wall(16,30);
wall(22,30);
wall(23,30);
wall(24,30);
wall(25,30);
wall(26,30);
wall(27,30);
wall(28,30);
wall(30,30);
wall(31,30);
wall(32,30);
wall(34,30);
wall(36,30);
wall(37,30);
wall(38,30);
wall(40,30);
wall(42,30);
wall(44,30);
wall(46,30);
wall(48,30);
wall(49,30);
wall(50,30);
wall(51,30);
wall(52,30);
wall(53,30);
wall(54,30);
wall(56,30);
wall(58,30);
wall(59,30);
wall(60,30);
wall(61,30);
wall(62,30);
wall(64,30);
wall(65,30);
wall(66,30);
wall(68,30);

wall(14,31);
wall(16,31);
wall(28,31);
wall(34,31);
wall(42,31);
wall(44,31);
wall(46,31);
wall(48,31);
wall(54,31);
wall(56,31);
wall(60,31);
wall(68,31);

wall(10,32);
wall(11,32);
wall(12,32);
wall(14,32);
wall(16,32);
wall(17,32);
wall(18,32);
wall(19,32);
wall(20,32);
wall(21,32);
wall(22,32);
wall(23,32);
wall(24,32);
wall(26,32);
wall(28,32);
wall(29,32);
wall(30,32);
wall(31,32);
wall(32,32);
wall(34,32);
wall(36,32);
wall(37,32);
wall(38,32);
wall(39,32);
wall(40,32);
wall(41,32);
wall(42,32);
wall(44,32);
wall(45,32);
wall(46,32);
wall(48,32);
wall(49,32);
wall(50,32);
wall(52,32);
wall(54,32);
wall(56,32);
wall(57,32);
wall(58,32);
wall(60,32);
wall(62,32);
wall(63,32);
wall(64,32);
wall(65,32);
wall(66,32);

wall(14,33);
wall(16,33);
wall(20,33);
wall(24,33);
wall(26,33);
wall(34,33);
wall(36,33);
wall(48,33);
wall(52,33);
wall(54,33);
wall(58,33);
wall(60,33);
wall(64,33);

wall(10,34);
wall(11,34);
wall(12,34);
wall(13,34);
wall(14,34);
wall(16,34);
wall(18,34);
wall(20,34);
wall(21,34);
wall(22,34);
wall(24,34);
wall(26,34);
wall(27,34);
wall(28,34);
wall(30,34);
wall(31,34);
wall(32,34);
wall(34,34);
wall(36,34);
wall(38,34);
wall(39,34);
wall(40,34);
wall(41,34);
wall(42,34);
wall(43,34);
wall(44,34);
wall(45,34);
wall(46,34);
wall(48,34);
wall(50,34);
wall(51,34);
wall(52,34);
wall(54,34);
wall(55,34);
wall(56,34);
wall(58,34);
wall(60,34);
wall(61,34);
wall(62,34);
wall(63,34);
wall(64,34);
wall(66,34);
wall(67,34);
wall(68,34);
wall(69,34);

wall(18,35);
wall(24,35);
wall(28,35);
wall(30,35);
wall(36,35);
wall(38,35);
wall(44,35);
wall(50,35);
wall(52,35);
wall(54,35);
wall(58,35);
wall(64,35);
wall(66,35);

wall(10,36);
wall(11,36);
wall(12,36);
wall(13,36);
wall(14,36);
wall(16,36);
wall(17,36);
wall(18,36);
wall(19,36);
wall(20,36);
wall(21,36);
wall(22,36);
wall(23,36);
wall(24,36);
wall(25,36);
wall(26,36);
wall(27,36);
wall(28,36);
wall(30,36);
wall(31,36);
wall(32,36);
wall(33,36);
wall(34,36);
wall(36,36);
wall(38,36);
wall(40,36);
wall(42,36);
wall(43,36);
wall(44,36);
wall(46,36);
wall(47,36);
wall(48,36);
wall(50,36);
wall(52,36);
wall(54,36);
wall(56,36);
wall(58,36);
wall(59,36);
wall(60,36);
wall(62,36);
wall(64,36);
wall(65,36);
wall(66,36);
wall(68,36);
wall(69,36);

wall(16,37);
wall(22,37);
wall(26,37);
wall(30,37);
wall(34,37);
wall(36,37);
wall(38,37);
wall(40,37);
wall(46,37);
wall(48,37);
wall(50,37);
wall(52,37);
wall(54,37);
wall(56,37);
wall(62,37);
wall(66,37);

wall(12,38);
wall(14,38);
wall(15,38);
wall(16,38);
wall(18,38);
wall(19,38);
wall(20,38);
wall(22,38);
wall(23,38);
wall(24,38);
wall(26,38);
wall(28,38);
wall(29,38);
wall(30,38);
wall(32,38);
wall(33,38);
wall(34,38);
wall(36,38);
wall(38,38);
wall(40,38);
wall(41,38);
wall(42,38);
wall(43,38);
wall(44,38);
wall(45,38);
wall(46,38);
wall(48,38);
wall(50,38);
wall(52,38);
wall(54,38);
wall(55,38);
wall(56,38);
wall(57,38);
wall(58,38);
wall(59,38);
wall(60,38);
wall(61,38);
wall(62,38);
wall(64,38);
wall(65,38);
wall(66,38);
wall(67,38);
wall(68,38);

wall(12,39);
wall(16,39);
wall(20,39);
wall(24,39);
wall(26,39);
wall(30,39);
wall(32,39);
wall(36,39);
wall(38,39);
wall(40,39);
wall(44,39);
wall(48,39);
wall(50,39);
wall(52,39);
wall(68,39);

wall(12,40);
wall(13,40);
wall(14,40);
wall(16,40);
wall(17,40);
wall(18,40);
wall(19,40);
wall(20,40);
wall(21,40);
wall(22,40);
wall(24,40);
wall(26,40);
wall(27,40);
wall(28,40);
wall(30,40);
wall(34,40);
wall(35,40);
wall(36,40);
wall(38,40);
wall(40,40);
wall(42,40);
wall(44,40);
wall(45,40);
wall(46,40);
wall(48,40);
wall(50,40);
wall(52,40);
wall(54,40);
wall(55,40);
wall(56,40);
wall(57,40);
wall(58,40);
wall(59,40);
wall(60,40);
wall(62,40);
wall(63,40);
wall(64,40);
wall(65,40);
wall(66,40);
wall(67,40);
wall(68,40);

wall(12,41);
wall(16,41);
wall(20,41);
wall(24,41);
wall(26,41);
wall(30,41);
wall(36,41);
wall(38,41);
wall(42,41);
wall(48,41);
wall(50,41);
wall(52,41);
wall(54,41);
wall(62,41);

wall(12,42);
wall(16,42);
wall(18,42);
wall(20,42);
wall(22,42);
wall(23,42);
wall(24,42);
wall(25,42);
wall(26,42);
wall(28,42);
wall(29,42);
wall(30,42);
wall(31,42);
wall(32,42);
wall(33,42);
wall(34,42);
wall(36,42);
wall(38,42);
wall(39,42);
wall(40,42);
wall(41,42);
wall(42,42);
wall(43,42);
wall(44,42);
wall(45,42);
wall(46,42);
wall(47,42);
wall(48,42);
wall(50,42);
wall(52,42);
wall(54,42);
wall(56,42);
wall(58,42);
wall(59,42);
wall(60,42);
wall(62,42);
wall(64,42);
wall(65,42);
wall(66,42);
wall(67,42);
wall(68,42);
wall(69,42);

wall(12,43);
wall(18,43);
wall(20,43);
wall(22,43);
wall(30,43);
wall(34,43);
wall(36,43);
wall(48,43);
wall(50,43);
wall(56,43);
wall(58,43);
wall(60,43);

wall(12,44);
wall(13,44);
wall(14,44);
wall(15,44);
wall(16,44);
wall(18,44);
wall(19,44);
wall(20,44);
wall(22,44);
wall(24,44);
wall(26,44);
wall(27,44);
wall(28,44);
wall(29,44);
wall(30,44);
wall(32,44);
wall(34,44);
wall(36,44);
wall(38,44);
wall(39,44);
wall(40,44);
wall(41,44);
wall(42,44);
wall(43,44);
wall(44,44);
wall(45,44);
wall(46,44);
wall(47,44);
wall(48,44);
wall(50,44);
wall(51,44);
wall(52,44);
wall(53,44);
wall(54,44);
wall(55,44);
wall(56,44);
wall(57,44);
wall(58,44);
wall(60,44);
wall(62,44);
wall(63,44);
wall(64,44);
wall(65,44);
wall(67,44);
wall(68,44);

wall(18,45);
wall(20,45);
wall(24,45);
wall(26,45);
wall(30,45);
wall(32,45);
wall(36,45);
wall(38,45);
wall(46,45);
wall(48,45);
wall(50,45);
wall(58,45);
wall(60,45);
wall(68,45);

wall(12,46);
wall(14,46);
wall(15,46);
wall(16,46);
wall(18,46);
wall(20,46);
wall(21,46);
wall(22,46);
wall(23,46);
wall(24,46);
wall(26,46);
wall(28,46);
wall(30,46);
wall(31,46);
wall(32,46);
wall(33,46);
wall(34,46);
wall(36,46);
wall(38,46);
wall(40,46);
wall(41,46);
wall(42,46);
wall(43,46);
wall(44,46);
wall(46,46);
wall(48,46);
wall(50,46);
wall(52,46);
wall(53,46);
wall(54,46);
wall(55,46);
wall(56,46);
wall(58,46);
wall(60,46);
wall(61,46);
wall(62,46);
wall(63,46);
wall(64,46);
wall(65,46);
wall(66,46);
wall(67,46);
wall(68,46);

wall(12,47);
wall(16,47);
wall(18,47);
wall(24,47);
wall(26,47);
wall(28,47);
wall(38,47);
wall(40,47);
wall(44,47);
wall(46,47);
wall(48,47);
wall(50,47);
wall(52,47);
wall(56,47);
wall(58,47);
wall(60,47);


wall(12,48);
wall(13,48);
wall(14,48);
wall(16,48);
wall(18,48);
wall(19,48);
wall(20,48);
wall(21,48);
wall(22,48);
wall(24,48);
wall(26,48);
wall(27,48);
wall(28,48);
wall(30,48);
wall(32,48);
wall(33,48);
wall(34,48);
wall(35,48);
wall(36,48);
wall(37,48);
wall(38,48);
wall(40,48);
wall(42,48);
wall(44,48);
wall(46,48);
wall(48,48);
wall(50,48);
wall(52,48);
wall(53,48);
wall(54,48);
wall(56,48);
wall(58,48);
wall(60,48);
wall(61,48);
wall(62,48);
wall(63,48);
wall(64,48);
wall(65,48);
wall(66,48);
wall(67,48);
wall(68,48);

wall(12,49);
wall(16,49);
wall(18,49);
wall(22,49);
wall(24,49);
wall(30,49);
wall(32,49);
wall(38,49);
wall(40,49);
wall(42,49);
wall(44,49);
wall(50,49);
wall(56,49);
wall(58,49);
wall(60,49);

wall(12,50);
wall(14,50);
wall(15,50);
wall(16,50);
wall(20,50);
wall(22,50);
wall(24,50);
wall(26,50);
wall(27,50);
wall(28,50);
wall(29,50);
wall(30,50);
wall(32,50);
wall(34,50);
wall(35,50);
wall(36,50);
wall(37,50);
wall(38,50);
wall(40,50);
wall(42,50);
wall(44,50);
wall(45,50);
wall(46,50);
wall(47,50);
wall(48,50);
wall(49,50);
wall(50,50);
wall(51,50);
wall(52,50);
wall(53,50);
wall(54,50);
wall(56,50);
wall(58,50);
wall(60,50);
wall(62,50);
wall(63,50);
wall(64,50);
wall(66,50);
wall(67,50);
wall(68,50);
wall(69,50);

wall(12,51);
wall(14,51);
wall(16,51);
wall(20,51);
wall(24,51);
wall(28,51);
wall(30,51);
wall(32,51);
wall(36,51);
wall(38,51);
wall(40,51);
wall(42,51);
wall(48,51);
wall(56,51);
wall(58,51);
wall(60,51);
wall(62,51);
wall(64,51);
wall(66,51);

wall(12,52);
wall(14,52);
wall(16,52);
wall(17,52);
wall(18,52);
wall(19,52);
wall(20,52);
wall(21,52);
wall(22,52);
wall(24,52);
wall(25,52);
wall(26,52);
wall(28,52);
wall(30,52);
wall(32,52);
wall(33,52);
wall(34,52);
wall(36,52);
wall(38,52);
wall(40,52);
wall(42,52);
wall(43,52);
wall(44,52);
wall(45,52);
wall(46,52);
wall(48,52);
wall(49,52);
wall(50,52);
wall(51,52);
wall(52,52);
wall(53,52);
wall(54,52);
wall(55,52);
wall(56,52);
wall(58,52);
wall(60,52);
wall(62,52);
wall(64,52);
wall(66,52);
wall(67,52);
wall(68,52);

wall(12,53);
wall(14,53);
wall(16,53);
wall(26,53);
wall(28,53);
wall(36,53);
wall(38,53);
wall(40,53);
wall(46,53);
wall(58,53);
wall(60,53);
wall(62,53);
wall(64,53);

wall(10,54);
wall(11,54);
wall(12,54);
wall(14,54);
wall(16,54);
wall(18,54);
wall(19,54);
wall(20,54);
wall(21,54);
wall(22,54);
wall(23,54);
wall(24,54);
wall(26,54);
wall(28,54);
wall(29,54);
wall(30,54);
wall(31,54);
wall(32,54);
wall(33,54);
wall(34,54);
wall(36,54);
wall(38,54);
wall(40,54);
wall(42,54);
wall(43,54);
wall(44,54);
wall(45,54);
wall(46,54);
wall(47,54);
wall(48,54);
wall(49,54);
wall(50,54);
wall(51,54);
wall(52,54);
wall(53,54);
wall(54,54);
wall(55,54);
wall(56,54);
wall(58,54);
wall(60,54);
wall(62,54);
wall(64,54);
wall(65,54);
wall(66,54);
wall(67,54);
wall(68,54);

wall(14,55);
wall(18,55);
wall(24,55);
wall(26,55);
wall(34,55);
wall(36,55);
wall(38,55);
wall(56,55);
wall(58,55);

wall(12,56);
wall(14,56);
wall(16,56);
wall(18,56);
wall(19,56);
wall(20,56);
wall(21,56);
wall(22,56);
wall(24,56);
wall(26,56);
wall(27,56);
wall(28,56);
wall(29,56);
wall(30,56);
wall(31,56);
wall(32,56);
wall(34,56);
wall(36,56);
wall(38,56);
wall(39,56);
wall(40,56);
wall(41,56);
wall(42,56);
wall(43,56);
wall(44,56);
wall(46,56);
wall(48,56);
wall(50,56);
wall(51,56);
wall(52,56);
wall(53,56);
wall(54,56);
wall(55,56);
wall(56,56);
wall(58,56);
wall(59,56);
wall(60,56);
wall(62,56);
wall(63,56);
wall(64,56);
wall(66,56);
wall(67,56);
wall(68,56);
wall(69,56);

wall(12,57);
wall(16,57);
wall(22,57);
wall(24,57);
wall(28,57);
wall(32,57);
wall(34,57);
wall(36,57);
wall(40,57);
wall(44,57);
wall(46,57);
wall(48,57);
wall(50,57);
wall(62,57);
wall(64,57);
wall(68,57);

wall(12,58);
wall(13,58);
wall(14,58);
wall(15,58);
wall(16,58);
wall(17,58);
wall(18,58);
wall(19,58);
wall(20,58);
wall(22,58);
wall(24,58);
wall(26,58);
wall(28,58);
wall(30,58);
wall(32,58);
wall(34,58);
wall(36,58);
wall(37,58);
wall(38,58);
wall(40,58);
wall(41,58);
wall(42,58);
wall(44,58);
wall(46,58);
wall(48,58);
wall(50,58);
wall(51,58);
wall(52,58);
wall(53,58);
wall(54,58);
wall(55,58);
wall(56,58);
wall(58,58);
wall(59,58);
wall(60,58);
wall(62,58);
wall(64,58);
wall(66,58);
wall(68,58);

wall(12,59);
wall(14,59);
wall(18,59);
wall(20,59);
wall(22,59);
wall(24,59);
wall(26,59);
wall(30,59);
wall(42,59);
wall(44,59);
wall(46,59);
wall(48,59);
wall(56,59);
wall(58,59);
wall(60,59);
wall(62,59);
wall(68,59);

wall(12,60);
wall(14,60);
wall(16,60);
wall(18,60);
wall(20,60);
wall(21,60);
wall(22,60);
wall(24,60);
wall(26,60);
wall(27,60);
wall(28,60);
wall(30,60);
wall(31,60);
wall(32,60);
wall(33,60);
wall(34,60);
wall(35,60);
wall(36,60);
wall(37,60);
wall(38,60);
wall(40,60);
wall(42,60);
wall(44,60);
wall(46,60);
wall(48,60);
wall(50,60);
wall(51,60);
wall(52,60);
wall(53,60);
wall(54,60);
wall(55,60);
wall(56,60);
wall(58,60);
wall(60,60);
wall(62,60);
wall(63,60);
wall(64,60);
wall(65,60);
wall(66,60);
wall(67,60);
wall(68,60);

wall(12,61);
wall(16,61);
wall(18,61);
wall(26,61);
wall(40,61);
wall(42,61);
wall(44,61);
wall(46,61);
wall(48,61);
wall(50,61);
wall(58,61);
wall(60,61);
wall(68,61);

wall(12,62);
wall(13,62);
wall(14,62);
wall(16,62);
wall(18,62);
wall(19,62);
wall(20,62);
wall(22,62);
wall(23,62);
wall(24,62);
wall(26,62);
wall(28,62);
wall(30,62);
wall(31,62);
wall(32,62);
wall(33,62);
wall(34,62);
wall(36,62);
wall(37,62);
wall(38,62);
wall(39,62);
wall(40,62);
wall(42,62);
wall(44,62);
wall(45,62);
wall(46,62);
wall(48,62);
wall(50,62);
wall(51,62);
wall(52,62);
wall(53,62);
wall(54,62);
wall(55,62);
wall(56,62);
wall(58,62);
wall(60,62);
wall(61,62);
wall(62,62);
wall(63,62);
wall(64,62);
wall(65,62);
wall(66,62);
wall(68,62);

wall(14,63);
wall(16,63);
wall(20,63);
wall(22,63);
wall(28,63);
wall(30,63);
wall(34,63);
wall(46,63);
wall(48,63);
wall(56,63);
wall(58,63);
wall(66,63);
wall(68,63);

wall(12,64);
wall(13,64);
wall(14,64);
wall(16,64);
wall(17,64);
wall(18,64);
wall(20,64);
wall(22,64);
wall(24,64);
wall(25,64);
wall(26,64);
wall(28,64);
wall(30,64);
wall(32,64);
wall(34,64);
wall(35,64);
wall(36,64);
wall(38,64);
wall(39,64);
wall(40,64);
wall(41,64);
wall(42,64);
wall(43,64);
wall(44,64);
wall(45,64);
wall(46,64);
wall(48,64);
wall(49,64);
wall(50,64);
wall(51,64);
wall(52,64);
wall(53,64);
wall(54,64);
wall(56,64);
wall(58,64);
wall(60,64);
wall(61,64);
wall(62,64);
wall(64,64);
wall(66,64);
wall(68,64);

wall(12,65);
wall(16,65);
wall(22,65);
wall(24,65);
wall(26,65);
wall(28,65);
wall(32,65);
wall(34,65);
wall(36,65);
wall(38,65);
wall(48,65);
wall(54,65);
wall(56,65);
wall(58,65);
wall(60,65);
wall(62,65);
wall(64,65);
wall(66,65);
wall(68,65);

wall(12,66);
wall(14,66);
wall(15,66);
wall(16,66);
wall(17,66);
wall(18,66);
wall(19,66);
wall(20,66);
wall(22,66);
wall(24,66);
wall(26,66);
wall(27,66);
wall(28,66);
wall(29,66);
wall(30,66);
wall(31,66);
wall(32,66);
wall(34,66);
wall(36,66);
wall(38,66);
wall(40,66);
wall(41,66);
wall(42,66);
wall(43,66);
wall(44,66);
wall(45,66);
wall(46,66);
wall(48,66);
wall(50,66);
wall(51,66);
wall(52,66);
wall(54,66);
wall(56,66);
wall(58,66);
wall(60,66);
wall(62,66);
wall(64,66);
wall(66,66);
wall(68,66);

wall(12,67);
wall(22,67);
wall(24,67);
wall(26,67);
wall(28,67);
wall(34,67);
wall(36,67);
wall(38,67);
wall(40,67);
wall(48,67);
wall(50,67);
wall(52,67);
wall(54,67);
wall(56,67);
wall(58,67);
wall(60,67);
wall(62,67);
wall(64,67);
wall(68,67);

wall(12,68);
wall(13,68);
wall(14,68);
wall(15,68);
wall(16,68);
wall(17,68);
wall(18,68);
wall(19,68);
wall(20,68);
wall(22,68);
wall(24,68);
wall(26,68);
wall(28,68);
wall(30,68);
wall(31,68);
wall(32,68);
wall(34,68);
wall(36,68);
wall(38,68);
wall(40,68);
wall(42,68);
wall(43,68);
wall(44,68);
wall(45,68);
wall(46,68);
wall(47,68);
wall(48,68);
wall(50,68);
wall(52,68);
wall(54,68);
wall(55,68);
wall(56,68);
wall(58,68);
wall(60,68);
wall(62,68);
wall(63,68);
wall(64,68);
wall(65,68);
wall(66,68);
wall(67,68);
wall(68,68);

wall(16,69);
wall(22,69);
wall(28,69);
wall(32,69);
wall(36,69);
wall(38,69);
wall(40,69);
wall(50,69);
wall(58,69);
wall(60,69);
}