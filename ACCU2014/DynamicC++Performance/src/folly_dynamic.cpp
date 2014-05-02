#include "Poco/Stopwatch.h"
#include "folly/dynamic.h"
#include "folly/FBString.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace folly;
using Poco::Stopwatch;


void doFollyDynamic(std::vector<std::string> strvec) 
{
	std::cout << "folly::dynamic" << std::endl;
	std::cout << "==============" << std::endl;

	std::vector<std::string>::iterator it = strvec.begin();
	std::vector<std::string>::iterator end = strvec.end();

	Stopwatch sw;
	sw.start();
	for (; it != end; ++it)
	{
		dynamic var(*it);
		
		int i = var.asInt();
	
		double d = var.asDouble();

		var = i;
		std::string s = var.asString().c_str();

		var = d;
		s = var.asString().c_str();
	}
	sw.stop();
	std::cout << "folly::dynamic: " << sw.elapsed()/1000.0 << " [ms]" << std::endl;
	
	std::cout << "==============" << std::endl;
}
