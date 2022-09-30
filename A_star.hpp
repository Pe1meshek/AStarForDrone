#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

//������� �� ���������
int a1=1000,a2=1000,a3=1000;

//������� ���������� ��� �������� �����
void set_range_map(int x, int y, int z)
{
	a1=x; a2=y; a3=z;
}


class aPoint
{
public:
	int x,y,z;
	float way;
	float ves;
	aPoint* pP;
	//����������� ������
	aPoint(int ax, int ay, int az){x=ax; y=ay; z=az;} 
	//���������� ��������� ���������
	bool operator==(const aPoint& rhs) const 
	{
		if(this->x==rhs.x && this->y==rhs.y && this->z==rhs.z)
			return true;
		return false;
	}
};


bool b_mass(bool *mass, int x, int y, int z)
{
	//����� �� ��������� ������� ��������� ������, �.�. �����������
	if(x<0 || y<0 || z<0 || x>=a1 || y>=a2 || z>=a3)
		return true;
	return *(mass+a2*a3*x+a3*y+z);
}


float short_way(aPoint s, aPoint f)
{
	return sqrt((f.x-s.x)*(f.x-s.x) + (f.y-s.y)*(f.y-s.y) + (f.z-s.z)*(f.z-s.z));
}


bool is_clear(bool *mass, aPoint a, aPoint b)
{
	//��������������, ��� a � b �������� �����
	//������� �������� ��������� ���� �����
	int i = b.x-a.x;
	int j = b.y-a.y;
	int g = b.z-a.z;
	
	//���� ��� ��� ���������� ���������� �� 1, �� ����������� �������������� � ����
	if(abs(i)+abs(j)+abs(g)==3)
		if(b_mass(mass,a.x+i,a.y,a.z) || b_mass(mass,a.x,a.y+j,a.z) || b_mass(mass,a.x,a.y,a.z+g)
		|| b_mass(mass,a.x+i,a.y,a.z+g) || b_mass(mass,a.x+i,a.y+j,a.z) || b_mass(mass,a.x,a.y+j,a.z+g)) 
			return false;
		else return true;
		
	//���� ��� ���������� ���������� �� 1, �� ����������� �������������� � �����
	if(abs(i)+abs(j)==2)
		if(b_mass(mass,a.x+i,a.y,a.z) || b_mass(mass,a.x,a.y+j,a.z))
			return false;
		else return true;
		
	if(abs(i)+abs(g)==2)
		if(b_mass(mass,a.x+i,a.y,a.z) || b_mass(mass,a.x,a.y,a.z+g))
			return false;
		else return true;
		
	if(abs(j)+abs(g)==2)
		if(b_mass(mass,a.x,a.y+j,a.z) || b_mass(mass,a.x,a.y,a.z+g))
			return false;
		else return true;
		
	//����� ����������� �������������� � ���� �����, ����� ����������� ���
	return true;
}


void add_ves(bool *mass, vector<aPoint*>& otb, vector<aPoint*>& kch, aPoint* a, aPoint f)
{
	aPoint* n = NULL;
	//���� ��������� �������� �����
	for(int i=-1; i<2; i++)
	for(int j=-1; j<2; j++)
	for(int g=-1; g<2; g++)
	{
		//���� �� ������� �� �������, �� ������� ��� ��� ������
		if(a->x+i>=0 && a->x+i<a1 
		&& a->y+j>=0 && a->y+j<a2 
		&& a->z+g>=0 && a->z+g<a3)
		{
			n = new aPoint(a->x+i,a->y+j,a->z+g);
		}
		else continue;
		
		//���� ����� ������ �������� �����, �� ������� ���������� �����
		if((*n)==(*a)){delete n; continue;}
		
		//���� ����� ����� ���������� � ������� otbros, �� ������� �����
		for(int fo=0; fo<otb.size(); fo++)
			if(*n==*(otb[fo])){delete n; n=NULL; break;}
		if(!n) continue;
		
		//���� ����� ������������, �� ��������� � ������ otbros
		if(b_mass(mass,n->x,n->y,n->z))
		{
			otb.push_back(n);
			n=NULL;
			continue;
		}
		
		//���� ������������� � ��������� ����� ����� �����������, �� ������� ��
		if(!is_clear(mass, *a, *n)) { delete n; n=NULL; continue; }
		
		//����� ��������� ��������� �������������� ��������� �����
		n->way = a->way + short_way(*n, *a);
		n->ves = n->way + short_way(*n, f);
		n->pP = a;
		
		//���� ����� ������� kucha ���������� ����� ����� � �� �������������� ����
		//��� � ���������,	�� �������� ��, ����� ������� ����������
		for(int fo=0; fo<kch.size(); fo++)
			if(*n==*(kch[fo]))
			{
				if(n->ves<kch[fo]->ves)
				{
					delete kch[fo];
					kch[fo] = n;
				}
				else delete n;
				n=NULL; break;
			}
		if(!n) continue;
		
		//���� ����� ������� kucha ����� ����� ���, �� ���������
		kch.push_back(n);
		n=NULL;
	}
}

