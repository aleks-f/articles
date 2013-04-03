#ifndef DynamicCPP_DynamicVar_included
#define DynamicCPP_DynamicVar_included

#include <vector>

void doDynamicVar(const std::vector<std::string>&);


template <typename T>
void printDynamicVarSize()
{
	T t;
	Poco::Dynamic::Var v(t);
	std::string name(typeid(T).name());
	std::cout << "sizeof " << name << ": " << sizeof(T) << ", Dynamic::Var("  << name << "): " << sizeof(v) << std::endl;
}


#endif // DynamicCPP_DynamicVar_included
