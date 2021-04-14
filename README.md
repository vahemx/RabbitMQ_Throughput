ABOUT
========
An example of two (Publisher/Consumer) C++ command line applications communicating each other through RabbitMQ Message Broker.
Consumer publishes indexed messages at a given rate to RabbitMQ server and the Consumer acknowledges and prints out the messages from the server.
Applications use Custom Connection Handler static library which was implemented with [AMQP-CPP](https://github.com/CopernicaMarketingSoftware/AMQP-CPP) C++ library as an underlying RabbitMQ Client.
Built and tested under Windows 10 x64 platform only.

PREREQUISITES
==========
1. Download and install Erlang from [here](https://www.erlang.org/downloads)
Please note that RabbitMQ requires a 64-bit supported version of Erlang for Windows 10 to be installed.

2. Download and Install the latest (v3.8.14) RabbitMQ Message broker on you system from [here](https://www.rabbitmq.com/download.html)

3. Download and install Microsoft Visual Studio 2017 Community version from [here](https://visualstudio.microsoft.com/vs/older-downloads/). 
This step may be skipped in case you have the newer Visual Studio installed on your system.
You can use other C++ Compilers at your own preferance but you will have to setup build dependencies first.
	
4. Download and install CMake from [here](https://cmake.org/download/)

5. Download and install AMQP-CPP RabbitMQ C++ Client library from [here](https://github.com/CopernicaMarketingSoftware/AMQP-CPP) and follow this steps:
* Run Command prompt in administrator mode and "cd" into the downloaded library "AMQP-CPP-master" directory
* Run these commands to build AMQP library:
```
mkdir build
cd build
cmake ..
cmake --build . --target install
```
* After above mentioned step the AMQP library and its headers should be copied to "C:\Program Files (x86)\amqpcpp" directory.
	
6. Download C++ boost libary version 1.74 from [here](https://sourceforge.net/projects/boost/files/boost-binaries/1.74.0/boost_1_74_0-msvc-14.1-64.exe/download)
* After running the installer, boost library sources should be extracted into "C:\local\boost_1_74_0" directory. Publisher/Consumer application build dependencies include this directory for using boost headers and libs, so please make sure that this is the location where boost is extracted to.
* Run Command Prompt and cd into the extracted "C:\local\boost_1_74_0" directory. 
* Run these commands to build boost library:
```
bootstrap.bat
.\b2
```

BUILD
==========
To build Publisher and Consumer applications:
* Open "rabbitMQ_Throughput.sln" visual studio solution file
* Build solution from the "solution explorer"
* Build will generate executable binaries in "Debug" directory.

Run RabbitMQ Server
==========
Before running the applications you need to configure and start RabbitMQ messaging server.
* Run Command Prompt in administrator mode and "cd" into installed rabbitMQ Server "sbin" directory (C:\Program Files\RabbitMQ Server\rabbitmq_server-3.8.14\sbin)
* To enable web user interface (User Management Plugin) for RabbitMQ, and start the server, use the following commands:
```
rabbitmq-plugins enable rabbitmq_management
rabbitmq-service.bat
```
* Now you should have access to server Web UI Interface through this URL "http://localhost:15672" 
* Web UI Interface uses "guest" both for Login and Password by default.

HOW TO USE
==========
Publisher - Usage:
```
Publisher <N> [IP-address]
```
N - is the maximum number of messages to send per second
[IP-address] - IP address of the server. If not provided, the system will use localhost "127.0.0.1" by default

Consumer - Usage:
```
Consumer [IP-address]
```
[IP-address] - IP address of the server. If not provided, the system will use localhost "127.0.0.1" by default


