#pragma once
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/write.hpp>


using asio::ip::tcp;
using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
namespace this_coro = asio::this_coro;

class ZWebServer
{
public:
	bool start();
	awaitable<void> echo(tcp::socket socket);

	awaitable<void> listener();
private:
	asio::io_context io_context{ 1 };
};

