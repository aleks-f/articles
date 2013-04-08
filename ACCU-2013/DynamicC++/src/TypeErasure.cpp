#include "TypeErasure.h"

#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Stopwatch.h"


#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/concept_interface.hpp>
#include <boost/type_erasure/rebind_any.hpp>
/************/
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/tuple.hpp>
#include <boost/type_erasure/relaxed_match.hpp>
#include <boost/mpl/vector.hpp>
/************/
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
		return to = NumberFormatter::format(from);
	}
};

template<class F, class T>
struct to_number
{
	static T apply(const F& from, T& to)
	{
		return to = NumberParser::parse(from);
	}
};

namespace boost
{
	namespace type_erasure
	{
		template<class C, class T, class Base>
		struct concept_interface< ::to_string<C, T>, Base, C> : Base
		{
			typedef typename rebind_any<Base, T>::type IntType;
			T to_string(IntType arg = IntType())
			{
				return call(::to_string<C, T>(), *this, arg);
			}
		};

		template<class C, class T, class Base>
		struct concept_interface< ::to_number<C, T>, Base, C> : Base
		{
			typedef typename rebind_any<Base, T>::type StringType;
			T to_number(StringType arg = StringType())
			{
				return call(::to_number<C, T>(), *this, arg);
			}
		};
	}
}



void typeErasureTutorial()
{
	typedef any<
		boost::mpl::vector<
			copy_constructible<>,
			typeid_<>,
			addable<>,
			ostreamable<>,
			relaxed_match
		>
	> any_type;
	any_type x(1.1);
	any_type y(1);
	any_type z(x + y);
	std::cout << (x + y) << std::endl; // prints ???
	
	/*
	double d = 1.1;
	int i = 1;

	typedef boost::mpl::vector<
		copy_constructible<_a>,
		copy_constructible<_b>,
		typeid_<_a>,
		addable<_a, _b, _a>
	> requirements;

	tuple<requirements, _a, _b> t(d, i);
	any<requirements, _a> x(get<0>(t) + get<1>(t));
	std::cout << any_cast<double>(x) << std::endl; // 2.1
	std::cout << any_cast<int>(x) << std::endl; // 2.1
	*/
	/*
	double d = 1.1;
	int i = 1;

	typedef boost::mpl::vector<
		copy_constructible<_a>,
		copy_constructible<_b>,
		typeid_<_a>,
		addable<_a, _b, _a>,
		ostreamable<>
	> requirements;

	tuple<requirements, _a, _b> t(&d, 1.1);
	//any<requirements, _a> x(get<0>(t) + get<1>(t));
	*/
	//std::cout << any_cast<double>(x) << std::endl; // compile error

	int j = 123;
	any<to_string<_self, std::string>, _self&> ai(j);
	std::string str = ai.to_string();
	std::cout << str << std::endl;

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
		int i = ais.to_number();
		any<to_number<_self, double>, _self&> ads(*it);
		double d = ads.to_number();
		any<to_string<_self, std::string>, _self&> sai(i);
		*it = sai.to_string();
		any<to_string<_self, std::string>, _self&> sad(d);
		*it = sad.to_string();
	}
	sw.stop();
	std::cout << "boost::type_erasure: " << sw.elapsed()/1000.0 << " [ms]" << std::endl;
	
	std::cout << "==============" << std::endl;
}
