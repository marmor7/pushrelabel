#pragma once
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

const int INFINITY = INT_MAX;
const int LOG_NONE = 0;
const int LOG_1 = 1; //important
const int LOG_2 = 2;
const int LOG_3 = 3; //not important

const int DEBUG = LOG_3;

class Utils
{
public:
	Utils(void);
	~Utils(void);

	static string printValue(int val);

private:

};
