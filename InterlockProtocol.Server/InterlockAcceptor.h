#pragma once
#include <ace/Acceptor.h>

class InterlockAcceptor : ACE_Event_Handler
{
public:
	InterlockAcceptor();
	~InterlockAcceptor();
};

