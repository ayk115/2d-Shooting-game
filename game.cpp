#include<iostream>
#include<cmath>
#include<math.h>
#include<GL/glut.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
// Function Declarations
void drawScene();
void new_update(int value);
void drawBox(float len);
void drawSpiders(int i);
void drawCanon();
void drawLaser();
void drawLine();
void drawOuterBox();
void drawBucket(float bucket_len);
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void current();
void score();
void play();
void drawUpperBox();
void gameOver();
void drawBlackCount(float black);
void gamePaused();
void drawBlock();
void drawAssist();
void handleSelection(unsigned char key);
void handleMouseDrag(int x,int y);
char checkSelected();
// Global Variables
float box_len = 4.0f;

// Spider variables
float spi=0.1f;
float spi_vel[100000];
float spi_x[100000],spi_y[100000];
float spi_stat[100000]={0};
float spi_col[100000];

// Bucket variables
float bucket1_x=-1.5f;
float bucket2_x=1.5f;
float bucket_len=0.2f;
float bucket1_min_x=-1.8f;
float bucket1_max_x=1.8f;
float bucket2_min_x=-1.8f;
float bucket2_max_x=1.8f;
float bucket1_select=1.0f;
float bucket2_select=1.0f;

// Canon variables
float canon_x=0.0f,canon_y=-1.8f;
float canon_min_x=-1.9f;
float canon_max_x=1.9f;
float angle=0.0f;
float canon_select=0.5f;

// Laser varaibles
float laser_x[100000],laser_y[100000];
float laser_vel=0.05f;
int laser_stat[100000]={0};
float laser_vel_x[100000],laser_vel_y[100000];
float tip_x[100000],tip_y[100000];
float l_angle[100000]={0};
int laser=0;

// Alternate laser
float alt_laser_x[100000],alt_laser_y[100000];
float alt_laser_vel=0.01f;
float alt_laser_vel_x[100000],alt_laser_vel_y[100000];
float alt_l_angle[100000];
int laser_alt[100000]={0};

// Misc variables
char mode='b';
int points=0;
int on=1000;
int front=-1,count=0,high=-1;
int game_over=0;
int paus=0;
int game_play=0;
int level=5,game_level=0;
int black=0;
int black_timer=0;
// Mouse variables
int winw,winh;
float mousex,mousey,mousez;
double oglx,ogly;

