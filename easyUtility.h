#ifndef _EASYUTILITY_H_
#define _EASYUTILITY_H_

#include "easyConfig.h"


/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
extern int  EA_Set_Field_Def(ea_parser_t *pstParser, const ea_fields_def_t *pcstFieldDef, int iLen);


extern void EA_Free_Field(ea_parser_t *pstParser, int iFieldID);
extern void EA_Free_Field_All(ea_parser_t *pstParser);

extern int  Set_Field_Pure(ea_parser_t *pstParser, int iFieldID, const unsigned char *pcbData, int *piLength);
extern int  Set_Field_Bitmap(ea_parser_t *pstParser);

extern int  EA_Check_Fields_Def(const ea_fields_def_t *pcstFieldDef, int iLen);

extern int  EA_Bin2HexAsc(char *pstrDst, int *piDstLen, const unsigned char *pbSrc, int iSrcLen);
extern int  EA_HexAsc2Bin(unsigned char *pbDst, int *piDstLen, const char *pstrSrc);

/******************************************************************************/
#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif
/******************************************************************************/


#endif //_EASYUTILITY_H_


