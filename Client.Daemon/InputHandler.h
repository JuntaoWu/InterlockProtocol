#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_

#include <ace/OS.h>
#include <ace/Acceptor.h>
#include <ace/Get_Opt.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Svc_Handler.h>
#include <ace/Synch.h>
#include <ace/Thread_Manager.h>
#include <ace/SOCK_Stream.h>
#include <ace/Handle_Set.h>

#include <ace/Message_Block.h>
#include "OutputHandler.h"
#include "../Common/LogicHandler.h"

class OutputHandler;

class InputHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>
{
protected:
	OutputHandler *output_handler_;
	ACE_Handle_Set connected_clients_;

public:
	InputHandler(OutputHandler *handler = 0) : output_handler_(handler) {}
	~InputHandler();

	virtual int open(void *);
	virtual int close(u_int = 0);

protected:
	virtual int handle_input(ACE_HANDLE handle);
	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);
};

#endif // !_INPUT_HANDLER_H_