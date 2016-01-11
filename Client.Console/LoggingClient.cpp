#include "LoggingClient.h"

int LoggingClient::send(const ACE_Log_Record & log_record)
{
	const size_t max_payload_size =
		4 + //type
		8 + //timestamp
		4 + //pid
		4 + //message length
		ACE_Log_Record::MAXLOGMSGLEN + //message
		ACE_CDR::MAX_ALIGNMENT;

	ACE_OutputCDR payload(max_payload_size);
	payload << log_record;

	ACE_OutputCDR header(ACE_CDR::MAX_ALIGNMENT + 8);
	header << ACE_OutputCDR::from_boolean(ACE_CDR_BYTE_ORDER);

	ACE_CDR::ULong length = payload.total_length();
	header << ACE_CDR::ULong(length);

	iovec iov[2];
	iov[0].iov_base = header.begin()->rd_ptr();
	iov[0].iov_len = 8;
	iov[1].iov_base = payload.begin()->rd_ptr();
	iov[1].iov_len = length;

	return logging_peer_.sendv_n(iov, 2);
}
