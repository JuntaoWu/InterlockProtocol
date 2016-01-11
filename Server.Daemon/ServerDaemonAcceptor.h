#ifndef _SERVER_DAEMON_ACCEPTOR_H
#define _SERVER_DAEMON_ACCEPTOR_H

#include <ace/SOCK_Acceptor.h>
#include <ace/Acceptor.h>
#include "../Common/MessageHandler.h"

class ServerDaemonAcceptor : public ACE_Acceptor<MessageHandler, ACE_SOCK_Acceptor>
{
public:
	ServerDaemonAcceptor();
	ServerDaemonAcceptor(ACE_Reactor *r) : base(r) {}
	virtual ~ServerDaemonAcceptor();  // Destructor frees the SSL resources.

	typedef ACE_Acceptor<MessageHandler, ACE_SOCK_ACCEPTOR> base;
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

#endif //!_SERVER_DAEMON_ACCEPTOR_H
