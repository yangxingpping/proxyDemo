#include "UWebServer.h"
#include <asio/co_spawn.hpp>
#include <asio/stream_file.hpp>
#include <asio/detached.hpp>
#include <asio/executor_work_guard.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/write.hpp>

#include <thread>


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

static asio::io_context* _io{ nullptr };

asio::awaitable<std::vector<char>> async_read_file(const std::string& filepath) {
	std::vector<char> content;
	try {
		asio::stream_file file(*_io, filepath, asio::stream_file::read_only);
		//file.seek(0, SEEK_END);
		auto size = file.size();
		//file.seek(0, SEEK_SET);
		content.resize(static_cast<size_t>(size));
		co_await file.async_read_some(asio::buffer(content), asio::use_awaitable);
		//co_await asio::async_read(file, asio::buffer(content), asio::use_awaitable);
	}
	catch (const std::exception&) {
		
	}
	co_return content;
}

bool UWebServer::start()
{

	bool ret{ true };
	_io = &file_io();
    _app.get("/*", [](auto* res, auto* req) {
        std::string path = "public" + std::string(req->getUrl());
        asio::co_spawn(
			*_io,
			async_read_file(path),
            [res](std::exception_ptr eptr, std::vector<char> content) {
                if (eptr) {
                    res->writeStatus("500 Internal Server Error")->end("Read error");
                }
                else {
                    res->writeHeader("Content-Type", "text/html; charset=utf-8")
                        ->end({ content.data(), content.size() });
                }
            }
        );
        });
	return ret;
}
