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

class JFactors
{
	struct JFactor
	{
		static const int N=3;
		int powers[3];
		bool operator==(const JFactor& jf2)
		{
			//используем явный вид для скорости
			return (powers[0] == jf2.powers[0]) && (powers[1] == jf2.powers[1]) && (powers[2] == jf2.powers[2]);
		}
		JFactor(int n[N])
		{
			for (int i = 0; i < N; i++)
				powers[i] = n[i];
		}
	};

private:
	static const int MaxOrder = 10;
	static int order;
	static vector<JFactor> jfactors;
	static vector<string> jstrings;

	static void generate_all_Jfactors()
	{
		int temp;
		int cur_num = 0;
		int curJfac[3];
		for (int i = 0; i<(order + 1)*(order + 1)*(order + 1); i++)
		{
			temp = i;
			for (int j = 0; j<3; j++)
			{
				curJfac[j] = temp % (order + 1);
				temp /= (order + 1);
			}
			if (curJfac[0] + curJfac[1] + curJfac[2] == order)
			{
				jfactors.push_back(JFactor(curJfac));
			}

		}
	}

	static void generate_all_Jstrings()
	{
		bool not_first;
		string js[3];
		js[0] = "J1^";
		js[1] = "J2^";
		js[2] = "(J2-J1)^";
		ostringstream out;
		for (int i = 0; i<jfactors.size(); i++)
		{
			not_first = false;
			out.str("");
			for (int j = 0; j<3; j++)
			{
				if (jfactors[i].powers[j] != 0)
				{
					if (not_first)
						out << "*";
					else
						not_first = true;
					out << js[j] << jfactors[i].powers[j];
				}
			}
			jstrings.push_back(out.str());
		}
	}
public:
	
	
	static void setOrder(int ord)
	{
		if (ord > 0 && ord < MaxOrder)
		{
			order = ord;
			generate_all_Jfactors();
			generate_all_Jstrings();
		}
	}


	static int getNumberByPowers(int powers[N])
	{
		auto it = find(jfactors.begin(), jfactors.end(), JFactor(powers));
		if (it != jfactors.end())
		{
			return distance(jfactors.begin(), it);
		}
		else
		{
			return -1;
		}
	}
	static string getStringByNumber(int k)
	{
		if (k > 0 && k < jstrings.size())
		{
			return jstrings[k];
		}
		else
		{
			return "";
		}
	}

	static string getStringByPowers(int powers[N])
	{
		getStringByNumber(getNumberByPowers(powers));
	}



};

int JFactors::order = -1;