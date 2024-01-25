#include "iGraphics.h"
#include<stdlib.h>
#include<windows.h>
#include<mmsystem.h>


#define bgwidth 800
#define bgheight 512
#define car_start_coordinate_x 400
#define car_start_coordinate_y 0
#define first_barrier_count 5
#define second_barrier_count 15

char road[16][20] = {"road_01.bmp","road_02.bmp","road_03.bmp","road_04.bmp","road_05.bmp","road_06.bmp","road_07.bmp","road_08.bmp","road_09.bmp","road_10.bmp","road_11.bmp","road_12.bmp","road_13.bmp","road_14.bmp","road_15.bmp","road_16.bmp"};
char grass[4][20] = {"grass_01.bmp","grass_02.bmp","grass_03.bmp","grass_04.bmp"};
char barrier[2][20] = {"barrier_red.bmp","barrier_white.bmp"};
int line_start[4] = {248,324,400,476};
int second_barrier_begin=0;
int m=0;
int speed=100;
//char countdown[3][20] = {"1.bmp","2.bmp","3.bmp"};

int x = car_start_coordinate_x, y = car_start_coordinate_y, gamestate=0;  // x= horizontal position of the car, y= vertical position of the car
int score=0, music=1;
int yfinishline, xfinishline, showing_finishline=0;
int imgposy[16];  // starting position of the roads
int grassposy[4]; // starting position of the grass

//gamestate=0 -- homescreen
//gamestate=1 -- menu
//gamestate=2 -- game window
//gamestate=3 -- game end screen
//gamestate=4 -- resume screen
//gamestate=5 -- option screen
//gamestate=6 -- instruction screen
//gamestate=7 -- scorecard screen
//gamestate=8 -- highscore screen
//gamestate=9 -- credit

//function prototype
void collusion(int barrier_x_index, int barrier_y);
void store_score();
void read_score();
void High_score();
void show_scorecard();
void show_highscore();

struct barrier_coordinate
{
    int x;
    int y;
} barco,barco1,barco2;

typedef struct
{
    char name[30];
    int score;
}pscore;

int number_of_players;
pscore *PlayerScore;

// first barrier
void random_barrier_coordinate()
{
    barco.x= rand() % 4;
    //barco.y= (rand() % 310) + 200;
    barco.y= 512;

    second_barrier_begin++;
}

void barrier_change()
{
    score+=2;
    barco.y-=22;
    if(barco.y<0)
    {
        score+=10;
        random_barrier_coordinate();
    }
    collusion(barco.x,barco.y);
}

//second barrier
void random_barrier_coordinate2()
{
    if(second_barrier_begin>first_barrier_count)
    {
        do
        {
        barco1.x= rand() % 4;
        }
        while(barco.x==barco1.x);
        barco1.y= (rand() % 110) + 400;
    }
}

void barrier_change2()
{
    score+=3;
    barco1.y-=22;
    if(barco1.y<0)
    {
        score+=20;
        random_barrier_coordinate2();
    }
    collusion(barco1.x,barco1.y);
}

//barrier3
void random_barrier_coordinate3()
{
    if(second_barrier_begin>second_barrier_count)
    {
        do
        {
            barco2.x= rand() % 4;
        }
        while(barco.x==barco2.x || barco1.x==barco2.x);
        barco2.y= (rand() % 110) + 400;
    }
}

void barrier_change3()
{
    score+=5;
    barco2.y-=22;
    if(barco2.y<0)
    {
        score+=20;
        random_barrier_coordinate3();
    }
    collusion(barco2.x,barco2.y);
}


void starting_position()
{
    int p=0;
    for(int i=0; i<16; i++)
    {
        imgposy[i]=p;
        p+=32;
    }
    p=0;
    for(int i=0; i<4; i++)
    {
        grassposy[i]=p;
        p+=128;
    }

    xfinishline=228;
    yfinishline=36;
}

//background rendering
void backgroundchange()
{
    score+=5;
    //road
    for(int i=0; i<16; i++)
    {
        imgposy[i]-=32;
    }
    for(int i=0; i<16; i++)
    {
        if(imgposy[i]<0)
        {
            imgposy[i]=480;
        }
    }

    //sides
    for(int i=0; i<4; i++)
    {
        grassposy[i]-=128;
    }
    for(int i=0; i<4; i++)
    {
        if(grassposy[i]<0)
        {
            grassposy[i]=384;
        }
    }

    yfinishline-=18;
}

