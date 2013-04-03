#include "variant.h"
#include "variant_visitors.h"
#include "DynamicCpp.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Dynamic/Var.h"


using boost::variant;
using Poco::Stopwatch;
using Poco::Dynamic::Var;


void doVariant(const std::vector<std::string>& strvec)
{
	std::cout << "boost::variant" << std::endl;
	std::cout << "==============" << std::endl;

	printSize<char>();
	printSize<int>();
	printSize<float>();
	printSize<double>();
	printSize<std::string>();

	std::vector<std::string>::const_iterator it = strvec.begin();
	std::vector<std::string>::const_iterator end = strvec.end();

	Stopwatch sw;
	sw.start();
	for (; it != end; ++it)
	{
		
		boost::variant<int, double, std::string> u(*it);
		//std::cout << u << std::endl;
		
		int i = boost::apply_visitor(string_int_converter(), u);
		//std::cout << i << std::endl;
	
		double d = boost::apply_visitor(string_dbl_converter(), u);
		//std::cout << d << std::endl;

		u = i;
		std::string s = boost::apply_visitor(num_string_converter(), u);
		//std::cout << s << std::endl;

		u = d;
		s = boost::apply_visitor(num_string_converter(), u);
		//std::cout << s << std::endl;
	}
	sw.stop();
	std::cout << "variant: " << sw.elapsed()/1000.0 << " [ms]" << std::endl;
	
	std::cout << "==============" << std::endl;
}

