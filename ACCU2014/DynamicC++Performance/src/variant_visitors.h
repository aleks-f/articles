#ifndef DynamicCPP_variant_visitors_included
#define DynamicCPP_variant_visitors_included

#include "boost/variant/variant.hpp"


struct string_int_converter : public boost::static_visitor<int>
{
	int operator()(int i) const;

	int operator()(const std::string & str) const;
	
	int operator()(double d) const;
};

struct string_dbl_converter : public boost::static_visitor<double>
{
	double operator()(int i) const;

	double operator()(const std::string & str) const;

	double operator()(double d) const;
};


struct num_string_converter : public boost::static_visitor<std::string>
{
	std::string operator()(int i) const;

	std::string operator()(const std::string& str) const;

	std::string operator()(double d) const;
};


#endif // DynamicCPP_variant_visitors_included
