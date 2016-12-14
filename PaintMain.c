#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<strings.h>

#define GLUT_BITMAP_8_BY_13 1
#define GLUT_BITMAP_9_BY_13 2
#define GLUT_BITMAP_TIMES_ROMAN_10 3
#define GLUT_BITMAP_TIMES_ROMAN_24 4
#define GLUT_BITMAP_HELVETICA_10 5
#define GLUT_BITMAP_HELVETICA_12 6
#define GLUT_BITMAP_HELVETICA_18 7

int width=800 ,height=600,sub_menu,sub_menu1,sub_menu2;
float a1=-1, a2=-1,b1=-1,b2=-1,a3=-1,b3=-1,X,Y,X1,Y1,X2,Y2;
int c1 = 0,c2 = 0, c3 = 0,d1 = 0,d2 = 0,d3 = 0;
float a[100][2], b[100][2],c[100][2],nx[10]={0},ny[10]={0};int t=0;
int font1 = 3;
int i,j,k,draw,l=0,q = 0;
float currentPosX,currentPosY;
int flag=0, flag1,fill=0,m,n,wel=0,pol,count = 0, drawOb = 0,size=1,save = 0,ss = 0;
unsigned int selected;
FILE *fptr;
char fname[20];
char *image;
int s=0;
float textx,texty, textz=0.0;

 void drawstring(float x,float y,float z,char *string) /* to display text */
 {
 	char *c;
 	glRasterPos3f(x,y,z);

 	for(c=string;*c!='\0';c++)
 	{
 		glutBitmapCharacter((char*)font1,*c);
 	}
 }


void draw_pixel1(GLfloat x, GLfloat y)
{
 	if(x >= -0.7 && x <= 0.9 && y >= -0.7 && y <= 0.9) {
 		if(fill==1)
 			glPointSize(3);
 		else
 			glPointSize(size);
 			glBegin(GL_POINTS);
 			glVertex2f(x, y);
 			glEnd();
	}
}

void brush(int x, int y) /* PAINT BRUSH function */
 {
	float X = x / (float) width * (1.0 - (-1.0) )+ (-1.0);
	float Y = (1 - y / (float) height) * (1.0 - (-1.0) )+ (-1.0);
 	if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
 	{
 		glBegin(GL_POLYGON);
 		glVertex2f(X, Y);
 		glVertex2f(X+size*0.1, Y);
 		glVertex2f(X+size*0.1, Y+size*0.1);
 		glVertex2f(X, Y+size*0.1);
 		glEnd();
 		glFlush();
 }

 }

void edgedetect(float x1, float y1, float x2, float y2, int *le, int *re) /* to detect edges of the polygon to be filled */
 {
 	float mx, x, temp;

 	if((y2-y1)<0)
 	{
 		temp=y1;
 		y1=y2;
 		y2=temp;

 		temp=x1;
 		x1=x2;
 		x2=temp;
 	}

 	if((y2-y1)!=0)
 		mx=(x2-x1)/(y2-y1);
 	else
 		mx=x2-x1;

 	x=x1;
 	//printf("%.2f %.2f %.2f %.2f %.2f\n",x1,y1,x2,y2,mx);
 	for(i=y1;i<=y2;i++)
 	{
 		if(x<(float)le[i])
 			le[i]=(int)x;
 		if(x>(float)re[i])
 			re[i]=(int)x;
 		x+=mx;
 		//printf("%d %d\n",le[i],re[i]);
 	}
 }
 
 void scanfill(GLint num1, GLint num2) /* to FILL a Polygon using SCAN LINE ALGORITHM*/
 {
 	int le[1000], re[1000];
 	int p, q;
 	int bottom, top;

 	for(p=0;p<height;p++)
 	{
 		le[p]=width;
 		re[p]=0;
 	}
 	for(p=num1;p<num2;p++)
 	{
 		edgedetect(a[p][0], a[p][1], a[p+1][0], a[p+1][1], le, re);
 	}

 	edgedetect(a[num2][0], a[num2][1], a[num1][0], a[num1][1], le, re);

 	for(q=0;q<height;q++)
 		le[q]=le[q]+1;

 	bottom=0;
 	while(re[bottom]<le[bottom])
 		bottom++;

 	bottom++;

 	top=height-1;
 	while(re[top]<le[top])
 		top--;


 	for(q=bottom;q<top;q++)
 	{
 		if(le[q]<=re[q])
 		{
 			for(p=(int)le[q];p<(int)re[q];p++)
 			{
				float X = p / (float) width * (1.0 - (-1.0) )+ (-1.0);
        		float Y = (1 - q / (float) height) * (1.0 - (-1.0) )+ (-1.0);
				//printf("%f %f\n",X,Y);
 				draw_pixel1(X, Y);
			}
 		}
 	}
 }
 
 void detect_point(GLint num1, GLint num2, int x, int y) /* to detect which POLYGON TO BE FILLED */
 {
 	int le[1000], re[1000];
 	int p, q;

 	for(p=0;p<height;p++)
 	{
 		le[p]=width;
 		re[p]=0;
 	}

 	for(p=num1;p<num2;p++)
 	{
 		edgedetect(a[p][0], a[p][1], a[p+1][0], a[p+1][1], le, re);
 	}

 	edgedetect(a[num2][0], a[num2][1], a[num1][0], a[num1][1], le, re);

 	for(q=0;q<height;q++)
 	{
 		if(le[q]<=re[q])
 		{
 			for(p=(int)le[q];p<(int)re[q];p++)
 			{
 				if(x==p && y==q)
 					flag=num1;
 			}
 		}
 	}
 } 
	
