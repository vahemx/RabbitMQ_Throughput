#include "CustomConnectionHandler.h"

CustomConnectionHandler::CustomConnectionHandler(const AddressInfo & addressInfo)
	: m_sock(m_context),
	m_addressInfo(addressInfo)
{
	init(addressInfo);
}

void CustomConnectionHandler::init(const AddressInfo & addressInfo)
{
	try {
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(addressInfo.ip), addressInfo.port);
		m_sock.connect(ep);
	}
	catch (boost::system::system_error e) {
		m_quit = true;
		std::cout << e.code() << std::endl;
	}

}

CustomConnectionHandler::~CustomConnectionHandler()
{
	close();
}

void CustomConnectionHandler::onError( AMQP::Connection *connection, const char *message)
{
	std::cout << "AMQP error: " << message << std::endl;
}

void CustomConnectionHandler::onClosed(AMQP::Connection *connection)
{
	std::cout << "AMQP Connection closed" << std::endl;
	m_quit = true;
	close();
}

void CustomConnectionHandler::onReady(AMQP::Connection *connection)
{
	std::cout << "AMQP Connection Ready" << std::endl;
	m_isConnected = true;
}

bool CustomConnectionHandler::isConnected()
{
	return m_isConnected;
}

void CustomConnectionHandler::onData(AMQP::Connection *connection, const char *data, size_t size)
{
	m_pConnect = connection;

	try {
		boost::asio::write(m_sock, boost::asio::buffer(data, size));
	}
	catch (boost::system::system_error e) {
		std::cout << e.code() << std::endl;
	}
}

void CustomConnectionHandler::parseData()
{
	m_parseData.insert(m_parseData.end(), m_receivedBytes.begin(), m_receivedBytes.end());
	int parsed = m_pConnect->parse(m_parseData.data(), m_parseData.size());
	m_parseData.erase(m_parseData.begin(), m_parseData.begin() + parsed);

#if 0
	if (m_parseData.size() > 0)
	{
		std::cout << "DATA LEFT" << m_parseData.size() << std::endl;
	}
	else
	{
		std::cout << "OK" << std::endl;
	}
#endif
}

void CustomConnectionHandler::runLoop()
{
	try {
		while (!m_quit && m_sock.is_open())
		{
			int number_of_bytes_to_read = m_sock.available();
			if (number_of_bytes_to_read > 0)
			{
				m_receivedBytes.resize(number_of_bytes_to_read, 0);
				boost::asio::read(m_sock, boost::asio::buffer(m_receivedBytes, number_of_bytes_to_read));
				parseData();
			}
		}
	}
	catch (boost::system::system_error e) {
		std::cout << e.code() << std::endl;
	}
	
}

void CustomConnectionHandler::close()
{
	m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	m_sock.close();
}