void rendering_barrier()
{
    iShowBMP(line_start[barco.x],barco.y,barrier[0]);

    if(second_barrier_begin>first_barrier_count)
    {
        iShowBMP(line_start[barco1.x],barco1.y,barrier[1]);
    }
    if(second_barrier_begin>second_barrier_count)
    {
        iShowBMP(line_start[barco2.x],barco2.y,barrier[0]);
    }
}

//inputing Name
char intermediate_str[100], store_string[100];
int len, mode;

void drawTextBox()
{
    iSetColor(255, 255, 255);
    iText(275, 160, "Enter Your Name");
    iFilledRectangle(275, 120, 250, 30);
    iText(240, 100, "* Name Should Not exceed 30 characters *");
}

void show_name_on_the_screen()
{
    if(mode==1)
    {
        iSetColor(0, 0, 0);
        iText(280, 130, intermediate_str);
    }
    iSetColor(255, 255, 255);
    iText(215, 80, "* Click to activate the box, enter to finish. *");
}

void show_score_during_game()
{
    char score_during_game[100];
    sprintf(score_during_game,"%d",score);
    iSetColor(0,0,0);
    iText(656, 384, "Score:", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(656, 362, score_during_game, GLUT_BITMAP_TIMES_ROMAN_24);
}

void show_speed_during_game()
{
    char speed_during_game[100];
    sprintf(speed_during_game,"%d",16000/speed);
    iSetColor(0,0,0);
    iText(656, 128, "Speed:", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(656, 106, speed_during_game, GLUT_BITMAP_TIMES_ROMAN_24);
}

void iDraw()
{
    iClear();
    //printf("%d\n",score);
    if(gamestate==2)
    {
        //rendering road
        for(int i=0; i<16; i++)
        {
            iShowBMP(144,imgposy[i],road[i]);
        }

        //rendering sideview
        for(int i=0; i<4; i++)
        {
            iShowBMP(0,grassposy[i],grass[i]);
            iShowBMP(656,grassposy[i],grass[i]);
        }

        //rendering start-finish line
        iShowBMP(xfinishline,yfinishline,"finishline.bmp");

        //CAR
        iShowBMP2(x, y, "car.bmp",0);  // car width=26 (19-45), height=45(10-55), height from 0,0 = 55

        //random barrier
        if(m==1)
        {
            rendering_barrier();
        }

        //showing score
        show_score_during_game();

        //showing speed
        show_speed_during_game();

        //pause button
        if(showing_finishline==0)
        {
            iShowBMP(703,235,"pause.bmp");
        }
    }

    //show homescreen
    else if(gamestate==0)
    {
        iShowBMP(0,0,"bg.bmp");
        iText(630, 0, "Mouse RB = Home menu", GLUT_BITMAP_8_BY_13);
    }

    // show menubar
    else if(gamestate==1)
    {
        iShowBMP(0,0,"menu.bmp");
        iShowBMP2(300, 350, "newgame.bmp",0); // newgame sz 200 X 80
        iShowBMP2(300, 250, "option.bmp",0);
        iShowBMP2(300, 150, "quit.bmp",0);
    }

    // end screen
    else if(gamestate==3)
    {
        char show_score[100];
        sprintf(show_score,"Score: %d",score);
        High_score();

        iShowBMP(0,0,"gameover.bmp");
        iText(350, 407, show_score, GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(255, 255, 255);
        iText(630, 0, "Mouse RB = Home menu", GLUT_BITMAP_8_BY_13);
        if(x>532 || x<228)
        {
            iText(270, 350, "Next Time Stay In The Road" , GLUT_BITMAP_TIMES_ROMAN_24);
        }
        else if(score<5000)
        {
            iText(310, 350, "Dont Sleep and Drive" , GLUT_BITMAP_TIMES_ROMAN_24);
        }
        else if(score>PlayerScore[0].score)
        {
            iText(220, 350, "New HighScore!!! You Are A Champ!!!" , GLUT_BITMAP_TIMES_ROMAN_24);
        }
        else
        {
            iText(220, 350, "Good Game!! Lets grab the HIGHSCORE" , GLUT_BITMAP_TIMES_ROMAN_24);
        }
        drawTextBox();
        show_name_on_the_screen();
    }

    // resume screen
    else if(gamestate==4)
    {
        iShowBMP(0,0,"menu.bmp");
        iShowBMP2(300, 316, "menu-button.bmp",255);
        iShowBMP2(300, 216, "resume.bmp",255);
        iText(630, 0, "Mouse RB = Home menu", GLUT_BITMAP_8_BY_13);
    }

    // option screen
    else if(gamestate==5)
    {
        iShowBMP(0,0,"menu3.bmp");
        iShowBMP2(300, 400, "scorecard.bmp", 255);
        iShowBMP2(300, 300, "highscore.bmp", 255);
        iShowBMP2(300, 200, "instruction.bmp", 255);
        iShowBMP2(300, 100, "CREDIT2.bmp", 255);
    }

    //instruction screen
    else if(gamestate==6)
    {
        iShowBMP(0,0,"menu3.bmp");
        iShowBMP(100,50,"blackbg.bmp");

        iSetColor(255,223,0);
        iText(140,380,"Use",GLUT_BITMAP_HELVETICA_18);
        iShowBMP(240,400,"button3.bmp");
        iShowBMP(180,365,"button1.bmp");
        iShowBMP(300,365,"button2.bmp");
        iShowBMP(240,330,"button4.bmp");
        iText(360,380,"buttons for controlling the car.",GLUT_BITMAP_HELVETICA_18);

        iText(140,270,"Use",GLUT_BITMAP_HELVETICA_18);
        iShowBMP(180,250,"X.bmp");
        iText(240,270,"button for NITRO",GLUT_BITMAP_HELVETICA_18);

        //other
        iText(140,200,"1)Mouse left click to access the buttons",GLUT_BITMAP_HELVETICA_18);
        iText(140,170,"2)Press \"Space\" button to start the game",GLUT_BITMAP_HELVETICA_18);
        iText(140,140,"3)press \"Space\" button to pause and resume during the game",GLUT_BITMAP_HELVETICA_18);

        iText(310,60,"Thanks For Playing",GLUT_BITMAP_HELVETICA_18);
    }

    //scorecard screen
    else if(gamestate==7)
    {
        iShowBMP(0,0,"menu3.bmp");
        iShowBMP(100,50,"blackbgscorecard.bmp");
        iSetColor(255, 255, 255);
        show_scorecard();
    }

    //highscore screen
    else if(gamestate==8)
    {
        iShowBMP(0,0,"menu3.bmp");
        iShowBMP(100,50,"blackbghighscore.bmp");
        iSetColor(255, 255, 255);
        show_highscore();
    }

    //credit screen
    else if(gamestate==9)
    {
        iShowBMP(0,0,"menu3.bmp");
        iShowBMP(100,50,"blackbgcredit.bmp");

        iSetColor(89, 0, 255);
        iText(180,350,"Created By:",GLUT_BITMAP_HELVETICA_18);
        iSetColor(255, 255, 255);
        iText(180,320,"Abhishek Roy",GLUT_BITMAP_HELVETICA_18);
        iText(180,290,"Id: 2105033",GLUT_BITMAP_HELVETICA_18);
        iText(180,260,"CSE'21,BUET",GLUT_BITMAP_HELVETICA_18);

        iSetColor(89, 0, 255);
        iText(380,210,"Instructed and Supervised By:",GLUT_BITMAP_HELVETICA_18);
        iSetColor(255, 255, 255);
        iText(380,180,"Hashibul Hisham Sir",GLUT_BITMAP_HELVETICA_18);
        iText(380,150,"CSE,BUET",GLUT_BITMAP_HELVETICA_18);
    }
}

void iMouseMove(int mx, int my)
{
}
void iPassiveMouseMove(int mx, int my)
{
}
void iMouse(int button, int state, int mx, int my)
{
    //Menu from homescreen/resume screen
    if ((gamestate==0 || gamestate==3 || gamestate==4) && button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        gamestate=1;
        PlaySound("click.wav", NULL, SND_ASYNC);
    }

    //newgame
    else if(gamestate==1 && mx>=300 && mx<=500 && my>=350 && my<=430)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            PlaySound("click.wav", NULL, SND_ASYNC);
            gamestate=2;
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            starting_position();
            random_barrier_coordinate();
            second_barrier_begin=0;
            showing_finishline=1;
            m=0;
            x=car_start_coordinate_x;
            y=car_start_coordinate_y;
            score=0;
        }
    }

    //option button
    else if(gamestate==1 && mx>=300 && mx<=500 && my>=250 && my<=330)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            gamestate=5;
            PlaySound("click.wav", NULL, SND_ASYNC);
        }
    }

    //option--back button
    else if(gamestate==5 && mx>=0 && mx<=50 && my>=462 && my<=512)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            gamestate=1;
            PlaySound("click.wav", NULL, SND_ASYNC);
        }
    }

    //option--instruction button
    else if(gamestate==5 && mx>=300 && mx<=500 && my>=200 && my<=280)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            gamestate=6;
            PlaySound("click.wav", NULL, SND_ASYNC);
        }
    }

    //option--scorecard button
    else if(gamestate==5 && mx>=300 && mx<=500 && my>=400 && my<=480)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            gamestate=7;
            PlaySound("click.wav", NULL, SND_ASYNC);
        }
    }

    //option--highscore button
    else if(gamestate==5 && mx>=300 && mx<=500 && my>=300 && my<=380)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            gamestate=8;
            PlaySound("click.wav", NULL, SND_ASYNC);
        }
    }

    //option--credit button
    else if(gamestate==5 && mx>=300 && mx<=500 && my>=100 && my<=180)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            gamestate=9;
            PlaySound("click.wav", NULL, SND_ASYNC);
        }
    }

    //option--scorecard,instruction,highscore,credit--back button
    else if((gamestate==6 || gamestate==7 || gamestate==8 || gamestate==9)&& mx>=0 && mx<=50 && my>=462 && my<=512)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            gamestate=5;
            PlaySound("click.wav", NULL, SND_ASYNC);
        }
    }

    // Quit button
    else if(gamestate==1 && mx>=300 && mx<=500 && my>=150 && my<=230)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            exit(0);
            PlaySound("click.wav", NULL, SND_ASYNC);
        }
    }

    //resume screen--resume game
    else if(gamestate==4 && mx>=300 && mx<=500 && my>=216 && my<=296)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && showing_finishline==1)
        {
            //PlaySound("click.wav", NULL, SND_ASYNC);
            PlaySound("crew.wav", NULL, SND_ASYNC | SND_LOOP);
            iResumeTimer(0);
            iResumeTimer(1);
            iResumeTimer(2);
            iResumeTimer(3);
            gamestate=2;
            showing_finishline=0;
        }
    }

    //resume screen--menu button
    else if(gamestate==4 && mx>=300 && mx<=500 && my>=316 && my<=396)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            PlaySound("click.wav", NULL, SND_ASYNC);
            gamestate=1;
        }
    }

    //endscreen name input
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && gamestate==3)
    {
        if(mx >= 275 && mx <= 525 && my >= 120 && my <= 150 && mode == 0)
        {
            mode = 1;
        }
    }

    //gamescreen--pause button
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && gamestate==2)
    {
        if(mx >= 703 && mx <= 753 && my >= 235 && my <= 285 && showing_finishline==0)
        {
            PlaySound(NULL,0,0);
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);
            gamestate=4;
            showing_finishline=1;
        }
    }
}

