#ifndef _EASYISO8583_H_
#define _EASYISO8583_H_

#include "easyConfig.h"


/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
extern int  EA_Parser_Init_PBOC(ea_parser_t *pstParser);
extern void EA_Parser_Final(ea_parser_t *pstParser);

extern int  EA_Unpack(ea_parser_t *pstParser, const unsigned char *pcbData, int iLen);
extern int  EA_Pack(ea_parser_t *pstParser, unsigned char *pbData, int *piLen);

extern int  EA_Field_Exist(ea_parser_t *pstParser, int iFieldID);

extern int  EA_Set_Field(ea_parser_t *pstParser, int iFieldID, const unsigned char *pcbData, int iDataLen, int iTrueLen);
extern int  EA_Set_Field_Auto(ea_parser_t *pstParser, int iFieldID, const char *pstrData);
extern int  EA_Get_Field(ea_parser_t *pstParser, int iFieldID, unsigned char *pbData, int *piLen);
extern int  EA_Get_Field_Auto(ea_parser_t *pstParser, int iFieldID, char *pbData, int *piDataLen);




/******************************************************************************/
#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif
/******************************************************************************/


#endif //_EASYISO8583_H_


