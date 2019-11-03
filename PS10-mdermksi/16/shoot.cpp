#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include "fssimplewindow.h"


class GameObject
{
public:
	int x,y;
};

class GameObjectWithState : public GameObject
{
public:
	int state;
};



void DrawSpaceShip(int x,int y)
{
	glColor3ub(0,255,0);

	glBegin(GL_QUADS);

	glVertex2i(x-15,y);
	glVertex2i(x+15,y);
	glVertex2i(x+15,y-10);
	glVertex2i(x-15,y-10);

	glVertex2i(x-5,y);
	glVertex2i(x+5,y);
	glVertex2i(x+5,y-19);
	glVertex2i(x-5,y-19);

	glEnd();
}

void DrawMissile(int x,int y)
{
	glColor3ub(255,0,0);

	glBegin(GL_LINES);
	glVertex2i(x,y);
	glVertex2i(x,y+10);
	glEnd();
}

void DrawTarget(int tx,int ty,int tw,int th)
{
	glColor3ub(0,255,0);

	glBegin(GL_QUADS);
	glVertex2i(tx   ,ty);
	glVertex2i(tx+tw,ty);
	glVertex2i(tx+tw,ty+th);
	glVertex2i(tx   ,ty+th);
	glEnd();
}

class Missile : public GameObjectWithState
{
public:
	void Initialize(void);
	bool Launch(int sx,int sy);
	void Move(void);
	int CheckCollision(class Target t);
	void Disappear(void);
	void Draw(void);
};

class Target : public GameObjectWithState
{
public:
	int w,h,v;

	void Initialize(void);
	void Disappear(void);
	void Move(void);
	void Draw(void);
};

class Player : public GameObject
{
public:
	void Initialize(void);
	void Draw(void);
	void KeyPress(int key,class ShootingGame &game);
};

class Explosion
{
public:
	class Point
	{
	public:
		double x,y;
	};

	class Particle
	{
	public:
		Point p,v;
	};

	enum
	{
		nParticle=30
	};

	int state;
	int counter;
	Particle particle[nParticle];

	void Initialize(void);
	bool Explode(int x,int y);
	void Draw(void);
	void Move(void);
};

class ShootingGame
{
public:
	const int nMissile;
	const int nTarget;
	const int nExplosion;

	Player player;
	std::vector <Missile> missile;
	std::vector <Target> target;
	std::vector <Explosion> explosion;

	ShootingGame();
	~ShootingGame();
	void Run(void);
};

ShootingGame::ShootingGame() : nMissile(5), nTarget(5), nExplosion(2)
{
	missile.resize(nMissile);
	target.resize(nTarget);
	explosion.resize(nExplosion);
}
ShootingGame::~ShootingGame()
{
}
void Explosion::Initialize(void)
{
	state=0;
}

bool Explosion::Explode(int x,int y)
{
	if(0==state)
	{
		state=1;
		counter=0;
		for(auto &par : particle)
		{
			par.p.x=(double)x;
			par.p.y=(double)y;
			par.v.x=(double)(rand()%1001-500)/50.0;
			par.v.y=(double)(rand()%1001-500)/50.0;
		}
		return true;
	}
	return false;
}

void Explosion::Draw(void)
{
	if(0!=state)
	{
		glPointSize(2);
		glColor3ub(255,0,0);
		glBegin(GL_POINTS);
		for(auto par : particle)
		{
			glVertex2d(par.p.x,par.p.y);
		}
		glEnd();
	}
}

void Explosion::Move(void)
{
	if(0!=state)
	{
		++counter;
		if(50<=counter)
		{
			state=0;
		}

		for(auto &par : particle)
		{
			par.p.x+=par.v.x;
			par.p.y+=par.v.y;
		}
	}
}


void Missile::Initialize(void)
{
	state=0;
}

bool Missile::Launch(int sx,int sy)
{
	if(0==state)
	{
		state=1;
		x=sx;
		y=sy;
		return true;
	}
	return false;
}

void Missile::Move(void)
{
	if(0!=state)
	{
		y-=10;
		if(y<0)
		{
			Disappear();
		}
	}
}

void Missile::Disappear(void)
{
	state=0;
}

int Missile::CheckCollision(Target t)
{
	int dx=x-t.x;
	int dy=y-t.y;

	if(0<=dx && dx<=t.w && 0<=dy && dy<=t.h && 0!=t.state && this->state!=0)
	{
		return 1;
	}
	return 0;
}

void Missile::Draw(void)
{
	if(0!=state)
	{
		DrawMissile(x,y);
	}
}


void Target::Initialize(void)
{
	x=0;
	y=80+rand()%20;
	w=100+rand()%20;
	h=20+rand()%10;
	v=10+rand()%5;
	state=1;
}

void Target::Disappear(void)
{
	state=0;
}

void Target::Move(void)
{
	x+=v;
	if(800<=x)
	{
		x=0;
	}
}

void Target::Draw(void)
{
	if(0!=state)
	{
		DrawTarget(x,y,w,h);
	}
}


void Player::Initialize(void)
{
	x=400;
	y=500;
}
void Player::Draw(void)
{
	DrawSpaceShip(x,y);
}
void Player::KeyPress(int key,class ShootingGame &game)
{
	if(FSKEY_LEFT==key)
	{
		x-=10;
	}
	else if(FSKEY_RIGHT==key)
	{
		x+=10;
	}

	if(FSKEY_SPACE==key)
	{
		for(auto &m : game.missile)
		{
			if(true==m.Launch(x,y))
			{
				break;
			}
		}
	}
}

void ShootingGame::Run(void)
{
	player.Initialize();

	for(auto &m : missile)
	{
		m.Initialize();
	}

	for(auto &t : target)
	{
		t.Initialize();
	}

	for(auto &ex : explosion)
	{
		ex.Initialize();
	}


	// int missileCounter=0;

	for(;;)
	{
		FsPollDevice();

		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}

		player.KeyPress(key,*this);

		for(auto &ex : explosion)
		{
			ex.Move();
		}
		for(auto &m : missile)
		{
			m.Move();
		}
		for(auto &t : target)
		{
			if(0!=t.state)
			{
				t.Move();

				for(auto &m : missile)
				{
					if(0!=m.state &&
					   0!=m.CheckCollision(t))
					{
						printf("Hit!\n");

						for(auto &ex : explosion)
						{
							if(true==ex.Explode(m.x,m.y))
							{
								break;
							}
						}

						t.Disappear();
						m.Disappear();
					}
				}
			}
		}

		int nAlive=0;
		for(auto t : target)
		{
			nAlive+=t.state;
		}
		for(auto ex : explosion)
		{
			nAlive+=ex.state;
		}
		if(0==nAlive)
		{
			break;
		}


		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		player.Draw();
		for(auto m : missile)
		{
			m.Draw();
		}
		for(auto t : target)
		{
			t.Draw();
		}
		for(auto ex : explosion)
		{
			ex.Draw();
		}
		FsSwapBuffers();

		FsSleep(10);	// std::this_thread chrono thread
	}
}

int main(void)
{
	srand((int)time(NULL));

	FsOpenWindow(0,0,800,600,1);

	ShootingGame game;
	game.Run();

	return 0;
}
