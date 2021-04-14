#include "functions.h"
#include <vector>
#include <iostream>

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

int parseArguments(int argc, char *argv[], std::string & ip_address)
{
	auto usage = [&]() {
		std::cout << "Usage:\t" << argv[0] << "\t[IP-address]" << std::endl <<
			"IP-address - RabbitMQ server address" << std::endl;
	};

	if (argc <= 2)
	{
		ip_address = "127.0.0.1";
		if (argc == 2)
		{
			std::string ip_input(argv[1]);
			if (!validateIP(ip_input))
			{
				std::cout << "Invalid input." << std::endl;
				usage();
				return EXIT_FAILURE;
			}
			ip_address = ip_input;
		}
	}
	else
	{
		std::cout << "Invalid input." << std::endl;
		usage();
		return EXIT_FAILURE;
	}
		
	return EXIT_SUCCESS;
}
