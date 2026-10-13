#include "proxy/proxy.h"
#include "spdlog/spdlog.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

class Dog
{
public:
	void Eat() const
	{
		SPDLOG_INFO("Dog is eating");
	}
	string Name() const
	{
		return "Dog";
	}
};

class Cat
{
public:
	void Eat() const
	{
		SPDLOG_INFO("Cat is eating");
	}
	string Name() const
	{
		return "Cat";
	}
};

PRO_DEF_MEM_DISPATCH(MemEat, Eat);
PRO_DEF_MEM_DISPATCH(MemName, Name);

class AnimalProxy : public pro::facade_builder
	::add_convention<MemEat, void() const>
	::add_convention<MemName, string() const>
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
		SPDLOG_INFO("Animal name: {}", animal->Name());
	}
	SPDLOG_INFO("hello,world");
	return 0;
}
