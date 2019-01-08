#include "easyUtility.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>




int EA_Set_Field_Def(ea_parser_t *pstParser, const ea_fields_def_t *pcstFieldDef, int iLen) 
{
	int ret;

	if ((NULL == pstParser) || (NULL == pcstFieldDef) || (iLen <= 0))
	{
		return EA_PARAMETER_ERR;
	}

	ret = EA_Check_Fields_Def(pcstFieldDef, iLen);
	if (EA_OK != ret)
	{
		return ret;
	}

	pstParser->fieldsDef = pcstFieldDef;
    pstParser->fieldsLen = iLen;

	return EA_OK;
}


void EA_Free_Field(ea_parser_t *pstParser, int iFieldID)
{
	if ((NULL != pstParser) && (NULL != pstParser->fields[iFieldID].data))
	{
		free(pstParser->fields[iFieldID].data);

		pstParser->fields[iFieldID].data = NULL;
		pstParser->fields[iFieldID].length = 0;
	}
}


void EA_Free_Field_All(ea_parser_t *pstParser)
{
	int i = 0;

	if (NULL == pstParser)
	{
		return;
	}

	while (i < pstParser->fieldsLen)
	{
		EA_Free_Field(pstParser, i);
		i ++;
	}
}

int Set_Field_Pure(ea_parser_t *pstParser, int iFieldID, const unsigned char *pcbData, int *piLength)
{
	int i_tmp;

	if ((NULL == pstParser) || (NULL == pstParser->fieldsDef) || (NULL == pcbData) || (NULL == piLength))
	{
		return EA_PARAMETER_ERR;
	}

	if ((iFieldID < 0) || (iFieldID > (pstParser->fieldsLen-1)))
	{
		return EA_NOT_FOUND;
	}

	EA_Free_Field(pstParser, iFieldID);

	if (1 == iFieldID)
	{//bitmap
		if ((pcbData[0]&0x80) && (pstParser->fieldsDef[1].length < 16))
		{//128bit
			return EA_FORMAT_ERR;
		}

		if ((!(pcbData[0]&0x80)) && (pstParser->fieldsDef[1].length < 8))
		{//64bit
			return EA_FORMAT_ERR;
		}
	}

	switch (pstParser->fieldsDef[iFieldID].lengthType) 
	{
	case EA_FORMAT_FIXED:
		i_tmp = pstParser->fieldsDef[iFieldID].length;
		if ((EA_FORMAT_BCD == pstParser->fieldsDef[iFieldID].dataType) ||
			(EA_FORMAT_CNBCD == pstParser->fieldsDef[iFieldID].dataType))
		{
			i_tmp = (i_tmp + 1) / 2;
		}
		break;

	case EA_FORMAT_LVAR:
	case EA_FORMAT_LLVAR:
		i_tmp = (pcbData[0] / 16 * 10) + (pcbData[0] % 16);
		if (i_tmp > pstParser->fieldsDef[iFieldID].length)
		{
			return EA_FORMAT_ERR;
		}

		if ((EA_FORMAT_BCD == pstParser->fieldsDef[iFieldID].dataType) ||
			(EA_FORMAT_CNBCD == pstParser->fieldsDef[iFieldID].dataType))
		{
			i_tmp = (i_tmp + 1 + 2) / 2;
		}
		else
		{
			i_tmp = i_tmp + 1;
		}
		break;

	case EA_FORMAT_LLLVAR:
	case EA_FORMAT_LLLLVAR:
		i_tmp = (pcbData[0] / 16 * 1000) + (pcbData[0] % 16 * 100) + 
				(pcbData[1] / 16 * 10) + (pcbData[1] % 16);

		if (i_tmp > pstParser->fieldsDef[iFieldID].length)
		{
			return EA_FORMAT_ERR;
		}

		if ((EA_FORMAT_BCD == pstParser->fieldsDef[iFieldID].dataType) ||
			(EA_FORMAT_CNBCD == pstParser->fieldsDef[iFieldID].dataType))
		{
			i_tmp = (i_tmp + 1 + 4) / 2;
		}
		else
		{
			i_tmp = i_tmp + 2;
		}
		break;

	default:
		return EA_FORMAT_ERR;
	}

	if (*piLength < i_tmp)
	{
		return EA_FORMAT_ERR;
	}

	*piLength = i_tmp;

	pstParser->fields[iFieldID].data = (unsigned char *)malloc(*piLength);
	if (NULL == pstParser->fields[iFieldID].data)
	{
		return EA_MEMORY_NOT_ENOUGH;
	}

	memcpy(pstParser->fields[iFieldID].data, pcbData, *piLength);

	pstParser->fields[iFieldID].length = *piLength;

	return EA_OK;
}


