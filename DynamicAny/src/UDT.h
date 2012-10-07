//
// SSN.h
//


#ifndef UDT_HEADER_INCLUDED
#define UDT_HEADER_INCLUDED


#include "Poco/DynamicAny.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"


using Poco::NumberFormatter;
using Poco::DynamicAny;


class SSN
{
public:
	SSN(const SSN& ssn): _ssn(ssn._ssn) { }

	SSN(const DynamicAny& da): _ssn(da) { }

	operator SSN ()
	{
		return *this;
	}

	std::string sSSN() const
	{
		return format();
	}

	int nSSN() const
	{
		return _ssn;
	}

private:
	std::string format() const
		/// format integer as SSN
	{
		std::string tmp;
		std::string str =
			NumberFormatter::format(_ssn);
		tmp.clear();
		tmp.append(str, 0, 3);
		tmp += '-';
		tmp.append(str, 3, 2);
		tmp += '-';
		tmp.append(str, 5, 4);
		return tmp;
	}

	int _ssn;
};


namespace Poco {


template <>
class DynamicAnyHolderImpl<SSN>: public DynamicAnyHolder
{
public:
	DynamicAnyHolderImpl(const SSN& val): _val(val)
	{
	}

	~DynamicAnyHolderImpl()
	{
	}
	
	const std::type_info& type() const
	{
		return typeid(SSN);
	}
	
	void convert(Int32& val) const
	{
		val = _val.nSSN();
	}

	void convert(std::string& val) const
	{
		val = _val.sSSN();
	}

	DynamicAnyHolder* clone() const
	{
		return new DynamicAnyHolderImpl(_val);
	}
	
	const SSN& value() const
	{
		return _val;
	}

private:
	DynamicAnyHolderImpl();
	SSN _val;
};


} // namespace Poco


#endif // UDT_HEADER_INCLUDED
