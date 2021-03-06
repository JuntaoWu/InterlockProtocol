/*
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#ifndef _LOGIC_HANDLER_H
#define _LOGIC_HANDLER_H

#include "ace/FILE_IO.h"
#include "ace/SOCK_Stream.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Message_Block;
ACE_END_VERSIONED_NAMESPACE_DECL

class LogicHandler
{
protected:
	ACE_FILE_IO log_file_;

	ACE_SOCK_Stream peer_;

public:
	ACE_SOCK_Stream &peer();
	ACE_FILE_IO &log_file();

	void SetHandle(ACE_HANDLE handle);
	void SetFileName(const char *filename);

public:
	// Initialization and termination methods.
	LogicHandler();

	LogicHandler(ACE_HANDLE handle);

	LogicHandler(const ACE_SOCK_Stream &peer);

	int Close();

	// Receive one log record from a connected client.   Returns
	// length of record on success and <mblk> contains the
	// hostname, <mblk->cont()> contains the log record header
	// (the byte order and the length) and the data.  Returns -1 on
	// failure or connection close.
	int ReceiveRecord(ACE_Message_Block *&log_record);

	// Write one record to the log file.  The <mblk> contains the
	// hostname and the <mblk->cont> contains the log record.
	// Returns length of record written on success, or -1 on failure.
	int WriteLogRecord(ACE_Message_Block *log_record);

	// Log one record by calling <recv_log_record> and
	// <write_log_record>.  Returns 0 on success and -1 on failure.
	int HandleRecord();
private:
	void Parse(ACE_Message_Block *block);
};

#endif /* _LOGGING_HANDLER_H */
