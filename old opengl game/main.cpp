#include <math.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "SDL_mixer.h"
#include "lib.h"
#define floorsize 0.4

//class that handles the current state of the camera
class camstate
{
    private:
    float sx,sz,vz,az;
    public:
    bool isBraked;          //whether the brakes are pressed or not
    void Control();         //controls the movement of the car,updates the variables sx,sz,vz and az
    void MoveCam();         //updates the position of the camera depending on the values of sx,sz,vz and az
    void SetCheck();        //sets the last checkpoint through which the car passed
    void Reset();           //to restart the game,resets various values to load the game to its initial state
    void CheckCollision();  //checks te collision between the car and red squares
    void AssignCheck();     //assigns the last checkpoint values to the car
    void SoundControl();    //contols the sound depending on vz,az
    camstate()
    {
        sx=0;               //displacement along the x direction
        sz=0;               //dispacement along the z direction
        vz=0;               //velocity along the z direction
        az=0;               //accelaration along z direction
        isBraked = false;   //whether brakes are pressed or not
    }
};
camstate cam;               //the object for camera state
camstate checkpoint;        //object for the state of checkpoint


//class to contain details of 3D objects being loaded
class objectstate
{
    private:
    int object;//stores the index of  3D object,each object will have a different value of
    public:
    void PutObj(float x,float y,float z,float a,float b,float c,float d,float s);//to put object at a given loacation
    void LoadObj(int a)
    {
        object=a;
    }
};
objectstate tractor,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,topscore,starttext,midtext,endtext;

struct hole
{
    float x,z;
    hole(float a,float c)
    {
        x=a;
        z=c;
    }
};
std::vector<hole*> holes;

int runstate=0;
int intime=0,oldtime,newtime,checktime=0,penalty=0;
int score[5];
float angle=0;
objloader obj;
std::fstream file;

void SoundInit();//loads sound files
void DrawFloor();//draw the floor
void InitHole();//initialise the position of holes and save them in a vector
void ReadScore();//read score from file
void DisplayTimeAndScore();
void DisplayThree(int,float,float,float,float,float);//to display three digit numbers
void ChangeScore();//to update the score into file if end is reached
void KillA();//to close audio and delete chunks
void SoundReset();//reset sound
Mix_Chunk* all1,*race,*raceend,*brake,*drive1,*drive2,*turn;
Mix_Music* music;

