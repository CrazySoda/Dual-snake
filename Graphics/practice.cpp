#include <iostream>
#include <windows.h>
#include "iGraphics.h"
#include <string.h>

//game mechanics constant
#define ORIGIN_X 0
#define ORIGIN_Y 0
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define FIELD_WIDTH 700
#define SNAKE_SIZE 20
#define FOOD_SIZE 20
#define MAX_SNAKE_LENGTH 100
int r=0,g=0,b=0;

//inputing Name
char temp_str[100], store_string[100];
int len, mode;

//pause game
void pausegame();

//music
void playmusic();
char music0[]="music\\start.wav";
char music1[]="music\\game.wav";
char music2[]="music\\end.wav";
char hallm[]="music\\hall.wav";
bool music;
//winner
int winner=0;
//highscore
int highscore;
struct buttonCoOrdinate
{
    int x;
    int y;
} bCoOrdinate[3];

//menu
int mposx, mposy;
char homemenu[20]="bp\\snakeMenu.bmp";
char endgame[20]="bp\\snake_end.bmp";
char credit[20]="bp\\credits.bmp";
char musicOn[20]="bp\\Music_On.bmp";
char musicOff[20]="bp\\music_Off.bmp";
char dont[30]="bp\\DontbeSnek.bmp";
char hall[30]="bp\\highscores.bmp";
int gamestate=0;
int gameover=0;

//game mechanics
int speed1=1;
int speed2=1;
int snake1_x[MAX_SNAKE_LENGTH], snake1_y[MAX_SNAKE_LENGTH];
int snake1_length = 3;
int snake2_x[MAX_SNAKE_LENGTH], snake2_y[MAX_SNAKE_LENGTH];
int snake2_length=3;
char direction1 = 'M';
char direction2 = 'L';
int food_x = 200, food_y=100;

//score#1
int score1 = 0;
char score1_buffer[100];
char winnerplayer1[]="Player-white won";

//score#2
int score2 = 0;
char score2_buffer[100];
char winnerplayer2[]="Player-green won";

//draw
char draw[]="Draw";

void generate_food()
{
    food_x = rand() % (FIELD_WIDTH / FOOD_SIZE) * FOOD_SIZE;
    food_y = rand() % (SCREEN_HEIGHT / FOOD_SIZE) * FOOD_SIZE;
}
//collision function prototype
void check_snake1_collision();
void check_snake2_collision();


//Highscore
typedef struct
{
    char name[30];
    int score;
} snake_score;

int number_of_players;
snake_score *SnakeScore;
//drawTextBox
void drawTextBox()
{
    iSetColor(255, 255, 255);
    iText(300, 115, "Enter Your Name");
    iFilledRectangle(300, 80, 250, 30);
    iText(300, 70, "* Name Should Not exceed 30 characters *");
}
void show_name_on_the_screen()
{
    if(mode==1)
    {
        iSetColor(0, 0, 0);
        iText(310, 90, temp_str);
    }
    iSetColor(255, 255, 255);
    iText(300, 60, "* Click to activate the box, enter to finish. *");
}
//store score and name in a text file
void store_score()
{
    FILE *f1, *f2;
    f1= fopen("score.txt","a");
    f2= fopen("playername.txt","a");
    fprintf(f2,"%s\n",store_string);
    if (winner == 1)
    {
        fprintf(f1, "%d\n",score1);
    }
    else if(winner == 2)
    {
        fprintf(f1,"%d\n",score2);
    }

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

    SnakeScore= (snake_score*)calloc(number_of_players,sizeof(snake_score));

    char line[31];
    for(int i=0; i<number_of_players; i++)
    {
        //fgets(line,31,fp);
        fscanf(fp,"%s",SnakeScore[i].name);
        fscanf(tp,"%d",&SnakeScore[i].score);
    }

    fclose(fp);
    fclose(tp);
}

