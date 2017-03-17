#include "stdafx.h"
#include "MatrixOfResults.h"

MatrixOfResults::MatrixOfResults(int s)
{
	if (s != 0)
	{
		clearAndSetSize(s);
	}
}

void MatrixOfResults::clearAndSetSize(int s)
{
	if (matrix.size() != 0)
	{
		for (int i = 0; i < matrix.size(); i++)
			matrix[i].clear();
	}
	matrix.clear();

	for (int i = 0; i < s; i++)
	{
		matrix.push_back(std::vector<res>());
		for (int j = 0; j < s; j++)
		{
			matrix[i].push_back(res());
			for (int k = 0; k < resAmount; k++)
				matrix[i][j].factors[k] = 0;
		}
	}
}

bool MatrixOfResults::add(int x, int y, const res &curTerm)
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

void MatrixOfResults::printMatrix(std::string fname, int Order)
{
	std::ofstream matrixRes(fname.c_str(), std::ios::out);
	int size = matrix.size();

	matrixRes << size << "\n{";
	matrixRes.setf(std::ios::fixed);
	matrixRes << std::setprecision(10);

	for (int ii = 0; ii<size; ii++)
	{
		matrixRes << "{";
		for (int jj = 0; jj<size; jj++)
		{
			for (int kk = 0; kk<(Order + 2)*(Order + 1) / 2; kk++)
			{

				if (abs(matrix[ii][jj].factors[kk])>0.0000000000001)
					matrixRes << matrix[ii][jj].factors[kk] << "*" << JFactors::getStringByNumber(kk) << "+";
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