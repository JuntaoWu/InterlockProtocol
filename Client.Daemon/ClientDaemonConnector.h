#ifndef _CLIENT_DAEMON_CONNECTOR_H_
#define _CLIENT_DAEMON_CONNECTOR_H_

#include <ace/Connector.h>
#include <ace/Sock_Connector.h>

#include "OutputHandler.h"

class ClientDaemonConnector : public ACE_Connector<OutputHandler, ACE_SOCK_Connector>
{
protected:
	OutputHandler *handler_;
	ACE_INET_Addr remote_addr_;
public:
	typedef ACE_Connector<OutputHandler, ACE_SOCK_Connector> PARENT;
	
	ClientDaemonConnector(OutputHandler *handler = 0) : handler_(handler) {}
	
	~ClientDaemonConnector();

	virtual int open(ACE_Reactor *r = ACE_Reactor::instance(), int flags = 0);

	virtual int reconnect();

protected:
	virtual int connect_svc_handler(OutputHandler *svc_handler,
		const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
		ACE_Time_Value *timeout,
		const ACE_SOCK_Connector::PEER_ADDR &local_addr,
		int reuse_addr, int flags, int perms);
};

#endif // !_CLIENT_DAEMON_CONNECTOR_H_