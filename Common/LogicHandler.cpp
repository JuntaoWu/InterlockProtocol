#include "LogicHandler.h"
#include <ace/FILE_Connector.h>
#include <ace/OS.h>
#include <ace/Message_Block.h>
#include <ace/CDR_Stream.h>
#include <ace/Log_Record.h>
#include <iostream>
using namespace std;

LogicHandler::LogicHandler() : log_file_(ACE_FILE_IO())
{

}

LogicHandler::LogicHandler(ACE_HANDLE handle)
{
	peer_.set_handle(handle);
}

LogicHandler::LogicHandler(const ACE_SOCK_Stream & peer)
	: peer_(peer)
{

}

inline int LogicHandler::Close()
{
	return peer_.close();
}

//block   => hostname  | payload
//payload => byteorder | length | ACE_Log_Record
int LogicHandler::ReceiveRecord(ACE_Message_Block *& block)
{
	ACE_INET_Addr peer_addr;
	peer_.get_remote_addr(peer_addr);

	block = new ACE_Message_Block(MAXHOSTNAMELEN + 1);
	peer_addr.get_host_name(block->wr_ptr(), MAXHOSTNAMELEN);

	block->wr_ptr(strlen(block->wr_ptr() + 1));

	ACE_Message_Block *payload = new ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE);
	ACE_CDR::mb_align(payload);

	if (peer_.recv_n(payload->wr_ptr(), 8) == 8) {
		payload->wr_ptr(8);

		ACE_InputCDR cdr(payload);

		ACE_CDR::Boolean byte_order;
		cdr >> ACE_InputCDR::to_boolean(byte_order);
		
		cdr.reset_byte_order(byte_order);
		
		ACE_CDR::ULong length;
		cdr >> length;

		ACE_CDR::grow(payload, 8 + ACE_CDR::MAX_ALIGNMENT + length);

		if (peer_.recv_n(payload->wr_ptr(), length) > 0) {
			payload->wr_ptr(length);
			block->cont(payload);

			//if (ACE::debug()) {
			Parse(block);
			//}

			return length;
		}
	}

	payload->release();
	block->release();
	payload = block = 0;
	return -1;
}

//block   => hostname  | payload
//payload => byteorder | length | ACE_Log_Record
void LogicHandler::Parse(ACE_Message_Block *block)
{
	char *hostname = block->rd_ptr();
	ACE_Message_Block *payload = block->cont();
	ACE_InputCDR input_cdr(payload);  //skip first block hostname.

    //!header
	ACE_CDR::Boolean byte_order;
	input_cdr >> ACE_InputCDR::to_boolean(byte_order);
	input_cdr.reset_byte_order(byte_order);

	ACE_CDR::ULong length;
	input_cdr >> length;
	//!end header

	ACE_Log_Record record;
	input_cdr >> record;

	record.print(hostname, ACE_Log_Msg::VERBOSE, cout);
	cout << endl;
}

int LogicHandler::WriteLogRecord(ACE_Message_Block * block)
{
	if (log_file_.send_n(block) == -1) {
		return -1;
	}
	if (ACE::debug()) {
		Parse(block);
	}
	return block->total_length();
}

int LogicHandler::HandleRecord()
{
	ACE_Message_Block *block = 0;
	if (ReceiveRecord(block) == -1) {
		return -1;
	}
	else {
		int result = WriteLogRecord(block);
		block->release();
		return result == -1 ? -1 : 0;
	}
}

ACE_SOCK_Stream & LogicHandler::peer()
{
	return peer_;
}

ACE_FILE_IO & LogicHandler::log_file()
{
	return log_file_;
}

void LogicHandler::SetHandle(ACE_HANDLE handle)
{
	peer_.set_handle(handle);
}

void LogicHandler::SetFileName(const char * filename)
{
	log_file_.close();
	ACE_FILE_Connector connector;
	connector.connect(log_file_, ACE_FILE_Addr(filename), 0, ACE_Addr::sap_any, 0, O_RDWR | O_CREAT | O_APPEND, ACE_DEFAULT_FILE_PERMS);
}
