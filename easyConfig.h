#ifndef _EASYCONFIG_H_
#define _EASYCONFIG_H_




#define EA_FORMAT_FIXED     0 // Length fixed, field data only have the value
#define EA_FORMAT_LVAR      1 // Length variable - 0..9, field data have the length[1B] + value
#define EA_FORMAT_LLVAR     2 // Length variable - 0..99, field data have the length[1B] + value
#define EA_FORMAT_LLLVAR    3 // Length variable - 0..999, field data have the length[2B] + value
#define EA_FORMAT_LLLLVAR   4 // Length variable - 0..9999, field data have the length[2B] + value


#define EA_FORMAT_BIN       0 //Binary. include 'B'
#define EA_FORMAT_BCD       1 //Number, right side, filled with zero at the front of the valid numbers. include 'N'
#define EA_FORMAT_CNBCD     2 //Number, left side, filled with zero at the end of the valid numbers. include 'CN'
#define EA_FORMAT_ASC       3 //Character, left side, filled with space at the end of the valid Characters. include 'A' 'AN' 'ANS' 'AS' 'NS'



typedef struct _iso8583_fields_def {
	int  dataType;  
	int  length;
	int  lengthType; 
	const char *name;
} ea_fields_def_t;


typedef struct _iso8583_field {
	unsigned char *data;
	int            length;
} ea_field_t;


typedef struct _iso8583_parser
{
	const ea_fields_def_t *fieldsDef;
	int                    fieldsLen;
	ea_field_t            *fields;
} ea_parser_t;


#define EA_OK                  0
#define EA_MEMORY_NOT_ENOUGH   1
#define EA_PARAMETER_ERR       2
#define EA_FORMAT_ERR          3
#define EA_NOT_FOUND           4

#ifndef NULL
#define NULL    0
#endif




/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

extern  ea_fields_def_t  g_pboc_fields_def[129];


int EA_Set_One_Field_Def_PBOC(int iFieldID, int iFieldType, int iFieldLen, int iFieldLenType);


/******************************************************************************/
#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif
/******************************************************************************/


#endif //_EASYCONFIG_H_
