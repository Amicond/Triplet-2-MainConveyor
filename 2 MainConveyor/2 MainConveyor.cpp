//Edit 05.02.2017 First changes from singlet version
//                Change order of basis order
//				  getE0()added
//				  output files names are changed


//edit 08.02.2017 change N->node_num in all "act_*" functions

//edit 08.02.2017  change vIn: read proper file for each route

//edit 09.02.2017 struct states: change array to vector, all act procedures received init for "tempst"

// MainConveyor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyFunctions.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <time.h>

using namespace std;
ofstream logfile("log.txt",ios::out);


vector<edge> edges;//ребра текущего маршрута
vector<step> nodes;//вершины текущего маршрута

vector<state> *ref1,*ref2;
int node_nums_of_edges[N][2];

int minus1(int *nodeSet,int n) //вычисляем знак для текущего слагаемого в рду теории возмущения
{
	int res=1;
	for(int i=0;i<n-1;i++)
	{
		if(nodeSet[i]==0)
			res*=-1;
	}
	return res;
}
res **Matrix[matrixResAmount]; //своя матрица под каждое слагаемое ряда в 6 порядке
res **MatrixFull;//Суммарная матрица на маршрут

void eval_cur_route(int r[][2],int OrderLength, int RouteLength,vector<edge> &edges,vector<step> &nodes,int &RealLength)
	//заполняет для данного маршрута список ребер и их количество
{
	for(int i=0;i<8;i++)
	{
		node_nums_of_edges[i][0]=0;
		node_nums_of_edges[i][1]=0;
	}
	//Проверяем реальную длину маршрута+ строим маску - сколько раз каждое ребро должно встречаться
	int num_ed[10];//число ребер
	edge cur;
	bool flag;
	RealLength=0;//Реальное количество различных "ГЛАВНЫХ" операторов
	edges.clear();
	for(int i=0;i<10;i++)
	{
		num_ed[i]=0;
	}
	for (int i=0;i<2*RouteLength;i+=2)//список ребер
	{
		cur.set(r[i][0],r[i][1],r[i+1][0],r[i+1][1]);
		flag=true;
		for(unsigned int j=0;j<edges.size();j++)
		{
			if(cur==edges[j])
			{
				flag=false;
				num_ed[j]++;
			}
		}
		if(flag)
		{
			edges.push_back(cur);
			num_ed[edges.size()-1]++;
		}
	}
	nodes.clear();
	//nodes.push_back(step(r[0][0],r[0][1]));
	for(int i=0;i<2*RouteLength;i++)//вычисляем номера вершин
	{
		if(find(nodes.begin(),nodes.end(),step(r[i][0],r[i][1]))==nodes.end())
			nodes.push_back(step(r[i][0],r[i][1]));
	}
	for(unsigned int i=0;i<edges.size();i++)//заполняем номера в "nodes" начала и конца для каждого ребра из "edges"
	{
		node_nums_of_edges[i][0]=distance(nodes.begin(),find(nodes.begin(),nodes.end(),step(edges[i].x1,edges[i].y1)));
		node_nums_of_edges[i][1]=distance(nodes.begin(),find(nodes.begin(),nodes.end(),step(edges[i].x2,edges[i].y2)));
	}
	RealLength=edges.size();
}




//////////////////////////////////////////////////////////////////////////
res finalvalue3(vector<state> &v1,vector<state> &v2,int **Jfactors,int n) //полный перебор, можно улучшить
{
	res fv;
	for(int i=0;i<(n+2)*(n+1)/2;i++)
		fv.factors[i]=0;
	unsigned int i1,i2;
	int tmpres[3];
	for(i2=0;i2<v2.size();i2++)
	{
		for(i1=0;i1<v1.size();i1++)
		{
			if(v1[i1]==v2[i2])
			{
				ofstream outtestFin("outfin3.txt",ios::app);
				outtestFin<<i1<<" "<<i2<<"\n";
				outtestFin.close();
				for(int ttt=0;ttt<3;ttt++)
				{
					tmpres[ttt]=0;
					tmpres[ttt]=v1[i1].coeff[ttt]+v2[i2].coeff[ttt];
				}
				/*
				if(tmpres[0]==3)
					fv.factors[0]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[0]==2&&tmpres[1]==1)
					fv.factors[1]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[0]==2&&tmpres[2]==1)
					fv.factors[2]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[0]==1&&tmpres[1]==2)
					fv.factors[3]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[0]==1&&tmpres[1]==1&&tmpres[2]==1)
					fv.factors[4]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[0]==1&&tmpres[2]==2)
					fv.factors[5]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[1]==3)
					fv.factors[6]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[1]==2&&tmpres[2]==1)
					fv.factors[7]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[1]==1&&tmpres[2]==2)
					fv.factors[8]+=v1[i1].factor*v2[i2].factor;
				if(tmpres[2]==3)
					fv.factors[9]+=v1[i1].factor*v2[i2].factor;
				*/
				for(int ra=0;ra<(n+2)*(n+1)/2;ra++)
				{
					if((tmpres[0]==Jfactors[ra][0])&&(tmpres[1]==Jfactors[ra][1])&&(tmpres[2]==Jfactors[ra][2]))
					{
						fv.factors[ra]+=v1[i1].factor*v2[i2].factor;
					}
				}
			}
		}
	}
	return fv;
}

