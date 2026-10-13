#include "UWebServer.h"
#include <asio/co_spawn.hpp>
#include <asio/stream_file.hpp>
#include <asio/detached.hpp>
#include <asio/executor_work_guard.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/signal_set.hpp>
#include <asio/write.hpp>

#include <thread>
#include <string>
using std::string;

using asio::ip::tcp;
using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
namespace this_coro = asio::this_coro;

namespace
{
	asio::io_context& file_io()
	{
		static asio::io_context io;
		static bool started = [] {
			std::thread([] {
				auto work = asio::make_work_guard(io);
				io.run();
			}).detach();
			return true;
		}();
		(void)started;
		return io;
	}
}

asio::awaitable<std::vector<char>> async_read_file(const std::string& filepath) {
	std::vector<char> content;
	try {
		auto ex = co_await this_coro::executor;
		asio::stream_file file(ex, filepath, asio::stream_file::read_only);
		content.resize(static_cast<size_t>(file.size()));
		co_await asio::async_read(file, asio::buffer(content), asio::use_awaitable);
	}
	catch (const std::exception&) {

	}
	co_return content;
}

bool UWebServer::start()
{
	uWS::Loop* loop = uWS::Loop::get();
    _app.get("/*", [loop](auto* res, auto* req) {
        std::string path = "public" + std::string(req->getUrl());
		string str{ "hello,world" };
		res->end(str.data(), str.length());
		//     asio::co_spawn(
			//file_io(),
			//async_read_file(path),
			//// Runs on the asio thread; defer bounces back to the uWS loop
			//// thread, which is the only thread allowed to touch res.
   //         [loop, res](std::exception_ptr eptr, std::vector<char> content) {
			//	loop->defer([res, eptr, content = std::move(content)]() mutable {
	  //              if (eptr) {
	  //                  res->writeStatus("500 Internal Server Error")->end("Read error");
	  //              }
	  //              else {
	  //                  res->writeHeader("Content-Type", "text/html; charset=utf-8")
	  //                      ->end({ content.data(), content.size() });
	  //              }
			//	});
   //         }
   //     );
        });
	_app.listen(9001, [](auto* listenSocket) {

		if (listenSocket) {
			std::cout << "Listening on port " << 9001 << std::endl;
		}
		else {
			std::cout << "Failed to load certs or to bind to port" << std::endl;
		}

		}).run();
	return true;
}
