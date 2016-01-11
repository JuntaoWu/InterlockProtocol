#ifndef _CLIENT_DAEMON_H_
#define _CLIENT_DAEMON_H_

#include <ace/Service_Object.h>
#include <ace/Get_Opt.h>
#include "ClientDaemonAcceptor.h"
#include "ClientDaemonConnector.h"
#include "OutputHandler.h"

class ClientDaemon : public ACE_Service_Object
{
private:
	ACE_INET_Addr local_addr_;
protected:
	OutputHandler output_handler_;
	ClientDaemonAcceptor acceptor_;
	ClientDaemonConnector connector_;
public:
	ClientDaemon() : acceptor_(&output_handler_), connector_(&output_handler_) {}
	~ClientDaemon();

	virtual int init(int argc, ACE_TCHAR *argv[]);
	virtual int fini();
	virtual int info(ACE_TCHAR **bufferp, size_t length = 0) const;
	virtual int suspend();
	virtual int resume();
};

#endif // !_CLIENT_DAEMON_H_
