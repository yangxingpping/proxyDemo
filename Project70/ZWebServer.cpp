#include "ZWebServer.h"

awaitable<void> ZWebServer::echo(tcp::socket socket)
{
	try
	{
		char data[1024];
		for (;;)
		{
			std::size_t n = co_await socket.async_read_some(asio::buffer(data), use_awaitable);
			co_await async_write(socket, asio::buffer(data, n), use_awaitable);
		}
	}
	catch (std::exception& e)
	{
		std::printf("echo Exception: %s\n", e.what());
	}
}

awaitable<void> ZWebServer::listener()
{
	auto executor = co_await this_coro::executor;
	tcp::acceptor acceptor(executor, { tcp::v4(), 55555 });
	for (;;)
	{
		tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
		co_spawn(executor, echo(std::move(socket)), detached);
	}
}

bool ZWebServer::start()
{
	bool ret{ true };
	asio::signal_set signals(io_context, SIGINT, SIGTERM);
	signals.async_wait([&](auto, auto) { io_context.stop(); });

	co_spawn(io_context, listener(), detached);

	io_context.run();
	return ret;
}