//////////////////////////////////////////////////////////////////////////
//03.02.2014
int find_last_group(vector<state> &cur,int start_n)//возвращает номер последнего элемента равного заданному, работает для отсортированных массивов
///Можно улучшить с помощью бинарного поиска
{
	int last=start_n;
	for(int i=start_n+1;i<cur.size();i++)
	{
		if(cur[start_n]==cur[i])
		{
			last=i;
		}
		else
		{
			break;
		}
	}

	return last;

}

res finalvalue4(vector<state> &v1,vector<state> &v2,int **Jfactors,int n)
{
	res fv;
	for(int i=0;i<(n+2)*(n+1)/2;i++)
		fv.factors[i]=0;
	unsigned int i1,i2;
	int tmpres[3];
	i2=0;// указывает текущую группу
	i1=0;// указывает текущую группу
	int last1,last2;

	while((i2<v2.size())&&(i1<v1.size()))
	{

		if(v2[i2]==v1[i1])//организуем суммирование
		{
			last1=find_last_group(v1,i1);
			last2=find_last_group(v2,i2);
			for(int i=i1;i<=last1;i++)
			{
				for(int j=i2;j<=last2;j++)
				{

					//Start test #1
					//////////////////////////////////////////////////////////////////////////
					/*ofstream outtestFin("outfin4.txt",ios::app);
					outtestFin<<i<<" "<<j<<"\n";
					outtestFin.close();*/
					//////////////////////////////////////////////////////////////////////////
					//End test #1
					//////////////////////////////////////////////////////////////////////////
					for(int ttt=0;ttt<3;ttt++)
					{
						tmpres[ttt]=0;
						tmpres[ttt]=v1[i].coeff[ttt]+v2[j].coeff[ttt];
					}

					for(int ra=0;ra<(n+2)*(n+1)/2;ra++)
					{
						if((tmpres[0]==Jfactors[ra][0])&&(tmpres[1]==Jfactors[ra][1])&&(tmpres[2]==Jfactors[ra][2]))
						{
							fv.factors[ra]+=v1[i].factor*v2[j].factor;
						}
					}

					//////////////////////////////////////////////////////////////////////////
				}
			}
			i1=last1+1;
			i2=last2+1;
		}
		else if(v2[i2]<v1[i1])
		{
			i2=1+find_last_group(v2,i2);//перешли к следующей группе
		}
		else
		{
			i1=1+find_last_group(v1,i1);
		}
	}
	return fv;
}