void High_score()
{
    read_score();
    snake_score temp;
    for(int i=0; i<number_of_players-1; i++)
    {
        for(int j=i+1; j<number_of_players; j++)
        {
            if(SnakeScore[j].score>SnakeScore[i].score)
            {
                strcpy(temp.name,SnakeScore[j].name);
                temp.score=SnakeScore[j].score;

                strcpy(SnakeScore[j].name,SnakeScore[i].name);
                SnakeScore[j].score=SnakeScore[i].score;

                strcpy(SnakeScore[i].name,temp.name);
                SnakeScore[i].score=temp.score;
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
        sprintf(individual_name,"%d) %s",index,SnakeScore[i].name);
        sprintf(individual_score,"%d",SnakeScore[i].score);
        iText(scorenamepositionx, scorepositiony,individual_name, GLUT_BITMAP_HELVETICA_18);
        iText(scorepositionx, scorepositiony,individual_score, GLUT_BITMAP_HELVETICA_18);
        scorepositiony-=50;
        index++;
    }

    free(SnakeScore);
}


// graphics Window
void iDraw()
{
    iClear();
    printf("%d",gamestate);
    if (gamestate == 0)//menu
    {
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        iPauseTimer(4);
        iPauseTimer(5);
        iPauseTimer(6);
        iPauseTimer(7);
        iPauseTimer(8);
        iShowBMP(0,0,homemenu);
        iSetColor(180,0,0);
        iFilledRectangle(FIELD_WIDTH, 458, 200, 50);
        iFilledRectangle(FIELD_WIDTH, 458-80, 200, 50);
        iFilledRectangle(FIELD_WIDTH, 458-2*80, 200, 50);
        iFilledRectangle(FIELD_WIDTH, 458-3*80, 200, 50);
        iFilledRectangle(FIELD_WIDTH, 458-4*80, 200, 50);
        iSetColor(255,255,255);
        iText(FIELD_WIDTH+30, 458+20, "NEW GAME", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(FIELD_WIDTH+30+15, 458+20-80, "SCORES", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(FIELD_WIDTH+30+15, 458+20-2*80, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(FIELD_WIDTH+30+10-2, 458+20-3*80, "SETTINGS", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(FIELD_WIDTH+30+40, 458+20-4*80, "QUIT", GLUT_BITMAP_TIMES_ROMAN_24);

    }
    else if (gamestate==1)//game_field
    {

        iResumeTimer(0);
        iResumeTimer(1);
        iResumeTimer(2);
        iResumeTimer(3);
        iResumeTimer(4);
        iResumeTimer(5);
        iResumeTimer(6);
        iResumeTimer(7);
        iResumeTimer(8);
        iSetColor(0,255,0);
        iRectangle(0,0,FIELD_WIDTH,SCREEN_HEIGHT);
        //draw snake#1
        for (int i = 0; i < snake1_length; i++)
        {
            iSetColor(255,255,255);
            iFilledRectangle(snake1_x[i], snake1_y[i], SNAKE_SIZE, SNAKE_SIZE);
        }
        //draw snake#2
        for (int i = 0; i < snake2_length; i++)
        {
            iSetColor(55,255,55);
            iFilledRectangle(snake2_x[i], snake2_y[i], SNAKE_SIZE, SNAKE_SIZE);
        }
        //draw food
        iSetColor(255, 0, 0);
        iFilledRectangle(food_x, food_y, FOOD_SIZE, FOOD_SIZE);
        //draw score#1
        iSetColor(255,255,255);
        itoa(score1,score1_buffer,10);
        iText(SCREEN_WIDTH-100, SCREEN_HEIGHT - 20, "Player-1:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(SCREEN_WIDTH-70, SCREEN_HEIGHT - 40, score1_buffer,GLUT_BITMAP_TIMES_ROMAN_24);

        //draw score#2
        iSetColor(55,255,55);
        itoa(score2,score2_buffer,10);
        iText(SCREEN_WIDTH-100, SCREEN_HEIGHT - 220, "Player-2:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(SCREEN_WIDTH-70, SCREEN_HEIGHT - 240, score2_buffer,GLUT_BITMAP_TIMES_ROMAN_24);
        //check collision with other snake
        check_snake1_collision();
        check_snake2_collision();
    }
    else if(gamestate == 2)//gameover
    {
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        iPauseTimer(4);
        iPauseTimer(5);
        iPauseTimer(6);
        iPauseTimer(7);
        iPauseTimer(8);
        iShowBMP(0,0,endgame);
        iSetColor(255,255,255);
        iFilledRectangle((SCREEN_WIDTH/2)-100,SCREEN_HEIGHT/2,300,50);
        iFilledRectangle((SCREEN_WIDTH/2)-100,SCREEN_HEIGHT/2 + 50,300,50);

        if(winner == 2)
        {
            iSetColor(0,0,0);
            iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 + 20, winnerplayer2,GLUT_BITMAP_TIMES_ROMAN_24);
            iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 + 70, "Thanks for playing",GLUT_BITMAP_TIMES_ROMAN_24);
            iSetColor(255, 255, 255);
            char show_score[100];
            sprintf(show_score,"Score: %d",score2);
            High_score();

            iText(350, 407, show_score, GLUT_BITMAP_TIMES_ROMAN_24);
            iSetColor(255, 255, 255);
            if(score1<20)
            {
                iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 - 70, "What are you guys doing", GLUT_BITMAP_TIMES_ROMAN_24);
            }
            if(score1>SnakeScore[0].score)
            {
                iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 - 50, "New HighScore!!! You Are A Champ!!!", GLUT_BITMAP_TIMES_ROMAN_24);
            }
            else
            {
                iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 - 50, "Good Game!! Lets grab the HIGHSCORE", GLUT_BITMAP_TIMES_ROMAN_24);
            }
            drawTextBox();
            show_name_on_the_screen();
        }
        else if(winner == 1)
        {
            iSetColor(0,0,0);
            iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 + 20, winnerplayer1,GLUT_BITMAP_TIMES_ROMAN_24);
            iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 + 70, "Thanks for playing",GLUT_BITMAP_TIMES_ROMAN_24);
            iSetColor(255, 255, 255);
            char show_score[100];
            sprintf(show_score,"Score: %d",score1);
            High_score();

            iText(350, 407, show_score, GLUT_BITMAP_TIMES_ROMAN_24);
            iSetColor(255, 255, 255);
            if(score1<20)
            {
                iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 - 70, "What are you guys doing", GLUT_BITMAP_TIMES_ROMAN_24);
            }
            if(score1>SnakeScore[0].score)
            {
                iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 - 50, "New HighScore!!! Player-1 you are a Chsamp!!!", GLUT_BITMAP_TIMES_ROMAN_24);
            }
            else
            {
                iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 - 50, "Good Game!! Lets grab Player-1 HIGHSCORE", GLUT_BITMAP_TIMES_ROMAN_24);
            }
            drawTextBox();
            show_name_on_the_screen();
        }



    }
    else if(gamestate == 3)//draw
    {
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        iPauseTimer(4);
        iPauseTimer(5);
        iPauseTimer(6);
        iPauseTimer(7);
        iPauseTimer(8);
        iShowBMP(0,0,endgame);
        iSetColor(255,255,255);
        iFilledRectangle((SCREEN_WIDTH/2)-100,SCREEN_HEIGHT/2,300,50);
        iFilledRectangle((SCREEN_WIDTH/2)-100,SCREEN_HEIGHT/2 + 50,300,50);
        if(winner == 3)
        {
            iSetColor(0,0,0);
            iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 + 20, draw,GLUT_BITMAP_TIMES_ROMAN_24);
            iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 + 70, "Thanks for playing",GLUT_BITMAP_TIMES_ROMAN_24);
            iText((SCREEN_WIDTH/2)-70, SCREEN_HEIGHT/2 - 30, "No highscorer in Draw",GLUT_BITMAP_TIMES_ROMAN_24);
        }

    }
    else if(gamestate == 4)//credits
    {

        iShowBMP(0,0,credit);
    }
    else if(gamestate == 5)//musicOn
    {
        iShowBMP(0,0,musicOn);
    }
    else if(gamestate == 6)//musicOff
    {
        iShowBMP(0,0,musicOff);
    }
    else if(gamestate == 7)//troll
    {
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        iPauseTimer(4);
        iPauseTimer(5);
        iPauseTimer(6);
        iPauseTimer(7);
        iPauseTimer(8);
        iShowBMP(0,0,dont);
    }
    else if(gamestate == 8)//troll
    {
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        iPauseTimer(4);
        iPauseTimer(5);
        iPauseTimer(6);
        iPauseTimer(7);
        iPauseTimer(8);
        iShowBMP(0,0,dont);
    }
    if(gamestate == 9)
    {
    iShowBMP(0,0,hall);
    iSetColor(255, 255, 255);
    show_highscore();
    }
}


