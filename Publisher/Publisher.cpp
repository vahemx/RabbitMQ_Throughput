#include "CustomConnectionHandler.h"
#include "functions.h"



int main(int argc, char *argv[])
{
	int MaxFrequency;
	std::string ip_address;
	int ret = parseArguments(argc, argv, MaxFrequency, ip_address);
	if (ret != EXIT_SUCCESS)
	{
		return ret;
	}


	AddressInfo info;
	info.ip = ip_address;
	info.port = 5672;
	info.loginName = "guest";
	info.loginPwd = "guest";

	CustomConnectionHandler handler(info);

	std::thread thread;
	AMQP::Connection connection(&handler, AMQP::Login(info.loginName, info.loginPwd));
	AMQP::Channel channel(&connection);

	channel.onError([](const char *message) {
		std::cout << message << std::endl;
	});

	channel.onReady([&]()
	{
		if (handler.isConnected())
		{
			channel.declareExchange("Custom_Exchange", AMQP::fanout)
				.onError([&](const char *message) {
				std::cout << message << std::endl;
			});

			channel.declareQueue("Custom_Queue").onError([](const char *message) {
				std::cout << message << std::endl;
			});

			channel.bindQueue("Custom_Exchange", "Custom_Queue", "Custom_Routing_Key")
				.onError([&](const char *message) {
				std::cout << message << std::endl;
			})
				.onSuccess([&]() {
				std::thread msgThread(&messageThread, MaxFrequency, &channel);
				msgThread.detach();
			});
		}
	});
	handler.runLoop();

	return EXIT_SUCCESS;
}

void close();