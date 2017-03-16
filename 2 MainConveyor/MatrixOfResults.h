#pragma once
#include "stdafx.h"
#include "JFactors.h"
#include "MyFunctions.h"




class MatrixOfResults
{
private:
	std::vector<std::vector<res>> matrix;
public:
	MatrixOfResults(int s = 0);

	void set_size(int s);

	bool add(int x, int y, const res &curTerm);

	void printMatrix(std::string fname, int Order);
};