void draw_point(float x, float y)
{
	if(x >= -0.7 && x <= 0.9 && y >= -0.7 && y <= 0.9)
	{
		//printf("%f %f\n",x,y);
		glPointSize(size);
		glBegin(GL_POINTS);
		glVertex2f(x,y);
		glEnd();
		glFlush();
		}
}

void draw_point1(int x, int y)
{
	X = x / (float) width * (1.0 - (-1.0) )+ (-1.0);
	Y = (1 - y / (float) height) * (1.0 - (-1.0) )+ (-1.0);
	if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
	{
		//printf("%f %f\n",x,y);
		glPointSize(2);
		glBegin(GL_POINTS);
		glVertex2f(X,Y);
		glEnd();
		glFlush();
	}
}


void eraser(int x, int y) /* ERASER function */
{
	float X = x / (float) width * (1.0 - (-1.0) )+ (-1.0);
	float Y = (1 - y / (float) height) * (1.0 - (-1.0) )+ (-1.0);
 	glColor3f(1, 1, 1);

 	if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
 	{
 		glBegin(GL_POLYGON);
 		glVertex2f(X, Y);
 		glVertex2f(X+0.01, Y);
 		glVertex2f(X+0.01, Y+0.01);
 		glVertex2f(X, Y+0.01);
 		glEnd();
 		glFlush();
 	}
 	glColor3f(0,0,0);
 }
 