bool is_wall_on_way(bool *mass, aPoint a, aPoint b)
{
	//������� �������� ��������� ���� �����
	float vx = b.x-a.x;
	float vy = b.y-a.y;
	float vz = b.z-a.z;
	
	//���������� �������� ������������ �������
	float max=1.0f;
	float mv[] = {vx,vy,vz};
	for(int i=0;i<3;i++)
		if(mv[i]!=0) {max=abs(mv[i]); break;}
	for(int i=0;i<3;i++)
		if(max<abs(mv[i]) && mv[i]!=0) max=abs(mv[i]);
	
	//������ ��������� �������� �� ������ ���
	vx/=max; vy/=max; vz/=max;
	
	//�������� ����������: ������ ����� ����, ��������� �� �����, 
	//���������� ���������� ����������� ���������� ���������
	vector<aPoint*> way;
	aPoint* p = &a;
	float nx=a.x, ny=a.y, nz=a.z;

	//����������� ���������� ��������� � 
	//���������� ������� ����������� �������
	for(int i=0;i<max-1;i++)
	{
		nx+=vx; ny+=vy; nz+=vz;
		p = new aPoint(round(nx),round(ny),round(nz));
		if(b_mass(mass,p->x,p->y,p->z))
		{	
			for(int i=0;i<way.size();i++) delete way[i];
			return true;
		}
		way.push_back(p);
	}
	p = new aPoint(b.x,b.y,b.z);
	way.push_back(p);
	
	//���������� ������ �����, �������������� � ��� �������,
	//����� ����������� ������������� �� �������, � ����� ���� �� ���������
	int dx, dy, dz;
	for(int i=way.size()-1;i>1;i--)
	{
		dx = way[i]->x - way[i-2]->x;
		dy = way[i]->y - way[i-2]->y;
		dz = way[i]->z - way[i-2]->z;
		if( abs(dx)<2 && abs(dy)<2 && abs(dz)<2 )
		{
			delete way[i-1];
			way.erase(way.begin()+i-1);
			i++;
		}
	}
	
	//������� ����� � ��������� �� ����������������� ����������� �� ���
	aPoint st = a;
	for(int i=0;i<way.size();i++)
	{
		if(!is_clear(mass,st,*(way[i])))
		{
			//� ������ ����������� ����������� ����������� ������ � ���������� true
			for(int i=0;i<way.size();i++) delete way[i];
			return true;
		}
		st = *(way[i]);
	}

	//���� ����������� �� ����������, �� ����������� ������ � ���������� false 
	for(int i=0;i<way.size();i++) delete way[i];
	return false;
}

