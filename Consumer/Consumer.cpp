#include "CustomConnectionHandler.h"
#include "functions.h"
#include <string>

int main(int argc, char *argv[])
{
	std::string ip_address;
	int ret = parseArguments(argc, argv, ip_address);
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

	AMQP::Connection connection(&handler, AMQP::Login(info.loginName, info.loginPwd));
	AMQP::Channel channel(&connection);

	channel.onError([](const char *message) {
		std::cout << message << std::endl;
	});

	channel.declareQueue("Custom_Queue");
	channel.consume("Custom_Queue").onReceived(
		[&](const AMQP::Message &message,
			uint64_t deliveryTag,
			bool redelivered)
	{
		std::cout << "Received " << std::string(message.body(), message.body() + message.bodySize()) << std::endl;
		channel.ack(deliveryTag);
	});

	std::cout << "Waiting for messages to consume and acknowledge" << std::endl;
	handler.runLoop();

	return EXIT_SUCCESS;
}