void init()
{
    glClearColor(0,0,0.,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glShadeModel(GL_FLAT);
    gluPerspective(45,640/480,1,500);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    float amb[]={0.15,0.15,0.15,1};
    glLightfv(GL_LIGHT1,GL_AMBIENT,amb);
    float col[]={1,1,1,1};
    glLightfv(GL_LIGHT1,GL_DIFFUSE,col);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE,GL_LINEAR);
    glFogf(GL_FOG_START,1);
    glFogf(GL_FOG_END,17);
    float fogcol[]={0,0,0,1};
    glFogfv(GL_FOG_COLOR,fogcol);
    tractor.LoadObj(obj.load("tractor.obj"));
    starttext.LoadObj(obj.load("text.obj"));
    a0.LoadObj(obj.load("0.obj"));
    a1.LoadObj(obj.load("1.obj"));
    a2.LoadObj(obj.load("2.obj"));
    a3.LoadObj(obj.load("3.obj"));
    a4.LoadObj(obj.load("4.obj"));
    a5.LoadObj(obj.load("5.obj"));
    a6.LoadObj(obj.load("6.obj"));
    a7.LoadObj(obj.load("7.obj"));
    a8.LoadObj(obj.load("8.obj"));
    a9.LoadObj(obj.load("9.obj"));
    topscore.LoadObj(obj.load("topscore.obj"));
    midtext.LoadObj(obj.load("midtext.obj"));
    endtext.LoadObj(obj.load("endtext.obj"));
    file.open("score.txt",std::ios::in);
    ReadScore();
    SoundInit();
}
float w=0,x=0,y=0,z=0;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if(runstate==0)
    {
        float pos[]={1.2,15,30,1};
        glLightfv(GL_LIGHT1,GL_POSITION,pos);
        starttext.PutObj(0.07,0.35,-7,90,1,0,0,0.6);
        glTranslatef(0,1,-8);
        glRotatef(60,1,0,0);
        DrawFloor();
    }
    else if(runstate==1)
    {
        float pos[]={2,-2.7,100,1};
        glLightfv(GL_LIGHT1,GL_POSITION,pos);
        glTranslatef(0,1-w,-8+x);
        glRotatef(60-y,1,0,0);
        DrawFloor();
        tractor.PutObj(0,-0.6545,-z,180-angle,0,1,0,0.06);
        w+=0.001;
        x+=0.008;
        y+=0.06;
        z+=0.002;
        if(w>1)
        {
            runstate=2;
            intime=0;
        }
    }
    else
    {
        float pos[]={0,1,-2,1};
        glLightfv(GL_LIGHT1,GL_POSITION,pos);
        if(runstate==2)
        {
            cam.Control();
            midtext.PutObj(0,0.8,-2,90,1,0,0,0.1);
        }
        else if(runstate==3)
        {
            endtext.PutObj(-0.22,0.5,-10,90,1,0,0,1);
            DisplayThree((intime+penalty)/1000,-0.2,0.5,-10,0.33,0.15);
            Mix_HaltChannel(-1);
        }
        tractor.PutObj(0,-0.6545,-2,180-angle,0,1,0,0.06);
        glPushMatrix();
        cam.MoveCam();
        DrawFloor();
        glPopMatrix();
        DisplayTimeAndScore();
    }
}
bool isRunning=true,pause=false;
int main(int argc,char* args[])
{
    SDL_Event event;
    bool GameHasStarted=false;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetVideoMode(1024,650,32,SDL_SWSURFACE|SDL_OPENGL);
    glViewport(0,0,1024,650);
    SDL_WM_SetCaption("cube",NULL);
    init();
    InitHole();
    oldtime=SDL_GetTicks();
    while(isRunning)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type==SDL_QUIT)
            isRunning=false;
            else if(event.type==SDL_KEYUP)
            {
                if(GameHasStarted==false && event.key.keysym.sym==SDLK_RETURN)
                {
                    runstate=1;
                    GameHasStarted=true;
                }
                else if(event.key.keysym.sym==SDLK_q)
                {
                    cam.SetCheck();
                    intime=checktime;
                    penalty+=5000;
                }
                else if(event.key.keysym.sym==SDLK_p)
                {
                    pause=!pause;
                }
                else if(event.key.keysym.sym==SDLK_y && runstate==3 || event.key.keysym.sym==SDLK_F2)
                {
                    cam.Reset();
                    checkpoint.Reset();
                    SoundReset();
                    runstate=0;
                    intime=0;
                    penalty=0;
                    checktime=0;
                    GameHasStarted=false;
                }
                else if(event.key.keysym.sym==SDLK_n && runstate==3)
                {
                    isRunning=false;
                }
            }
        }
        newtime=SDL_GetTicks();
        if(!pause)
        {
            if(runstate!=3)
            {
                intime=intime+newtime-oldtime;
                cam.CheckCollision();
            }
            if(Mix_Paused(0))
            Mix_Resume(0);
            if(Mix_Paused(1))
            Mix_Resume(1);
            if(Mix_Paused(2))
            Mix_Resume(2);
            if(Mix_Paused(4))
            Mix_Resume(4);
            if(Mix_Paused(5))
            Mix_Resume(5);
            if(Mix_Paused(6))
            Mix_Resume(6);
            display();
            SDL_GL_SwapBuffers();
        }
        else
        {
            if(Mix_Playing(0))
            Mix_Pause(0);
            if(Mix_Playing(1))
            Mix_Pause(1);
            if(Mix_Playing(2))
            Mix_Pause(2);
            if(Mix_Playing(4))
            Mix_Pause(4);
            if(Mix_Playing(5))
            Mix_Pause(5);
            if(Mix_Playing(6))
            Mix_Pause(6);
        }
        oldtime=newtime;
    }
    KillA();
    return 0;
}
void camstate::Control()
{

    Uint8* state=SDL_GetKeyState(NULL);
    //z axis motion
    if(state[SDLK_UP])
    {
        cam.az-=0.00002;
        if(cam.az<-0.002)
        cam.az=-0.002;
    }
    if(state[SDLK_DOWN])
    {
        cam.az=0.0004;
    }
    if(state[SDLK_z])
    {
        isBraked = true;
        cam.az=0.001;
    }
    else
    {
        isBraked = false;
    }
    if(!state[SDLK_UP]&&!state[SDLK_DOWN]&&!state[SDLK_z])
    {
        cam.az=0;
    }
    cam.vz+=cam.az;
    if(cam.sz>-100)
    {
        if(cam.vz<-0.065)
        cam.vz=-0.065;
    }
    else if(cam.sz>-375)
    {
        if(cam.vz<-0.075)
        cam.vz=-0.075;
    }
    else
    {
        if(cam.vz<-0.085)
        cam.vz=-0.085;
    }
    if(cam.vz<0)
    cam.vz+=0.00003;
    if(cam.vz>0)
    cam.vz-=0.001;
    cam.sz+=cam.vz;

    if(-2+cam.sz>1)
    cam.sz=3;
    else if(-2+cam.sz<-799)
    {
        cam.sz=-797;
        ChangeScore();
        runstate=3;
    }

    SoundControl();

    //x axis motion
    int width=4;
    if(-2+cam.sz<=-200 && -2+cam.sz>-400)
    width=3;
    else if(-2+cam.sz<=-400 && -2+cam.sz>-600)
    width=2;
    else if(-2+cam.sz<=-600&&-2+cam.sz>-800)
    width=1;
    if(state[SDLK_LEFT]&&!state[SDLK_RIGHT])
    {
        cam.sx-=0.01;
        angle=-20;
    }
    else if(state[SDLK_RIGHT]&&!state[SDLK_LEFT])
    {
        cam.sx+=0.01;
        angle=20;
    }
    else
    {
        angle=0;
    }
    if(cam.sx<-(width+0.46)*floorsize)
    cam.sx=-(width+0.3)*floorsize;
    else if(cam.sx>(width+0.46)*floorsize)
    cam.sx=(width+0.3)*floorsize;

    cam.AssignCheck();
    //y axis motion
}

