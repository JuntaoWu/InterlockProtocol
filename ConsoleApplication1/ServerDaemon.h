#pragma once
#include <ace/Service_Object.h>

class ServerDaemon : public ACE_Service_Object
{
public:
	ServerDaemon();
	~ServerDaemon();
};

