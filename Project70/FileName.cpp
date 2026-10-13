#include <proxy/proxy.h>
#include <spdlog/spdlog.h>
#include <cpr/cpr.h>

#include "asio.hpp"
#include "ZWebServer.h"

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

struct write_some_handler
{
	write_some_handler() {}
	void operator()(const asio::error_code&, std::size_t) {}
	write_some_handler(write_some_handler&&) {}
private:
	write_some_handler(const write_some_handler&);
};

struct read_some_handler
{
	read_some_handler() {}
	void operator()(const asio::error_code&, std::size_t) {}
	read_some_handler(read_some_handler&&) {}
private:
	read_some_handler(const read_some_handler&);
};

void asio_file_test()
{
	using namespace asio;

	try
	{
		io_context ioc;
		const io_context::executor_type ioc_ex = ioc.get_executor();
		char mutable_char_buffer[128] = "";
		const char const_char_buffer[128] = "";
		asio::error_code ec;
		const std::string path;

		stream_file file1(ioc, "xyz.txt", stream_file::read_only);

		stream_file::executor_type ex = file1.get_executor();
		(void)ex;

		//file1.open("", stream_file::read_only, ec);


		bool is_open = file1.is_open();
	/*
		file1.close();
		file1.close(ec);*/
		asio::uint64_t s1 = file1.size();
		(void)s1;
		asio::uint64_t s2 = file1.size(ec);
		(void)s2;
		//file1.sync_all();
		//file1.sync_all(ec);
		//file1.sync_data();
		//file1.sync_data(ec);
		/*asio::uint64_t s3 = file1.seek(0, stream_file::seek_set);
		(void)s3;
		asio::uint64_t s4 = file1.seek(0, stream_file::seek_set, ec);
		(void)s4;
		file1.write_some(buffer(mutable_char_buffer));
		file1.write_some(buffer(const_char_buffer));
		file1.write_some(buffer(mutable_char_buffer), ec);
		file1.write_some(buffer(const_char_buffer), ec);
		file1.async_write_some(buffer(mutable_char_buffer), write_some_handler());
		file1.async_write_some(buffer(const_char_buffer), write_some_handler());*/
		//file1.async_read_some()
		
	}
	catch (std::exception& e)
	{
		auto errdesc = e.what();
		int j{ 0 };
	}
}

void asio_block_test()
{
	ZWebServer server;
	server.start();
}

int main(void)
{
	proxy_test();
	cpr_test();
	asio_file_test();
	//asio_block_test();
	SPDLOG_INFO("hello,world");
	return 0;
}