void camstate::MoveCam()
{
    glTranslatef(-sx,0,-sz);
}

void camstate::SetCheck()
{
    sx=checkpoint.sx;
    sz=checkpoint.sz;
    az=0;
    vz=0;
}

void camstate::Reset()
{
    sx=0;
    sz=0;
    vz=0;
    az=0;
}

void camstate::CheckCollision()
{
    float num;
    int pos;
    num=(2.85-cam.sz+floorsize)/floorsize;
    pos=floor(num);
    if(cam.sx<holes[pos]->x+floorsize/2+0.011 && cam.sx>holes[pos]->x-floorsize/2+0.011)
    cam.vz=0.05;
}

void camstate::AssignCheck()
{
    if(cam.vz<0)
    {
        if((cam.sz<-99.9&&cam.sz>-100.1)||(cam.sz<-199.9&&cam.sz>-200.1)||(cam.sz<-299.9&&cam.sz>-300.1)||(cam.sz<-399.9&&cam.sz>-400.1)||(cam.sz<-499.9&&cam.sz>-500.1)||(cam.sz<-599.9&&cam.sz>-600.1)|(cam.sz<-699.9&&cam.sz>-700.1))
        {
            checkpoint.sx=cam.sx;
            checkpoint.sz=cam.sz;
            checktime=intime;
        }
    }
}

void objectstate::PutObj(float x,float y,float z,float a,float b,float c,float d,float s)
{
    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(a,b,c,d);
    glScalef(s,s,s);
    glCallList(object);
    glPopMatrix();
}

void SoundInit()
{
    Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
    all1 = Mix_LoadWAV("all.wav");
    drive1 = Mix_LoadWAV("drive1.wav");
    race = Mix_LoadWAV("race.wav");
    raceend = Mix_LoadWAV("raceend.wav");
    brake= Mix_LoadWAV("brake.wav");
    turn = Mix_LoadWAV("turn.wav");
    Mix_Volume(0,30);//all1
    Mix_Volume(1,70);//brake
    Mix_Volume(2,25);//drive1
    Mix_Volume(4,50);//race
    Mix_Volume(5,60);//raceend
    Mix_Volume(6,70);//turn
}

