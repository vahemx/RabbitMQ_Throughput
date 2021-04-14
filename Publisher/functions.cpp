#include "functions.h"

static bool isNumber(const std::string &str)
{
	return !str.empty() &&
		(str.find_first_not_of("[0123456789]") == std::string::npos);
}

static std::vector<std::string> split(const std::string &str, char delim)
{
	auto i = 0;
	std::vector<std::string> list;

	auto pos = str.find(delim);

	while (pos != std::string::npos)
	{
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}

	list.push_back(str.substr(i, str.length()));

	return list;
}

static bool validateIP(std::string ip)
{
	std::vector<std::string> list = split(ip, '.');

	// if the token size is not equal to four
	if (list.size() != 4) {
		return false;
	}

	// validate each token
	for (std::string str : list)
	{
		// verify that the string is a number or not, and the numbers
		// are in the valid range
		if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0) {
			return false;
		}
	}

	return true;
}

void messageThread(const int & MaxFrequency, AMQP::Channel *channel)
{
	int index = 1;
	while (true)
	{
		int processed_messages = 0;
		std::chrono::milliseconds int_ms;
		std::uint64_t ms = 0;
		auto start = std::chrono::high_resolution_clock::now();
		while (ms < 1000 && processed_messages < MaxFrequency)
		{
			std::string message = "Hello World<" + std::to_string(index) + ">";
			channel->publish("Custom_Exchange", "Custom_Routing_Key", message);
			std::cout << "published message: " << message << std::endl;
			++index;
			++processed_messages;
			auto end = std::chrono::high_resolution_clock::now();
			int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			ms = int_ms.count();
		}
		std::cout << std::endl;

		auto end = std::chrono::high_resolution_clock::now();
		int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		int diff = static_cast<int> (1000 - int_ms.count());
		if (diff > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(diff));
		}

	}
}

int parseArguments(int argc, char *argv[], int & MaxFrequency, std::string & ip_address)
{
	auto usage = [&]() {
		std::cout << "Usage:\t" << argv[0] << "\t<N>\t[IP-address]" << std::endl <<
			"N - is the Max number of messages to send per second" << std::endl <<
			"IP-address - RabbitMQ server address" << std::endl;
	};

	if (argc == 3)
	{
		std::string number_input(argv[1]);
		std::string ip_input(argv[2]);
		if ( !isNumber(number_input) || !validateIP(ip_input) )
		{
			std::cout << "Invalid input." << std::endl;
			usage();
			return EXIT_FAILURE;
		}
		ip_address = ip_input;
		MaxFrequency = MaxFrequency = std::atoi(number_input.c_str());
	}
	else if (argc == 2)
	if (argc == 2)
	{
		std::string input(argv[1]);
		if (input.compare("--help") == 0 || input.compare("-h") == 0)
		{
			usage();
			return EXIT_SUCCESS;
		}

		if (!isNumber(input))
		{
			std::cout << "Invalid input." << std::endl;
			usage();
			return EXIT_FAILURE;
		}

		MaxFrequency = std::atoi(input.c_str());
		ip_address = "127.0.0.1";
	}
	else
	{
		std::cout << "Invalid input." << std::endl;
		usage();
		return EXIT_FAILURE;
	}

	if (MaxFrequency <= 0)
	{
		std::cout << "Invalid input." << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
