#include "stdafx.h"
#include "ServerDaemon.h"
#include <ace/Service_Config.h>
#include "ace/Get_Opt.h"

ServerDaemon::~ServerDaemon()
{
}


int ServerDaemon::init(int argc, ACE_TCHAR * argv[])
{
	u_short server_daemon_port = ACE_DEFAULT_LOGGING_SERVER_PORT;

	ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("r:"), 0);
	get_opt.long_option(ACE_TEXT("server_port"), 'r', ACE_Get_Opt::ARG_REQUIRED);

	for (int c; (c = get_opt()) != -1; ) {
		switch (c)
		{
		case 'r':
			server_daemon_port = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		default:
			break;
		}
	}

	ACE_INET_Addr server_daemon_addr(server_daemon_port);

	if (acceptor_.open(server_daemon_addr) == -1) {
		return -1;
	}
	local_addr_ = server_daemon_addr;

	return 0;
}

int ServerDaemon::fini() {
	reactor()->remove_handler
		(this,
			ACE_Event_Handler::ACCEPT_MASK
			| ACE_Event_Handler::DONT_CALL);
	return acceptor_.close();
}


int ServerDaemon::info(ACE_TCHAR **bufferp,
	size_t length) const
{

	ACE_TCHAR buf[BUFSIZ];
	ACE_OS::sprintf
		(buf, ACE_TEXT("%hu"), local_addr_.get_port_number());
	ACE_OS::strcat
		(buf, ACE_TEXT("/tcp # lists services in daemon\n"));
	if (*bufferp == 0)
		*bufferp = ACE::strnew(buf);
	else
		ACE_OS::strncpy(*bufferp, buf, length);

	return ACE_Utils::truncate_cast<int> (ACE_OS::strlen(*bufferp));
}

int ServerDaemon::suspend()
{
	return reactor()->suspend_handler(this);
}

int ServerDaemon::resume()
{
	return reactor()->resume_handler(this);
}


ACE_FACTORY_DEFINE(ACE_Local_Service, ServerDaemon)

// Define the ACE_Static_Svc_Descriptor that conveys the service information
// to the ACE_Service_Config.
ACE_STATIC_SVC_DEFINE(
	Server_Descriptor,
	ACE_TEXT("Server_Daemon"),
	ACE_SVC_OBJ_T,
	&ACE_SVC_NAME(ServerDaemon),
	ACE_Service_Type::DELETE_THIS
	| ACE_Service_Type::DELETE_OBJ,
	0 // This object is not initially active.
	)

	// Define the class that will register this service with ACE_Service_Config
	// at program startup.
	ACE_STATIC_SVC_REQUIRE(Server_Descriptor)