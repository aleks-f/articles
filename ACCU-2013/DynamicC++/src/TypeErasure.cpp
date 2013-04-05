#include "TypeErasure.h"

#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Stopwatch.h"


#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/concept_interface.hpp>
#include <boost/type_erasure/rebind_any.hpp>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace boost::type_erasure;
using Poco::NumberParser;
using Poco::NumberFormatter;
using Poco::Stopwatch;


template<class F, class T>
struct to_string
{
	static T apply(const F& from, T& to)
	{
		return NumberFormatter::format(from);
	}
};

template<class F, class T>
struct to_number
{
	static T apply(const F& from, T& to)
	{
		return NumberParser::parse(from);
	}
};

namespace boost
{
	namespace type_erasure
	{
		template<class C, class T, class Base>
		struct concept_interface< ::to_string<C, T>, Base, C> : Base
		{
			T to_string(typename rebind_any<Base, T&>::type arg)
			{
				return call(::to_string<C, T>(), *this, arg);
			}
		};

		template<class C, class T, class Base>
		struct concept_interface< ::to_number<C, T>, Base, C> : Base
		{
			T to_number(typename rebind_any<Base, T&>::type arg)
			{
				return call(::to_number<C, T>(), *this, arg);
			}
		};
	}
}



void doTypeErasure(std::vector<std::string> strvec) 
{
	std::cout << "boost::type_erasure" << std::endl;
	std::cout << "==============" << std::endl;

	std::vector<std::string>::iterator it = strvec.begin();
	std::vector<std::string>::iterator end = strvec.end();

	Stopwatch sw;
	sw.start();
	for (; it != end; ++it)
	{
		any<to_number<_self, int>, _self&> ais(*it);
		int i = ais.to_number(i);
		any<to_number<_self, double>, _self&> ads(*it);
		double d = ads.to_number(d);
		any<to_string<_self, std::string>, _self&> sai(i);
		*it = sai.to_string(*it);
		any<to_string<_self, std::string>, _self&> sad(d);
		*it = sad.to_string(*it);
	}
	sw.stop();
	std::cout << "boost::type_erasure: " << sw.elapsed()/1000.0 << " [ms]" << std::endl;
	
	std::cout << "==============" << std::endl;
}
