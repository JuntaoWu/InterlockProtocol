#ifndef _OUTPUT_HANDLER_H_
#define _OUTPUT_HANDLER_H_

#include <ace/OS.h>
#include <ace/Get_Opt.h>
#include <ace/Connector.h>
#include <ace/SOCK_Connector.h>
#include <ace/Svc_Handler.h>
#include <ace/Synch.h>
#include <ace/Thread_Manager.h>
#include <ace/SOCK_Stream.h>
#include <ace/Handle_Set.h>

#include <ace/Log_Record.h>

#include <ace/Message_Block.h>
#include <ace/Time_Value.h>

#include <ace/Signal.h>

class ClientDaemonConnector;

class OutputHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>
{
protected:
	ClientDaemonConnector *connector_;

public:
	OutputHandler();
	~OutputHandler();

	enum { QUEUE_MAX = sizeof(ACE_Log_Record) * ACE_IOV_MAX };

	virtual int open(void *);

	virtual int put(ACE_Message_Block *block, ACE_Time_Value *time = 0);

protected:
	virtual int handle_input(ACE_HANDLE handle);

	virtual int svc();

	virtual int send(ACE_Message_Block *chunk[], size_t &count);
};

#if !defined(FLUSH_TIMEOUT)
#define FLUSH_TIMEOUT 120
#endif //!FLUSH_TIMEOUT

#endif // !_OUTPUT_HANDLER_H_
