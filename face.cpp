#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>

#include "helpFile.hpp"
#include "A_star.hpp"
// #pragma comment(lib,"glu32.lib")
using namespace sf;

#include <iostream>
using namespace std;

float angleX=180.f, angleY=90.f;
float x=-20.f,y=40.f,z=-20.f;
float speed = 0.2;
float PI = 3.141592;
const int range_X = 100;
const int range_Y = 100;
const int range_Z = 100;
bool mass[range_X][range_Y][range_Z];

int screenX=800, screenY=800;
float size = 20.f;
int xt = screenX/2;
int yt = screenY/2;


vector<aPoint*> way;
aPoint* pWay = NULL;
aPoint p_start(0,1,0);
aPoint p_finish(1,1,10);
// aPoint p_finish(3,4,5);

bool check(int x,int y, int z)
{
	if(x<0 || y<0 || z<0 || x>=range_X || y>=range_Y || z>=range_Z)
		return false;
	return mass[x][y][z];
}

void set_result()
{
	string sname = "./result/res" + to_string(time(0)) + ".txt";
	const char* name = sname.c_str();
	ofstream fout(name);
	// ofstream fout("./result/res.txt");
	for(int i=way.size()-1; i>=0;i--)
		fout<<way[i]->x<<":"<<way[i]->y<<":"<<way[i]->z<<endl;
	fout.close();
}


void set_map()
{
	string sname = "./maps/new" + to_string(time(0)) + ".txt";
	const char* name = sname.c_str();
	ofstream fout(name);
	// fout<<range_X<<" "<<range_Y<<" "<<range_Z<<endl;
	for(int i=0;i<range_X;i++)
	for(int j=0;j<range_Y;j++)
	for(int g=0;g<range_Z;g++)
		fout<<mass[i][j][g]<<" ";
	fout<<endl<<p_start.x<<" "<<p_start.y<<" "<<p_start.z;
	fout<<endl<<p_finish.x<<" "<<p_finish.y<<" "<<p_finish.z;
	fout.close();
}

