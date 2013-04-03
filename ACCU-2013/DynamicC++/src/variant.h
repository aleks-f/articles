#ifndef DynamicCPP_variant_included
#define DynamicCPP_variant_included


#include "boost/variant/variant.hpp"
#include "boost/variant/get.hpp"
#include <vector>

template <typename T>
void printSize()
{
	boost::variant<T> v;
	std::string name(typeid(T).name());
	std::cout << "sizeof " << name << ": " << sizeof(T) << ", variant<"  << name << ">: " << sizeof(boost::variant<T>) << std::endl;
}

void doVariant(const std::vector<std::string>&);

#endif // DynamicCPP_variant_included
