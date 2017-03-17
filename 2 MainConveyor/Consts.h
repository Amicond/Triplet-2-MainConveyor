#pragma once
#include "stdafx.h"

const int N = 5;
const std::string delim = "\\";
const std::string inp_fin = "final_data";
const std::string inp_matr = "matrixes" + delim + "b3_";
const std::string inp_route = "input_routes" + delim;
const std::string out_res = "results_";

const std::string type1 = "0";
const std::string type2 = "1";
const std::string type3 = "2";


const int DiffStates = 16; //количкество различных собственных состояний, равно размеру матриц
const int maxIntElem = 4; // количество слагаемых в операторе взаимодействия
const int Namount = 576;   //количество различных ground-состояний 6
const int resAmount = 45; //кол-во различных J факторов в 8 порядке


