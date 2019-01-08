#include <stdio.h>
#include <string.h>

#include "easyISO8583.h"
#include "easyUtility.h"


int main(int argc, char * argv[])
{
	int i;
	int ret;
	ea_parser_t myParser;

	const char *pData = "02007020268020e08a13166214830121730203000000000000000001000021201812161312090720000000376214830121730203d2406220065060088860403030303730303037343530303533323431333130303037303030305144474a313536060000000000000001399f2608c3f5cd326d5169d99f2701809f101307011703a00000010a010000000000ec5ac7d19f3704d694f8929f360200a3950580000000009a031812169c01009f02060000000000015f2a02015682027c009f1a0201569f03060000000000009f3303e0d9e89f3501259f1e0834333135383532369f4104000000219f090200208408a00000033301010100082400000101233131312020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020204130373030303030303031303030303030303030303030303030313030303434333135383444343331353835323632303138313231363133313230393330314135453535";
	unsigned char srcData[1024] = {0};
	int dataLen = sizeof(srcData);

	char strBuff[1024];
	int buffLen;

	ret = EA_Parser_Init_PBOC(&myParser);
	printf("EA_Parser_Init() ret = %d\n", ret);

	//test pack

	//N - Fixed
	ret = EA_Set_Field_Auto(&myParser, 0, "1234");//YES
	ret = EA_Set_Field_Auto(&myParser, 0, "12345");//NO
	ret = EA_Set_Field_Auto(&myParser, 0, "123");//NO

	//CN - LLVar
	ret = EA_Set_Field_Auto(&myParser, 2, "1234567891234567891");//YES
	ret = EA_Set_Field_Auto(&myParser, 2, "123456789123456789");//YES
	ret = EA_Set_Field_Auto(&myParser, 2, "12345678912345678912");//NO

	//ASC - Fixed
	ret = EA_Set_Field_Auto(&myParser, 41, "12345678");//YES
	ret = EA_Set_Field_Auto(&myParser, 41, "123456789");//NO
	ret = EA_Set_Field_Auto(&myParser, 41, "1234567");//NO

	//B - Fixed
	ret = EA_Set_Field_Auto(&myParser, 52, "1234567812345678");//YES
	ret = EA_Set_Field_Auto(&myParser, 52, "12345678123456781");//NO
	ret = EA_Set_Field_Auto(&myParser, 52, "123456781234567");//NO

	dataLen = sizeof(srcData);
	ret = EA_Pack(&myParser, srcData, &dataLen);
	printf("EA_Pack() ret = %d\n", ret);

	buffLen = sizeof(strBuff);
	EA_Bin2HexAsc(strBuff, &buffLen, srcData, dataLen);
	printf("EA_Pack() result = %s\n", strBuff);

	EA_Parser_Final(&myParser);

	//test unpack
	ret = EA_Parser_Init_PBOC(&myParser);
	printf("EA_Parser_Init() ret = %d\n", ret);

	dataLen = sizeof(srcData);
	EA_HexAsc2Bin(srcData, &dataLen, pData);

	ret = EA_Unpack(&myParser, srcData, dataLen);
	printf("EA_Unpack() ret = %d\n", ret);

	for (i = 0; i < 65; i++)
	{
		if (EA_OK == EA_Field_Exist(&myParser, i))
		{
			dataLen = sizeof(srcData);
			ret = EA_Get_Field(&myParser, i, srcData, &dataLen);

			buffLen = sizeof(strBuff);
			memset(strBuff, 0, buffLen);
			EA_Bin2HexAsc(strBuff, &buffLen, srcData, dataLen);

			printf("Field=%d data = %s\n", i, strBuff);


			buffLen = sizeof(strBuff);
			memset(strBuff, 0, buffLen);
			ret = EA_Get_Field_Auto(&myParser, i, strBuff, &buffLen);

			printf("Field=%d data auto = %s\n\n", i, strBuff);
		}
	}

	EA_Parser_Final(&myParser);

	return 0;
}



