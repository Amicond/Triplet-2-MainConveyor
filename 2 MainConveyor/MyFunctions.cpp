#include "stdafx.h"
#include "MyFunctions.h"

//////////////////////////////////////////////////////////////////////////
double Vmatrix[4][3][DiffStates][DiffStates];//����� ������� � �������, ��� ������� SP,SM ��� SZ,����� ����, ����� �������
double VmatrixInside[DiffStates][DiffStates];//���������� ��� ���������� �����������
double Energie[DiffStates]; //������� ���������
inter curInter [N][maxIntElem]; //������ ���������� �������������
int interAmount[N];// ���-�� ��-��� � ������ ���������

std::vector<state> vIn[Namount],vOut1[Namount],vOut2[Namount]; //������� ��������-���������� �� ������� ���� ��� ������� ������ ��� ��������, �� ��������� ���� - ��������
std::vector<state> vOutTemp1,vOutTemp2;//������������� �������
std::vector<state> vtemp;
std::vector<state> temp,temp2,tempvec,tempvec2;
state init;
//////////////////////////////////////////////////////////////////////////

double getE0(int node_num){
	return Energie[0] * (node_num -1)+Energie[2];//
}

//����������� 0- (0,0); ����� ������ ������� �������
void returnV(inter curInter[][maxIntElem],int interAmount[],int interN,int n1,int n2,int dx,int dy) //n1,n2 - ���������� ������ ��������������� �������, dir -����������� �� ������� ������� �� �������
{

	/*switch(dir)
	{
	case 0: dx=1;   dy=0;  break;
	case 1: dx=1;   dy=1;  break;
	case 2: dx=0;   dy=1;  break;
	case 3: dx=-1;  dy=1;  break;
	case 4: dx=-1;  dy=0;  break;
	case 5: dx=-1;  dy=-1; break;
	case 6: dx=0;   dy=-1; break;
	case 7: dx=1;   dy=-1; break;
	}*/
	int q=-1;
	if((dx==1)&&(dy==0))// �� ����������� �������
	{
		q++;
		curInter[interN][q].Jtype=0;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=1;  curInter[interN][q].v2=0;

		q++;
		curInter[interN][q].Jtype=0;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=2;  curInter[interN][q].v2=3;

		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=1;  curInter[interN][q].v2=3;

		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=2;  curInter[interN][q].v2=0;

	}
	if((dx==1)&&(dy==1)) //�� ��������� ������ �����
	{
		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=1;  curInter[interN][q].v2=3;
	}

	if((dx==0)&&(dy==1))//������ �����
	{
		q++;
		curInter[interN][q].Jtype=0;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=0;  curInter[interN][q].v2=3;

		q++;
		curInter[interN][q].Jtype=0;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=1;  curInter[interN][q].v2=2;

		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=0;  curInter[interN][q].v2=2;

		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=1;  curInter[interN][q].v2=3;
	}
	if((dx==-1)&&(dy==1)) //�� ��������� ����� �����
	{
		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=0;  curInter[interN][q].v2=2;
	}//��
	if((dx==-1)&&(dy==0))//������ �����
	{
		q++;
		curInter[interN][q].Jtype=0;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=0;  curInter[interN][q].v2=1;

		q++;
		curInter[interN][q].Jtype=0;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=3;  curInter[interN][q].v2=2;

		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=0;  curInter[interN][q].v2=2;

		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=3;  curInter[interN][q].v2=1;
	}//��
	if((dx==-1)&&(dy==-1)) //�� ��������� ����� ����
	{
		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=3;  curInter[interN][q].v2=1;
	}//��
	if((dx==0)&&(dy==-1))//������ ����
	{
		q++;
		curInter[interN][q].Jtype=0;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=2;  curInter[interN][q].v2=1;

		q++;
		curInter[interN][q].Jtype=0;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=3;  curInter[interN][q].v2=0;

		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=2;  curInter[interN][q].v2=0;

		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=3;  curInter[interN][q].v2=1;
	}//��

	if((dx==1)&&(dy==-1)) //�� ��������� ������ ����
	{
		q++;
		curInter[interN][q].Jtype=1;
		curInter[interN][q].n1=n1; curInter[interN][q].n2=n2;
		curInter[interN][q].v1=2;  curInter[interN][q].v2=0;
	}//��
	interAmount[interN]=q+1;

}