int main()
{
	srand(time(0));
	set_range_map(range_X, range_Y, range_Z);
	
	
	ifstream fin("./act_map/act.txt");
	if(fin.is_open())
	{		
		for(int i=0;i<range_X;i++)
		for(int j=0;j<range_Y;j++)
		for(int g=0;g<range_Z;g++)
			fin>>mass[i][j][g];
		int k1, k2, k3;
		fin>>k1>>k2>>k3;
		p_start = aPoint(k1,k2,k3);
		fin>>k1>>k2>>k3;
		p_finish = aPoint(k1,k2,k3);
	}
	else
	{
		for(int i=0;i<range_X;i++)
		for(int j=0;j<range_Y;j++)
		for(int g=0;g<range_Z;g++)
			mass[i][j][g]=0;
		
		for(int x=0;x<20;x++)
			for(int y=0;y<20;y++)
				for(int z=0;z<20;z++)
				{
					if(y==0||rand()%100==1)
						mass[x][y][z]=1;
				}
		p_start = aPoint(0,1,0);
		p_finish = aPoint(1,1,0);
	}
	
	
	
    RenderWindow window(VideoMode(screenX, screenY), "DROncraft", Style::Default, ContextSettings(32));

	
	Texture crs;
	crs.loadFromFile("img/Png.png",IntRect(160, 160, 210, 210));
	Sprite s(crs); 
	s.scale(0.5,0.5); 
	s.setPosition(screenX/2-25,screenY/2-25);
	
	Texture tt;
	tt.loadFromFile("img/mee.png");
	Sprite bg(tt);

	
	GLuint cube[6];
	cube[0] = LoadTexture("img/s1.png");
	cube[1] = LoadTexture("img/s2.png");
	cube[2] = LoadTexture("img/s3.png");
	cube[3] = LoadTexture("img/s4.png");
	cube[4] = LoadTexture("img/s5.png");
	cube[5] = LoadTexture("img/s6.png");
	
	GLuint skybox[6];
	skybox[0] = LoadTexture("img/sb_back.jpg");
	skybox[1] = LoadTexture("img/sb_front.jpg");
	skybox[2] = LoadTexture("img/sb_left.jpg");
	skybox[3] = LoadTexture("img/sb_right.jpg");
	skybox[4] = LoadTexture("img/sb_bottom.jpg");
	skybox[5] = LoadTexture("img/sb_top.jpg");
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f,1.f,1.f,1800.f);
	glEnable(GL_TEXTURE_2D);
	
	ShowCursor(false);
	
	Clock clock;
	bool mLeft=0,mRight=0;
	bool Setings=0;
	
	while(window.isOpen())
	{
		float time = clock.getElapsedTime().asMilliseconds();
		
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
			if (event.type == Event::MouseButtonPressed)
			{
				if(event.mouseButton.button == Mouse::Right) mRight=1;
				if(event.mouseButton.button == Mouse::Left) mLeft=1;
			}
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape) window.close();
				if (event.key.code == Keyboard::Tab) 
				{
					if(Setings) ShowCursor(false);
					else ShowCursor(true);	
					Setings=!Setings;
					SetCursorPos(xt,yt);
				}
			}
        }
		
		glClearColor(1,1,1,0);
		
		float dx=0,dy=0,dz=0;
		if(!Setings)
		{
			if(Keyboard::isKeyPressed(Keyboard::W))
			{
				dx += -sin(angleX/180*PI)*speed;
				dz += -cos(angleX/180*PI)*speed;
			}
			if(Keyboard::isKeyPressed(Keyboard::S))
			{
				dx += sin(angleX/180*PI)*speed;
				dz += cos(angleX/180*PI)*speed;
			}
			if(Keyboard::isKeyPressed(Keyboard::A))
			{
				dx += sin((angleX-90)/180*PI)*speed;
				dz += cos((angleX-90)/180*PI)*speed;
			}
			if(Keyboard::isKeyPressed(Keyboard::D))
			{
				dx += sin((angleX+90)/180*PI)*speed;
				dz += cos((angleX+90)/180*PI)*speed;
			}
			if(Keyboard::isKeyPressed(Keyboard::Q))
			{
				dy += speed;
			}
			if(Keyboard::isKeyPressed(Keyboard::E))
			{
				dy += -speed;
			}
			if(Keyboard::isKeyPressed(Keyboard::R))
			{
				way = A_star(&mass[0][0][0],p_start,p_finish);
				if(way.size()) pWay=way[0];	
			}
			if(Keyboard::isKeyPressed(Keyboard::F))
			{
				way = A_star(&mass[0][0][0],p_start,p_finish);
				way = optimazzz(&mass[0][0][0], way);
				if(way.size()) pWay=way[0];
			}
			if (event.key.code == Keyboard::Z || event.key.code == Keyboard::X) 
			{
				float mx=x,my=y,mz=z;
				int X=x,Y=y,Z=z,oldX=x,oldY=y,oldZ=z;
				int dist=0;
				while(dist<120)
				{
					dist++;
					mx += -sin(angleX/180*PI);	X=mx/size;
					my += tan(angleY/180*PI);	Y=my/size;
					mz += -cos(angleX/180*PI);	Z=mz/size;
					
					if(check(X,Y,Z))
						if(oldX>=0 && oldX <range_X && oldY>=0 && oldY <range_Y && oldZ>=0 && oldZ <range_Z)
							if(event.key.code == Keyboard::Z) { p_start = aPoint(oldX,oldY,oldZ); break; }
							else { p_finish = aPoint(oldX,oldY,oldZ); break; }
					oldX=X; oldY=Y; oldZ=Z;
				}
				// way = A_star(&mass[0][0][0],p_start,p_finish);
				// if(way.size()) pWay=way[0];
				pWay=NULL;
			}
			if (event.key.code == Keyboard::T) { set_result(); }
			if(Keyboard::isKeyPressed(Keyboard::P)){ speed += 0.005; }
			if(Keyboard::isKeyPressed(Keyboard::O)){ speed -= 0.005; }
			
			if(Keyboard::isKeyPressed(Keyboard::N)){ set_map(); set_result(); }
			
			if( (dx>0 && check(x/size+1,y/size,z/size)==false) || (dx<0 && check(x/size-1,y/size,z/size)==false) )
				x+=dx;
			if( (dy>0 && check(x/size,y/size+1,z/size)==false) || (dy<0 && check(x/size,y/size-1,z/size)==false) )
				y+=dy;
			if( (dz>0 && check(x/size,y/size,z/size+1)==false) || (dz<0 && check(x/size,y/size,z/size-1)==false) )
				z+=dz;
			


			POINT mousexy;
			GetCursorPos(&mousexy);
			xt = window.getPosition().x + screenX/2;
			yt = window.getPosition().y + screenY/2;
			angleX += (xt-mousexy.x)/3;	// 4 - ????????????????
			angleY += (yt-mousexy.y)/3;
			if(angleY<-89.f) angleY=-89.f;
			if(angleY>89.f) angleY=89.f;
			SetCursorPos(xt,yt);


			if(mRight||mLeft)
			{
				float mx=x,my=y,mz=z;
				int X=x,Y=y,Z=z,oldX=x,oldY=y,oldZ=z;
				int dist=0;
				while(dist<120)
				{
					dist++;
					mx += -sin(angleX/180*PI);	X=mx/size;
					my += tan(angleY/180*PI);	Y=my/size;
					mz += -cos(angleX/180*PI);	Z=mz/size;
					
					if(check(X,Y,Z))
					{
						if(mLeft) {mass[X][Y][Z]=0; break;}
						else
						{
							if(oldX>=0 && oldX <range_X && oldY>=0 && oldY <range_Y && oldZ>=0 && oldZ <range_Z)
								mass[oldX][oldY][oldZ]=1; break;
						}
					}
					oldX=X; oldY=Y; oldZ=Z;
				}
			}
			mLeft=0; mRight=0;
		}
		else
		{
			// SetCursor(oldCursor);
		}
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(x,y,z, x-sin(angleX/180*PI), y+tan(angleY/180*PI), z-cos(angleX/180*PI), 0,1,0);

		
		
		for(int x=0;x<range_X;x++)
			for(int y=0;y<range_Y;y++)
				for(int z=0;z<range_Z;z++)
				{
					if(!mass[x][y][z]) continue;
					
					glTranslatef(x*size+size/2,y*size+size/2,z*size+size/2);
					createBox(cube,size/2);	
					glTranslatef(-x*size-size/2,-y*size-size/2,-z*size-size/2);
				}
		
		
		glTranslatef(x,y,z);
		createBox(skybox,1000);	
		glTranslatef(-x,-y,-z);
		
		window.pushGLStates();
		window.draw(s);
		window.popGLStates();
		
		
		glLineWidth(10);
		glBegin(GL_LINE_STRIP);
	 		glColor3f(0.5,0.5,1);
			aPoint* pt = pWay;
			while(pt)
			{
				glVertex3f((pt->x+0.5)*size, (pt->y+0.5)*size, (pt->z+0.5)*size);
				pt=pt->pP;
			}
			// glColor3f(1,1,1);
		glEnd();
		
		glPointSize(30);
		glBegin(GL_POINTS);
	 		glColor3f(1,0,0);
			glVertex3f((p_start.x+0.5)*size, (p_start.y+0.5)*size, (p_start.z+0.5)*size);
			glColor3f(0,1,0);
			glVertex3f((p_finish.x+0.5)*size, (p_finish.y+0.5)*size, (p_finish.z+0.5)*size);
			glColor3f(1,1,1);
		glEnd();
		
		if(Setings)
		{
			window.pushGLStates();
			window.draw(bg);
			window.popGLStates();
		}
        window.display();
    }
	
	glDeleteTextures(6,&cube[0]);
	glDeleteTextures(6,&skybox[0]);
	
    return 0;
}

// g++ -c face.cpp -IF:\SFML-2.5.1\include
// g++ face.o -o DROncraft -LF:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lglu32
// DROncraft.exe