bool check_cur_operator_set(bool &Res,int OrderLength, int RealLength,int *termorder,int *op_set,vector<edge> edges)//проверяем можкт ли быть не 0 по данной конфигурации
{
	int start=0;
	int end;
	int mask[10];
	Res=true;
	bool last=false;
	bool if_find;
	vector<step> nodes;
	step cur_node(0,0);
	for(int i=0;i<OrderLength;i++)
	{
		if(i==OrderLength-1)
		{
			end=i;
			last=true;
		}
		if((termorder[i]==0)||(last==true))
		{
			end=i;//нашли группу
			for(int j=0;j<10;j++)
				mask[j]=0;
			for(int j=start;j<=end;j++)
			{
				if((unsigned int)op_set[j]<edges.size())//нашли ребро
				{
					cur_node.sx=edges[op_set[j]].x1; //выбираем его первую вершину
					cur_node.sy=edges[op_set[j]].y1;
					if_find=false;
					for(unsigned int k=0;k<nodes.size();k++)//ищем ее
					{
						if(nodes[k]==cur_node)//если нашли увеличиваем ее кол-во
						{
							mask[k]++;
							if_find=true;
						}
					}
					if(!if_find)//если не нашли добавляем
					{
						nodes.push_back(cur_node);
						mask[nodes.size()-1]=1;
					}

					cur_node.sx=edges[op_set[j]].x2;//выбираем его 2ую вершину
					cur_node.sy=edges[op_set[j]].y2;
					if_find=false;
					for(unsigned int k=0;k<nodes.size();k++)//ищем ее
					{
						if(nodes[k]==cur_node)//если нашли увеличиваем ее кол-во
						{
							mask[k]++;
							if_find=true;
						}
					}
					if(!if_find)//если не нашли добавляем
					{
						nodes.push_back(cur_node);
						mask[nodes.size()-1]=1;
					}
				}
			}

			for(int j=0;j<OrderLength;j++)
			{
				if(mask[j]==1)
				{
					Res=false;
					break;
				}
			}

			if(!Res)
				break;
			start=i+1;
		}
		if(!Res)
			break;
	}
	return Res;
}

void read_Route( int r[][2],istringstream &s)
{
	char c=' ';
	while(c!='n')
	{
		s>>c;
	}
	s>>c;//пропускаем 1 символ
	int num=0;
	while(N*2-1>=num)
	{

		s>>r[num][0];
		s>>c;
		s>>r[num][1];
		num++;

		s>>c;
		s>>c;
		s>>c;//пропускаем 3 символа
		s>>r[num][0];
		s>>c;
		s>>r[num][1];
		num++;
		s>>c;
		s>>c;
	}

}


void  clear_res_Matrix(res **ans,int size)
{
	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++)
			for(int k=0;k<resAmount;k++)
				ans[i][j].factors[k]=0;
}

void add_res_Matrix(res **ans,res **cur,int size)
{
	for(int i=0;i<size;i++)
		for(int j=i;j<size;j++)
			for(int k=0;k<resAmount;k++)
				ans[i][j].factors[k]+=cur[i][j].factors[k];
}

//void res_Extend(res **ans,int size) //дозаполняем матрицу до симметричной
//{
//	for(int i=0;i<size;i++)
//		for(int j=i+1;j<size;j++)
//			for(int k=0;k<resAmount;k++)
//				ans[j][i].factors[k]=ans[i][j].factors[k];
//}


