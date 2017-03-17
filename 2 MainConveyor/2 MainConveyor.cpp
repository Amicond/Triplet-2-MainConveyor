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
#include "JFactors.h"
#include "MatrixOfResults.h"
#include "MyFunctions.h"



std::ofstream logfile("log.txt", std::ios::out);


std::vector<edge> edges;//ребра текущего маршрута
std::vector<step> nodes;//вершины текущего маршрута

std::vector<state> *ref1,*ref2;
int node_nums_of_edges[N][2];

int minus1(int *nodeSet,int n) //вычисляем знак для текущего слагаемого в рду теории возмущения
{
	if (n == 1) return 1;//для первого порядка
	int res=1;
	for(int i=0;i<n-1;i++)
	{
		if(nodeSet[i]==0)
			res*=-1;
	}
	return res;
}

/*res **MatrixFull;*///Суммарная матрица на маршрут

void eval_cur_route(int r[][2],int OrderLength, int RouteLength, std::vector<edge> &edges, std::vector<step> &nodes,int &RealLength)
	//заполняет для данного маршрута список ребер и их количество
{
	for(int i=0;i<N;i++)
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


//03.02.2014
int find_last_group(std::vector<state> &cur,int start_n)//возвращает номер последнего элемента равного заданному, работает для отсортированных массивов
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

res finalvalue4(std::vector<state> &v1, std::vector<state> &v2,int n)
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
					//складываем степени J-множителей
					for(int ttt=0;ttt<JFactors::getAmountOfPowers();ttt++)
					{
						tmpres[ttt]=v1[i].coeff[ttt]+v2[j].coeff[ttt];
					}

					//преобразуем три степени в номер ячейки массива и заполняем ее нужным фактором
					fv.factors[JFactors::getNumberByPowers(tmpres)]+=v1[i].factor*v2[j].factor;
					
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

bool check_cur_operator_set(bool &Res,int OrderLength, int RealLength,int *termorder,int *op_set, std::vector<edge> edges)//проверяем можкт ли быть не 0 по данной конфигурации
{
	int start=0;
	int end;
	int mask[10];
	Res=true;
	bool last=false;
	bool if_find;
	std::vector<step> nodes;
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

void read_Route( int r[][2], std::istringstream &s)
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
	std::ifstream inproutesNums("input.txt", std::ios::in);
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

	std::stringstream fname;
	std::string fstring;
	std::stringstream sscanner;
	
	std::ifstream config("config.txt", std::ios::in);//хранит текущий порядок и кол-во маршрутов всех длин, начиная с 2
	std::ifstream terms;//слагаемые ряда теории возмущений
	std::ifstream operatorsset;//ненулевые наборы операторов
	std::ofstream matrixRes;  //выходные файлы




	int Q; //для тестов
	int type,Order, subOrder; //Тип маршрутов
	int routesAmount [3][N+1];
	std::vector<int*> nodeSets;
	int *termOrder;
	int edge_num;//число ребер в маршруте
	int node_num;//число вершин в маршруте
	int r[20][2]; //2*10 - на 10 ребер с запасом

	//Start Test variables
	//long start,end;
	std::string out_string;
	std::string str_type;
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
	std::vector<int> goodNotLoopNums;
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

	for(int i=1;i<=N;i++)
	{
        config>>Q>>routesAmount[0][i]>>routesAmount[1][i]>>routesAmount[2][i];
	}
	//////////////////////////////////////////////////////////////////////////
	//Test input data


	if(Order>N)
	{
		logfile<<"\nWrong Const or Config file\n";
		logfile.close();
		std::cout<<"!!!!!!!!!!!!!!!!!!";
		std::cin>>Q;
		return 3;
	}

	//End Test input data
	//////////////////////////////////////////////////////////////////////////
	

	JFactors::setOrder(Order);
	

	sscanner.str("");
	sscanner<<inp_fin<<delim<<Order<<"order.txt\0";
	std::string s=sscanner.str();
	
	terms.open(s.c_str(), std::ios::in);

	//считываем все слагаемые для данного порядка
	//отдельно для первого порядка
	if (Order == 1)
	{
		termOrder = new int[1];
		termOrder[0] = -1;
		nodeSets.push_back(termOrder);
	}
	else//для всех остальных
	{
		while (!terms.eof())
		{
			getline(terms, s);
			sscanner.str(s);
			if (s.length() > 0)
			{
				termOrder = new int[Order - 1];
				for (int i = 0; i < Order - 1; i++)
				{
					sscanner >> termOrder[i];
				}
				nodeSets.push_back(termOrder);
			}
		}
	}
	int *cur_operator_set;
	bool result;

//Чтение Матриц
	//чтение матриц из файлов
	std::ifstream in;
	s="";
	for( int i=0;i<4;i++)
	{
		std::ostringstream tmp;
		tmp<<(i+1);
		s=inp_matr+"noutp_s"+tmp.str()+".txt"; //Sp
		in.open(s.c_str(), std::ios::in);
		for(int j=0;j<DiffStates;j++)
		{
			for(int k=0;k<DiffStates;k++)
			{
				in>>Vmatrix[i][0][j][k];
			}
		}
		in.close();

		s=inp_matr+"noutm_s"+tmp.str()+".txt"; //Sm
		in.open(s.c_str(), std::ios::in);
		for(int j=0;j<DiffStates;j++)
		{
			for(int k=0;k<DiffStates;k++)
			{
				in>>Vmatrix[i][1][j][k];
			}
		}
		in.close();

		s=inp_matr+"noutz_s"+tmp.str()+".txt"; //z=> второй индекс 2
		in.open(s.c_str(), std::ios::in);
		for(int j=0;j<DiffStates;j++)
		{
			for(int k=0;k<DiffStates;k++)
			{
				in>>Vmatrix[i][2][j][k];
			}
		}
		in.close();
	}

	in.open((inp_matr+"inside_matr.txt").c_str(), std::ios::in);
	for(int i=0;i<DiffStates;i++)
		for(int j=0;j<DiffStates;j++)
		{
			in>>VmatrixInside[i][j];
		}
	in.close();

	in.open((inp_matr+"energy.txt").c_str(), std::ios::in);
	for(int i=0;i<DiffStates;i++)
	{
		in>>Energie[i];
	}
	//Конец Чтения Матриц

	//Создаем массив входных состоянний
	state init;//в будущем для четния
	std::vector<state> *vIn;
	int vec_amount = 3 * (Order + 1)*(int)pow((double)2, Order);
	vIn = new std::vector<state>[vec_amount];


	//Инициализируем матрицы результатов
	/*MatrixFull=new res*[vec_amount];
	for(int i=0;i<vec_amount;i++)
		MatrixFull[i]=new res[vec_amount];
	clear_res_Matrix(MatrixFull,vec_amount);*/
	MatrixOfResults fullMatrix;
	fullMatrix.clearAndSetSize(vec_amount);
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

			

			
			
			sscanner.str("");
			sscanner<<inp_route<<Order<<"_"<<str_type<<delim<<Order<<"_"<<i<<"_"<<j<<"_routeNum_"<<str_type<<".txt";
			s=sscanner.str();
			operatorsset.open(s.c_str(), std::ios::in);
			getline(operatorsset,s);
			std::istringstream route;
			route.str(s);
			
			operatorsset >> edge_num >> node_num;
			//если это не случай внутреннего оператора, то считываем ребра
			if (edge_num > 0)
			{
				read_Route(r, route);
				eval_cur_route(r, Order, i, edges, nodes, edge_num);

				for (int ll = 0; ll < edge_num; ll++)
				{
					returnV(curInter, interAmount, ll, node_nums_of_edges[ll][0], node_nums_of_edges[ll][1], edges[ll].x2 - edges[ll].x1, edges[ll].y2 - edges[ll].y1);
				}
			}

			//заполняем спины каждый раз
			sscanner.str("");
			sscanner << "spins" << delim << node_num << "spins_order.txt";
			std::ifstream inStates((sscanner.str()).c_str(), std::ios::in);

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
			
			//clear_res_Matrix(MatrixFull,vec_amount);//Очищаем матрицы результатов для нового маршрута
			fullMatrix.clearAndSetSize(real_size);
			
			int zz=0;		
			while(!operatorsset.eof())
			{
				getline(operatorsset,s);

				if(s.length()>0)
				{

					std::cout<<Order<<" "<<i<<" "<<j<<" zz="<<zz<<"\n";

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
										case 1:
											act(*ref1, *ref2, Vmatrix, cur_operator_set[mm], node_num);
											break;
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
								//для случая первого порядка просто копируем
								if (Order == 1)
								{
									act_copy(vIn[ll], vOut2[ll]);
								}
								//для всех остальных порядков
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
							}

							for(int x=0;x<real_size;x++)
							{
								for(int y=0; y<real_size;y++)
								{
									res tmpres;
									
									tmpres=finalvalue4(vOut1[x],vOut2[y],Order);

									if(minus1(nodeSets[k],Order)==-1)
										tmpres.minus();
									fullMatrix.add(x, y, tmpres);
								}
							}
						}
					}
				}
			}
			
			
			//имя выходного файла
			sscanner << out_res + str_type + delim << Order << "_" << i << "_" << j << "_res_" << str_type << ".txt" << "\0";
			//печать текущей матрицы
			fullMatrix.printMatrix(sscanner.str(),Order);
			
			operatorsset.close();
		}
	}
	delete[] cur_operator_set;

	logfile.close();

	return 0;
}