vector<aPoint*> optimazzz(bool *mass, vector<aPoint*>& vec)
{
	//���� ���� ��������, �� �������������� ������
	if(vec.size()<2) return vec;
	
	//�������� ������� res ��� ��������������� ��������
	//� �������� res ����������� ������� ��������� ��������, ������� ������ ���������
	vector<aPoint*> res;
	aPoint* act = vec[0];	//������ �� �������� �����
	res.push_back(act);
	vec.erase(vec.begin());
	aPoint finish = *(vec[vec.size()-1]);
	int iact = 0;	//������ �������� �����
	
	//������� ����� ����
	//�� ������ ����� ������������ ���� ����� ������� � �������������
	while(!(*act==finish))
	{
		bool f = false;
		for(int i=vec.size()-1;vec[i]!=act;i--)
			if(!is_wall_on_way(mass, *act, *(vec[i])))
			{
				iact = i;
				act=vec[i];
				res.push_back(act);
				vec.erase(vec.begin()+i);
				f=true;
				break;
			}
		if(f) continue;
		res.push_back(act);
		vec.erase(vec.begin()+iact);
		act = act->pP;
		iact++;
	}

	//���������� ������ �� ���������� ������ ������ ������������ ��������
	for(int i=0;i<res.size()-1;i++)
		res[i]->pP=res[i+1];
	res[res.size()-1]->pP=NULL;
	
	//�������� ��������� �������� � ����������� ����������
	for(int i=0;i<vec.size();i++) delete vec[i];
	return res;
}

vector<aPoint*> A_star(bool *mass, aPoint& start, aPoint& finish)
{
	//������ ����� ��������
	vector<aPoint*> res;
	
	//���� ����� ��� ����� - ������������ �����������, �� ������� �� �������
	if(b_mass(mass,start.x,start.y,start.z) || b_mass(mass,finish.x,finish.y,finish.z))
		return res;
	
	//���� ����� ��� ����� �������� ������������� �������, �.�.
	//� ��� ���������� �������� ����, �� ������� �� ��������
	if(b_mass(mass,start.x+1,start.y,start.z) && b_mass(mass,start.x-1,start.y,start.z)
	&& b_mass(mass,start.x,start.y+1,start.z) && b_mass(mass,start.x,start.y-1,start.z)
	&& b_mass(mass,start.x,start.y,start.z+1) && b_mass(mass,start.x,start.y,start.z-1) || 
		b_mass(mass,finish.x+1,finish.y,finish.z) && b_mass(mass,finish.x-1,finish.y,finish.z)
	&& b_mass(mass,finish.x,finish.y+1,finish.z) && b_mass(mass,finish.x,finish.y-1,finish.z)
	&& b_mass(mass,finish.x,finish.y,finish.z+1) && b_mass(mass,finish.x,finish.y,finish.z-1))
		return res;
	
	//�������������� ��������� �����
	start.way=0;
	start.ves=0;
	start.pP=NULL;
	
	vector<aPoint*> otbros;		//������ ��� ����������� � ������������ �����
	vector<aPoint*> kucha;		//������ ����� �������������� �������� 
	aPoint* active = &start;	//��������� �� �������� �����
	aPoint* old_active = &start;//��������� �� ���������� �������� �����
	int ia = 0;		//������ �������� �����
	float min;		//����������� ���
	
	//�������� ���� ���������, ����������� �� ���������� ������
	while(!(*active==finish))
	{
		//����������� ����� ������ � �������� ��������
		add_ves(mass, otbros, kucha, active, finish);
		
		//���������� ����� � ����������� �����, ������� ���������� ��������
		min=kucha[0]->ves;
		ia=0;
		for(int i=0;i<kucha.size();i++)
			if(kucha[i]->ves<min)
			{
				min=kucha[i]->ves;
				ia=i;
			}
		otbros.push_back(active);
		active = kucha[ia];
		kucha.erase(kucha.begin()+ia);
	}
	
	//���������� ������� res, �������������� ������� �� ����������
	res.push_back(active);
	active = active->pP;
	while(active)
	{
		res.push_back(active);
		for(int i=0;i<otbros.size();i++)
			if(*(otbros[i])==*active)
			{
				otbros.erase(otbros.begin()+i);
				break;
			}
		active = active->pP;
	}
	
	//������������ ������, ����������� ����������
	for(int i=0;i<kucha.size();i++) delete kucha[i];
	for(int i=0;i<otbros.size();i++) delete otbros[i];
	return res;
}

#endif