#include "proxy/proxy.h"
#include "spdlog/spdlog.h"

#include <vector>
using std::vector;

class Dog
{
public:
	void Eat() const
	{
		SPDLOG_INFO("Dog is eating");
	}
};

class Cat
{
public:
	void Eat() const
	{
		SPDLOG_INFO("Cat is eating");
	}
};

PRO_DEF_MEM_DISPATCH(MemName, Eat);

class AnimalProxy : public pro::facade_builder
	::add_convention<MemName, void() const>
	::build
{

};

int main(void)
{
	vector<pro::proxy<AnimalProxy>> animals;
	animals.push_back(pro::make_proxy<AnimalProxy, Dog>());
	animals.push_back(pro::make_proxy<AnimalProxy, Cat>());
	for (auto& animal : animals)
	{
		animal->Eat();
	}
	SPDLOG_INFO("hello,world");
	return 0;
}