void iMouseMove(int x,int y)
{
}
/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my)
{
    //check
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        printf("x = %d, y= %d\n",mx,my);
    }
    //play game
    if(mx >= FIELD_WIDTH && mx <= FIELD_WIDTH+200 && my >= 458 && my <= 458+50 && gamestate == 0)
    {
        gamestate=1;
        playmusic();
    }
    //scores
    if(mx >= FIELD_WIDTH && mx <= FIELD_WIDTH+200 && my >= 458-80 && my <= 458-80+50 && gamestate == 0)
    {
        gamestate=9;
        playmusic();
    }
    //credits
    if(mx >= FIELD_WIDTH && mx <= FIELD_WIDTH+200 && my >= 458-160 && my <= 458-160+50 && gamestate == 0)
    {
        gamestate=4;
    }
    //settings
    if(mx >= FIELD_WIDTH && mx <= FIELD_WIDTH+200 && my >= 458-240 && my <= 458-240+50 && gamestate == 0)
    {
        gamestate=5;
    }
    //musicOff
    if(mx >= 205 && mx <= 445 && my >= 255 && my <= 295 && gamestate == 5)
    {
        gamestate=6;
        playmusic();

    }
    //musicOn
    if(mx >= 205 && mx <= 445 && my >= 295 && my <= 330 && gamestate == 6)
    {
        gamestate=5;
        playmusic();

    }
    if(mx >= FIELD_WIDTH && mx <= FIELD_WIDTH+200 && my >= 458-320 && my <= 458-320+50 && gamestate == 0)
    {
        exit(0);
    }