void SoundReset()
{
    Mix_HaltChannel(0);
    Mix_HaltChannel(1);
    Mix_HaltChannel(2);
    Mix_HaltChannel(4);
    Mix_HaltChannel(5);
    Mix_HaltChannel(6);
}

void camstate::SoundControl()
{
    if(!Mix_Playing(0))
    Mix_PlayChannel(0,all1,0);

    if(angle!=0)
    {
        if(!Mix_Playing(6))
        Mix_FadeInChannel(6,brake,0,100);
        Mix_Volume(4,30);
    }
    else
    {
        Mix_Volume(4,50);
        Mix_HaltChannel(6);
    }

    if(cam.az<0)
    {
        if(Mix_Playing(1))
        Mix_HaltChannel(1);
        if(!Mix_Playing(4))
        Mix_PlayChannel(4,race,0);
    }
    else if(cam.az==0)
    {
        if(Mix_Playing(1))
        Mix_HaltChannel(1);
        if(Mix_Playing(4))
        {
            Mix_HaltChannel(4);
            if(!Mix_Playing(5)&&cam.vz<-0.06)
            Mix_PlayChannel(5,raceend,0);
        }
    }
    else
    {
        if(Mix_Playing(4))
        {
            Mix_HaltChannel(4);
        }
        if(!Mix_Playing(1)&&cam.isBraked)
        Mix_PlayChannel(1,brake,0);

    }

    if(cam.vz>0)
    {
        Mix_FadeOutChannel(4,100);
        Mix_HaltChannel(2);
        Mix_HaltChannel(3);
        if(Mix_Playing(1))
        Mix_HaltChannel(1);
    }
    else
    {
        if(cam.vz>-0.07)
        {
            if(cam.vz<-0.01)
            {
                if(Mix_Playing(3))
                Mix_HaltChannel(3);
                if(!Mix_Playing(2))
                Mix_FadeInChannel(2,drive1,0,500);
            }
            else
            {
                if(Mix_Playing(1))
                Mix_HaltChannel(1);
                if(Mix_Playing(5))
                Mix_FadeOutChannel(5,50);
                if(Mix_Playing(2))
                Mix_HaltChannel(2);
                if(Mix_Playing(3))
                Mix_HaltChannel(3);
                if(Mix_Playing(4))
                Mix_FadeOutChannel(4,100);
            }
        }
        else
        {
            if(Mix_Playing(2))
            Mix_FadeOutChannel(2,500);
            if(!Mix_Playing(3))
            Mix_FadeInChannel(3,drive2,0,500);
        }
    }
}

