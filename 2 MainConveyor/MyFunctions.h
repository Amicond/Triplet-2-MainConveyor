#include "stdafx.h"
#include <vector>
#include <string>
using namespace std;

///


///
const int N=8;

//start test const
const int function_flag=4;
//end test const

const string delim="\\";
const string inp_fin="final_data";
const string inp_matr="matrixes"+delim+"b3_";
const string inp_route="input_routes"+delim;
const string out_res="results_";

const string type1="0";
const string type2="1";
const string type3="2";


const int DiffStates = 16; //количкество различных собственных состояний, равно размеру матриц
const int maxIntElem=4; // количество слагаемых в операторе взаимодействия
const int Namount=576;   //количество различных ground-состояний 6
const int resAmount=45; //кол-во различных J факторов в 8 порядке
const int matrixResAmount=429; //кол-во различных слагаемых в ряду теории возмущений в 8 порядке


struct step
{
	step(int x,int y)
	{
		sx=x;
		sy=y;
	}
	int sx;
	int sy;
	bool operator==(step s2)
	{
		if((sx==s2.sx)&&(sy==s2.sy))
		{
			return true;
		}
		else
			return false;
	}
};

struct state
{
	double factor; //числовой множитель

	//!!! проверить char на ок
	char coeff[3];  //степени коэффициентов J1,J2 и (J2-J1)
	vector<char> states;// номера состояний из соответствующих плакетов
	bool operator<(const state &s) const//только для 2ух состояний, переделать в высших порядках
	{
		int i=0;
		while(i<states.size())
		{
			if(states[i]<s.states[i]) return true;
			else if(states[i]>s.states[i]) return false;
			i++;
		}
		i=0;
		while(i<3)
		{
			if(coeff[i]<s.coeff[i]) return true;
			else if(coeff[i]>s.coeff[i]) return false;
			i++;
		}
		return false;
	}


	bool operator==(const state &s) const //состояния равны если номера всех состояний равны
	{
		bool Res=true;
		for(int i=0;i<states.size();i++)
			if(states[i]!=s.states[i])
			{
				Res=false;
				break;
			}
			return Res;
	}

	bool check(const state &s)
	{
		bool Res=true;
		for(int i=0;i<states.size();i++)
			if(states[i]!=s.states[i])
			{
				Res=false;
				break;
			}
		if(Res)
		{
			for(int i=0;i<3;i++)
				if(s.coeff[i]!=coeff[i])
				{
					Res=false;
					break;
				}
		}
		return Res;
	}
};

struct inter
{
	char n1,n2; //номера плакетов, нумерация с 0
	char v1,v2; //номера вершин, нумерация с 0
	char Jtype; // 0-J1, 1-J2, 2-(J2-J1)
};

struct res
{
	double factors[matrixResAmount]; //итоговые множители при полном сворачивании
	//28 - число различных факторов в 6 порядке


	//OK
	res& operator+=(res &tmp)
	{
		for(int i=0;i<matrixResAmount;i++)
		{
			factors[i]+=tmp.factors[i];
		}
		return *this;
	}
	void minus()
	{
		for(int i=0;i<matrixResAmount;i++)
			factors[i]*=-1;
	}
};


struct edge //для хранения координат ребер
{
	int x1,y1,x2,y2;
	bool operator==(const edge &e2) const
	{
		if(((x1==e2.x1)&&(x2==e2.x2)&&(y1==e2.y1)&&(y2==e2.y2))||((x1==e2.x2)&&(x2==e2.x1)&&(y1==e2.y2)&&(y2==e2.y1)))
		{
			return true;
		}
		else
			return false;
	}
	void set(int X1,int Y1,int X2,int Y2)
	{
		x1=X1;
		y1=Y1;
		x2=X2;
		y2=Y2;
	}

};


//////////////////////////////////////////////////////////////////////////
extern double Vmatrix[4][3][16][16];//Номер вершины в плакете, тип матрицы SP,SM или SZ,номер ряда, номер столбца
extern double VmatrixInside[16][16];//Специально для внутренних воздействий
extern double Energie[16]; //Энергии состояний
extern inter curInter [N][maxIntElem]; //массив операторов взамодействий
extern int interAmount[N];// кол-во эл-тов в каждом операторе

extern vector<state> vIn[Namount],vOut1[Namount],vOut2[Namount]; //вектора состоянй-используем по очереди один как входной другой как выходной, на следующем шаге - наоборот
extern vector<state> vOutTemp1,vOutTemp2;//Промежуточные вектора
extern vector<state> vtemp;
extern vector<state> temp,temp2,tempvec,tempvec2;
extern state init;
//////////////////////////////////////////////////////////////////////////

double getE0(int node_num);//определяет энергию уровня, к которму определяются поправки


void returnV(inter curInter[][maxIntElem],int interAmount[],int interN,int n1,int n2,int dx,int dy);//устанавливает оператор взаимодействия вдоль ребра


void generate_all_Jfactors(int n,int **Jfactors);

void generate_all_Jstrings(int n,int **Jfactors,string *strarr);


void collect(vector<state> &outvec,vector<state> &invec);

void act(vector<state> &inv,vector<state> &outv,double Vmatrix[4][3][16][16],int interNumber, int node_num);

void act_ground(vector<state> &inv,vector<state> &outv,double Vmatrix[4][3][16][16],int interNumber, int node_num);

void act_energy(vector<state> &inv,vector<state> &outv,double Vmatrix[4][3][16][16],int interNumber, int node_num);

void act_energy_power(vector<state> &inv,vector<state> &outv,int power,double Vmatrix[4][3][16][16],int interNumber, int node_num);

void act_inside(vector<state> &inv,vector<state> &outv,int plaquetNumber, int node_num);

void act_inside_ground(vector<state> &inv,vector<state> &outv,int plaquetNumber, int node_num);

void act_inside_enrgy_power(vector<state> &inv,vector<state> &outv,int power,int plaquetNumber, int node_num);

void generate_procedure_order(int *termorder,int* operatororder,int edge_amount,int num,int *Res,int *power);
