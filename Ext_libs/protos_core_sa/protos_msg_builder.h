#ifndef PROTOS_MSG_BUILDER_H_
#define PROTOS_MSG_BUILDER_H_

#include "protos_msg2.h"

namespace Protos
{
	struct Packet;

	/*!
		����� ��� ������ ��������� �� �������.
	*/
	class MsgBuilder
	{
	public:
		MsgBuilder();
		bool Grab(const Packet& packet);
		bool IsReady() const;
	
	private:
		bool Long;				///< true/false: �������� long/short ���������
		unsigned char Index;	///< ��������� ������ ������ long ���������
		union
		{
			unsigned char  LenBytes[2];
			unsigned short Len;	///< ��������� ����� ������ long ���������
		};
		bool Ready;				///< ������ ���������

	public:
		Msg2 Msg;				///< ������� ���������
	};

}//namespace Protos


#endif /* PROTOS_MSG_BUILDER_H_ */