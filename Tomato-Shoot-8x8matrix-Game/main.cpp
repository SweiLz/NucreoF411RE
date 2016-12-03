#include "mbed.h"
#include "my8x8RGB.h"

Ticker UpdateScreen,TCK_player;
Timeout w8;
Timer timer1;
InterruptIn BTN_Up(PA_14),BTN_Down(PA_15),BTN_Shoot(PA_13);;

class bullet
{
public:
    int y,x;
    uint8_t enable;
    void display() {

        if(enable==1) {
            rgb_point(y,6-x,0,0,100);
        }
    }
    void RST() {
        rgb_point(y,7-x,0,0,0);
        y=0;
        x=0;
        enable=0;
    }

};

class enemy
{
public:
    int y,x;
    uint8_t level,enable;
    int t_delay;
    void display() {
        if(enable==1) {
            if(level==2 && x>=0)rgb_point(y,x,100,100,0);
            if(level==1 && x>=0)rgb_point(y,x,100,0,0);

        }
    }

    void RST() {
        rgb_point(y,x,0,0,0);
        y=0;
        x=0;
        enable=0;
        level=0;
    }

};


/******************************/
//define game variable
#define enemy_size1 10
#define enemy_size2 15
#define enemy_size3 20
#define bullet_speed 50
#define btn_delay 0.02
uint8_t player_location=1; // in Colum
bullet mybullet[10];
enemy wave1[enemy_size1];
enemy wave2[enemy_size2];
enemy wave3[enemy_size3];
enemy wave4[enemy_size2];
enemy wave5[enemy_size3];
uint8_t currentbullet=0;
uint8_t gameLevel=1;
int t_ms_easy[4]= {800,1000,1200,1400};
int t_ms_normal[4]= {500,600,700,800};
int t_ms_hard[4]= {500,400,400,500};
int CN[2]= {2,1};
/******************************/
void createWall();
void createWall2(uint8_t r,uint8_t g,uint8_t b);
void gameOver();

void PMD();
void PMU();
void playerMoveDown();
void playerMoveUp();
void SHT();
void shooting();
void bulletUpdate();

void genarate_enemy(enemy stage[],int enemy_size,int t_ms[]);
void genarate_enemy_rand(enemy stage[],int enemy_size,int t_ms[]);
void newSetting();
void enemyUpdate(enemy temp[],int enemy_size);
void TomaTosetting();
void TomaToloop();

int main()
{
    TomaTosetting();

    while(1) {
        TomaToloop();

    }
}

void createWall()
{
    for(int i=0; i<8; i++) {
        rgb_point(0,i,50,50,50);
        rgb_point(7,i,50,50,50);
    }

}

void createWall2(uint8_t r,uint8_t g,uint8_t b)
{
    for(int i=0; i<8; i++) {
        rgb_point(0,i,r,g,b);
        rgb_point(7,i,r,g,b);
    }

}

void PMD()
{
    rgb_point(player_location,7,0,0,0);
    player_location++;
    if(player_location>=7) {
        player_location=6;
    }
    rgb_point(player_location,7,0,50,0);
}
void playerMoveDown()
{

    w8.attach(&PMD,btn_delay);
}

void PMU()
{
    rgb_point(player_location,7,0,0,0);
    player_location--;
    if(player_location<=0) {
        player_location=1;
    }
    rgb_point(player_location,7,0,50,0);

}
void playerMoveUp()
{

    w8.attach(&PMU,btn_delay);
}

void SHT()
{
    mybullet[currentbullet].y=player_location;
    mybullet[currentbullet].enable=1;
    currentbullet++;
    if(currentbullet>=10)currentbullet=0;
}

void shooting()
{
    w8.attach(&SHT,btn_delay);
}

void bulletUpdate()
{

    for(int i=0; i<10; i++) {
        if(mybullet[i].enable==1) {
            mybullet[i].display();
            if(timer1.read_ms()%bullet_speed==0) {
                rgb_point(mybullet[i].y,6-mybullet[i].x,0,0,0);
                mybullet[i].x++;
                if(mybullet[i].x>=7)mybullet[i].RST();
            }
        }

    }
}


