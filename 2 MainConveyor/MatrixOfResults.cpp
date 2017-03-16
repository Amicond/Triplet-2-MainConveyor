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

class MatrixOfResults
{
private:
	vector<vector<res>> matrix;
public:
	MatrixOfResults(int s=0)
	{
		if (s != 0)
		{
			set_size(s);
		}
	}

	void set_size(int s)
	{
		if (matrix.size()!=0)
		{
			for (int i = 0; i < matrix.size(); i++)
				matrix[i].clear();
		}
		matrix.reserve(s);
		for (int i = 0; i < s; i++)
			matrix[i].reserve(s);
	}

	bool add(int x,int y,const res &curTerm)
	{
		int s = matrix.size();
		if (x < 0 || x >= s || y < 0 || y >= s)
			return false;
		else
		{
			matrix[x][y] += curTerm;
			return true;
		}
	}

	void printMatrix(string fname)
	{
		
		ofstream matrixRes(fname.c_str(), ios::out);
		int size = matrix.size();

		matrixRes << size << "\n{";
		matrixRes.setf(ios::fixed);
		matrixRes << setprecision(10);

		for (int ii = 0; ii<size; ii++)
		{
			matrixRes << "{";
			for (int jj = 0; jj<size; jj++)
			{
				for (int kk = 0; kk<(Order + 2)*(Order + 1) / 2; kk++)
				{

					if (abs(matrix[ii][jj].factors[kk])>0.0000000000001)
						matrixRes << matrix[ii][jj].factors[kk] << "*" << strarr[kk] << "+";
				}
				if (jj<size - 1)
					matrixRes << "0,";
				else
					matrixRes << "0";
			}
			if (ii<size - 1)
				matrixRes << "},\n";
			else
				matrixRes << "}\n";

		}
		matrixRes << "}";
		matrixRes.close();
	}


};