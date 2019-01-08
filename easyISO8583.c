#include "easyISO8583.h"

#include <stdlib.h>
#include <string.h>

#include "easyUtility.h"


/** @brief initialize parser context with PBOC standard
 *
 * @param[in]
 *    @pstParser    - parser context
 * @param[out]
 *    @pstParser    - parser context
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-27
 */
int EA_Parser_Init_PBOC(ea_parser_t *pstParser) 
{
    int ret;

	if (NULL == pstParser)
	{
		return EA_PARAMETER_ERR;
	}

    ret = EA_Set_Field_Def(pstParser, g_pboc_fields_def, sizeof(g_pboc_fields_def)/sizeof(g_pboc_fields_def[0]));
    if (EA_OK != ret) {
        return ret;
    }

    pstParser->fields = (ea_field_t *)malloc(sizeof(ea_field_t) * pstParser->fieldsLen);
    if (NULL == pstParser->fields)
    {
        return EA_MEMORY_NOT_ENOUGH;
    }

    memset(pstParser->fields, 0, sizeof(ea_field_t) * pstParser->fieldsLen);

    return ret;
}


/** @brief release parser context
 *
 * @param[in]
 *    @pstParser  -  parser context
 * @param[out]
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-27
 */
void EA_Parser_Final(ea_parser_t *pstParser)
{
	if (NULL == pstParser)
	{
		return;
	}

	EA_Free_Field_All(pstParser);

    if (NULL != pstParser->fields)
    {
        free(pstParser->fields);
        pstParser->fields = NULL;
    }

    pstParser->fieldsDef = NULL;
    pstParser->fieldsLen = 0;
}


/** @brief pack iso8583 package
 *
 * @param[in]
 *    @pstParser - parser context
 *    @piLen     - the @pbData memory length
 * @param[out]
 *    @pbData    - finished package
 *    @piLen     - finished package length
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-27
 */
int EA_Pack(ea_parser_t *pstParser, unsigned char *pbData, int *piLen)
{
	int totalLen = 0, i = 0;
	int ret;

	if ((NULL == pstParser) || (NULL == pstParser->fieldsDef) || (NULL == pbData) || (NULL == piLen))
	{
		return EA_PARAMETER_ERR;
	}

	ret = Set_Field_Bitmap(pstParser);
	if (EA_OK != ret)
	{
		return ret;
	}

	for (i = 0; i < pstParser->fieldsLen; i++)
	{
		totalLen += pstParser->fields[i].length;
	}

	if (totalLen > *piLen)
	{
		return EA_PARAMETER_ERR;
	}

	totalLen = 0;

	for (i = 0; i < pstParser->fieldsLen; i++)
	{
		if ((NULL != pstParser->fields[i].data) && (0 != pstParser->fields[i].length))
		{
			memcpy(pbData+totalLen, pstParser->fields[i].data, pstParser->fields[i].length);
			totalLen += pstParser->fields[i].length;
		}
	}

	*piLen = totalLen;

	return EA_OK;
}


/** @brief unpack iso8583 package
 *
 * @param[in]
 *    @pstParser - parser context
 *    @pcbData   - package
 *    @piLen     - the package length
 * @param[out]
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-27
 */
