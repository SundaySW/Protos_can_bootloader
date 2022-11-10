#ifndef PROTOS_CMD_ID_H_
#define PROTOS_CMD_ID_H_

enum
{	
	// ����� � �������� � ���������� OW-�������� � ����
	OW_SEARCH = 0xA0,
	// ������� ������ ID ��� ������������� OW-���������
	// � ������(�����������) � EEPROM �������(OW-����������)
	OW_SET_NEW_PARAM_ID_AND_REGISTER = 0xA1,
	// ������ ROM'� OW-��������� �� ��� ID
	OW_GET_ROM_BY_PARAM_ID = 0xA2,
	// ������(ID) ���� ������������ � ���� OW ����������
	OW_GET_EXISTING_PARAMS = 0xA5,
	// ������� ������ �� EEPROM ������� �����������(OW-����������)
	// ���� ��������(������ ��� ��������) 0xFF ��������� ��� ������
	OW_DEL_REG_TABLE_ROW = 0xA6,
	//������ �� ��������� ������ �������
	OW_WRITE_SCRATCHPAD = 0xA7,
	OW_READ_REG_TABLE_ROW = 0xB7,
	//������ �� ��������� ������ �������
	OW_READ_SCRATCHPAD = 0xA8,
	//������ � EEPROM
	OW_WRITE_CONFIG = 0xA9,
	//������ �� EEPROM
	OW_READ_CONFIG  = 0xAA,
	
	//RelayExtensionBoard Relay Control Cmd
	//data[1] - relay channel, data[2] - relay action (0=off, 1=on, 2=toggle)
	RELAYEB_CTRL_SINGLE_RELAY = 0x10,
	
	//Relays on G debug brd
	RELAY_ONE_CHANNEL = 0x20,
	RELAY_SOLID_ONE_CHANNEL = 0x21,
	RELAY_4CH_K1 = 0x22,
	RELAY_4CH_K2 = 0x23,
	RELAY_4CH_K3 = 0x24,
	RELAY_4CH_K4 = 0x25,
	
};



#endif /* PROTOS_CMD_ID_H_ */