void collect(std::vector<state> &outvec, std::vector<state> &invec)
{
	int index=0;
	sort(invec.begin(),invec.end());
	outvec.push_back(invec[0]);
	for(unsigned int i=1;i<invec.size();i++)
	{
		if(invec[i].check(outvec[index]))
		{
			outvec[index].factor+=invec[i].factor;
		}
		else
		{
			if(abs(outvec[index].factor)<=0.000000000000001)
			{
				outvec.pop_back();
				--index;
			}
			++index;
			outvec.push_back(invec[i]);
		}
	}
}



/* #0 */void act_copy(std::vector<state> &inv, std::vector<state> &outv)//������ �������� ���������, ����� ��� ����� ������������� ����� � ������ �������
{
	for (auto a : inv)
	{
		outv.push_back(a);
	}
}

/* #1 */void act(std::vector<state> &inv, std::vector<state> &outv,double Vmatrix[4][3][DiffStates][DiffStates], int interNumber, int node_num)
{
	state tempst;
	int second_ort;
	double curE,E0;

	//initialisation
	E0=getE0(node_num);
	temp2.clear();
	for (int i = 0; i < node_num; i++)
		tempst.states.push_back(DiffStates); //DiffStates- ������ ���������, ������� �� ������������ � ��������
	

	//computation
	for(unsigned int inSt=0;inSt<inv.size();inSt++)
	{
		for(int i=0;i<interAmount[interNumber];i++) //���������� ��� ��-�� ��������������
		{
			//temp2.clear();
			//�-�������
			for(int ort=0;ort<3;ort++)
			{
				temp.clear();//������� ��������� ������ ���������, ����� ��� y � z ���������
				for(int j=0;j<DiffStates;j++)//��������� ��������� ����������� 1�� �����-
				{
					if(Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j]!=0)
					{
						//��� 1�� ������� ��� �����, ������ ���� ��������

						if(ort==1||ort==0)//�-����� �������, �� ���� �������� �� -1, ������ 1 ���
							tempst.factor=inv[inSt].factor*0.5*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j];
						else
							tempst.factor=inv[inSt].factor*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j];
						//�������� ������ ���������, ��� ��� �� ����� ����� ����� ��������
						for(int oldst=0;oldst<node_num;oldst++)
							tempst.states[oldst]=inv[inSt].states[oldst];

						//������ ���������
						tempst.states[curInter[interNumber][i].n1]=j;

						tempst.coeff[0]=inv[inSt].coeff[0];
						tempst.coeff[1]=inv[inSt].coeff[1];
						tempst.coeff[2]=inv[inSt].coeff[2];
						//������ ��� �������� ������ ������ ��������!!!
						tempst.coeff[curInter[interNumber][i].Jtype]++;

						temp.push_back(tempst);
					}
				}
				switch(ort)
				{
				case 0: second_ort=1; break;
				case 1: second_ort=0; break;
				case 2: second_ort=2; break;
				}
				for(int k=0;k<temp.size();k++) //��������� �� ���������� ��������� 2�� ��������
				{
					for(int j=0;j<DiffStates;j++)
					{
						if(Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].states[curInter[interNumber][i].n2]][j]!=0)
						{
							for(int oldst=0;oldst<node_num;oldst++)
								tempst.states[oldst]=temp[k].states[oldst];
							tempst.states[curInter[interNumber][i].n2]=j;

							curE=0;
							for(int eInd=0;eInd<node_num;eInd++)
								curE+=Energie[tempst.states[eInd]];

							if(curE!=E0)
							{
								tempst.factor=temp[k].factor*Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].states[curInter[interNumber][i].n2]][j];

								tempst.coeff[0]=temp[k].coeff[0];
								tempst.coeff[1]=temp[k].coeff[1];
								tempst.coeff[2]=temp[k].coeff[2];

								temp2.push_back(tempst);  //���������� � �������� ������
							}
						}
					}
				}
			}
		}
	}
	//��������� � �������� �������� ������
	outv.clear();
	if(temp2.size())
		collect(outv,temp2);

}