//store score and name in a text file
void store_score()
{
    FILE *f1, *f2;
    f1= fopen("score.txt","a");
    f2= fopen("playername.txt","a");
    fprintf(f2, "%s\n",store_string);
    fprintf(f1, "%d\n",score);

    fclose(f1);
    fclose(f2);
}

void read_score()
{
    FILE *fp, *tp;
    fp= fopen("playername.txt","r");
    tp= fopen("score.txt","r");

    //counting player number
    number_of_players=0;
    char p;
    p=getc(fp);
    while(p != EOF)
    {
        //printf("%c",p);
        if(p == '\n')
        {
            number_of_players++;
        }
        p=getc(fp);
    }
    fseek(fp, 0, SEEK_SET);

    PlayerScore= (pscore*)calloc(number_of_players,sizeof(pscore));
    //printf("\n%d %d\n",number_of_players,PlayerScore);

    char line[31];
    for(int i=0; i<number_of_players; i++)
    {
        //fgets(line,31,fp);
        fscanf(fp,"%s",PlayerScore[i].name);
        fscanf(tp,"%d",&PlayerScore[i].score);
    }

    fclose(fp);
    fclose(tp);
}

void show_scorecard()
{
    read_score();
    int quaso=5,index=1;
    int scorenamepositionx=140, scorepositionx=400, scorepositiony=320;
    char individual_score[100],individual_name[100];

    for(int i=number_of_players-1; quaso>0 && i>=0 ; i--,quaso--)
    {
        sprintf(individual_name,"%d) %s",index,PlayerScore[i].name);
        sprintf(individual_score,"%d",PlayerScore[i].score);
        iText(scorenamepositionx, scorepositiony,individual_name, GLUT_BITMAP_HELVETICA_18);
        iText(scorepositionx, scorepositiony,individual_score, GLUT_BITMAP_HELVETICA_18);
        scorepositiony-=50;
        index++;
    }

    free(PlayerScore);
}

