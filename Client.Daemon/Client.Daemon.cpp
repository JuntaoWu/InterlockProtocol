// Client.Daemon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Select_Reactor.h>
#include <ace/Service_Config.h>

#include "ClientDaemon.h"

int main(int argc, char *argv[])
{
	ACE_Select_Reactor *select_reactor;
	ACE_NEW_RETURN(select_reactor, ACE_Select_Reactor, 1);
	ACE_Reactor *reactor;
	ACE_NEW_RETURN(reactor, ACE_Reactor(select_reactor, 1), 1);
	ACE_Reactor::close_singleton();
	ACE_Reactor::instance(reactor, 1);

	ACE_Service_Config::open(argc, argv);

	ACE_Reactor::instance()->run_reactor_event_loop();
    return 0;
}

