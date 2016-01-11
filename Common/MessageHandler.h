#pragma once
#include <string>
#include <ace/Acceptor.h>
#include <ace/FILE_IO.h>
#include <ace/FILE_Connector.h>
#include <ace/SOCK_Stream.h>

#include "LogicHandler.h"

class MessageHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>
{
protected:
	LogicHandler handler_;

public:
	MessageHandler() : handler_() {}
	~MessageHandler();

	virtual int open(void *);
	virtual int svc();
};

