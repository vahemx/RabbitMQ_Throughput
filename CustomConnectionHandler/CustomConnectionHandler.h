#ifndef _CUSTOM_CONNECTION_HANDLER
#define _CUSTOM_CONNECTION_HANDLER

#include <amqpcpp.h>
#include <string>
#include <boost/asio.hpp>

typedef struct _addressInfo
{
	std::string ip;
	unsigned short port;
	std::string loginName;
	std::string loginPwd;
} AddressInfo;


class CustomConnectionHandler : public AMQP::ConnectionHandler
{
public:
	CustomConnectionHandler(const AddressInfo & addressInfo);
	virtual ~CustomConnectionHandler();
	CustomConnectionHandler(const CustomConnectionHandler&) = delete;
	CustomConnectionHandler& operator=(const CustomConnectionHandler&) = delete;

	virtual void onData(AMQP::Connection *connection, const char *data, size_t size);
	virtual void onReady(AMQP::Connection *connection);
	virtual void onError(AMQP::Connection *connection, const char *message);
	virtual void onClosed(AMQP::Connection *connection);

	bool isConnected();
	void runLoop();

private:
	AMQP::Connection* m_pConnect;
	boost::asio::io_context m_context;
	boost::asio::ip::tcp::socket m_sock;
	AddressInfo m_addressInfo;
	bool m_quit = false;
	bool m_isConnected = false;
	std::vector<char> m_receivedBytes;
	std::vector<char> m_parseData;
	
	void init(const AddressInfo & addressInfo);
	void parseData();
	void close();
};

#endif
