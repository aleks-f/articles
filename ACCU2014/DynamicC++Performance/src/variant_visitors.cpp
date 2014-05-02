#include "variant_visitors.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"


using boost::variant;
using Poco::NumberParser;
using Poco::NumberFormatter;


int string_int_converter::operator()(int i) const
{
	return i;
}

int string_int_converter::operator()(const std::string & str) const
{
	return NumberParser::parse(str);
}

int string_int_converter::operator()(double d) const
{
	return static_cast<int>(d);
}



double string_dbl_converter::operator()(int i) const
{
	return i;
}

double string_dbl_converter::operator()(const std::string & str) const
{
	return NumberParser::parseFloat(str);
}

double string_dbl_converter::operator()(double d) const
{
	return d;
}


std::string num_string_converter::operator()(int i) const
{
	return NumberFormatter::format(i);
}

std::string num_string_converter::operator()(const std::string& str) const
{
	return str;
}

std::string num_string_converter::operator()(double d) const
{
	return NumberFormatter::format(d);
}