int EA_Unpack(ea_parser_t *pstParser, const unsigned char *pcbData, int iLen)
{
	int offset = 0;
	int i = 0, j = 0, k = 0;
	int ret;
	int tmpLen;
	unsigned char b_tmp;

	if ((NULL == pstParser) || (NULL == pcbData) || (iLen <= 0))
	{
		return EA_PARAMETER_ERR;
	}

	EA_Free_Field_All(pstParser);

	//field-0
	offset = 0;
	tmpLen = iLen - offset;
	ret = Set_Field_Pure(pstParser, 0, pcbData+offset, &tmpLen);
	if (EA_OK != ret)
	{
		return ret;
	}

	//field-1
	offset += tmpLen;
	tmpLen = iLen - offset;
	ret = Set_Field_Pure(pstParser, 1, pcbData+offset, &tmpLen);
	if (EA_OK != ret)
	{
		return ret;
	}


	j = 0;
	k = 6;
	i = 2;

	while (i < pstParser->fieldsLen)
	{
		b_tmp = pstParser->fields[1].data[j];
		if ((pstParser->fields[1].data[j]) & ((1<<k)&0xFF))
		{// field exist
			offset += tmpLen;
			tmpLen = iLen - offset;
			ret = Set_Field_Pure(pstParser, i, pcbData+offset, &tmpLen);
			if (EA_OK != ret)
			{
				return ret;
			}
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

	return ret;
}


/** @brief check one field is exist in the parser context.
 *
 * @param[in]
 *    @pstParser - parser context
 *    @iFieldID  - field id
 * @param[out]
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-28
 */
int EA_Field_Exist(ea_parser_t *pstParser, int iFieldID)
{
	if ((NULL == pstParser) || (NULL == pstParser->fields))
	{
		return EA_PARAMETER_ERR;
	}

	if ((iFieldID < 0) || (iFieldID > (pstParser->fieldsLen-1)))
	{
		return EA_NOT_FOUND;
	}

	if (NULL == pstParser->fields[iFieldID].data)
	{
		return EA_NOT_FOUND;
	}

	return EA_OK;
}


/** @brief set one field into the parser context
 *
 * @param[in]
 *    @pstParser - parser context
 *    @iFieldID  - field id
 *    @pcbData   - field data
 *    @iDataLen  - field data length
 *    @iTrueLen  - field formatting length
 * @param[out]
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-28
 */
int EA_Set_Field(ea_parser_t *pstParser, int iFieldID, const unsigned char *pcbData, int iDataLen, int iTrueLen)
{
	int iNumH, iNumL, iChkLen;

	if ((NULL == pstParser) || (NULL == pstParser->fieldsDef) || 
		(NULL == pstParser->fields) || (NULL == pcbData))
	{
		return EA_PARAMETER_ERR;
	}

	if ((iFieldID < 0) || (iFieldID > (pstParser->fieldsLen-1)))
	{
		return EA_PARAMETER_ERR;
	}

	if (1 == iFieldID)
	{//bitmap
		return EA_OK;
	}

	EA_Free_Field(pstParser, iFieldID);

	//check length
	iChkLen = pstParser->fieldsDef[iFieldID].length;

	if (EA_FORMAT_FIXED == pstParser->fieldsDef[iFieldID].lengthType)
	{
		if (iTrueLen != iChkLen)
		{
			return EA_FORMAT_ERR;
		}
	}
	else
	{
		if (iTrueLen > iChkLen)
		{
			return EA_FORMAT_ERR;
		}
	}

	if ((EA_FORMAT_BCD == pstParser->fieldsDef[iFieldID].dataType) ||
		(EA_FORMAT_CNBCD == pstParser->fieldsDef[iFieldID].dataType))
	{
		iChkLen = (iChkLen + 1) / 2;

		if ((iTrueLen+1)/2 != iDataLen)
		{
			return EA_PARAMETER_ERR;
		}
	}
	else
	{
		if (iTrueLen != iDataLen)
		{
			return EA_PARAMETER_ERR;
		}
	}

	if (EA_FORMAT_FIXED == pstParser->fieldsDef[iFieldID].lengthType)
	{
		if (iDataLen != iChkLen)
		{
			return EA_FORMAT_ERR;
		}
	}
	else
	{
		if (iDataLen > iChkLen)
		{
			return EA_FORMAT_ERR;
		}
	}

	switch (pstParser->fieldsDef[iFieldID].lengthType) 
	{
	case EA_FORMAT_FIXED:
		pstParser->fields[iFieldID].data = (unsigned char *)malloc(iDataLen);
		if (NULL == pstParser->fields[iFieldID].data)
		{
			return EA_MEMORY_NOT_ENOUGH;
		}
		memcpy(pstParser->fields[iFieldID].data, pcbData, iDataLen);
		pstParser->fields[iFieldID].length = iDataLen;
		break;

	case EA_FORMAT_LVAR:
	case EA_FORMAT_LLVAR:
		pstParser->fields[iFieldID].data = (unsigned char *)malloc(iDataLen+1);
		if (NULL == pstParser->fields[iFieldID].data)
		{
			return EA_MEMORY_NOT_ENOUGH;
		}

		pstParser->fields[iFieldID].data[0] = iTrueLen/10*16 + iTrueLen%10;

		memcpy(pstParser->fields[iFieldID].data+1, pcbData, iDataLen);
		pstParser->fields[iFieldID].length = iDataLen+1;
		break;

	case EA_FORMAT_LLLVAR:
	case EA_FORMAT_LLLLVAR:
		pstParser->fields[iFieldID].data = (unsigned char *)malloc(iDataLen+2);
		if (NULL == pstParser->fields[iFieldID].data)
		{
			return EA_MEMORY_NOT_ENOUGH;
		}

		iNumH = iTrueLen / 100;
		iNumL = iTrueLen % 100;

		pstParser->fields[iFieldID].data[0] = iNumH/10*16 + iNumH%10;
		pstParser->fields[iFieldID].data[1] = iNumL/10*16 + iNumL%10;

		memcpy(pstParser->fields[iFieldID].data+2, pcbData, iDataLen);
		pstParser->fields[iFieldID].length = iDataLen+2;
		break;

	default:
		return EA_FORMAT_ERR;
	}

	return EA_OK;
}


/** @brief set one field into the parser context, don't care for data type.
 *
 * @param[in]
 *    @pstParser - parser context
 *    @iFieldID  - field id
 *    @pstrData  - field string data
 * @param[out]
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-27
 */
int EA_Set_Field_Auto(ea_parser_t *pstParser, int iFieldID, const char *pstrData)
{
	int ret = EA_OK;
	int iTrueLen;
	unsigned char *pbValue = NULL;
	int iValLen;
	const int iStrDataLen = strlen(pstrData);

	if ((NULL == pstParser) || (NULL == pstParser->fieldsDef) || 
		(NULL == pstParser->fields) || (NULL == pstrData))
	{
		return EA_PARAMETER_ERR;
	}

	if ((iFieldID < 0) || (iFieldID > (pstParser->fieldsLen-1)))
	{
		return EA_PARAMETER_ERR;
	}

	if (1 == iFieldID)
	{//bitmap
		return EA_OK;
	}

	//check length
	iTrueLen = pstParser->fieldsDef[iFieldID].length;

	if (EA_FORMAT_BIN == pstParser->fieldsDef[iFieldID].dataType)
	{
		iTrueLen = iTrueLen * 2;
	}

	if (EA_FORMAT_FIXED == pstParser->fieldsDef[iFieldID].lengthType)
	{
		if (iStrDataLen != iTrueLen)
		{
			return EA_FORMAT_ERR;
		}
	}
	else
	{
		if (iStrDataLen > iTrueLen)
		{
			return EA_FORMAT_ERR;
		}
	}

	switch (pstParser->fieldsDef[iFieldID].dataType)
	{
		case EA_FORMAT_BCD:
		case EA_FORMAT_CNBCD:
			if (iStrDataLen % 2)
			{//need pad one zero
				iValLen = iStrDataLen + 2;
				pbValue = (unsigned char *)malloc(iValLen);
				if (NULL == pbValue)
				{
					return EA_MEMORY_NOT_ENOUGH;
				}

				if (EA_FORMAT_BCD == pstParser->fieldsDef[iFieldID].dataType)
				{//padding left
					strcpy((char *)pbValue+1, pstrData);
					pbValue[0] = 0x30;
				}
				else
				{//padding right
					strcpy((char *)pbValue, pstrData);
					pbValue[iStrDataLen] = 0x30;
				}

				pbValue[iValLen-1] = 0;
			}
			else
			{
				iValLen = iStrDataLen + 1;
				pbValue = (unsigned char *)malloc(iValLen);
				if (NULL == pbValue)
				{
					return EA_MEMORY_NOT_ENOUGH;
				}

				strcpy((char *)pbValue, pstrData);
			}

			ret = EA_HexAsc2Bin(pbValue, &iValLen, (const char *)pbValue);
			if (EA_OK != ret)
			{
				break;
			}

			ret = EA_Set_Field(pstParser, iFieldID, pbValue, iValLen, iStrDataLen);
			break;

		case EA_FORMAT_ASC:
			ret = EA_Set_Field(pstParser, iFieldID, (const unsigned char *)pstrData, iStrDataLen, iStrDataLen);
			break;

		case EA_FORMAT_BIN:
			if (iStrDataLen % 2)
			{
				return EA_FORMAT_ERR;
			}

			iValLen = iStrDataLen / 2 + 1;
			pbValue = (unsigned char *)malloc(iValLen);
			if (NULL == pbValue)
			{
				return EA_MEMORY_NOT_ENOUGH;
			}

			ret = EA_HexAsc2Bin(pbValue, &iValLen, pstrData);
			if (EA_OK != ret)
			{
				break;
			}

			ret = EA_Set_Field(pstParser, iFieldID, pbValue, iValLen, iValLen);
			break;

		default:
			ret = EA_FORMAT_ERR;
			break;
	}

	if (NULL != pbValue)
	{
		free(pbValue);
	}

	return ret;
}


/** @brief get one field data from the parser context
 *
 * @param[in]
 *    @pstParser - parser context
 *    @iFieldID  - field id
 * @param[out]
 *    @pbData    - field all data(if NULL, get nothing, but can get the length)
 *    @piLen     - field all data length
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-27
 */
int EA_Get_Field(ea_parser_t *pstParser, int iFieldID, unsigned char *pbData, int *piLen)
{
	int ret;

	if ((NULL == pstParser) || (NULL == pstParser->fieldsDef) || 
		(NULL == pstParser->fields) || (NULL == piLen))
	{
		return EA_PARAMETER_ERR;
	}

	ret = EA_Field_Exist(pstParser, iFieldID);
	if (EA_OK != ret)
	{
		return ret;
	}
	

	if (NULL != pbData)
	{
		if (*piLen < pstParser->fields[iFieldID].length)
		{
			return EA_PARAMETER_ERR;
		}

		*piLen = pstParser->fields[iFieldID].length;

		memcpy(pbData, pstParser->fields[iFieldID].data, *piLen);
	}
	else
	{
		*piLen = pstParser->fields[iFieldID].length;
	}

	return EA_OK;
}



/** @brief get one field data from the parser context
 *
 * @param[in]
 *    @pstParser - parser context
 *    @iFieldID  - field id
 * @param[out]
 *    @pbData    - field string data (if NULL, get nothing, but can get the length)
 *    @piDataLen - field data memory length
 * @return
 *    error code
 * @author
 *    lining.zhang 2018-12-28
 */
int EA_Get_Field_Auto(ea_parser_t *pstParser, int iFieldID, char *pbData, int *piDataLen)
{
	int ret;
	int iValLen;
	int iFatLen;
	unsigned char *pbFieldVal = NULL;
	char *pbBuff = NULL;
	int iBuffLen = 0;

	if ((NULL == pstParser) || (NULL == pstParser->fieldsDef) || 
		(NULL == pstParser->fields) || (NULL == piDataLen))
	{
		return EA_PARAMETER_ERR;
	}

	ret = EA_Field_Exist(pstParser, iFieldID);
	if (EA_OK != ret)
	{
		return ret;
	}
	
	if (EA_FORMAT_FIXED == pstParser->fieldsDef[iFieldID].lengthType)
	{
		iValLen = pstParser->fields[iFieldID].length;
		iFatLen = pstParser->fieldsDef[iFieldID].length;
		pbFieldVal = pstParser->fields[iFieldID].data;
	}
	else if ((EA_FORMAT_LVAR == pstParser->fieldsDef[iFieldID].lengthType) ||
			 (EA_FORMAT_LLVAR == pstParser->fieldsDef[iFieldID].lengthType))
	{
		iValLen = pstParser->fields[iFieldID].length - 1;
		iFatLen = (pstParser->fields[iFieldID].data[0] / 16 * 10) + 
				  (pstParser->fields[iFieldID].data[0] % 16);
		pbFieldVal = pstParser->fields[iFieldID].data + 1;
	}
	else if ((EA_FORMAT_LLLVAR == pstParser->fieldsDef[iFieldID].lengthType) ||
			 (EA_FORMAT_LLLLVAR == pstParser->fieldsDef[iFieldID].lengthType))
	{
		iValLen = pstParser->fields[iFieldID].length - 2;

		iFatLen = (pstParser->fields[iFieldID].data[0] / 16 * 1000) + 
				  (pstParser->fields[iFieldID].data[0] % 16 * 100) + 
				  (pstParser->fields[iFieldID].data[1] / 16 * 10) + 
				  (pstParser->fields[iFieldID].data[1] % 16);
		pbFieldVal = pstParser->fields[iFieldID].data + 2;
	}
	else
	{
		return EA_FORMAT_ERR;
	}

	if (EA_FORMAT_BIN == pstParser->fieldsDef[iFieldID].dataType)
	{
		iFatLen = iFatLen * 2;
	}

	if (NULL != pbData)
	{
		if (*piDataLen < iFatLen)
		{
			return EA_PARAMETER_ERR;
		}

		*piDataLen = iFatLen;

		if (EA_FORMAT_ASC != pstParser->fieldsDef[iFieldID].dataType)
		{//bin bcd cnbcd
			if (EA_FORMAT_BIN == pstParser->fieldsDef[iFieldID].dataType)
			{
				if ((iValLen*2 != iFatLen))
				{
					return EA_FORMAT_ERR;
				}
			}
			else
			{
				if ((iValLen*2 != iFatLen) && (iValLen*2 != (iFatLen+1)))
				{
					return EA_FORMAT_ERR;
				}
			}

			iBuffLen = iValLen * 2 + 2;
			pbBuff = (char *)malloc(iBuffLen);
			if (NULL == pbBuff)
			{
				return EA_MEMORY_NOT_ENOUGH;
			}

			ret = EA_Bin2HexAsc(pbBuff, &iBuffLen, pbFieldVal, iValLen);
			if (EA_OK != ret)
			{
				goto end_err;
			}

            if (iValLen*2 == (iFatLen+1))
            {
                if (EA_FORMAT_CNBCD == pstParser->fieldsDef[iFieldID].dataType)
                {
                    memcpy(pbData, pbBuff, iFatLen);
                }
                else
                {
                    memcpy(pbData, pbBuff+1, iFatLen);
                }
            }
            else
            {
                memcpy(pbData, pbBuff, iFatLen);
            }
			
		}
		else
		{//asc
			if (iValLen != iFatLen)
			{
				ret = EA_FORMAT_ERR;
			}

			memcpy(pbData, pbFieldVal, iValLen);
		}
	}

	*piDataLen = iFatLen;
	
end_err:
	if (NULL != pbBuff)
	{
		free(pbBuff);
	}

	return EA_OK;
}


