#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "CustomConnectionHandler.h"

void messageThread(const int & MaxFrequency, AMQP::Channel *channel);
int parseArguments(int argc, char *argv[], int & MaxFrequency, std::string &ip_address);

#endif
