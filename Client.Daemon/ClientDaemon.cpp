#include "ClientDaemon.h"



ClientDaemon::ClientDaemon()
{
}


ClientDaemon::~ClientDaemon()
{
}

int ClientDaemon::init(int argc, ACE_TCHAR * argv[])
{
	u_short client_daemon_port = ACE_DEFAULT_SERVICE_PORT;
	u_short server_daemon_port = ACE_DEFAULT_LOGGING_SERVER_PORT;

	ACE_TCHAR server_daemon_host[MAXHOSTNAMELEN];
	ACE_OS_String::strcpy(server_daemon_host, ACE_LOCALHOST);
    
	ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("p:r:s:"), 0);
	get_opt.long_option(ACE_TEXT("client_port"), 'p', ACE_Get_Opt::ARG_REQUIRED);
	get_opt.long_option(ACE_TEXT("server_port"), 'r', ACE_Get_Opt::ARG_REQUIRED);
	get_opt.long_option(ACE_TEXT("server_name"), 's', ACE_Get_Opt::ARG_REQUIRED);

	for (int c; (c = get_opt()) != -1; ) {
		switch (c)
		{
		case 'p':
			client_daemon_port = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		case 'r':
			server_daemon_port = static_cast<u_short>(ACE_OS::atoi(get_opt.opt_arg()));
			break;
		case 's':
			ACE_OS_String::strsncpy(server_daemon_host, get_opt.opt_arg(), MAXHOSTNAMELEN);
			break;
		default:
			break;
		}
	}

	ACE_INET_Addr client_daemon_addr(client_daemon_port);
	ACE_INET_Addr server_daemon_addr(server_daemon_port, server_daemon_host);

	if (acceptor_.open(client_daemon_addr) == -1) {
		return -1;
	}

	OutputHandler *oh = &output_handler_;
	if (connector_.connect(oh, server_daemon_addr) == -1) {
		return -1;
	}

	return 0;
}

int ClientDaemon::fini() {
	reactor()->remove_handler
		(this,
			ACE_Event_Handler::ACCEPT_MASK
			| ACE_Event_Handler::DONT_CALL);
	return acceptor_.close();
}


int ClientDaemon::info(ACE_TCHAR **bufferp,
	size_t length) const {
	ACE_INET_Addr local_addr;
	acceptor_.get_local_addr(local_addr);

	ACE_TCHAR buf[BUFSIZ];
	ACE_OS::sprintf
		(buf, ACE_TEXT("%hu"), local_addr.get_port_number());
	ACE_OS::strcat
		(buf, ACE_TEXT("/tcp # lists services in daemon\n"));
	if (*bufferp == 0)
		*bufferp = ACE::strnew(buf);
	else
		ACE_OS::strncpy(*bufferp, buf, length);

	return ACE_Utils::truncate_cast<int> (ACE_OS::strlen(*bufferp));
}

int ClientDaemon::suspend()
{
	return reactor()->suspend_handler(this);
}

int ClientDaemon::resume()
{
	return reactor()->resume_handler(this);
}
