#pragma once
#include <string>
#include <ace/Acceptor.h>
#include <ace/FILE_IO.h>
#include <ace/FILE_Connector.h>
#include <ace/SOCK_Stream.h>

#include "LogicHandler.h"

class MessageHandler : ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>
{
protected:
	ACE_FILE_IO log_file_;
	LogicHandler handler_;

public:
	MessageHandler() : handler_(log_file_) {}
	~MessageHandler();

	virtual int open();
	virtual int svc();
};