void palette(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
 {
 	glBegin(GL_QUADS);
 	glVertex2f(x1, y1);
 	glVertex2f(x2, y2);
 	glVertex2f(x3, y3);
 	glVertex2f(x4, y4);
 	glEnd();
 }
 
 void reset() /* resets the variables in which vertices are stored,after a polygon is drawn */
 {
 	a1=-1;
 	a2=-1;
 	b1=-1;
 	b2=-1;
 	a3=-1;
 	b3=-1;
 	
 	c1 = 0;
 	c2 = 0;
 	c3 = 0;
 	d1 = 0;
 	d2 = 0;
 	d3 = 0;
 }

 
 void display(void)
 {
 	glClearStencil(0);
 	glClearColor (1.0, 1.0, 1.0, 1.0);
 	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 	
 	if(wel==0 || wel==1)
 	{

 		glClearColor (0.0, 0.0, 0.0, 1.0);
 		glClear(GL_COLOR_BUFFER_BIT);
 		glColor3f(0, 1, 1);
 		drawstring(-0.3,0.8,0.0, "A MINI PROJECT ON");
 		drawstring(-0.6,0.70,0, "DESIGN AND IMPLEMENTION OF DRAWING APPLICATION");
 		drawstring(-0.1,0.4, 0, "BY:");
 		drawstring(-0.7,0.2, 0.0, "MOHAMMED ISMAIL");
 		drawstring(-0.65,0.1, 0.0, "4BP13CS026");
 		drawstring(0.3, 0.2,0.0, "ANWAR SADATH");
 		drawstring(0.32, 0.1, 0.0, "4BP13CS003");
 		drawstring(-0.3,-0.15, 0.0, "UNDER THE GUIDANCE OF:");
 		drawstring(-0.7,-0.25, 0.0, "Mr. ATHEEQ");
 		drawstring(-0.7,-0.35, 0.0, "LECTURER, DEPT. OF CSE");
 		drawstring(-0.7,-0.45, 0.0, "BEARYS INSTITUTE OF TECHNOLOGY");
 		drawstring(0.3,-0.25, 0.0, "Ms POORNIMA");
 		drawstring(0.3,-0.35, 0.0, "LECTURER, DEPT. OF CSE");
 		drawstring(0.3,-0.45, 0.0, "BEARYS INSTITUTE OF TECHNOLOGY");
 		wel=1;
 		glFlush();
 		glClearColor (1.0, 1.0, 1.0, 1.0);
 		glClear(GL_COLOR_BUFFER_BIT);
 	}
 	
	if(wel==2)
 	{ 
		count++;
		if(count>1)
 		{
		glDrawBuffer(GL_FRONT); /* selecting GL_FRONT buffer to write*/
 		glRasterPos2f(-0.78,-0.78);
 		glDrawPixels(0.98,0.9, GL_RGB, GL_UNSIGNED_BYTE, image); /* to write pixels into the selected buffer */
		}
 		glEnable(GL_STENCIL_TEST);
 		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
 		glColor3f(0.5, 0.5, 0.5);
 		glColor3f(0.85, 0.85, 0.85);
 		glPointSize(1);
 		glLineWidth(2);
 
 		//if(drawOb == 1) drawObjects();

 		glColor3f(0,0,0);
 		glBegin(GL_LINE_LOOP);
 		glVertex2f(-0.8,-1);
 		glVertex2f(0,-1);
 		glVertex2f(0,-0.8);
 		glVertex2f(-0.8,-0.8);
 		glEnd();
 
 		glColor3f(0, 0, 0);
 		palette(-0.8, -1, -0.7, -1, -0.7, -0.9, -0.8, -0.9);

 		glColor3f(1, 1, 1);
 		palette(-0.8, -0.9, -0.7, -0.9, -0.7, -0.8, -0.8, -0.8);


 		glColor3f(0, 0, 1);
 		palette(-0.7, -1, -0.6, -1, -0.6, -0.9, -0.7, -0.9);


 		glColor3f(1, 0, 1);
 		palette(-0.7, -0.9, -0.6, -0.9, -0.6, -0.8, -0.7, -0.8);

 		glColor3f(1, 1, 0);
 		palette(-0.6, -1, -0.5, -1, -0.5, -0.9, -0.6, -0.9);

 		glColor3f(1, 0, 0);
 		palette(-0.6, -0.9, -0.5, -0.9, -0.5, -0.8, -0.6, -0.8);

 		glColor3f(0, 1, 0);
 		palette(-0.5, -1, -0.4, -1, -0.4, -0.9, -0.5, -0.9);

 		glColor3f(0, 1, 1);
 		palette(-0.5, -0.9, -0.4, -0.9,-0.4, -0.8, -0.5, -0.8);


 		glColor3f(0.5, 0.5, 0);
 		palette(-0.4,-1,-0.3,-1,-0.3,-0.9,-0.4,-0.9);


 		glColor3f(0.1, 0.4, 0.6);
 		palette(-0.4,-0.9,-0.3,-0.9,-0.3,-0.8,-0.4,-0.8);

 		glColor3f(0.4, 0.1, 0.1);
 		palette(-0.3,-1,-0.2,-1,-0.2,-0.9,-0.3,-0.9);


 		glColor3f(0.9, 0.1, 0.5);
 		palette(-0.3,-0.9,-0.2,-0.9,-0.2,-0.8,-0.3,-0.8);


 		glColor3f(0.5, 0.1, 0.5);
 		palette(-0.2,-1,-0.1,-1,-0.1,-0.9,-0.2,-0.9);


 		glColor3f(0.3, 0.1, 0.5);
 		palette(-0.2,-0.9,-0.1,-0.9,-0.1,-0.8,-0.2,-0.8);

 		glColor3f(0.0, 0.5, 0.1);
 		palette(-0.1,-1,0,-1,0,-0.9,-0.1,-0.9);


 		glColor3f(0.9, 0.8, 0.0);
 		palette(-0.1,-0.9,0,-0.9,0,-0.8,-0.1,-0.8);


 		glColor3f(0.7, 0.0, 0.0);
 		palette(0,-1,0.1,-1,0.1,-0.9,0,-0.9);
 
 		glColor3f(0.5, 0.5, 0.5);
 		palette(0,-0.9,0.1,-0.9,0.1,-0.8,0,-0.8);

 		glColor3f(0, 0, 0);

 		glBegin(GL_LINE_LOOP);/*To Draw the toolbox border*/
 		glVertex2f(-1,0.2);
 		glVertex2f(-0.8,0.2);
 		glVertex2f(-0.8,0.8);
 		glVertex2f(-1,0.8);
 		glEnd();

 		glBegin(GL_LINES); /* To Draw the separators in the toolbox */
 		glVertex2f(-0.9,0.7);
 		glVertex2f(-0.9,0.2);
 		glVertex2f(-1,0.2);
 		glVertex2f(-0.8,0.2);
 		glVertex2f(-1,0.3);
 		glVertex2f(-0.8,0.3);
 		glVertex2f(-1,0.4);
 		glVertex2f(-0.8,0.4);
 		glVertex2f(-1,0.5);
 		glVertex2f(-0.8,0.5);
 		glVertex2f(-1,0.6);
 		glVertex2f(-0.8,0.6);
 		glVertex2f(-1,0.7);
 		glVertex2f(-0.8,0.7);
 		glVertex2f(-1,0.8);
 		glVertex2f(-0.8,0.8);
 		glVertex2f(-1,0.9);
 		glVertex2f(-0.8,0.9);
 		glVertex2f(-0.9,0.7);
 		glVertex2f(-0.9,0.8);
 		glVertex2f(-1,0.8);
 		glVertex2f(-1,0.9);
 		glVertex2f(-0.9,0.8);
 		glVertex2f(-0.9,0.9);
 		glVertex2f(-0.8,0.8);
 		glVertex2f(-0.8,0.9);
 		glEnd();
 
 		glBegin(GL_LINE_LOOP);
 		glVertex2f(-0.95,-0.95);
 		glVertex2f(-0.85,-0.95);
 		glVertex2f(-0.85,-0.85);
 		glVertex2f(-0.95,-0.85);
 		glEnd();
 
 		glBegin(GL_LINE_LOOP);
		glVertex2f(-0.99,0.52);
 		glVertex2f(-0.91,0.52);
 		glVertex2f(-0.91,0.54);
 		glVertex2f(-0.94,0.54);
 		glVertex2f(-0.91,0.58);
 		glVertex2f(-0.95,0.58);
 		glEnd();
 
 		// To Draw drawing area
 		glBegin(GL_LINE_LOOP);
 		glVertex2f(-0.78,-0.78);
 		glVertex2f(0.98,-0.78);
 		glVertex2f(0.98,0.9);
 		glVertex2f(-0.78,0.9);
 		glEnd();
 
 		// To Draw the TextBox
 		glBegin(GL_LINES);
 		glVertex2f(-0.98,0.22);
 		glVertex2f(-0.95,0.28);
 		glVertex2f(-0.95,0.28);
 		glVertex2f(-0.92,0.22);
 		glVertex2f(-0.93,0.24);
 		glVertex2f(-0.97,0.24);
 		glEnd();
 	
 		glBegin(GL_LINES); /* To Draw LINE OPTION on tool bar */
 		glVertex2f(-0.89,+0.79);
 		glVertex2f(-0.82,0.72);
 		glEnd();


 		glBegin(GL_LINE_LOOP); /* To Draw TRIANGLE OPTION on tool bar */
 		glVertex2f(-0.95,+0.68);
 		glVertex2f(-0.99,0.62);
 		glVertex2f(-0.91,0.62);
 		glEnd();

 		glBegin(GL_LINE_LOOP); /* To Draw RECTANGLE OPTION on tool bar */
 		glVertex2f(-0.89,+0.68 );
 		glVertex2f(-0.81,0.68);
 		glVertex2f(-0.81,+0.62 );
 		glVertex2f(-0.89, +0.62);
 		glEnd();

 		// 3D objects
 		glPushMatrix();
 		glTranslated(-0.95,0.85,0.0);
 		glutSolidTeapot(0.025);				// Teapot
 		glPopMatrix();
 		glFlush();
 
 		glPushMatrix();
 		glTranslated(-0.85,0.85,0);
 		glutSolidCone(0.025,0.025,50,50);		// Cone
 		glPopMatrix();
 		glFlush();

 		glPushMatrix();
 		glTranslated(-0.85,0.25,0);
 		glutSolidSphere(0.025,25,25);			//Sphere
 		glPopMatrix();
 		glFlush();
 
 		glPushMatrix();
 		glTranslated(-0.95,0.45,0);
 		glutSolidCube(0.05);					//Cube
 		glPopMatrix();
 		glFlush();
 
 		glPushMatrix();
 		glTranslated(-0.85,0.55,0);
 		glutSolidTorus(0.009,0.03,20,20);		//Torus
 		glPopMatrix();
 		glFlush();

 		glBegin(GL_TRIANGLES); /* To Draw PENCIL OPTION on tool bar */
 		glVertex2f(-0.97,0.73);
 		glVertex2f(-0.95,0.7);
 		glVertex2f(-0.94,0.73);
 		glEnd();

 		glBegin(GL_LINE_LOOP);
 		glVertex2f(-0.97,0.79);
 		glVertex2f(-0.94,0.79);
 		glVertex2f(-0.94,0.73);
 		glVertex2f(-0.97,0.73);
 		glEnd();

 		glColor3f(0, 0, 0);
 		glBegin(GL_QUADS); /* To Draw ERASER OPTION on tool bar */
 		glVertex2f(-0.88,0.43);
 		glVertex2f(-0.82,0.43);
 		glVertex2f(-0.82,0.47);
 		glVertex2f(-0.88,0.47);
 		glEnd();

 		for(i=0;i<40;i++) /* To Draw AIR BRUSH OPTION on tool bar */
 		{
 			j=rand()%10;
 			k=rand()%10;
 			glBegin(GL_POINTS);
 			glVertex2f(-1+j,0.5-k);
 			glEnd();

 		}
 
 		glColor3f(0, 0.3, 1);
 		glBegin(GL_QUADS); /* To Draw COLOUR FILL OPTION on tool bar */
 		glVertex2f(-0.93,0.38);
 		glVertex2f(-0.96,0.35);
 		glVertex2f(-0.92,0.3);
 		glVertex2f(-0.9,0.33);
 		glEnd();
 
 		glColor3f(0, 0, 0);
 		glPointSize(3);
 		glBegin(GL_POINTS);
 		glVertex2f(-0.97,0.34 );
 		glEnd();
 		glPointSize(1.5);
 		glBegin(GL_POINTS);
 		glVertex2f(-0.98,0.32);
 		glEnd();
 		glPointSize(1);
		glColor3f(0.4, 0.1, 0.1);

 		//drawCircle(-0.85,0.55,0.025); /* To Draw CIRCLE OPTION on tool bar */
 
 		glBegin(GL_QUADS); /* to draw BRUSH OPTION on tool bar */
 		glVertex2f(-0.88,0.34);
 		glVertex2f(-0.88,0.36);
 		glVertex2f(-0.83,0.36);
 		glVertex2f(-0.83,0.34);
 		glEnd();
 		glColor3f(0, 0, 0);
 		glBegin(GL_LINES);
 		glVertex2f(-0.81,0.38);
 		glVertex2f(-0.83,0.36);
 		glVertex2f(-0.81,0.36);
 		glVertex2f(-0.83,0.355);
 		glVertex2f(-0.81,0.34);
 		glVertex2f(-0.83,0.35);
 		glVertex2f(-0.81,0.33);
 		glVertex2f(-0.83,0.34);
 		glEnd();
 		
		 /* to display text */
 		/*glColor3f(0, 0, 0);
 		drawstring(-0.7,0.93, 0.0, "OPEN");
 		drawstring(-0.4,0.93,0.0, "SAVE");
 		drawstring(-0.1,0.93, 0.0, "CLEAR");*/

 		glFlush();
 		glReadBuffer(GL_FRONT);
 		glReadPixels(-0.78, -0.78, 0.98, 0.9,GL_RGB, GL_UNSIGNED_BYTE, image); /* to read pixels from buffer to memory */
 	}
}

/* RESHAPE FUNCTION */
void myReshape(int w,int h) 
 {
 	width=w;
 	height=h;
 	glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();	
 	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
 	glMatrixMode(GL_MODELVIEW);
 	glViewport(0,0,width,height);
 	glFlush();
 	glutPostRedisplay();
 }

void myKey(unsigned char key, int x, int y)
{
	float X = x / (float) width * (1.0 - (-1.0) )+ (-1.0);
	float Y = (1 - y / (float) height) * (1.0 - (-1.0) )+ (-1.0);
    glutBitmapCharacter((char*)font1,key);
	if(key==13 && save == 0)
	{	
		currentPosY -= 0.08;
		glRasterPos2f(currentPosX , currentPosY);
		return;
	}
	if((key =='a'||key=='b'||key=='c'||key=='d'||key=='e'||key=='f'||key=='g'||key=='h'||key=='i'||key=='j'||key=='k'||key=='l'||key=='m') && save == 1)
 	{
 		fname[s]=key;
 		glRasterPos3f (textx, texty,textz);
 		glColor3f(0, 0, 0);
 		glutBitmapCharacter((char*)font1, fname[s]); /* to display FILENAME */
 		textx=textx+0.02;
		s++;
 	}

 	if((key=='n'||key=='o'||key=='p'||key=='q'||key=='r'||key=='s'||key=='t'||key=='u'||key=='v'||key=='w'||key=='x'||key=='y'||key=='z') && save == 1)
 	{
 		fname[s]=key;
 		glRasterPos3f (textx, texty,textz);
 		glColor3f(0, 0, 0);
 		glutBitmapCharacter((char*)font1, fname[s]);
 		textx=textx+0.02;
 		s++;
 	}

 	if((key=='A'||key=='B'||key=='C'||key=='D'||key=='E'||key=='F'||key=='G'||key=='H'||key=='I'||key=='J'||key=='K'||key=='L'||key=='M') && save == 1)
 	{
 		fname[s]=key;
 		glRasterPos3f (textx, texty,textz);
 		glColor3f(0, 0, 0);
 		glutBitmapCharacter((char*)font1, fname[s]);
 		textx=textx+0.02;
 		s++;
 	}

 	if((key=='N'||key=='O'||key=='P'||key=='Q'||key=='R'||key=='S'||key=='T'||key=='U'||key=='V'||key=='W'||key=='X'||key=='Y'||key=='Z') && save == 1)
 	{
 		fname[s]=key;
 		glRasterPos3f (textx, texty,textz);
 		glColor3f(0, 0, 0);
 		glutBitmapCharacter((char*)font1, fname[s]);
 		textx=textx+0.02;
 		s++;
 	}

 	if((key=='_'||key=='0'||key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9') && save == 1)
 	{
 		fname[s]=key;
 		glRasterPos3f (textx, texty,textz);
 		glColor3f(0, 0, 0);
		glutBitmapCharacter((char*)font1, fname[s]);
 		textx=textx+0.02;
 		s++;
 	}
 	glFlush();

 	if(key=='\r' && save==1) /* to SAVE current image on a file */
 	{
 	fname[s]='\0';
 	}

	if(key=='\r' && save==2)
 	{ /* to OPEN an existing image file */
 		fname[s]='\0';
 		if ((fptr = fopen(fname,"r")) == NULL)
 		{
 			fprintf(stderr,"Failed to open file\n");
 			return;
 		}

 		save=0;
 		s=0;
 		fread(image,width*height*3,1,fptr);
 		fclose(fptr);
 		display();
	}
}

void opacity(float a, float b, float c)
{
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glVertex2f(-1,0.91);
	glVertex2f(0.98,0.91);
	glVertex2f(0.98,1);
	glVertex2f(-1,1);
	glEnd();
	glColor4f(a,b,c,1);
	glBegin(GL_QUADS);
	glVertex2f(-1,0.91);
	glVertex2f(0.98,0.91);
	glVertex2f(0.98,1);
	glVertex2f(-1,1);
	glEnd();
	glColor3f(a,b,c);
}
 
 void mouse(int btn,int state,int x,int y)
 {
 	GLfloat r, r1, r2;
 	int num1=0, num2=0;

 	int i1, i2;
 	int flag2;

 	flag2=0;
 	float X = x / (float) width * (1.0 - (-1.0) )+ (-1.0);
	float Y = (1 - y / (float) height) * (1.0 - (-1.0) )+ (-1.0);
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		if(wel==1)
 		{
 			wel=2;
 			display();
 		}
 		if(X >= -0.8 && X <= -0.7 && Y >= -1 && Y <= -0.9){
 		glColor3f(0, 0, 0);
		opacity(0,0,0);
		}
 		else if(X >= -0.8 && X <= -0.7 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(1, 1, 1);
		opacity(1,1,1);
		}

 		else if(X >= -0.7 && X <= -0.6 && Y >= -1 && Y <= -0.9){
 		glColor3f(0, 0, 1);
		opacity(0,0,1);
		}

 		else if(X >= -0.7 && X <= -0.6 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(1, 0, 1);
		opacity(1,0,1);
		}

 		else if(X >= -0.6 && X <= -0.5 && Y >= -1 && Y <= -0.9){
 		glColor3f(1, 1, 0);
		opacity(1,1,0);
		}

		else if(X >= -0.6 && X <= -0.5 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(1, 0, 0);
		opacity(1,0,0);
		}
	
 		else if(X >= -0.5 && X <= -0.4 && Y >= -1 && Y <= -0.9){
 		glColor3f(0, 1, 0);
		opacity(0,1,0);
		}
	
 		else if(X >= -0.5 && X <= -0.4 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(0, 1, 1);
		opacity(0,1,1);
		}

 		else if(X >= -0.4 && X <= -0.3 && Y >= -1 && Y <= -0.9){
 		glColor3f(0.5, 0.5, 0);
		opacity(0.5,0.5,0);
		}

 		else if(X >= -0.4 && X <= -0.3 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(0.1, 0.4, 0.6);
		opacity(0.1,0.4,0.6);
		}

 		else if(X >= -0.3 && X <= -0.2 && Y >= -1 && Y <= -0.9){
 		glColor3f(0.4, 0.1, 0.1);
		opacity(0.4,0.1,0.1);
		}

 		else if(X >= -0.3 && X <= -0.2 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(0.9, 0.1, 0.5);
		opacity(0.9,0.1,0.5);
		}

 		else if(X >= -0.2 && X <= -0.1 && Y >= -1 && Y <= -0.9){
 		glColor3f(0.5, 0.1, 0.5);
		opacity(0.5,0.1,0.5);
		}

 		else if(X >= -0.2 && X <= -0.1 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(0.3, 0.1, 0.5);
		opacity(0.3,0.1,0.5);
		}

 		else if(X >= -0.1 && X <= 0 && Y >= -1 && Y <= -0.9){
 		glColor3f(0.0, 0.5, 0.1);
		opacity(0.0,0.5,0.1);
		}

 		else if(X >= -0.1 && X <= 0 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(0.9, 0.8, 0.0);
		opacity(0.9,0.8,0.0);
		}

 		else if(X >= 0 && X <= 0.1 && Y >= -1 && Y <= -0.9){
 		glColor3f(0.7, 0.0, 0.0);
		opacity(0.7,0.0,0.0);
		}

 		else if(X >= 0 && X <= 0.1 && Y >= -0.9 && Y <= -0.8){
 		glColor3f(0.5, 0.5, 0.5);
 		opacity(0.5,0.5,0.5);
		}

	 	if(X >= -1 && X <= -0.9 && Y >= 0.8 && Y <= 0.9){
			draw = 1;
		}
		else if(X >= -0.9 && X <= -0.8 && Y >= 0.8 && Y <= 0.9)
		{
			reset();
			draw = 2;
		}
		else if(X >= -1 && X <= -0.9 && Y >= 0.7 && Y <= 0.8)
		{
			reset();
			draw = 3;
		}
		else if(X >= -0.9 && X <= -0.8 && Y >= 0.7 && Y <= 0.8)
		{
			reset();
			draw = 4;
		}
		else if(X >= -1 && X <= -0.9 && Y >= 0.6 && Y <= 0.7)
		{
			reset();
			draw = 5;
		}
		else if(X >= -0.9 && X <= -0.8 && Y >= 0.6 && Y <= 0.7)
		{
			reset();
			draw = 6;
		}
		else if(X >= -1 && X <= -0.9 && Y >= 0.5 && Y <= 0.6)
		{
			reset();
			draw = 7;
		}
		else if(X >= -0.9 && X <= -0.8 && Y >= 0.5 && Y <= 0.6)
		{
			reset();
			draw = 8;
		}
		else if(X >= -1 && X <= -0.9 && Y >= 0.4 && Y <= 0.5)
		{
			reset();
			draw = 9;
		}
		else if(X >= -0.9 && X <= -0.8 && Y >= 0.4 && Y <= 0.5)
		{
			reset();
			draw = 10;
		}
		else if(X >= -1 && X <= -0.9 && Y >= 0.3 && Y <= 0.4)
		{
			reset();
			draw = 11;
		}
		else if(X >= -0.9 && X <= -0.8 && Y >= 0.3 && Y <= 0.4)
		{
			reset();
			draw = 12;
		}
		else if(X >= -1 && X <= -0.9 && Y >= 0.2 && Y <= 0.3)
		{
			reset();

			draw = 13;	
		}
		else if(X >= -0.9 && X <= -0.8 && Y >= 0.2 && Y <= 0.3)
		{
				reset();
				draw = 14;
		}
		else if(X >= -1 && X <= -0.9 && Y >= 0.1 && Y <= 0.2)
		{
			reset();
			draw = 15;	
		}
		else if(X >= -1 && X <= -0.9 && Y >= 0 && Y <= 0.1)
		{
			draw = 16;	
		}
			
				switch(draw)
				{
					case 1:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
							glPushMatrix();
							glTranslated(X,Y,0);
							glutWireTeapot(0.3);
							glPopMatrix();
							glFlush();
						}
						break;
					case 2:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
							glPushMatrix();
							glTranslated(X,Y,0);
							glRotated(25,1,0,0);
							glutWireCone(0.25,0.25,50,50);
							glPopMatrix();
							glFlush();
						}
						break;
					case 3:
						if(pol==1)
 						{
 							m++;
 							a[m][0]='$';
 							a[m][1]='$';

 							pol=0;
 						}
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
							glutMotionFunc(draw_point1);
						break;
					case 4:
						if(pol==1)
 						{
 							m++;
 							a[m][0]='$';
 							a[m][1]='$';

 							pol=0;
 						}
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
								draw_point(X, Y);
 								a2=a1;
 								b2=b1;
 								a1=X;
 								b1=Y;
 								if(a1 >= -0.7 && a1 <= 0.9 && b1 >= -0.7 && b1 <= 0.9)
 								{
 									if(a2 >= -0.7 && a2 <= 0.9 && b2 >= -0.7 && b2 <= 0.9)
 									{
 										glLineWidth(size);
 										glBegin(GL_LINES);
 										glVertex2f(a2, b2);
	 									glVertex2f(a1, b1);
	 									glEnd();
 										reset();

 									}
 								}
 								glFlush();
						}
						break;
					case 5:
						if(pol==1)
 						{
 							m++;
 							a[m][0]='$';
 							a[m][1]='$';

 							pol=0;
 						}
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
							draw_point(X,Y);
							a3 = a2;
							b3 = b2;
							a2 = a1;
							b2 = b1;
							a1 = X;
							b1 = Y;

							c3 = c2;
							d3 = d2;
							c2 = c1;
							d2 = d1;
							c1 = x;
							d1 = y;
				
							if(a1 >= -0.7 && a1 <= 0.9 && b1 >= -0.7 && b1 <= 0.9)
							{
								if(a2 >= -0.7 && a2 <= 0.9 && b2 >= -0.7 && b2 <= 0.9)
								{
									if(a3 >= -0.7 && a3 <= 0.9 && b3 >= -0.7 && b3 <= 0.9){
									glLineWidth(size);
									glBegin(GL_LINE_LOOP);
									glVertex2f(a3,b3);
									glVertex2f(a2,b2);
									glVertex2f(a1,b1);
									glEnd();

									m++;
 									a[m][0]=c3;
 									a[m][1]=d3;

 									m++;
 									a[m][0]=c2;
 									a[m][1]=d2;

									m++;
									a[m][0]=c1;
 									a[m][1]=d1;

 									m++;
 									a[m][0]='$';
 									a[m][1]='$';

									reset();
								}
								}
							}
						}
						glFlush();
						break;
					case 6:
						glStencilFunc(GL_ALWAYS,  1, -1);
						if(pol==1)
 						{
 							m++;
 							a[m][0]='$';
 							a[m][1]='$';

 							pol=0;
 						}
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
							draw_point(X,Y);
							a2=a1;
 							b2=b1;
							a1=X;
 							b1=Y;
 							
 							c2 = c1;
 							d2 = d1;
 							c1 = x;
 							d1 = y;
 							
 							
 							if(a1 >= -0.7 && a1 <= 0.9 && b1 >= -0.7 && b1 <= 0.9)
							{
								if(a2 >= -0.7 && a2 <= 0.9 && b2 >= -0.7 && b2 <= 0.9)
								{
									drawOb = 1;
									glStencilFunc(GL_ALWAYS,  1, -1);
									glLineWidth(size);
 									glBegin(GL_LINE_LOOP);
 									glVertex2f(a2, b2);
 									glVertex2f(a1, b2);
 									glVertex2f(a1, b1);
 									glVertex2f(a2, b1);
 									glEnd();
 									m++;
 									a[m][0]=c2;
 									a[m][1]=d2;

 									m++;
 									a[m][0]=c1;
 									a[m][1]=d2;

									m++;
									a[m][0]=c1;
 									a[m][1]=d1;

 									m++;
 									a[m][0]=c2;
 									a[m][1]=d1;

 									m++;
 									a[m][0]='$';
 									a[m][1]='$';
 									count++;
 									reset();
								}
							}
							glFlush();	
						}
						break;
					case 7:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
							//printf("%f %f\n",X,Y);
							draw_point(X, Y);
 							a2=a1;
 							b2=b1;
 							a1=X;
 							b1=Y;
							
							m++;
 							a[m][0]=x;
 							a[m][1]=y;
							printf("%d ",m);
 							if(a1 >= -0.7 && a1 <= 0.9 && b1 >= -0.7 && b1 <= 0.9)
 							{
 								if(a2 >= -0.7 && a2 <= 0.9 && b2 >= -0.7 && b2 <= 0.9)
 								{
 									glLineWidth(size);
 									glBegin(GL_LINES);
 									glVertex2f(a2, b2);
 									glVertex2f(a1, b1);
 									glEnd();	
 								}
 							}	
							glFlush();
						}
						break;
					case 8:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
 							glPushMatrix();
 							glTranslated(X,Y,0);
 							glutSolidTorus(0.08,0.2,10,10);					
 							glPopMatrix();
 							glFlush();
 						}
 						break;
 					case 9:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
 						{
 							glPushMatrix();
							glTranslated(X,Y,0);
							glutSolidCube(0.25);
							glPopMatrix();
							glFlush();
 						}
 						break;
 					case 10:
						if(pol==1)
 						{
 							m++;
 							a[m][0]='$';
 							a[m][1]='$';

 							pol=0;
 						}
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
 							glutMotionFunc(eraser);
						break;
					case 11:
 							a[m][0]='$';
 							a[m][1]='$';
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
 						{
 							flag1=0;
 							for(i=1;i<=m;i++)
 							{
 								num2=i-1;
 								for(i1=i;a[i1][0]!='$';i1++)
 								{
 									num2++;
 								}
 								num1=i;
 								detect_point(num1, num2, x, y);
 								if(flag!=0)
 								{
 									scanfill(num1, num2);
 									flag=0;
 									flag1=1;
 								}
 								i=num2+1;
 							}
 							if(flag1==0 && flag2==0)
 							{

 								for(i=1;i<=n;i++)
 								{
 									num2=i-1;
 									for(i1=i;b[i1][0]!='$';i1++)
 									{
 										num2++;
 									}
 									num1=i;

 									r1=sqrt((b[num1][0]-b[num2][0])*(b[num1][0]-b[num2][0])+(b[num1][1]-b[num2][1])*(b[num1][1]-b[num2][1]));
 									r2=sqrt((b[num1][0]-X)*(b[num1][0]-X)+(b[num1][1]-Y)*(b[num1][1]-Y));
 									if(r2<r1)
 									{
 										for(i2=0;i2<r1;i2=i2+2)
 										{
 											fill=1;
 										}

 										fill=0;
 										flag2=1;
 									}
 									i=num2+1;
 								}
 							}
 							glFlush();
 						}
 						break;
 					case 12:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
							glutMotionFunc(brush);
						}
						break;
					case 13:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)

						{
							currentPosX = X;
							currentPosY = Y;
 							glRasterPos2f(currentPosX, currentPosY);
							glutKeyboardFunc(myKey);	
						}
						break;
					case 14:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
							glPushMatrix();
							glTranslated(X,Y,0);
							glutSolidSphere(0.25,25,25);
							glPopMatrix();
							glFlush();
						}
						break;
					/*case 15:
						if(X >= -0.7 && X <= 0.9 && Y >= -0.7 && Y <= 0.9)
						{
							glPushMatrix();
							glScalef(0.9,0.9,0);
							glTranslatef(X,Y,0);
							glBegin(GL_LINE_LOOP);
 							glVertex2f(0.02,0.48);
 							glVertex2f(-0.01,0.43);
 							glVertex2f(-0.01,0.37);
 							glVertex2f(0,0.34);
 							glVertex2f(0.03,0.34);
 							glVertex2f(0.05,0.38);
 							glVertex2f(0.05,0.43);
 							glEnd();	
 							glPopMatrix();
 							glFlush();
						}
						break;*/
						
				}
				
				if(X >= -0.8 && X <= -0.5 && Y >= 0.91 && Y <= 0.95) /* to OPEN an existing FILE */
 				{
 					save=2;
 					glColor3f(0, 0, 0);
 					ss = 1;
 					drawstring(0.2,0.93, 0.0, "Enter filename: ");
 					textx=0.55;
 					texty=0.94;
 				}

 				else if(X >= -0.5 && X <= -0.2 && Y >= 0.91 && Y <= 0.95) /* to SAVE the current image on to a FILE */
 				{
 					save=1;
 					glColor3f(0, 0, 0);
 					ss = 1;
 					drawstring(0.2,0.93, 0.0, "Enter filename: ");
 					textx=0.55;
 					texty=0.94;
 					save = 0;
 				}

 				else if(X >= -0.2 && X <= 0 && Y >= 0.91 && Y <= 0.95) /* to CLEAR the DRAWING AREA */
				{
 					for(i=1;i<=m;i++)
 						a[i][0]=a[i][1]=0;
 					for(i=1;i<=n;i++)
 						b[i][0]=b[i][1]=0;
 					m=n=0;
 					count=0;
 					display();
 				}
				palette(-0.95,-0.95,-0.85,-0.95, -0.85,-0.85, -0.95,-0.85); /* to display SELECTED COLOUR */
 				glFlush();
 				
 				glReadBuffer(GL_FRONT);
 				glReadPixels(-0.78, -0.78,0.98,0.9,GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP)
 	{
 		glReadBuffer(GL_FRONT);
 		glReadPixels(-0.78, -0.78,0.98,0.9,GL_RGB, GL_UNSIGNED_BYTE, image);
 	}	
}

