// Client.Console.cpp : Defines the entry point for the console application.
//

#include "LoggingClient.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	cout << "Press Enter to start." << endl;
	cin.get();  //get enter key.
	u_short logger_port = argc > 1 ? atoi(argv[1]) : 0;
	const char *logger_host = argc > 2 ? argv[2] : ACE_DEFAULT_SERVER_HOST;

	int result;

	ACE_INET_Addr server_addr;

	if (logger_port != 0) {
		result = server_addr.set(logger_port, logger_host);
	}
	else {
		result = server_addr.set(7777, logger_host);
	}

	if (result == -1) {
		ACE_ERROR_RETURN((LM_ERROR, "lookup %s, %p\n", logger_port == 0 ? "ace_logger" : argv[1], logger_host), 1);
	}

	ACE_SOCK_Connector connector;
	LoggingClient client;

	if (connector.connect(client.peer(), server_addr) < 0) {
		ACE_ERROR_RETURN((LM_ERROR, "%p\n", "connect()"), 1);
	}

	cin.width(ACE_Log_Record::MAXLOGMSGLEN);

	while (true) {
		string user_input;
		getline(cin, user_input, '\n');
		if (!cin || cin.eof()) {
			break;
		}
		ACE_Time_Value now(ACE_OS::gettimeofday());
		ACE_Log_Record record(LM_INFO, now, ACE_OS::getpid());
		record.msg_data(user_input.c_str());

		if (client.send(record) == -1) {
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "client.send()"), 1);
		}
	}
	return 0;
}