//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	int startRouteNum,finRouteNum;
	ifstream inproutesNums("input.txt",ios::in);
	//запуск нескольких маршрутов
	inproutesNums>>startRouteNum>>finRouteNum;

	/*
	//запуск ровно 1 маршрута
	inproutesNums>>startRouteNum;
	finRouteNum=startRouteNum;
	*/
	inproutesNums.close();

	///Проверка на совпадение размеров
	if(3*(N)*(int)pow((double)2,N-1)>Namount)
	{

		logfile<<"Change Namount";
        logfile.close();
		return 1;
	}

	stringstream fname;
	string fstring;
	stringstream sscanner;
	
	ifstream config("config.txt",ios::in);//хранит текущий порядок и кол-во маршрутов всех длин, начиная с 2
	ifstream terms;//слагаемые ряда теории возмущений
	ifstream operatorsset;//ненулевые наборы операторов
	ofstream matrixRes;  //выходные файлы




	int Q; //для тестов
	int type,Order, subOrder; //Тип маршрутов
	int routesAmount [3][N+1];
	vector<int*> nodeSets;
	int *termOrder;
	int edge_num;//число ребер в маршруте
	int node_num;//число вершин в маршруте
	int r[20][2]; //2*10 - на 10 ребер с запасом

	int **Jfactors; //храним возможные наборы J-факторов
	string *strarr;

	//Start Test variables
	//long start,end;
	string out_string;
	string str_type;
	//End Test Var


	getline(config,out_string);//Считываем заголовок - он не важен
	config>>type>>Order>>subOrder;
	switch(type)
	{
        case 0: str_type=type1; break;
        case 1: str_type=type2; break;
        case 2: str_type=type3; break;
        default: logfile<<"Type error!!"; logfile.close(); return 2;

	}
	//Если NotLoops и subOrder==Order считываем good file
	bool goodNotLoopCase=false;
	vector<int> goodNotLoopNums;
	/*if((type==2)&&(subOrder==Order))
	{
		goodNotLoopCase=true;
		sscanner<<inp_route<<delim<<Order<<"_good_nl.txt";
		string s=sscanner.str();
		ifstream in(s.c_str(),ios::in);
		int cur;
		while(!in.eof())
		{
			in>>cur;
			goodNotLoopNums.push_back(cur);
		}
	}*/
	getline(config,out_string);//Считываем заголовок - он не важен
	getline(config,out_string);//Считываем заголовок - он не важен

	for(int i=2;i<=N;i++)
	{
        config>>Q>>routesAmount[0][i]>>routesAmount[1][i]>>routesAmount[2][i];
	}
	//////////////////////////////////////////////////////////////////////////
	//Test input data


	if(Order>N)
	{
		logfile<<"\nWrong Const or Config file\n";
		logfile.close();
		cout<<"!!!!!!!!!!!!!!!!!!";
		cin>>Q;
		return 3;
	}

	//End Test input data
	//////////////////////////////////////////////////////////////////////////


	Jfactors=new int*[(Order+2)*(Order+1)/2];
	strarr=new string[(Order+2)*(Order+1)/2];
	for(int i=0;i<(Order+2)*(Order+1)/2;i++)
	{
		Jfactors[i]=new int[3];
	}
	generate_all_Jfactors(Order,Jfactors);
	generate_all_Jstrings(Order,Jfactors,strarr);

	//test out
	/*
	for(int i=0;i<(n+1)*(n+2)/2;i++)
		cout<<strarr[i]<<"\n";

	for(int h=0;h<(n+2)*(n+1)/2;h++)
	{
		cout<<Jfactors[h][0]<<" "<<Jfactors[h][1]<<" "<<Jfactors[h][2]<<"\n";
	}*/

	sscanner.str("");
	sscanner<<inp_fin<<delim<<Order<<"order.txt\0";
	string s=sscanner.str();
	//test out
	//cout<<s<<"\n";
	terms.open(s.c_str(),ios::in);

	//считываем все слагаемые для данного порядка
	while(!terms.eof())
	{
		getline(terms,s);
		sscanner.str(s);
		if(s.length()>0)
		{
			termOrder=new int[Order-1];
			for(int i=0;i<Order-1;i++)
			{
				sscanner>>termOrder[i];
			}
			nodeSets.push_back(termOrder);
		}

	}
	int *cur_operator_set;
	bool result;

//Чтение Матриц
	//чтение матриц из файлов
	ifstream in;
	s="";
	for( int i=0;i<4;i++)
	{
		ostringstream tmp;
		tmp<<(i+1);
		s=inp_matr+"noutp_s"+tmp.str()+".txt"; //Sp
		in.open(s.c_str(),ios::in);
		for(int j=0;j<16;j++)
		{
			for(int k=0;k<16;k++)
			{
				in>>Vmatrix[i][0][j][k];
			}
		}
		in.close();

		s=inp_matr+"noutm_s"+tmp.str()+".txt"; //Sm
		in.open(s.c_str(),ios::in);
		for(int j=0;j<16;j++)
		{
			for(int k=0;k<16;k++)
			{
				in>>Vmatrix[i][1][j][k];
			}
		}
		in.close();

		s=inp_matr+"noutz_s"+tmp.str()+".txt"; //z=> второй индекс 2
		in.open(s.c_str(),ios::in);
		for(int j=0;j<16;j++)
		{
			for(int k=0;k<16;k++)
			{
				in>>Vmatrix[i][2][j][k];
			}
		}
		in.close();
	}

	in.open((inp_matr+"inside_matr.txt").c_str(),ios::in);
	for(int i=0;i<16;i++)
		for(int j=0;j<16;j++)
		{
			in>>VmatrixInside[i][j];
		}
	in.close();

	in.open((inp_matr+"energy.txt").c_str(),ios::in);
	for(int i=0;i<16;i++)
	{
		in>>Energie[i];
	}
	//Конец Чтения Матриц

	//Задаем порядок спинов
	state init;
	vector<state> *vIn;
	int vec_amount = 3 * (Order + 1)*(int)pow((double)2, Order);
	vIn = new vector<state>[vec_amount];


	sscanner.str("");
	sscanner<<"spins"<<delim<<Order<<"spins_order.txt";
	ifstream inStates((sscanner.str()).c_str(),ios::in);
	
	init.coeff[0]=init.coeff[1]=init.coeff[2]=0;
	init.factor=1;
	int cur_st;
	for(int i=0;i<vec_amount;i++)
	{
		init.states.clear();
		for(int j=0;j<Order;j++) ///TODO N->n
		{
			inStates >> cur_st;
			init.states.push_back(cur_st);
		}
		vIn[i].push_back(init);
	}

	//test
	//ofstream ccout("ttt.txt",ios::out);
	//end test
	//

	//Инициализируем матрицы результатов
	MatrixFull=new res*[vec_amount];
	for(int i=0;i<vec_amount;i++)
		MatrixFull[i]=new res[vec_amount];
	clear_res_Matrix(MatrixFull,vec_amount);
	//Конец инициализации;