// Main function
int main(int argc, char **argv) 
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w;
	int windowHeight = h;
	printf("Enter width and height of the window : ");
	scanf("%d%d",&windowWidth,&windowHeight);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);
	glutCreateWindow("Assignment 1");  // Setup the window
	initRendering();

	// Register callbacks
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	glutMotionFunc(handleMouseDrag);
	glutReshapeFunc(handleResize);
	glutTimerFunc(10, new_update, 0);
	glutMainLoop();
	return 0;
}
// Function to create new spiders and to ensure that they are produced randomly.
void current()
{
	int i;
	if(front<100000)
	{
		for(i=0;count<=level;i++)
		{
			if(i>front || spi_y[i]<=-5.0f)
			{
				float decimal=(rand()%100)*1.0f/100;
				float integer=(rand()%4)*1.0f;
				spi_x[i]=(decimal+integer)-2;
				if(spi_x[i]>1.95f)
					spi_x[i]-=0.05f;
				else if(spi_x[i]<-1.95f)
					spi_x[i]+=0.05f;
				spi_vel[i]=3*((rand()%10*1.0f)/10000+0.0003f);
				spi_y[i]=2.3f;spi_stat[i]=0;
				spi_col[i]=rand()%3;
				front++;
				count++;
			}
		}
	}
}
// Function to draw objects on the screen
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(game_play==0)
	{
		// Draw Box
		glPushMatrix();
		glTranslatef(0.0f, 0.0f,-5.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		drawBox(box_len);
		glPushMatrix();

		// Play!!!
		glPushMatrix();
		glColor3f(0.0f,0.0f,0.0f);
		play();
		glPopMatrix();
	}
	else if(game_over==0 && game_play==1)
	{
		current();
		// Draw Box
		glPushMatrix();
		glTranslatef(0.0f, 0.0f,-5.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		drawBox(box_len);
		glPushMatrix();

		//draw lower line
		glPushMatrix();
		glTranslatef(0.0f,-1.6f,0.0f);
		glColor3f(0.0f,0.0f,0.0f);
		drawLine();
		glPopMatrix();

		//draw Assist
		if(black>=5)
		{
			glPushMatrix();
			glTranslatef(canon_x,canon_y,0.0f);
			glRotatef(angle,0.0f,0.0f,1.0f);
			glColor3f(0.0f,0.0f,0.0f);
			drawAssist();
			glPopMatrix();
		}
		//draw laser
		int i;
		for(i=0;i<=high;i++)
		{
			if(laser_stat[i]==1)
			{
				glPushMatrix();
				glTranslatef(laser_x[i],laser_y[i],0.0f);
				glRotatef(l_angle[i],0.0f,0.0f,1.0f);
				glColor3f(1.0f,0.0f,0.0f);
				drawLaser();
				glPopMatrix();
			}
		}

		// draw alternate laser
		for(i=0;i<=high;i++)
		{
			if(laser_alt[i]==1)
			{
				glPushMatrix();
				glTranslatef(alt_laser_x[i],alt_laser_y[i],0.0f);
				glRotatef(alt_l_angle[i],0.0f,0.0f,1.0f);
				glColor3f(1.0f,0.0f,0.0f);
				drawLaser();
				glPopMatrix();
			}
		}
		//draw bucket1
		glPushMatrix();
		glTranslatef(bucket1_x,-1.8f,0.0f);
		glColor3f(bucket1_select,0.0f,0.0f);
		drawBucket(bucket_len);
		glPopMatrix();

		//draw bucket2
		glPushMatrix();
		glTranslatef(bucket2_x,-1.8f,0.0f);
		glColor3f(0.0f,bucket2_select,0.0f);
		drawBucket(bucket_len);
		glPopMatrix();

		// draw canon
		glPushMatrix();
		glTranslatef(canon_x,canon_y,0.0f);
		glRotatef(angle,0.0f,0.0f,1.0f);
		glColor3f(0.0f,0.0f,canon_select);
		drawCanon();
		glPopMatrix();

		// draw spiders
		for(i=0;i<=front;i++)
		{
			glPushMatrix();
			glTranslatef(spi_x[i],spi_y[i],0.0f);
			if(spi_vel[i]!=0)
				drawSpiders(i);
			else 
				drawBlock();
			glPopMatrix();
		}
		// Outer box 1 
		glPushMatrix();
		glTranslatef(3.0f,0.0f,0.0f);
		glColor3f(1.0f,1.0f,1.0f);
		drawOuterBox();
		glPopMatrix();

		// Outer box 2
		glPushMatrix();
		glTranslatef(-3.0f,0.0f,0.0f);
		glColor3f(1.0f,1.0f,1.0f);
		drawOuterBox();
		glPopMatrix();

		// Upper box	
		glPushMatrix();
		glTranslatef(0.0f,2.51f,0.0f);
		glColor3f(1.0f,1.0f,1.0f);
		drawUpperBox();
		glPopMatrix();

		// Score!!!
		glPushMatrix();
		glColor3f(1.0f,0.0f,0.0f);
		score();
		glPopMatrix();
		if(paus==1)
		{
			glPushMatrix();
			glColor3f(0.0f,0.0f,0.0f);
			gamePaused();
			glPushMatrix();
		}
	}
	else
	{
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -5.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		drawBox(box_len);
		glPushMatrix();

		glPushMatrix();
		glColor3f(1.0f,0.0f,0.0f);
		gameOver();
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

// Function to handle all calculations in the scene
// updated evry 10 milliseconds
void new_update(int value)
{
	int i;
	// Buckets catching spiders handled here.
	for(i=0;i<=front;i++)
	{
		if(spi_x[i]<=bucket1_x+0.1f && spi_x[i]>=bucket1_x-0.1f && spi_y[i]<=-1.6f && spi_y[i]>=-1.7f)
		{
			spi_vel[i]=0;
			spi_y[i]=-5.0f;
			spi_stat[i]=0;
			front--;
			if(spi_col[i]==1)
				points++;
			else 
				points--;
			system("aplay capture.wav&");
		}
		if(spi_x[i]<=bucket2_x+0.1f && spi_x[i]>=bucket2_x-0.1f && spi_y[i]<=-1.6f && spi_y[i]>=-1.7f)
		{
			spi_vel[i]=0;
			spi_y[i]=-5.0f;
			spi_stat[i]=0;
			front--;
			if(spi_col[0]==0)
				points++;
			else 
				points--;
			system("aplay capture.wav&");
		}
		if(canon_x>spi_x[i]-0.05f && canon_x<spi_x[i]+0.05f && spi_y[i]-0.05f<=-1.7f)
		{
			points=0;
			game_over=1;
		}
	}
	// Spiders hitting the lasers handled here.
	for(i=0;i<=high;i++)
	{
		if(laser_stat[i]==1)
		{
			laser_x[i]-=laser_vel_x[i];
			laser_y[i]+=laser_vel_y[i];
			tip_x[i]-=laser_vel_x[i];
			tip_y[i]+=laser_vel_y[i];
			if(laser_y[i]>=2.0f)
			{
				laser_stat[i]=0;
			}
			if(tip_x[i]<-2.0f || tip_x[i]>2.0f)
			{
				alt_laser_x[i]=laser_x[i];
				alt_laser_y[i]=laser_y[i];
				alt_laser_vel_x[i]=laser_vel_x[i];
				alt_laser_vel_y[i]=laser_vel_y[i];
				alt_l_angle[i]=l_angle[i];
				laser_alt[i]=1;
				laser_y[i]=tip_y[i];//-0.2*cos(l_angle[i]*PI/180);
				laser_x[i]=tip_x[i];//+0.2*sin(l_angle[i]*PI/180);
				laser_vel_x[i]=-1*laser_vel_x[i];
				l_angle[i]-=2*l_angle[i];
			}
		}
		if(laser_alt[i]==1)
		{
			alt_laser_x[i]-=alt_laser_vel_x[i];
			alt_laser_y[i]+=alt_laser_vel_y[i];
			if(alt_laser_x[i]>=2.2f && alt_laser_x[i]<=-2.2f)
			{
				laser_alt[i]=0;
			}
		}
		// Check if laser goes out of the scene.
	}
	for(i=0;i<=front;i++)
	{
		int j;
		for(j=0;j<=high;j++)
		{
	//		if((laser_x[j]-spi_x[i])*(laser_x[j]-spi_x[i])+(laser_y[j]-spi_y[i])*(laser_y[j]-spi_y[i])<=0.12*0.12f && spi_vel[i]!=0)
			if(tip_x[j]>spi_x[i]-0.05f && tip_x[j]<spi_x[i]+0.05f && tip_y[j]>spi_y[i]-0.05f && tip_y[j]<spi_y[i]+0.05f && laser_stat[j]==1)
			{
				spi_vel[i]=0;
				spi_y[i]=-5.0f;
				spi_stat[i]=0;
				front--;
				laser_stat[j]=0;
				if(spi_col[i]==2)
				{
					black++;
					if(black==5)
						system("aplay power.wav&");
					points++;
				}
				else
					points--;
			}
		}
	}
	// Laser bending handled here.
	// Position of the spiders change here. Also spiders hitting the base of the scene handled here.
	for(i=0;i<=front;i++)
	{
		if(spi_y[i]<0.0f && spi_stat[i]==0)
		{
			count--;
			spi_stat[i]=1;
		}
		if(spi_y[i]>=-1.85f)
		{
			spi_y[i]-=spi_vel[i];
		}
		// Blocking the base of the scene when spiders land is handled here.
		else if(spi_y[i]<-1.85f && spi_vel[i]>0)
		{
			points-=5;
			spi_vel[i]=0.0f;
			system("aplay block.wav&");
			if(spi_x[i]<canon_max_x && spi_x[i]>canon_x)
			{
				canon_max_x=spi_x[i]-0.1f;
			}
			else if(spi_x[i]>canon_min_x && spi_x[i]<canon_x)
			{
				canon_min_x=spi_x[i]+0.1f;
			}
			if(spi_x[i]<bucket1_max_x && spi_x[i]>bucket1_x+0.1f)
			{
				bucket1_max_x=spi_x[i]-0.15f;
			}
			else if(spi_x[i]>bucket1_min_x && spi_x[i]<bucket1_x-0.1f)
			{
				bucket1_min_x=spi_x[i]+0.15f;
			}
			if(spi_x[i]<bucket2_max_x && spi_x[i]>bucket2_x+0.1f)
			{
				bucket2_max_x=spi_x[i]-0.15f;
			}
			else if(spi_x[i]>bucket2_min_x && spi_x[i]<bucket2_x-0.1f)
			{
				bucket2_min_x=spi_x[i]+0.15f;
			}
		}
	}
	on+=10;game_level+=10;
	if(game_level>=10000)
	{
		level++;
		game_level=1;
	}
	if(black>=5)
	{
		black_timer+=10;
	}
	if(black_timer>=20000)
	{
		black_timer=0;
		black=0;
	}
	if(paus==0)
	{
		glutTimerFunc(10, new_update, 0);
	}
}

// All draw functions
void drawLine()
{
	glBegin(GL_LINES);
	glVertex3f( -2.0f,0.0f,0.0f);
	glVertex3f( 2.0f,0.0f,0.0f);
	glEnd();
}
void drawUpperBox()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-2.0f,-0.5f);
	glVertex2f(2.0f,-0.5f);
	glVertex2f(2.0f,0.5f);
	glVertex2f(-2.0f,0.5f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void drawOuterBox()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-1.0f,-3.0f);
	glVertex2f(1.0f,-3.0f);
	glVertex2f(1.0f,3.0f);
	glVertex2f(-1.0f,3.0f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBox(float len) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawSpiders(int i) 
{
	glColor3f(0,0,0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f( 0.0f,0.0f,0.0f);
	glVertex3f( 0.04f,0.03f,0.0f);
	glVertex3f(0.05f,0.05f,0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f( 0.0f,0.0f,0.0f);
	glVertex3f(-0.04f,0.03f,0.0f);
	glVertex3f( -0.05f,0.05f,0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f( 0.0f,0.0f,0.0f);
	glVertex3f(0.04f,-0.03f,0.0f);
	glVertex3f( 0.05f,-0.05f,0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f( 0.0f,0.0f,0.0f);
	glVertex3f(-0.04f,-0.03f,0.0f);
	glVertex3f( -0.05f,-0.05f,0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f( -0.05f,0.0f,0.0f);
	glVertex3f( 0.05f,0.0f,0.0f);
	glEnd();
	float x,y,z;
	int t;
	if(spi_col[i]==0)
		glColor3f(0.0f,0.5f,0.0f);
	else if(spi_col[i]==1)
		glColor3f(1.0f,0.0f,0.0f);
	else if(spi_col[i]==2)
		glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_TRIANGLE_FAN);
	for(t = 0; t <= 360; t +=1)
	{
		x = 0.025*sin(DEG2RAD(t));
		y = 0.05*cos(DEG2RAD(t));
		z = 0;
		glVertex3f(x,y,z);
	}
	glEnd();
	glPushMatrix();
	glTranslatef(0.0,-0.0625,0);
	glBegin( GL_TRIANGLE_FAN);
	for(t = 0; t <= 360; t +=1)
	{
		x = 0.0125*sin(DEG2RAD(t));
		y = 0.0125*cos(DEG2RAD(t));
		z = 0;
		glVertex3f(x,y,z);
	}
	glEnd();
	glPopMatrix();
}

void drawCanon()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(0.04f,-0.1f);
	glVertex2f(0.08f,-0.1f);
	glVertex2f(0.08f,0.02f);
	glVertex2f(0.04f,0.02f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-0.08f,-0.1f);
	glVertex2f(-0.04f,-0.1f);
	glVertex2f(-0.04f,0.02f);
	glVertex2f(-0.08f,0.02f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(on>=1000)
		glColor3f(0.0f,0.0f,0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-0.04f,-0.1f);
	glVertex2f(0.04f,-0.1f);
	glVertex2f(0.0f,0.1f);
	glVertex2f(0.0f,0.1f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void drawBucket(float len)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	glTranslatef(0.0f,0.0f,0.0f);
	glBegin( GL_TRIANGLE_FAN);
	int t,x,y,z;
	for(t = 0; t <= 360; t +=1)
	{
		x = (len)*sin(DEG2RAD(t));
		y = (len/4)*cos(DEG2RAD(t));
		z = 0;
		glVertex3f(x,y,z);
	}
	glEnd();
	glPopMatrix();
}
void drawLaser()
{
	glBegin(GL_LINES);
	glVertex3f( 0.0f,0.2f,0.0f);
	glVertex3f( 0.0f,-0.2f,0.0f);
	glEnd();
}
void drawAssist()
{
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,10,0.0f);
	glEnd();
}
void drawBlock()
{
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.2f,0.0f);
	glVertex3f(0.0f,-0.1f,0.0f);
	glEnd();
}
void drawBlackCount(float len)
{
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f,len/5,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glEnd();
}
void play()
{
	glRasterPos2f(-0.3f,0.0f);
	char string[50]="Click 'p' to Play!";
	int i;
	for(i=0;i<(int)strlen(string);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
}
void score()
{
	glRasterPos2f(2.5f,1.5f);
	char string[10]="SCORE : ";
	int i;
	for(i=0;i<(int)(strlen(string));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	char str[80];
	sprintf(str,"%d",points);
	for(i=0;i<(int)(strlen(str));i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);

}
void gameOver()
{
	game_play=0;
	glRasterPos2f(-0.3f,0.0f);
	char string[10]="GAME OVER";
	int i;
	for(i=0;i<(int)strlen(string);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
}
void gamePaused()
{
	glRasterPos2f(-0.3f,0.0f);
	char string[20]="GAME PAUSED";
	int i;
	for(i=0;i<(int)strlen(string);i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
}
// Initializing some openGL 3D rendering options
void initRendering() {
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {
	winw=w;
	winh=h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) 
{
	if(key=='p' && game_play==0)
	{
		game_play=1;
		game_over=0;
	}
	else if(key=='p' && paus==0)
	{
		paus=1;
	}
	else if(key=='p' && paus==1)
	{
		paus=0;
		glutTimerFunc(10,new_update,0);	
	}
	if (key == 27) {
		exit(0);     // escape key is pressed
	}
	if(key=='r')
	{
		mode='r';
		bucket1_select=0.5f;
		canon_select=1.0f;
		bucket2_select=1.0f;
	}
	else if(key=='b')
	{
		mode='b';
		canon_select=0.5f;
		bucket1_select=1.0f;
		bucket2_select=1.0f;
	}
	else if(key=='g')
	{
		mode='g';
		canon_select=1.0f;
		bucket2_select=0.5f;
		bucket1_select=1.0f;
	}
	else if(key==' ' && on>=1000)
	{
		system("aplay laser.wav&");
		int i;
		for(i=0;i<high;i++)
		{
			if(laser_stat[i]==0)
			{
				laser_x[i]=canon_x;
				laser_y[i]=canon_y;
				l_angle[i]=angle;
				tip_x[i]=laser_x[i]-0.2*sin(l_angle[i]*PI/180);
				tip_y[i]=laser_y[i]+0.2*cos(l_angle[i]*PI/180);
				laser_vel_x[i]=laser_vel*sin(l_angle[i]*PI/180);
				laser_vel_y[i]=laser_vel*cos(l_angle[i]*PI/180);
				laser_stat[i]=1;
				laser=1;
				break;
			}
		}
		if(laser==0)
		{
			high++;
			laser_x[high]=canon_x;
			laser_y[high]=canon_y;
			l_angle[high]=angle;
			tip_x[high]=canon_x-0.2*sin(l_angle[high]*PI/180);
			tip_y[high]=canon_y+0.2*cos(l_angle[high]*PI/180);
			laser_vel_x[high]=laser_vel*sin(l_angle[high]*PI/180);
			laser_vel_y[high]=laser_vel*cos(l_angle[high]*PI/180);
			laser_stat[high]=1;
		}
		on=0;laser=0;
	}
}
void handleKeypress2(int key, int x, int y) 
{
	if (key == GLUT_KEY_LEFT)
	{
		if(mode=='r') 
		{
			if(bucket1_x>bucket1_min_x)
			{
				bucket1_x-= 0.025;
			}
		}
		else if(mode=='b')
		{
			if(canon_x>canon_min_x)
			{
				canon_x-=0.025;
			}
		}
		else if(mode=='g')
		{
			if(bucket2_x>bucket2_min_x)
			{
				bucket2_x-=0.025;
			}
		}
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if(mode=='r') 
		{
			if(bucket1_x<bucket1_max_x)
			{
				bucket1_x+= 0.025;
			}
		}
		else if(mode=='b')
		{
			if(canon_x<canon_max_x)
			{
				canon_x+=0.025;
			}
		}
		else if(mode=='g')
		{
			if(bucket2_x<bucket2_max_x)
			{
				bucket2_x+=0.025;
			}
		}
	}
	if(key==GLUT_KEY_UP)
	{
		if(mode=='b' && angle<=70)
		{
			angle+=5;
		}
	}
	if(key==GLUT_KEY_DOWN)
	{
		if(mode=='b' && angle>=-70)
		{
			angle-=5;
		}
	}
}
void getCoord( int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	winZ=0;
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);      
	mousex=posX*100/2;
	mousey=(posY*100)/2;
	mousez=0.0f;//posZ*100;
	glutPostRedisplay();
}

char checkSelected()
{
	if(mousex>bucket1_x-0.05 && mousex<bucket1_x+0.05 && mousey>-1.8-0.05 && mousey<-1.8+0.05)
	{
		return 'r';
	}
	else if(mousex>bucket2_x-0.05 && mousex<bucket2_x+0.05 && mousey>-1.8-0.05 && mousey<-1.8+0.05)
	{
		return 'g';
	}
	else if(mousex>canon_x-0.05 && mousex<canon_x+0.05 && mousey>-1.8-0.05 && mousey<-1.8+0.05)
	{
		return 'b';
	}
	else 
		return 'z';	
}
float initial_x,initial_y;
void handleMouseclick(int button, int state, int x, int y) 
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		getCoord(x,y);
		mode=checkSelected();
	}
	else if(state==GLUT_DOWN && button==GLUT_RIGHT_BUTTON)
	{
		getCoord(x,y);
		mode=checkSelected();
		if(mode=='b')
			mode='c';
		initial_x=mousex;
		initial_y=mousey;
	}
}
void handleMouseDrag(int x,int y)
{
	getCoord(x,y);
	if(mode=='b' && mousex<canon_max_x && mousex>canon_min_x)
		canon_x=mousex;
	else if(mode=='r' && mousex<bucket1_max_x && mousex>bucket1_min_x)
		bucket1_x=mousex;
	else if(mode=='g' && mousex<bucket2_max_x && mousex>bucket2_min_x)
		bucket2_x=mousex;
	else if(mode=='c')
	{
		float side1=mousex-canon_x;
		float side2=mousey-canon_y;
		angle=-atan(float(side1)/side2)*180/PI;
	}
}
