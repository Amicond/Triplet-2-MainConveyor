#include "stdafx.h"
#include "MatrixOfResults.h"

MatrixOfResults::MatrixOfResults(int s)
{
	if (s != 0)
	{
		set_size(s);
	}
}

void MatrixOfResults::set_size(int s)
{
	if (matrix.size() != 0)
	{
		for (int i = 0; i < matrix.size(); i++)
			matrix[i].clear();
	}
	matrix.reserve(s);
	for (int i = 0; i < s; i++)
		matrix[i].reserve(s);
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