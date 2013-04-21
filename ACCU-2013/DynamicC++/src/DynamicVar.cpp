
#include "DynamicCpp.h"
#include "DynamicVar.h"
#include "Poco/Dynamic/Var.h"


using Poco::Stopwatch;
using Poco::Dynamic::Var;



void doDynamicVar(const std::vector<std::string>& strvec)
{
	std::string str("42");
	Var v1 = str; // "42"
	int i = v1; // 42
	v1 = i; // 42
	++v1; // 43 
	double d = v1; // 43.0
	Var v2 = d + 1.0; // 44.0
	float f = v2 + 1; // 45.0

	/*
	std::cout << "Poco::Dynamic::Var" << std::endl;
	std::cout << "==============" << std::endl;
	
	printDynamicVarSize<char>();
	printDynamicVarSize<int>();
	printDynamicVarSize<float>();
	printDynamicVarSize<double>();
	printDynamicVarSize<std::string>();
	*/
	std::vector<std::string>::const_iterator it = strvec.begin();
	std::vector<std::string>::const_iterator end = strvec.end();

	Stopwatch sw;
	sw.start();
	for (; it != end; ++it)
	{
		Var var(*it);
		
		int i = var;
	
		double d = var;

		var = i;
		std::string s = var.toString();

		var = d;
		s = var.toString();
	}
	sw.stop();
	std::cout << "Poco::Dynamic::Var: " << sw.elapsed()/1000.0 << " [ms]" << std::endl;
	
	std::cout << "==============" << std::endl;
}