/* #2 */void act_ground(std::vector<state> &inv, std::vector<state> &outv,double Vmatrix[4][3][DiffStates][DiffStates], int interNumber, int node_num)
{
	state tempst;
	int second_ort;
	double curE, E0;

	//initialisation
	E0 = getE0(node_num);
	temp2.clear();
	for (int i = 0; i < node_num; i++)
		tempst.states.push_back(DiffStates); //DiffStates- ������ ���������, ������� �� ������������ � ��������


	//computation
	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int i=0;i<interAmount[interNumber];i++) //���������� ��� ��-�� ��������������
		{
			//temp2.clear();
			//�-�������
			for(int ort=0;ort<3;ort++)
			{
				temp.clear();//������� ��������� ������ ���������, ����� ��� y � z ���������
				for(int j=0;j<DiffStates;j++)//��������� ��������� ����������� 1�� �����-
				{
					if(Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j]!=0)
					{
						//��� 1�� ������� ��� �����, ������ ���� ��������

						if(ort==1||ort==0)//�-����� �������, �� ���� �������� �� -1, ������ 1 ���
							tempst.factor=inv[inSt].factor*0.5*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j];
						else
							tempst.factor=inv[inSt].factor*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j];
						//�������� ������ ���������, ��� ��� �� ����� ����� ����� ��������
						for(int oldst=0;oldst<node_num;oldst++)
							tempst.states[oldst]=inv[inSt].states[oldst];

						//������ ���������
						tempst.states[curInter[interNumber][i].n1]=j;

						tempst.coeff[0]=inv[inSt].coeff[0];
						tempst.coeff[1]=inv[inSt].coeff[1];
						tempst.coeff[2]=inv[inSt].coeff[2];
						//������ ��� �������� ������ ������ ��������!!!
						tempst.coeff[curInter[interNumber][i].Jtype]++;

						temp.push_back(tempst);
					}
				}
				//if(sti==1)testout<<"temp size="<<temp.size()<<"\n";
				switch(ort)
				{
				case 0: second_ort=1; break;
				case 1: second_ort=0; break;
				case 2: second_ort=2; break;
				}
				for(int k=0;k<temp.size();k++) //��������� �� ���������� ��������� 2�� ��������
				{
					for(int j=0;j<DiffStates;j++)
					{
						if(Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].states[curInter[interNumber][i].n2]][j]!=0)
						{
							for(int oldst=0;oldst<node_num;oldst++)
								tempst.states[oldst]=temp[k].states[oldst];
							tempst.states[curInter[interNumber][i].n2]=j;

							curE=0;
							for(int eInd=0;eInd<node_num;eInd++)
								curE+=Energie[tempst.states[eInd]];

							if(curE==E0)
							{
								tempst.factor=temp[k].factor*Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].states[curInter[interNumber][i].n2]][j];

								tempst.coeff[0]=temp[k].coeff[0];
								tempst.coeff[1]=temp[k].coeff[1];
								tempst.coeff[2]=temp[k].coeff[2];

								temp2.push_back(tempst);  //���������� � �������� ������
							}
						}
					}
				}
				//if(sti==1)testout<<"temp2 size="<<temp2.size()<<"\n";
			}
		}
	}
	//��������� � �������� �������� ������
	outv.clear();
	if(temp2.size())
		collect(outv,temp2);
	//testout.close();

}

