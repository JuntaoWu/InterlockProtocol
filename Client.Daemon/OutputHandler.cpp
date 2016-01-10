#include "OutputHandler.h"

OutputHandler::OutputHandler()
{
}


OutputHandler::~OutputHandler()
{
}

int OutputHandler::open(void *connector)
{
	connector_ = static_cast<ClientDaemonConnector*>(connector);
	int buf_size = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
	peer().set_option(SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof buf_size);

	if (reactor()->register_handler(this, ACE_Event_Handler::READ_MASK) == -1) {
		return -1;
	}

	if (msg_queue()->activate() == ACE_Message_Queue_Base::ACTIVATED) {
		msg_queue()->high_water_mark(QUEUE_MAX);
		return activate(THR_SCOPE_SYSTEM);
	}
	else {
		return 0;
	}

}

int OutputHandler::put(ACE_Message_Block * block, ACE_Time_Value * timeout)
{
	int result;
	while ((result = putq(block, timeout)) == -1) {
		if (msg_queue()->state() != ACE_Message_Queue_Base::PULSED) {
			break;
		}
	}
	return result;
}

int OutputHandler::svc()
{
	ACE_Message_Block *chunk[ACE_IOV_MAX];
	size_t message_index = 0;
	ACE_Time_Value time_of_last_send(ACE_OS::gettimeofday());
	ACE_Time_Value timeout;

	ACE_Sig_Action no_sigpipe((ACE_SignalHandler)SIG_IGN);
	ACE_Sig_Action original_action;
	no_sigpipe.register_action(SIGPIPE, &original_action);

	while (true) {
		if (message_index == 0) {
			timeout = ACE_OS::gettimeofday();
			timeout += FLUSH_TIMEOUT;
		}
		ACE_Message_Block *block = 0;
		if (getq(block, &timeout) == -1) {
			if (errno == ESHUTDOWN) {
				if (connector_->reconnect() == -1) {
					break;
				}
				continue;
			}
			else if (errno != EWOULDBLOCK) {
				break;
			}
			else if (message_index == 0) {
				continue;
			}
		}
		else {
			if (block->size() == 0 && block->msg_type() == ACE_Message_Block::MB_STOP) {
				block->release();
				break;
			}
			chunk[message_index] = block;
			++message_index;
		}

		if (message_index >= ACE_IOV_MAX || (ACE_OS::gettimeofday() - time_of_last_send >= FLUSH_TIMEOUT)) {
			if (send(chunk, message_index) == -1) {
				break;
			}
			time_of_last_send = gettimeofday();
		}
	}

	if (message_index > 0) {
		send(chunk, message_index);
	}
	no_sigpipe.restore_action(SIGPIPE, original_action);
	return 0;
}