void point_size(int id) /* to CHANGE THE PIXEL SIZE */
 {
 	switch(id)
 	{
 		case 1:
			size++;
 			break;

 		case 2:
			if(size>1)
 				size--;
 			break;
 	}
 }
 
 void right(int id) /* to QUIT the editor */
 {
 	if(id == 1)
 		exit(0);
 }
 
 void font_style(int id)
 {
		switch(id)
		{
			case 1:
				font1 = 1;
				break;
			case 2:
				font1 = 2;
				break;
			case 3:
				font1 = 3;
				break; 
			case 4:
				font1 = 4;
				break;
			case 5:
				font1 = 5;
				break;
			case 6:
				font1 = 6;
				break;
			case 7:
				font1 = 7;
				break;
		}
		
		
 }
 
 int main(int argc, char **argv)
 {
	image = malloc(3*1450*900*sizeof(char)); /* to ALLOCATE MEMORY required to SAVE the file */
 	a[0][0]='$';
 	a[0][1]='$';
 	m=0;

	b[0][0]='$';
 	b[0][1]='$';
 	n=0;
 	
 	glutInit(&argc,argv);
 	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
 	glutInitWindowSize(800,600);
 	glutInitWindowPosition(0,0);
 	glutCreateWindow("Draw");
 	glEnable(GL_BLEND);
 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 	sub_menu=glutCreateMenu(point_size);
 	glutAddMenuEntry("Increase", 1);
 	glutAddMenuEntry("Decrease", 2);
 	sub_menu1 = glutCreateMenu(font_style);
 	glutAddMenuEntry("Normal 13",1);
 	glutAddMenuEntry("Normal 15",2);
 	glutAddMenuEntry("Times Roman 10",3);
 	glutAddMenuEntry("Times Roman 24",4);
 	glutAddMenuEntry("Helvetica 10",5);
 	glutAddMenuEntry("Helvetica 12",6);
 	glutAddMenuEntry("Helvetica 18",7);
 	glutCreateMenu(right);
 	glutAddSubMenu("Point Size", sub_menu);
 	glutAddSubMenu("Font",sub_menu1);
 	glutAddMenuEntry("Quit",1);
 	glutAttachMenu(GLUT_RIGHT_BUTTON);
 	glutReshapeFunc(myReshape);
 	glutDisplayFunc(display);
 	glutMouseFunc(mouse);
 	glutKeyboardFunc(myKey);
	 glutMainLoop();
}
 
 