void genarate_enemy(enemy stage[],int enemy_size,int t_ms[])
{
    for(int i=0; i<enemy_size; i++) {
        stage[i].x=0;
        stage[i].y=(rand()%6) +1;
        for(int j=0; j< enemy_size && i!=j; j++) {
            if(stage[i].y==stage[j].y && stage[i].x==stage[j].x)stage[i].x++;

        }
        stage[i].level=1;
        stage[i].enable=1;
        stage[i].t_delay=t_ms[rand()%4 +1];
        if(stage[i].y==0 || stage[i].y==7) {
            stage[i].enable=0;
            stage[i].RST();
        }

    }
}



void genarate_enemy_rand(enemy stage[],int enemy_size,int t_ms[])
{
    for(int i=0; i<enemy_size; i++) {
        stage[i].x=0;
        stage[i].y=rand()%6 +1;
        for(int j=0; j< enemy_size && i!=j; j++) {
            if(stage[i].y==stage[j].y)stage[i].x++;
            stage[i].enable=1;
        }
        stage[i].level=rand()%2 +1;
        stage[i].enable=1;
        stage[i].t_delay=t_ms[rand()%4 +1];
        if(stage[i].y==0 || stage[i].y==7) {
            stage[i].enable=0;
            stage[i].RST();
        }

    }
}

void newSetting()
{
    screen_off();
    createWall();
    rgb_point(player_location,7,0,50,0);
}

void enemyUpdate(enemy temp[],int enemy_size)
{
    uint8_t status=0;
    for(int i=0; i<enemy_size; i++) {
        if(temp[i].enable==1) {
            temp[i].display();
            if(timer1.read_ms()%temp[i].t_delay==0) {
                rgb_point(temp[i].y,temp[i].x,0,0,0);
                temp[i].x++;
                if(temp[i].x>7) { //Game Over
                    newSetting();
                    createWall2(255,10,255);

                    temp[i].RST();
                    // screen_color(100,10,10);
                    gameLevel=1;
                    genarate_enemy(wave1,enemy_size1,t_ms_easy);
                }
            }
            for(int j =0; j<10; j++) {
                if(temp[i].x==(6-mybullet[j].x) && temp[i].y==mybullet[j].y) {
                    mybullet[j].RST();
                    if(temp[i].level==2)temp[i].level=1;
                    else
                        temp[i].RST();
                }
            }
        } else status++;
    }
    if(status>=enemy_size) {
        wait_ms(500);
        gameLevel++;
        newSetting();
        if(gameLevel==2)genarate_enemy(wave2,enemy_size2,t_ms_normal);
        if(gameLevel==3)genarate_enemy(wave3,enemy_size2,t_ms_hard);
        if(gameLevel==4)genarate_enemy_rand(wave4,enemy_size1,t_ms_easy);
        if(gameLevel==5)genarate_enemy_rand(wave5,enemy_size2,t_ms_normal);
        if(gameLevel==6) {
            genarate_enemy_rand(wave5,enemy_size3,t_ms_normal);
            createWall2(0,30,255);
        }
    }

}

void TomaTosetting()
{
    timer1.start();
    char wb[3]= {128,128,128};
    INITRGB(wb);
    /***************Genarate Enemy*********************/
    genarate_enemy(wave1,enemy_size1,t_ms_easy);

    /***************Setting Button*******************/
    BTN_Down.rise(&playerMoveDown);
    BTN_Down.mode(PullUp);
    BTN_Up.rise(&playerMoveUp);
    BTN_Up.mode(PullUp);
    BTN_Shoot.rise(&shooting);
    BTN_Shoot.mode(PullUp);

    /************************************/
//gameSetting
    newSetting();
    /***********************************/

}
void TomaToloop()
{

    displayRGB();
    bulletUpdate();
    displayRGB();
    //displayRGB();
    if(gameLevel==1)enemyUpdate(wave1,enemy_size1);
    if(gameLevel==2)enemyUpdate(wave2,enemy_size2);
    if(gameLevel==3)enemyUpdate(wave3,enemy_size3);
    if(gameLevel==4)enemyUpdate(wave4,enemy_size1);
    if(gameLevel==5)enemyUpdate(wave5,enemy_size2);
    if(gameLevel==6)enemyUpdate(wave5,enemy_size3);//6
    displayRGB();

}

