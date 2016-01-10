#include "MessageHandler.h"
using namespace std;

MessageHandler::~MessageHandler()
{
}

int MessageHandler::open()
{
	ACE_INET_Addr remote_addr;
	peer().get_remote_addr(remote_addr);
	
	string host_name = remote_addr.get_host_name();
	const char *filename = (host_name + ".log").c_str();

	ACE_FILE_Connector connector;
	connector.connect(log_file_, ACE_FILE_Addr(filename), 0, ACE_Addr::sap_any, 0, O_RDWR | O_CREAT | O_APPEND, ACE_DEFAULT_FILE_PERMS);

	handler_.SetHandle(peer().get_handle());

	return activate(THR_NEW_LWP | THR_DETACHED);
}

int MessageHandler::svc()
{
	while (true) {
		switch (handler_.HandleRecord())
		{
		case -1:
			return -1; //Error occurred.
		case 0:
			return 0;  //Connection closed.
		default:
			continue;  //Handle Next Record.
		}
	}
	ACE_NOTREACHED(return 0);
}