//endscreen name input
    if(mx >= 300 && mx <= 550 && my >= 80 && my <= 110 && mode == 0 && (gamestate == 2||gamestate == 3))
    {
        mode = 1;
    }

}
void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case 'w':
    case 'W':
        if (direction1 != 'N')
        {
            direction1 = 'H';
        }
        break;
    case 's':
    case 'S':
        if (direction1 != 'H')
        {
            direction1 = 'N';
        }
        break;
    case 'a':
    case 'A':
        if (direction1 != 'M')
        {
            direction1 = 'B';
        }
        break;
    case 'd':
    case 'D':
        if (direction1 != 'B')
        {
            direction1 = 'M';
        }
        break;
    }
//inputing name of the player
    if(gamestate==3 || gamestate == 2)
    {
        int i;
        if(mode == 1)
        {
            if(key == '\r')
            {
                mode = 0;
                strcpy(store_string, temp_str);
                printf("%s\n", store_string);
                for(i = 0; i < len; i++)
                {
                    temp_str[i] = 0;
                }
                len = 0;
                store_score();
                gamestate = 0;
                playmusic();
                // score store
            }
            else
            {
                temp_str[len] = key;
                len++;
            }
        }
    }

}

//place your codes for other keys here


/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        if (direction2 != 'K')
        {
            direction2 = 'I';
        }
        break;
    case GLUT_KEY_DOWN:
        if (direction2 != 'I')
        {
            direction2 = 'K';
        }
        break;
    case GLUT_KEY_LEFT:

        if (direction2 != 'L')
        {
            direction2 = 'J';
        }
        break;
    case GLUT_KEY_RIGHT:
        if (direction2 != 'J')
        {
            direction2 = 'L';
        }
    case GLUT_KEY_F5:
        if(gamestate!=1)
            gamestate = 0;
            playmusic();
        break;
    case GLUT_KEY_F3:
        if(gamestate == 7)
        {
            gamestate = 2;
            playmusic();
        }
        else if(gamestate == 8)
        {
            gamestate = 3;
            playmusic();

        }

    }


}

//snake#1 movement
void move_snake1()
{
    for (int i = snake1_length - 1; i > 0; i--)
    {
        snake1_x[i] = snake1_x[i - 1];
        snake1_y[i] = snake1_y[i - 1];
    }
    switch (direction1)
    {
    case 'H':
        snake1_y[0] += speed1*SNAKE_SIZE;
        break;
    case 'N':
        snake1_y[0] -= speed1*SNAKE_SIZE;
        break;
    case 'B':
        snake1_x[0] -= speed1*SNAKE_SIZE;
        break;
    case 'M':
        snake1_x[0] += speed1*SNAKE_SIZE;
        break;
    }

    // Check if the snake has gone off the edge of the screen
    if (snake1_x[0] < 0)
    {
        snake1_x[0] = FIELD_WIDTH - SNAKE_SIZE;
    }
    if (snake1_x[0] >= FIELD_WIDTH)
    {
        snake1_x[0] = 0;
    }
    if (snake1_y[0] < 0)
    {
        snake1_y[0] = SCREEN_HEIGHT - SNAKE_SIZE;
    }
    if (snake1_y[0] >= SCREEN_HEIGHT)
    {
        snake1_y[0] = 0;
    }

}

void check_snake1_collision()
{
    for(int i=1; i<snake2_length; i++)
    {
        if (snake1_x[0] == snake2_x[i] && snake1_y[0] == snake2_y[i])
        {
            gamestate = 7;
            winner = 2;

            break;
        }
    }
}