void High_score()
{
    read_score();
    pscore temp;
    for(int i=0; i<number_of_players-1; i++)
    {
        for(int j=i+1; j<number_of_players; j++)
        {
            if(PlayerScore[j].score>PlayerScore[i].score)
            {
                strcpy(temp.name,PlayerScore[j].name);
                temp.score=PlayerScore[j].score;

                strcpy(PlayerScore[j].name,PlayerScore[i].name);
                PlayerScore[j].score=PlayerScore[i].score;

                strcpy(PlayerScore[i].name,temp.name);
                PlayerScore[i].score=temp.score;
            }
        }
    }
}

void show_highscore()
{
    High_score();
    int quaso=5,index=1;
    int scorenamepositionx=140, scorepositionx=400, scorepositiony=320;
    char individual_score[100],individual_name[100];

    for(int i=0; quaso>0 && i<5 ; i++,quaso--)
    {
        sprintf(individual_name,"%d) %s",index,PlayerScore[i].name);
        sprintf(individual_score,"%d",PlayerScore[i].score);
        iText(scorenamepositionx, scorepositiony,individual_name, GLUT_BITMAP_HELVETICA_18);
        iText(scorepositionx, scorepositiony,individual_score, GLUT_BITMAP_HELVETICA_18);
        scorepositiony-=50;
        index++;
    }

    free(PlayerScore);
}