void KillA()
{
    Mix_FreeChunk(all1);
    Mix_FreeChunk(drive1);
    Mix_FreeChunk(drive2);
    Mix_FreeChunk(race);
    Mix_FreeChunk(raceend);
    Mix_FreeChunk(brake);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

void DrawFloor()
{
    glEnable(GL_COLOR_MATERIAL);
    int n=0,f=-1,width=4;
    float i,j;
    float y=-0.65;
    glBegin(GL_QUADS);
    for(j=1;j>=-800;j=j-floorsize)
    {
        if(j<-200 && j>-400)
        width=3;
        else if(j<-400 && j>-600)
        width=2;
        else if(j<-600&&j>-800)
        width=1;
        for(i=-4*floorsize;i<=4*floorsize+0.1;i=i+floorsize)
        {
            if(f==-1)
            glColor3f(0.3,0.45,0.3);
            else
            glColor3f(0.5,0.65,0.5);
            f=f*-1;
            if((2+j<-99.5&&2+j>-100.5)||(2+j<-199.5&&2+j>-200.5)||(2+j<-299.5&&2+j>-300.5)||(2+j<-399.5&&2+j>-400.5)||(2+j<-499.5&&2+j>-500.5)||(2+j<-599.5&&2+j>-600.5)||(2+j<-699.5&&2+j>-700.5))
            {
                glColor3f(0.4,0.5,0.4);
            }
            if(2+j<-795)
            glColor3f(1,1,1);
            if(holes[n]->x==i && holes[n]->z==j)
            {
                glColor3f(1,0,0);
            }
            if(i<-width*floorsize-0.1 || i>width*floorsize+0.1)
            continue;
            glNormal3f(0,1,0);
            glVertex3f(i-floorsize/2,y,j-floorsize/2);
            glVertex3f(i-floorsize/2,y,j+floorsize/2);
            glVertex3f(i+floorsize/2,y,j+floorsize/2);
            glVertex3f(i+floorsize/2,y,j-floorsize/2);
        }
        n++;
    }
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
}

void InitHole()
{
    float j,i;
    int t1=-1,t2,tempminus1=-1,tempminus2=-1;
    for(j=1;j>=-800;j=j-floorsize)
    {
        tempminus2=tempminus1;
        tempminus1=t1;
        t1=rand()%9;
        if(j<-600 && tempminus1+1==t1 && tempminus2+1==tempminus1)
            t1--;
        else if(tempminus1-1==t1 && tempminus2-1==tempminus1)
            t1++;
        for(t2=0,i=-4*floorsize;i<=4*floorsize;i=i+floorsize,t2++)
        {
            if(t1==t2)
            {
                holes.push_back(new hole(i,j));
            }
        }
    }
}

void ReadScore()
{
    char buffer[30];
    file.getline(buffer,30,'\n');
    sscanf(buffer,"%d %d %d %d %d ",&score[0],&score[1],&score[2],&score[3],&score[4]);
}

void DisplayTimeAndScore()
{
    int t;
    t=(intime+penalty)/1000;
    if(t>999)
    {
        DisplayThree(999,-3.9,3.5,-10,0.4,0.2);
    }
    else
    {
        DisplayThree(t,-3.9,3.5,-10,0.4,0.2);
    }
    topscore.PutObj(3.2,3.6,-10,90,1,0,0,0.6);
    DisplayThree(score[0],3.05,3.3,-10,0.3,0.15);
    DisplayThree(score[1],3.05,3.0,-10,0.3,0.15);
    DisplayThree(score[2],3.05,2.7,-10,0.3,0.15);
    DisplayThree(score[3],3.05,2.4,-10,0.3,0.15);
    DisplayThree(score[4],3.05,2.1,-10,0.3,0.15);
}

void ChangeScore()
{
    file.close();
    file.open("score.txt",std::ios::out);
    int i;
    for(i=0;i<5;i++)
    {
        if((intime+penalty)/1000<score[i])
        break;
    }
    for(int j=4;j>i;j--)
    {
        score[j]=score[j-1];
    }
    score[i]=(intime+penalty)/1000;
    file<<score[0]<<" "<<score[1]<<" "<<score[2]<<" "<<score[3]<<" "<<score[4]<<" ";
}

void DisplayThree(int t,float x,float y,float z,float scale,float gap)
{
    int t1,t2,t3;
    t3=t%10;
    t=t/10;
    t2=t%10;
    t=t/10;
    t1=t%10;

    if(t1==0)
    a0.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==1)
    a1.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==2)
    a2.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==3)
    a3.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==4)
    a4.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==5)
    a5.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==6)
    a6.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==7)
    a7.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==8)
    a8.PutObj(x,y,z,90,1,0,0,scale);
    else if(t1==9)
    a9.PutObj(x,y,z,90,1,0,0,scale);

    if(t2==0)
    a0.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==1)
    a1.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==2)
    a2.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==3)
    a3.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==4)
    a4.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==5)
    a5.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==6)
    a6.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==7)
    a7.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==8)
    a8.PutObj(x+gap,y,z,90,1,0,0,scale);
    else if(t2==9)
    a9.PutObj(x+gap,y,z,90,1,0,0,scale);

    if(t3==0)
    a0.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==1)
    a1.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==2)
    a2.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==3)
    a3.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==4)
    a4.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==5)
    a5.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==6)
    a6.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==7)
    a7.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==8)
    a8.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
    else if(t3==9)
    a9.PutObj(x+gap+gap,y,z,90,1,0,0,scale);
}
