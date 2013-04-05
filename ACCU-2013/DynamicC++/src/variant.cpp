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
		
		int i = boost::apply_visitor(string_int_converter(), u);
	
		double d = boost::apply_visitor(string_dbl_converter(), u);

		u = i;
		std::string s = boost::apply_visitor(num_string_converter(), u);

		u = d;
		s = boost::apply_visitor(num_string_converter(), u);
	}
	sw.stop();
	std::cout << "variant: " << sw.elapsed()/1000.0 << " [ms]" << std::endl;
	
	std::cout << "==============" << std::endl;
}

