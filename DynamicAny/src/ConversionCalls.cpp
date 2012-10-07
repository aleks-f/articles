#include "Poco/Any.h"
#include "Poco/DynamicAny.h"
#include "boost/lexical_cast.hpp"
#include "boost/any.hpp"


using Poco::Any;
using Poco::AnyCast;
using Poco::RefAnyCast;
using Poco::UnsafeAnyCast;
using Poco::DynamicAny;
using Poco::Int32;
using Poco::UInt16;


void staticCastInt32ToDouble(double& d, int i) {
	d = static_cast<double>(i);
}

void unsafeAnyCastAnyToDouble(double& d, Any& a) {
	d = *UnsafeAnyCast<double>(&a);
}

void lexicalCastInt32ToDouble(double& d, int i) {
	d = boost::lexical_cast<double>(i);
}

void convertInt32ToDouble(double& d, DynamicAny& da) {
	d = da.convert<double>();
}

void assignInt32ToDouble(double& d, DynamicAny& da) {
	d = da;
}

void lexicalCastInt32toUInt16(UInt16& us, Int32 j) {
	us = boost::lexical_cast<UInt16>(j);
}

void convertInt32toUInt16(UInt16& us, DynamicAny& da) {
	us = da.convert<UInt16>();
}

void assignInt32toUInt16(UInt16& us, DynamicAny& da) {
	us = da;
}

void lexicalCastStringToDouble(double& d, std::string& s) {
	d = boost::lexical_cast<double>(s); 
}

void convertStringToDouble(double& d, DynamicAny& ds) {
	d = ds.convert<double>();
}

void assignStringToDouble(double& d, DynamicAny& ds) {
	d = ds;
}

void anyCastRefDouble(double& d, Any& a) {
	d = RefAnyCast<double>(a);
}

void anyCastPtrDouble(double& d, Any& a) {
	d = *AnyCast<double>(&a);
}

void extractDouble(double& d, DynamicAny& da)
{
	d = da.extract<double>();
}

void anyCastRefString(std::string& s, Any& as) {
	s = RefAnyCast<std::string>(as);
}

void anyCastPtrString(std::string& s, Any& as) {
	s = *AnyCast<std::string>(&as);
}

void extractString(std::string& s, DynamicAny& ds)
{
	s = ds.extract<std::string>();
}