int Set_Field_Bitmap(ea_parser_t *pstParser)
{
	unsigned char bitmap[16] = {0};
	int bitmapLen = 0;
	int i = 0, j = 0, k = 8;

	if (NULL == pstParser)
	{
		return EA_PARAMETER_ERR;
	}
	
	if ((pstParser->fieldsLen < 2) || (pstParser->fieldsLen > 129))
	{
		return EA_FORMAT_ERR;
	}
	else if (pstParser->fieldsLen > 65)
	{//128bit
		bitmap[0] |= 0x80;
		bitmapLen = 16;
	}
	else
	{//64bit
		bitmapLen = 8;
	}
	
	j = 0;
	k = 6;
	i = 2;

	while (i < pstParser->fieldsLen)
	{
		if (NULL != pstParser->fields[i].data)
		{
			bitmap[j] |= 1 << k;
		}

		if (!(i % 8))
		{
			j ++;
			k = 7;
		}
		else
		{
			k --;
		}

		i ++;
	}

	bitmapLen = sizeof(bitmap);

	return Set_Field_Pure(pstParser, 1, bitmap, &bitmapLen);

}


int EA_Check_Fields_Def(const ea_fields_def_t *pcstFieldDef, int iLen)
{
	int i = 0;

	while (i < iLen)
	{
		if ((pcstFieldDef[i].dataType < EA_FORMAT_BIN) || 
			(pcstFieldDef[i].dataType > EA_FORMAT_ASC))
		{
			return EA_FORMAT_ERR;
		}

		if ((pcstFieldDef[i].lengthType < EA_FORMAT_FIXED) || 
			(pcstFieldDef[i].lengthType > EA_FORMAT_LLLLVAR))
		{
			return EA_FORMAT_ERR;
		}

		switch (pcstFieldDef[i].lengthType)
		{
		case EA_FORMAT_LVAR:
			if (pcstFieldDef[i].length > 9)
			{
				return EA_FORMAT_ERR;
			}
			break;

		case EA_FORMAT_LLVAR:
			if (pcstFieldDef[i].length > 99)
			{
				return EA_FORMAT_ERR;
			}
			break;

		case EA_FORMAT_LLLVAR:
			if (pcstFieldDef[i].length > 999)
			{
				return EA_FORMAT_ERR;
			}
			break;

		case EA_FORMAT_LLLLVAR:
			if (pcstFieldDef[i].length > 9999)
			{
				return EA_FORMAT_ERR;
			}
			break;
		}
		
		i ++;
	}

	return EA_OK;
}



int EA_Bin2HexAsc(char *pstrDst, int *piDstLen, const unsigned char *pbSrc, int iSrcLen)
{
	int i, j;
	char *pbOut = NULL;

	if ((NULL == pstrDst) || (NULL == piDstLen) || (*piDstLen < (iSrcLen*2 + 1)) || (NULL == pbSrc))
	{
		return EA_PARAMETER_ERR;
	}

	pbOut = (char *)malloc(iSrcLen*2 + 1);
	if (NULL == pbOut)
	{
		return EA_MEMORY_NOT_ENOUGH;
	}

	for (i=0, j=0; j<iSrcLen; i+=2, j++)
	{
		if (sprintf((char *)pbOut + i, "%02x", pbSrc[j]) < 0)
		{
			return EA_FORMAT_ERR;
		}
	}

	*piDstLen = i;
	strcpy(pstrDst, pbOut);

	if (NULL != pbOut)
	{
		free(pbOut);
	}

	return EA_OK;
}



int EA_HexAsc2Bin(unsigned char *pbDst, int *piDstLen, const char *pstrSrc)
{
	unsigned char  bTmp[3] = {0};
	int i, j, iTmp;
	const int ISrcLen = strlen(pstrSrc);

	if ((NULL == pbDst) || (NULL == piDstLen) || (*piDstLen < (ISrcLen/2)) || (NULL == pstrSrc))
	{
		return EA_PARAMETER_ERR;
	}

	for (i=0, j=0; i<ISrcLen; i+=2, j++)
	{
		bTmp[0] = pstrSrc[i];
		bTmp[1] = pstrSrc[i+1];

		if (sscanf((const char *)bTmp, "%2x", &iTmp) < 0)
		{
			return EA_FORMAT_ERR;
		}
		
		pbDst[j] = iTmp&0xFF;
	}

	*piDstLen = j;

	return EA_OK;
}
