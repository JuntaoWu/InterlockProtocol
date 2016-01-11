#include "stdafx.h"
#include "ServerDaemon.h"
#include <ace/Service_Config.h>

ServerDaemon::ServerDaemon()
{
}


ServerDaemon::~ServerDaemon()
{
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