void iKeyboard(unsigned char key)
{
    if (key == 'q')
    {
        exit(0);
    }
    if (key == ' ' && (gamestate==2 || gamestate==4))
    {
        // resuming game
        if(showing_finishline==1)
        {
            //PlaySound("crew.wav", NULL, SND_ASYNC | SND_LOOP);
            iResumeTimer(0);
            iResumeTimer(1);
            iResumeTimer(2);
            iResumeTimer(3);
            gamestate=2;
            showing_finishline=0;
            m=1;
        }

        //pause game
        else if(showing_finishline==0)
        {
            PlaySound(NULL,0,0);
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);
            gamestate=4;
            showing_finishline=1;
        }
    }

    //inputing name of the player
    if(gamestate==3)
    {
        int i;
        if(mode == 1)
        {
            if(key == '\r')
            {
                mode = 0;
                strcpy(store_string, intermediate_str);
                printf("%s\n", store_string);
                for(i = 0; i < len; i++)
                {
                    intermediate_str[i] = 0;
                }
                len = 0;
                store_score();    // score store
                gamestate=1;
            }
            else
            {
                intermediate_str[len] = key;
                len++;
            }
        }
    }

    //Nitro
    else if (key == 'x' && gamestate==2 && showing_finishline==0)
    {
        if(speed>=20)
        {
            y+=2;
            speed--;
        }
    }
}

// after nitro
void return_to_normal()
{
    if(speed<100)
    {
        speed+=1;
    }
}

void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
    if (key == GLUT_KEY_UP)
    {
        y+=2;
    }
    if (key == GLUT_KEY_DOWN && gamestate==2)
    {
        if(y>0)
        {
            y-=2;
        }
    }

    //right movement
    if (key == GLUT_KEY_RIGHT && gamestate==2)
    {
        x += 10;
        //out of the bounds
        if(x>532)
        {
            PlaySound(NULL,0,0);
            gamestate=3;
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);
        }
    }

    // left movement
    if (key == GLUT_KEY_LEFT && gamestate==2)
    {
        x -= 10;
        //out of the bounds
        if(x<228)
        {
            PlaySound(NULL,0,0);
            gamestate=3;
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);
        }
    }
}

void collusion(int barrier_x_index, int barrier_y)
{
    if((x+19>=line_start[barrier_x_index] && x+19<line_start[barrier_x_index]+76 && x+45>=line_start[barrier_x_index] && x+45<line_start[barrier_x_index]+76) && barrier_y >= y && barrier_y<=y+55)
    {
        gamestate=3;
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        PlaySound(NULL,0,0);
    }
    else if(x+19<line_start[barrier_x_index] && x+45>=line_start[barrier_x_index] && barrier_y >= y && barrier_y<=y+55)
    {
        gamestate=3;
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        PlaySound(NULL,0,0);
    }
    else if(x+19<=line_start[barrier_x_index]+76 && x+45>line_start[barrier_x_index]+76 && barrier_y >= y && barrier_y<=y+55)
    {
        gamestate=3;
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        PlaySound(NULL,0,0);
    }
}

int main()
{
    iSetTimer(speed,backgroundchange);
    iSetTimer(speed,barrier_change);
    iSetTimer(speed,barrier_change2);
    iSetTimer(speed,barrier_change3);
    iSetTimer(200,return_to_normal);

    iPauseTimer(0);
    iPauseTimer(1);
    iPauseTimer(2);
    iPauseTimer(3);

    iInitialize(bgwidth, bgheight, "NEED FOR SPEED");
    return 0;
}

