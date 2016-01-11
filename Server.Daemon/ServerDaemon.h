#ifndef _SERVER_DAEMON_H_
#define _SERVER_DAEMON_H_


#include <ace/Service_Object.h>
#include <ace/INET_Addr.h>

#include "ServerDaemonAcceptor.h"

class ServerDaemon : public ACE_Service_Object
{
private:
	ACE_INET_Addr local_addr_;
protected:
	ServerDaemonAcceptor acceptor_;

public:
	ServerDaemon() {}
	~ServerDaemon();

	virtual int init(int argc, ACE_TCHAR *argv[]);
	virtual int fini();
	virtual int info(ACE_TCHAR **bufferp, size_t length = 0) const;
	virtual int suspend();
	virtual int resume();
};

#endif // !_SERVER_DAEMON_H_