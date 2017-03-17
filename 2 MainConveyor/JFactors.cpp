#include "stdafx.h"
#include "JFactors.h"



bool JFactors::JFactor::operator==(const JFactor& jf2)const
{
	//используем явный вид для скорости
	return (powers[0] == jf2.powers[0]) && (powers[1] == jf2.powers[1]) && (powers[2] == jf2.powers[2]);
}

JFactors::JFactor::JFactor(int n[Npowers])
{
	for (int i = 0; i < Npowers; i++)
		powers[i] = n[i];
}

void JFactors::generate_all_Jfactors()
{
	jfactors.clear();
	int temp;
	int cur_num = 0;
	int curJfac[Npowers];
	for (int i = 0; i<(order + 1)*(order + 1)*(order + 1); i++)
	{
		temp = i;
		for (int j = 0; j<Npowers; j++)
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

void JFactors::generate_all_Jstrings()
{
	jstrings.clear();
	bool not_first;
	std::string js[Npowers];
	js[0] = "J1^";
	js[1] = "J2^";
	js[2] = "(J2-J1)^";
	std::ostringstream out;
	for (int i = 0; i<jfactors.size(); i++)
	{
		not_first = false;
		out.str("");
		for (int j = 0; j<Npowers; j++)
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

void JFactors::setOrder(int ord)
{
	if (ord > 0 && ord < MaxOrder)
	{
		order = ord;
		generate_all_Jfactors();
		generate_all_Jstrings();
	}
}

int JFactors::getNumberByPowers(int powers[Npowers])
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

std::string JFactors::getStringByNumber(int k)
{
	if (k >= 0 && k < jstrings.size())
	{
		return jstrings[k];
	}
	else
	{
		return "";
	}
}

std::string JFactors::getStringByPowers(int powers[Npowers])
{
	return getStringByNumber(getNumberByPowers(powers));
}

int JFactors::getAmountOfPowers()
{
	return Npowers;
}

int JFactors::order = -1;
std::vector<JFactors::JFactor> JFactors::jfactors;
std::vector<std::string> JFactors::jstrings;