#ifndef COMMANDS_H_
#define COMMANDS_H_

enum CMDMISC
{
	CMDMISC_NULL = 0,					  ///< ��� ����� ������ ������������ CMDMISC_ERROR � ����� CMDMISC_NULL
	CMDMISC_GET_FIRMWARE_VERSION = 0x14,  ///< ��������� ������ ��������
	CMDMISC_GET_ALL_PARAMS		 = 0x15,   ///< �������� ID ���� ���������� � ����� ���������.
};

enum 
{ 
	CMDMISC_ERROR_READ_UID = 1,			  ///< ������ ������ ����������� ������ ����������
};

#endif /* COMMANDS_H_ */