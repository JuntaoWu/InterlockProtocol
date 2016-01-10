#pragma once
#ifndef _MESSAGE_ACCEPTOR_H
#define _MESSAGE_ACCEPTOR_H

#include <ace/SOCK_Acceptor.h>
#include <ace/Acceptor.h>
#include "MessageHandler.h"

class MessageAcceptor : public ACE_Acceptor<MessageHandler, ACE_SOCK_Acceptor>
{
public:
	MessageAcceptor();
	MessageAcceptor(ACE_Reactor *r) : PARENT(r) {}
	
	virtual ~MessageAcceptor();  // Destructor frees the SSL resources.

	typedef ACE_Acceptor<MessageHandler, ACE_SOCK_ACCEPTOR> PARENT;
	typedef ACE_SOCK_Acceptor::PEER_ADDR PEER_ADDR;

	// Initialize the acceptor instance.
	virtual int open(const ACE_SOCK_Acceptor::PEER_ADDR &local_addr,
		ACE_Reactor *reactor = ACE_Reactor::instance(),
		int flags = 0, int use_select = 1, int reuse_addr = 1);

	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE,
		ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

	// Connection establishment and authentication hook method.
	virtual int accept_svc_handler(MessageHandler *sh);
};

#endif 
