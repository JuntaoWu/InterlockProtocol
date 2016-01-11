#include "ClientDaemonAcceptor.h"



ClientDaemonAcceptor::ClientDaemonAcceptor(OutputHandler *handler) : output_handler_(handler), input_handler_(handler)
{
}


ClientDaemonAcceptor::~ClientDaemonAcceptor()
{
}

int ClientDaemonAcceptor::make_svc_handler(InputHandler *& handler)
{
	handler = &input_handler_;
	return 0;
}

int ClientDaemonAcceptor::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
	base::handle_close(handle, mask);
	input_handler_.close();
	return 0;
}