void check_self1_collision()
{
    for (int i = 1; i < snake1_length; i++)
    {
        if (snake1_x[0] == snake1_x[i] && snake1_y[0] == snake1_y[i])
        {
            speed1*=-1;
            int new1_length = i;
            for (int j = new1_length; j < snake1_length; j++)
            {
                snake1_x[j] = 0;
                snake1_y[j] = 0;

            }
            snake1_length = new1_length;
            score1-=20;
        }
    }
}
//snake2
void move_snake2()
{
    for (int i = snake2_length - 1; i > 0; i--)
    {
        snake2_x[i] = snake2_x[i - 1];
        snake2_y[i] = snake2_y[i - 1];
    }
    switch (direction2)
    {
    case 'I':
        snake2_y[0] += speed2*SNAKE_SIZE;
        break;
    case 'K':
        snake2_y[0] -= speed2*SNAKE_SIZE;
        break;
    case 'J':
        snake2_x[0] -= speed2*SNAKE_SIZE;
        break;
    case 'L':
        snake2_x[0] += speed2*SNAKE_SIZE;
        break;
    }

    // Check if the snake has gone off the edge of the screen
    if (snake2_x[0] < 0)
    {
        snake2_x[0] = FIELD_WIDTH - SNAKE_SIZE;
    }
    if (snake2_x[0] >= FIELD_WIDTH)
    {
        snake2_x[0] = 0;
    }
    if (snake2_y[0] < 0)
    {
        snake2_y[0] = SCREEN_HEIGHT - SNAKE_SIZE;
    }
    if (snake2_y[0] >= SCREEN_HEIGHT)
    {
        snake2_y[0] = 0;
    }

}

void check_snake2_collision()
{
    for(int i=1; i<snake1_length; i++)
    {

        if (snake2_x[0] == snake1_x[i] && snake2_y[0] == snake1_y[i])
        {
            gamestate = 7;
            winner =1;
            break;
        }

    }
}
void check_self2_collision()
{
    for (int i = 1; i < snake2_length; i++)
    {
        if (snake2_x[0] == snake2_x[i] && snake2_y[0] == snake2_y[i])
        {
            int new2_length = i;
            speed2*=-1;
            for (int j = new2_length; j < snake2_length; j++)
            {
                snake2_x[j] = 0;
                snake2_y[j] = 0;

            }
            snake2_length = new2_length;
            score2 -=20;
        }

    }
}

void check_draw()
{
    if (snake2_x[0] == snake1_x[0] && snake2_y[0] == snake1_y[0])
    {
        gamestate = 8;
        winner = 3;
    }
}

void check_food_collision()
{
    if (snake1_x[0] == food_x && snake1_y[0] == food_y)
    {
        snake1_length++;
        generate_food();

        score1+=5;
    }
    else if (snake2_x[0] == food_x && snake2_y[0] == food_y)
    {
        snake2_length++;
        generate_food();
        score2+=5;
    }
}
void playmusic()
{
    if(gamestate==0)
    {
        PlaySound(music0,NULL,SND_LOOP|SND_ASYNC);
    }
    else if (gamestate == 1)
    {
        PlaySound(music1,NULL,SND_LOOP|SND_ASYNC);
    }
    else if(gamestate==2)
    {
        PlaySound(music2,NULL,SND_LOOP|SND_ASYNC);
    }
    else if(gamestate==3)
    {
        PlaySound(music2,NULL,SND_LOOP|SND_ASYNC);

    }
    else if(gamestate == 6)
    {
        PlaySound(0,0,0);
    }
    else if(gamestate == 5)
    {
        PlaySound(music0,NULL,SND_LOOP|SND_ASYNC);
    }
    else if(gamestate == 9)
    {
        PlaySound(hallm,NULL,SND_LOOP|SND_ASYNC);
    }
}

int main()
{
    snake1_y[0]=300,snake2_y[0]=200;
    //snake movements
    iSetTimer(60,move_snake1);
    iSetTimer(60,move_snake2);
    //check food collision
    iSetTimer(20,check_food_collision);
    //check self collision
    iSetTimer(20,check_self1_collision);
    iSetTimer(20,check_self2_collision);
    //check snake collision
    iSetTimer(5,check_draw);
    iSetTimer(20,check_snake1_collision);
    iSetTimer(20,check_snake2_collision);
    if(gamestate == 0)
    {
        playmusic();
    }

    iInitialize(SCREEN_WIDTH,SCREEN_HEIGHT,"My Snake Game");

    return 0;
}