//	clock_t t1,t2;
	cur_operator_set=new int[Order];
	int *procedure_order=new int[Order];
	int *power_order=new int[Order];
	int real_size;

	for(int i=subOrder;i<=subOrder;i++)//Перебираем все возможные длины маршрутов
	{

		for(int j=1;j<=routesAmount[type][subOrder];j++)//перебираем все доступные маршруты при данной длине
		{

			//Блок управления различными копиями, позволяет запускать только часть маршрутов
			if(j>finRouteNum)//если все вычислили, то выходим.
				exit(0);
			if(j<startRouteNum)
				continue;//пропускаем если не подходит по заданным номерам
			//Конец Блока управления различными копиями

			//Блок управления для случая NotLoop subOrder==Order
			if((goodNotLoopCase)&&(find(goodNotLoopNums.begin(),goodNotLoopNums.end(),j)==goodNotLoopNums.end()))
			{
				//continue;
			}

			clear_res_Matrix(MatrixFull,vec_amount);//Очищаем матрицы результатов для нового маршрута

			//ccout<<"j="<<j<<"\n";
			sscanner.str("");
			sscanner<<inp_route<<Order<<"_"<<str_type<<delim<<Order<<"_"<<i<<"_"<<j<<"_routeNum_"<<str_type<<".txt";
			s=sscanner.str();
			operatorsset.open(s.c_str(),ios::in);
			getline(operatorsset,s);
			istringstream route;
			route.str(s);
			
			read_Route(r,route);
			eval_cur_route(r,Order,i,edges,nodes,edge_num);

			for(int ll=0;ll<edge_num;ll++)
			{
				returnV(curInter,interAmount,ll,node_nums_of_edges[ll][0],node_nums_of_edges[ll][1],edges[ll].x2-edges[ll].x1,edges[ll].y2-edges[ll].y1);
			}
			operatorsset>>edge_num>>node_num;

			//заполняем спины каждый раз
			sscanner.str("");
			sscanner << "spins" << delim << node_num << "spins_order.txt";
			ifstream inStates((sscanner.str()).c_str(), ios::in);

			init.coeff[0] = init.coeff[1] = init.coeff[2] = 0;
			init.factor = 1;
			int cur_st;
			for (int i = 0; i<vec_amount; i++)
			{
				vIn[i].clear();
				init.states.clear();
				for (int j = 0; j<node_num; j++) ///TODO N->n
				{
					inStates >> cur_st; 
					init.states.push_back(cur_st);
				}
				vIn[i].push_back(init);
			}
			inStates.close();
			//конец заполнения спинов



			//Проверка на ошибку ребер
			if(edges.size()!=edge_num)
			{
				logfile<<"Edges wrong. n="<<Order<<" i="<<i<<" j="<<j<<" "<<str_type;
				logfile.close();
				return 4;
			}
			//Конец проверки на ошибку в ребрах



			getline(operatorsset,s);
			real_size=3*node_num*(int)pow((double)2,node_num-1);
			int zz=0;
			while(!operatorsset.eof())
			{


				getline(operatorsset,s);

				if(s.length()>0)
				{

					cout<<Order<<" "<<i<<" "<<j<<" zz="<<zz<<"\n";
					//test
					//ccout<<n<<" "<<i<<" "<<j<<" zz="<<zz<<" ";
					//ccout<<s<<" ";
					//end test
					zz++;

					sscanner.str(s);
					for(int k=0;k<Order;k++)
					{
						sscanner>>cur_operator_set[k];//считываем n операторов вдоль маршрута
					}

					for(unsigned int k=0;k<nodeSets.size();k++)
					{
						//Пока вычисляем все конфинурации, нужно будет написать новый чек если будут вычисления в старших порядках
						//check_cur_operator_set(result,Order,edge_num,nodeSets[k],cur_operator_set,edges);
						//if(result)//вычисляем кофигурацию
						{
							/*if(k==nodeSets.size()-1)
								t1=clock();*/
							for(int ll=0;ll<real_size;ll++)//очищаем выходные данные
							{
								vOut1[ll].clear();
								vOut2[ll].clear();
							}
							generate_procedure_order(nodeSets[k],cur_operator_set,edge_num,Order,procedure_order,power_order);


							for(int ll=0;ll<real_size;ll++)//вычисляем хвосты. начало
							{
								ref1=&vIn[ll];
								ref2=&vOutTemp1;
								for(int mm=0;mm<(Order+1)/2;mm++)
								{
									if(mm==(Order+1)/2-1)//если остался последний шаг
										ref2=&vOut1[ll];
									switch(procedure_order[mm])//выбираем процедуру
									{
										case 2:
											act_ground(*ref1,*ref2,Vmatrix,cur_operator_set[mm],node_num);
											break;
										case 3:
											act_energy(*ref1,*ref2,Vmatrix,cur_operator_set[mm], node_num);
											break;
										case 4:
											act_energy_power(*ref1,*ref2,power_order[mm],Vmatrix,cur_operator_set[mm], node_num);
											break;
										case 6:
											act_inside_enrgy_power(*ref1,*ref2,power_order[mm],cur_operator_set[mm]-edge_num, node_num);
											break;
										case 7:
											act_inside_ground(*ref1,*ref2,cur_operator_set[mm]-edge_num, node_num);
											break;
									}

									if(ref2==&vOutTemp1)
									{
										ref2=&vOutTemp2;
										ref1=&vOutTemp1;
										vOutTemp2.clear();
									}
									else
									if(ref2==&vOutTemp2)
									{
										ref2=&vOutTemp1;
										ref1=&vOutTemp2;
										vOutTemp1.clear();
									}
								}

							}


							///Вычисляем замыкающие хвосты

							for(int ll=0;ll<real_size;ll++)
							{
								ref1=&vIn[ll];
								ref2=&vOutTemp1;
								for(int mm=Order-1;mm>Order-1-Order/2;mm--)
								{
									if(mm==Order-Order/2)//если остался последний шаг
										ref2=&vOut2[ll];
									switch(procedure_order[mm])//выбираем процедуру
									{
									case 1:
										act(*ref1,*ref2,Vmatrix,cur_operator_set[mm], node_num);
										break;
									case 2:
										act_ground(*ref1,*ref2,Vmatrix,cur_operator_set[mm], node_num);
										break;
									case 3:
										act_energy(*ref1,*ref2,Vmatrix,cur_operator_set[mm], node_num);
										break;
									case 4:
										act_energy_power(*ref1,*ref2,power_order[mm],Vmatrix,cur_operator_set[mm], node_num);
										break;
									case 5:
										act_inside(*ref1,*ref2,cur_operator_set[mm]-edge_num, node_num);
										break;
									case 6:
										act_inside_enrgy_power(*ref1,*ref2,power_order[mm],cur_operator_set[mm]-edge_num, node_num);
										break;
									case 7:
										act_inside_ground(*ref1,*ref2,cur_operator_set[mm]-edge_num, node_num);
										break;
									}

									if(ref2==&vOutTemp1)
									{
										ref2=&vOutTemp2;
										ref1=&vOutTemp1;
										vOutTemp2.clear();
									}else
									if(ref2==&vOutTemp2)
									{
										ref2=&vOutTemp1;
										ref1=&vOutTemp2;
										vOutTemp1.clear();
									}
								}
								//logfile<<"curlen="<<i<<" curn="<<j<<" zz="<<zz<<" k="<<k<<" x="<<ll<<" size1="<<vOut1[ll].size()<<" size2="<<vOut2[ll].size()<<"\n";
							}


							//start=clock();
							/*ofstream outtestFin;
							if(function_flag==4)
							{
								outtestFin.open("outfin4.txt",ios::out);
							}
							else
							{
								outtestFin.open("outfin3.txt",ios::out);
							}
							outtestFin.close();*/
							for(int x=0;x<real_size;x++)
							{
								for(int y=0; y<real_size;y++)
								{
									//Start Test code #1
									res tmpres;

									/*if(function_flag==3)
									{
										ofstream outtestFin("outfin3.txt",ios::app);
										outtestFin<<"N "<<x<<" "<<y<<"\n";
										outtestFin.close();
										tmpres=finalvalue3(vOut1[x],vOut2[y],Jfactors,n);
										out_string="Test3-Results\\";
									}
									else if(function_flag==4)*/
									{
										//Start debug code #12
										//ofstream outtestFin("outfin4.txt",ios::app);
										//outtestFin<<"N "<<x<<" "<<y<<"\n";
										//outtestFin.close();
										//End debug code #12

										tmpres=finalvalue4(vOut1[x],vOut2[y],Jfactors,Order);
										//out_string="Test4-Results\\";
										out_string=out_res+str_type+delim;

									}
									//End Test code #1




									//Test for symmetry
									//////////////////////////////////////////////////////////////////////////
									//res tmpres2=finalvalue3(vOut1[y],vOut2[x],Jfactors,n);
									//////////////////////////////////////////////////////////////////////////
									//End Test for symmetry
									//Matrix[k][x][y]+=tmpres;
									if(minus1(nodeSets[k],Order)==-1)
										tmpres.minus();
									MatrixFull[x][y]+=tmpres;
								}
							}
							//end=clock();
							//cout<<"\n\nTest time="<<(end-start)*1.0<<"\n\n";
							//if(k==nodeSets.size()-1)
								//ccout<<float(clock()-t1)/CLOCKS_PER_SEC<<"\n";
						}
					}


					//записываем ответ в файлы - отдельно для каждого слагаемого
					/*for(unsigned int k=0;k<nodeSets.size();k++)
					{
						int minus_factor=minus1(nodeSets[k],n);
						res_Extend(Matrix[k],real_size);
						sscanner.str("");
						sscanner<<n<<"ord_"<<i<<"realLen_"<<j<<"RouteNum_"<<k<<"termNum.txt";
						matrixRes.open(sscanner.str(),ios::out);
						matrixRes<<"{";
						for(int ii=0;ii<vec_amount;ii++)
						{
							matrixRes<<"{";
							for(int jj=0;jj<vec_amount;jj++)
							{
								for(int kk=0;kk<(n+2)*(n+1)/2;kk++)
								{

									if(abs(Matrix[k][ii][jj].factors[kk])>0.0000000000001)
										matrixRes<<Matrix[k][ii][jj].factors[kk]*minus_factor<<"*"<<strarr[kk]<<"+";
								}
								if(jj<vec_amount-1)
									matrixRes<<"0,";
								else
									matrixRes<<"0";
							}
							if(ii<vec_amount-1)
								matrixRes<<"},\n";
							else
								matrixRes<<"}\n";

						}
						matrixRes<<"}";
						matrixRes.close();
					}*/
				}
			}
			//записываем ответ в файл - итог для маршрута
			//res_Extend(MatrixFull,real_size);
			sscanner.str("");
			sscanner<<out_string<<Order<<"_"<<i<<"_"<<j<<"_res_"<<str_type<<".txt";
			matrixRes.open((sscanner.str()).c_str(),ios::out);
			matrixRes<<real_size<<"\n{";
			matrixRes.setf(ios::fixed);
			matrixRes<<setprecision(10);

			for(int ii=0;ii<real_size;ii++)
			{
				matrixRes<<"{";
				for(int jj=0;jj<real_size;jj++)
				{
					for(int kk=0;kk<(Order+2)*(Order+1)/2;kk++)
					{

						if(abs(MatrixFull[ii][jj].factors[kk])>0.0000000000001)
							matrixRes<<MatrixFull[ii][jj].factors[kk]<<"*"<<strarr[kk]<<"+";
					}
					if(jj<real_size-1)
						matrixRes<<"0,";
					else
						matrixRes<<"0";
				}
				if(ii<real_size-1)
					matrixRes<<"},\n";
				else
					matrixRes<<"}\n";

			}
			matrixRes<<"}";
			matrixRes.close();

			operatorsset.close();
		}
	}
	delete[] cur_operator_set;

	logfile.close();

	return 0;
}

