#include "LogicHandler.h"

LogicHandler::LogicHandler(ACE_FILE_IO & log_file) : log_file_(log_file)
{
}

LogicHandler::LogicHandler(ACE_HANDLE handle) : log_file_(ACE_FILE_IO())
{
	peer_.set_handle(handle);
}

LogicHandler::LogicHandler(ACE_FILE_IO & log_file, ACE_HANDLE handle) : log_file_(log_file)
{
	peer_.set_handle(handle);
}

LogicHandler::LogicHandler(ACE_FILE_IO & log_file, const ACE_SOCK_Stream & peer)
	: log_file_(log_file), peer_(peer)
{
}

inline int LogicHandler::Close()
{
	return peer_.close();
}

int LogicHandler::ReceiveRecord(ACE_Message_Block *& log_record)
{
	return 0;
}

int LogicHandler::WriteLogRecord(ACE_Message_Block * log_record)
{
	return 0;
}

int LogicHandler::HandleRecord()
{
	return 0;
}

inline ACE_SOCK_Stream & LogicHandler::peer()
{
	return peer_;
}

inline void LogicHandler::SetHandle(ACE_HANDLE handle)
{
	peer_.set_handle(handle);
}
