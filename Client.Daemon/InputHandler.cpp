#include "InputHandler.h"

InputHandler::~InputHandler()
{
}

int InputHandler::open(void *)
{
	ACE_HANDLE handle = peer().get_handle();
	if (reactor()->register_handler(handle, this, ACE_Event_Handler::READ_MASK) == -1) {
		return -1;
	}
	connected_clients_.set_bit(handle);
	return 0;
}

int InputHandler::close(u_int)
{
	ACE_Message_Block *shutdown_message = 0;
	ACE_NEW_RETURN(shutdown_message,
		ACE_Message_Block(0, ACE_Message_Block::MB_STOP),
		-1);
	output_handler_->put(shutdown_message);

	reactor()->remove_handler(connected_clients_, ACE_Event_Handler::READ_MASK);
	return output_handler_->wait();
}

int InputHandler::handle_input(ACE_HANDLE handle)
{
	ACE_Message_Block *block = 0;
	LogicHandler logic_handler(handle);

	if (logic_handler.ReceiveRecord(block) != -1) {
		if (output_handler_->put(block->cont()) != -1) {
			block->cont(0);
			block->release();
			return 0;
		}
		else {
			block->release();
		}
	}

	return -1;
}

int InputHandler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask)
{
	connected_clients_.clr_bit(handle);
	return ACE_OS::closesocket(handle);
}
