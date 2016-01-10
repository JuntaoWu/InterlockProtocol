#pragma once
#include <ace/Acceptor.h>
#include <ace/INET_Addr.h>
#include <ace/Reactor.h>
#include <ace/Svc_Handler.h>
#include <ace/FILE_IO.h>
#include <ace/Acceptor.h>
#include <ace/Reactor.h>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include "MessageHandlerBase.h"

class InterlockHandler
	: public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH> {
protected:
	ACE_FILE_IO log_file_; // File of log records.

						   // Connection to peer service handler.
	MessageHandlerBase logging_handler_;

public:
	InterlockHandler() : logging_handler_() {}

	virtual int open(void *);

	virtual int svc(void) {
		for (;;)
			switch (logging_handler_.log_record()) {
			case -1: return -1; // Error.
			case 0: return 0;   // Client closed connection.
			default: continue;  // Default case.
			}
		/* NOTREACHED */
		return 0;
	}
};