/* #3 */void act_energy(std::vector<state> &inv, std::vector<state> &outv,double Vmatrix[4][3][DiffStates][DiffStates],int interNumber, int node_num)
{
	state tempst;
	int second_ort;
	double curE, E0;

	//initialisation
	E0 = getE0(node_num);
	temp2.clear();
	for (int i = 0; i < node_num; i++)
		tempst.states.push_back(DiffStates); //DiffStates- ������ ���������, ������� �� ������������ � ��������
	if (inv.size()>50)
		temp2.reserve(30000);
											 
											 
	//computation
	for(int inSt=0;inSt<inv.size();inSt++)
	{


		for(int i=0;i<interAmount[interNumber];i++) //���������� ��� ��-�� ��������������
		{
			//temp2.clear();
			//�-�������
			for(int ort=0;ort<3;ort++)
			{
				temp.clear();//������� ��������� ������ ���������, ����� ��� y � z ���������
				for(int j=0;j<DiffStates;j++)//��������� ��������� ����������� 1�� �����-
				{
					if(Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j]!=0)
					{
						//��� 1�� ������� ��� �����, ������ ���� ��������

						if(ort==1||ort==0)//�-����� �������, �� ���� �������� �� -1, ������ 1 ���
							tempst.factor=inv[inSt].factor* 0.5*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j];
						else
							tempst.factor=inv[inSt].factor*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j];
						//�������� ������ ���������, ��� ��� �� ����� ����� ����� ��������
						for(int oldst=0;oldst<node_num;oldst++)
							tempst.states[oldst]=inv[inSt].states[oldst];

						//������ ���������
						tempst.states[curInter[interNumber][i].n1]=j;

						tempst.coeff[0]=inv[inSt].coeff[0];
						tempst.coeff[1]=inv[inSt].coeff[1];;
						tempst.coeff[2]=inv[inSt].coeff[2];;
						//������ ��� �������� ������ ������ ��������!!!
						tempst.coeff[curInter[interNumber][i].Jtype]++;

						temp.push_back(tempst);
					}
				}
				switch(ort)
				{
				case 0: second_ort=1; break;
				case 1: second_ort=0; break;
				case 2: second_ort=2; break;
				}
				for(int k=0;k<temp.size();k++) //��������� �� ���������� ��������� 2�� ��������
				{
					for(int j=0;j<DiffStates;j++)
					{
						if(Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].states[curInter[interNumber][i].n2]][j]!=0)
						{
							for(int oldst=0;oldst<node_num;oldst++)
								tempst.states[oldst]=temp[k].states[oldst];
							tempst.states[curInter[interNumber][i].n2]=j;

							curE=0;
							for(int eInd=0;eInd<node_num;eInd++)
								curE+=Energie[tempst.states[eInd]];


							if(curE!=E0)
							{
								tempst.factor=temp[k].factor*Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].states[curInter[interNumber][i].n2]][j]/(E0-curE);
								tempst.coeff[0]=temp[k].coeff[0];
								tempst.coeff[1]=temp[k].coeff[1];
								tempst.coeff[2]=temp[k].coeff[2];

								temp2.push_back(tempst);  //���������� � �������� ������
							}

						}
					}
				}
			}
		}
	}

	//��������� � �������� �������� ������
	outv.clear();
	if(temp2.size())
		collect(outv,temp2);

}

