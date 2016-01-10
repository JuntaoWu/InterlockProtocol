#include "stdafx.h"
#include "MessageAcceptor.h"


MessageAcceptor::MessageAcceptor()
{
}


MessageAcceptor::~MessageAcceptor()
{
	/*SSL_free(this->ssl_);
	SSL_CTX_free(this->ssl_ctx_);*/
}

int MessageAcceptor::open(const ACE_SOCK_Acceptor::PEER_ADDR & local_addr, ACE_Reactor * reactor, int flags, int use_select, int reuse_addr)
{
	if (PARENT::open(local_addr, reactor, flags, use_select, reuse_addr) != 0) {
		return -1;
	}
	//Process the SSL authenticate

	//end the process.
	return 0;
}

int MessageAcceptor::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
	PARENT::handle_close(h, mask);
	delete this;
	return 0;
}

int MessageAcceptor::accept_svc_handler(MessageHandler * sh)
{
	if (PARENT::accept_svc_handler(sh) == -1) {
		return -1;
	}
	
//	SSL_clear(ssl_);  // Reset for new SSL connection.
//
//#if defined (ACE_WIN32)
//					  // ACE_WIN32 is the only platform where ACE_HANDLE is not an int.
//					  // See ace/config-lite.h for the typedefs.
//	SSL_set_fd(ssl_, reinterpret_cast<int> (sh->get_handle()));
//#else
//	SSL_set_fd(ssl_, sh->get_handle());
//#endif /* ACE_WIN32 */
//
//	SSL_set_verify(ssl_,
//			SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
//			0);
//
//	if (SSL_accept(ssl_) == -1 || SSL_shutdown(ssl_) == -1) {
//		return -1;
//	}

	return 0;
}
