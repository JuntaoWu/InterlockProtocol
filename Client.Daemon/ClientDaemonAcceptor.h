#ifndef _CLIENT_DAEMON_ACCEPTOR_H
#define _CLIENT_DAEMON_ACCEPTOR_H

#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include "InputHandler.h"

class InputHandler;

class ClientDaemonAcceptor : public ACE_Acceptor<InputHandler, ACE_SOCK_ACCEPTOR>
{
protected:
	InputHandler input_handler_;
public:
	typedef ACE_Acceptor<InputHandler, ACE_SOCK_ACCEPTOR> base;
	ClientDaemonAcceptor();
	~ClientDaemonAcceptor();

	virtual int make_svc_handler(InputHandler * &handler);
	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask);

	ACE_INET_Addr local_address() { return peer_acceptor_addr_; }
};

#endif //!_CLIENT_DAEMON_ACCEPTOR_H