/* #4 */void act_energy_power(std::vector<state> &inv, std::vector<state> &outv,int power,double Vmatrix[4][3][DiffStates][DiffStates],int interNumber, int node_num)
{
	state tempst;
	int second_ort;
	double curE,E0;


	//initialisation
	E0 = getE0(node_num);
	temp2.clear();
	for (int i = 0; i < node_num; i++)
		tempst.states.push_back(DiffStates); //DiffStates- ������ ���������, ������� �� ������������ � ��������
	if (inv.size()>50)
		temp2.reserve(30000);

	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int i=0;i<interAmount[interNumber];i++) //���������� ��� ��-�� ��������������
		{
			//temp2.clear();
			//�-�������
			for(int ort=0;ort<3;ort++)
			{
				temp.clear();//������� ��������� ������ ���������, ����� ��� y � z ���������
				for(int j=0;j<DiffStates;j++)//��������� ��������� ����������� 1�� �����-
				{
					if(Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j]!=0)
					{
						//��� 1�� ������� ��� �����, ������ ���� ��������

						if(ort==1||ort==0)//�-����� �������, �� ���� �������� �� -1, ������ 1 ���
							tempst.factor=inv[inSt].factor* 0.5*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j];
						else
							tempst.factor=inv[inSt].factor*Vmatrix[curInter[interNumber][i].v1][ort][inv[inSt].states[curInter[interNumber][i].n1]][j];
						//�������� ������ ���������, ��� ��� �� ����� ����� ����� ��������
						for(int oldst=0;oldst<node_num;oldst++)
							tempst.states[oldst]=inv[inSt].states[oldst];
						/*tempst.states[0]=inv[inSt].states[0];
						tempst.states[1]=inv[inSt].states[1];
						tempst.states[2]=inv[inSt].states[2];*/

						//������ ���������
						tempst.states[curInter[interNumber][i].n1]=j;

						tempst.coeff[0]=inv[inSt].coeff[0];
						tempst.coeff[1]=inv[inSt].coeff[1];;
						tempst.coeff[2]=inv[inSt].coeff[2];;
						//������ ��� �������� ������ ������ ��������!!!
						tempst.coeff[curInter[interNumber][i].Jtype]++;

						temp.push_back(tempst);
					}
				}
				switch(ort)
				{
				case 0: second_ort=1; break;
				case 1: second_ort=0; break;
				case 2: second_ort=2; break;
				}
				for(int k=0;k<temp.size();k++) //��������� �� ���������� ��������� 2�� ��������
				{
					for(int j=0;j<DiffStates;j++)
					{
						if(Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].states[curInter[interNumber][i].n2]][j]!=0)
						{
							tempst.factor=temp[k].factor*Vmatrix[curInter[interNumber][i].v2][second_ort][temp[k].states[curInter[interNumber][i].n2]][j];

							/*
							tempst.states[0]=temp[k].states[0];
							tempst.states[1]=temp[k].states[1];
							tempst.states[2]=temp[k].states[2];*/
							for(int oldst=0;oldst<node_num;oldst++)
								tempst.states[oldst]=temp[k].states[oldst];
							tempst.states[curInter[interNumber][i].n2]=j;

							tempst.coeff[0]=temp[k].coeff[0];
							tempst.coeff[1]=temp[k].coeff[1];
							tempst.coeff[2]=temp[k].coeff[2];


							curE=0;
							for(int eInd=0;eInd<node_num;eInd++)
							{
								curE+=Energie[tempst.states[eInd]];
							}


							if(curE!=E0)
							{
								tempst.factor=tempst.factor/pow((E0-curE),power);
								temp2.push_back(tempst);  //���������� � �������� ������
							}

						}
					}
				}
			}
		}
	}
	//��������� � �������� �������� ������
	outv.clear();
	if(temp2.size())
		collect(outv,temp2);

}

/* #5 */void act_inside(std::vector<state> &inv, std::vector<state> &outv,int plaquetNumber, int node_num)
{
	state tempst;
	double curE,E0;
	
	//initialisation
	E0 = getE0(node_num);
	temp.clear();
	for (int i = 0; i < node_num; i++)
		tempst.states.push_back(DiffStates); //DiffStates- ������ ���������, ������� �� ������������ � ��������
	

	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int j=0;j<DiffStates;j++)
		{
			if(VmatrixInside[inv[inSt].states[plaquetNumber]][j]!=0)
			{


				for(int oldst=0;oldst<node_num;oldst++)
					tempst.states[oldst]=inv[inSt].states[oldst];
				tempst.states[plaquetNumber]=j;

				curE=0;
				for(int eInd=0;eInd<node_num;eInd++)
					curE+=Energie[tempst.states[eInd]];

				if(E0!=curE)
				{

					tempst.factor=inv[inSt].factor*VmatrixInside[inv[inSt].states[plaquetNumber]][j];

					tempst.coeff[0]=inv[inSt].coeff[0];
					tempst.coeff[1]=inv[inSt].coeff[1];
					tempst.coeff[2]=inv[inSt].coeff[2]+1;

					temp.push_back(tempst);
				}

			}
		}
	}
	outv.clear();
	if(temp.size())
		collect(outv,temp);

}

/* #6 */void act_inside_enrgy_power(std::vector<state> &inv, std::vector<state> &outv,int power,int plaquetNumber, int node_num)
{
	state tempst;
	double curE,E0;

	//initialisation
	E0 = getE0(node_num);
	temp.clear();
	for (int i = 0; i < node_num; i++)
		tempst.states.push_back(DiffStates); //DiffStates- ������ ���������, ������� �� ������������ � ��������


	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int j=0;j<DiffStates;j++)
		{
			if(VmatrixInside[inv[inSt].states[plaquetNumber]][j]!=0)
			{


				for(int oldst=0;oldst<node_num;oldst++)
					tempst.states[oldst]=inv[inSt].states[oldst];
				tempst.states[plaquetNumber]=j;

				curE=0;
				for(int eInd=0;eInd<node_num;eInd++)
					curE+=Energie[tempst.states[eInd]];

				if(E0!=curE)
				{

					tempst.factor=inv[inSt].factor*VmatrixInside[inv[inSt].states[plaquetNumber]][j]/pow(double(E0-curE),power);

					tempst.coeff[0]=inv[inSt].coeff[0];
					tempst.coeff[1]=inv[inSt].coeff[1];
					tempst.coeff[2]=inv[inSt].coeff[2]+1;

					temp.push_back(tempst);
				}

			}
		}
	}
	outv.clear();
	if(temp.size())
		collect(outv,temp);

}

