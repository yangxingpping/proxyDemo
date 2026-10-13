#include <proxy/proxy.h>
#include <spdlog/spdlog.h>
#include <cpr/cpr.h>

#include "asio/asio.hpp"

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
		return _name;
	}
	void setName(string newName)
	{
		_name = newName;
	}
private:
	string _name{ "Dog" };
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
		return _name;
	}
	void setName(string newName)
	{
		_name = newName;
	}
private:
	string _name{ "Cat" };
};

PRO_DEF_MEM_DISPATCH(MemEat, Eat);
PRO_DEF_MEM_DISPATCH(MemName, Name);
PRO_DEF_MEM_DISPATCH(MemSetName, setName);

class AnimalProxy : public pro::facade_builder
	::add_convention<MemEat, void() const>
	::add_convention<MemName, string() const>
	::add_convention<MemSetName, void(string) >
	::build
{
};

void proxy_test()
{
	vector<pro::proxy<AnimalProxy>> animals;
	animals.push_back(pro::make_proxy<AnimalProxy, Dog>());
	animals.push_back(pro::make_proxy<AnimalProxy, Cat>());
	for (auto& animal : animals)
	{
		animal->Eat();
		SPDLOG_INFO("Animal name: {}", animal->Name());
		animal->setName(string("newName"));
		SPDLOG_INFO("Animal New name: {}", animal->Name());
	}
}

void cpr_test()
{
	cpr::Response r = cpr::Get(cpr::Url{ "http://172.20.10.5" },
		cpr::Authentication{ "user", "pass", cpr::AuthMode::BASIC },
		cpr::Parameters{ {"anon", "true"}, {"key", "value"} });
	r.status_code;                  // 200
	r.header["content-type"];       // application/json; charset=utf-8
	auto resp = r.text;                         // JSON text string

	int j{ 0 };
}

int main(void)
{
	proxy_test();
	cpr_test();

	SPDLOG_INFO("hello,world");
	return 0;
}
