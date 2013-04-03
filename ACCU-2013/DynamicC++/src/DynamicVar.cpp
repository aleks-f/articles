
#include "DynamicCpp.h"
#include "DynamicVar.h"
#include "Poco/Dynamic/Var.h"


using Poco::Stopwatch;
using Poco::Dynamic::Var;



void doDynamicVar(const std::vector<std::string>& strvec)
{
	std::cout << "Poco::Dynamic::Var" << std::endl;
	std::cout << "==============" << std::endl;

	printDynamicVarSize<char>();
	printDynamicVarSize<int>();
	printDynamicVarSize<float>();
	printDynamicVarSize<double>();
	printDynamicVarSize<std::string>();

	std::vector<std::string>::const_iterator it = strvec.begin();
	std::vector<std::string>::const_iterator end = strvec.end();

	Stopwatch sw;
	sw.start();
	for (; it != end; ++it)
	{
		Var var(*it);
		//std::cout << u.convert<std::string>() << std::endl;
		
		int i = var;
		//std::cout << i << std::endl;
	
		double d = var;
		//std::cout << d << std::endl;

		var = i;
		std::string s = var.toString();
		//std::cout << s << std::endl;

		var = d;
		s = var.toString();
		//std::cout << s << std::endl;
	}
	sw.stop();
	std::cout << "Poco::Dynamic::Var: " << sw.elapsed()/1000.0 << " [ms]" << std::endl;
	
	std::cout << "==============" << std::endl;
}