/* #7 */void act_inside_ground(std::vector<state> &inv, std::vector<state> &outv,int plaquetNumber, int node_num)
{
	state tempst;
	double curE,E0;

	//initialisation
	E0 = getE0(node_num);
	temp.clear();

	for (int i = 0; i < node_num; i++)
		tempst.states.push_back(DiffStates); //DiffStates- ������ ���������, ������� �� ������������ � ��������
	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int j=0;j<DiffStates;j++)//���������� ��� �������� ������� ������ ������� ��������
		{
			if(VmatrixInside[inv[inSt].states[plaquetNumber]][j]!=0)
			{


				for(int oldst=0;oldst<node_num;oldst++)
					tempst.states[oldst]=inv[inSt].states[oldst];
				tempst.states[plaquetNumber]=j;

				curE=0;
				for(int eInd=0;eInd<node_num;eInd++)
					curE+=Energie[tempst.states[eInd]];

				if(E0==curE)
				{

					tempst.factor=inv[inSt].factor*VmatrixInside[inv[inSt].states[plaquetNumber]][j];

					tempst.coeff[0]=inv[inSt].coeff[0];
					tempst.coeff[1]=inv[inSt].coeff[1];
					tempst.coeff[2]=inv[inSt].coeff[2]+1;

					temp.push_back(tempst);
				}

			}
		}
	}
	outv.clear();
	if(temp.size())
		collect(outv,temp);

}

void generate_procedure_order(int *termorder,int* operatororder,int edge_amount,int num,int *Res,int *power)
{
	//������ ������� ��������
	if (num == 1)
	{
		power[0] = 0;
		if (edge_amount == 1)
			Res[0] = 2;
		else
			Res[0] = 7;
		return;
	}
	//����� ������ ��� ��������� ��������
	for(int i=0;i<num;i++)
	{
		power[i]=0;
	}
	for(int i=0;i<(num+1)/2;i++)
	{
		if(termorder[i]==0)//ground ������
		{
			if(operatororder[i]<edge_amount)//�������� ��������
				Res[i]=2;
			else
				Res[i]=7;
		}
		else if(termorder[i]==1)//����������� � 1�� �������
		{
			if(operatororder[i]<edge_amount)
				Res[i]=3;
			else
			{
				Res[i]=6;
				power[i]=1;
			}

		}
		else
		{
			if(operatororder[i]<edge_amount)
				Res[i]=4;
			else
				Res[i]=6;
			power[i]=termorder[i];
		}
	}

	//�������� ���
	for(int i=0;i<(num/2);i++)
	{
		if(termorder[num-i-2]==0)//ground ������
		{
			if(operatororder[num-i-1]<edge_amount)//�������� ��������
				Res[num-i-1]=2;
			else
				Res[num-i-1]=7;
		}
		else if(termorder[num-i-2]==1)//����������� � 1�� �������
		{
			if(operatororder[num-i-1]<edge_amount)
				Res[num-i-1]=3;
			else
			{
				Res[num-i-1]=6;
				power[num-i-1]=1;
			}

		}
		else
		{
			if(operatororder[num-i-1]<edge_amount)
				Res[num-i-1]=4;
			else
				Res[num-i-1]=6;
			power[num-i-1]=termorder[num-i-2];
		}
	}
	//����� ��������� ������� ��������� ���� ��� �����������
	if((Res[num-num/2]==3)||(Res[num-num/2]==4))
		Res[num-num/2]=1;
	if(Res[num-num/2]==6)
		Res[num-num/2]=5;

}
