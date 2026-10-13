#pragma once
#include <uwebsockets/App.h>
class UWebServer
{
public:
	bool start();
private:
	uWS::App _app;
};

