#pragma once
#include <ace/OS.h>
#include <ace/CDR_Stream.h>

#include <ace/INET_Addr.h>
#include <ace/Sock_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/Log_Record.h>
#include <ace/Stream.h>
#include <string>

class LoggingClient
{
private:
	ACE_SOCK_Stream logging_peer_;
public:
	LoggingClient() {}
	~LoggingClient() { logging_peer_.close(); }
	
	int send(const ACE_Log_Record &log_record);

	ACE_SOCK_Stream &peer() { return logging_peer_; }
};

