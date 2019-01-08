#include "easyConfig.h"


/*
 * PBOC fields property template
 */
ea_fields_def_t  g_pboc_fields_def[129] = {
	/*   0 */ {EA_FORMAT_BCD,     4, EA_FORMAT_FIXED , "Message Type Indicator"}, //
	/*   1 */ {EA_FORMAT_BIN,     8, EA_FORMAT_FIXED , "Bitmap"}, //
	/*   2 */ {EA_FORMAT_CNBCD,  19, EA_FORMAT_LLVAR , "Primary Account Number"}, //
	/*   3 */ {EA_FORMAT_BCD,     6, EA_FORMAT_FIXED , "Processing Code"}, //
	/*   4 */ {EA_FORMAT_BCD,    12, EA_FORMAT_FIXED , "Amount"}, //
	/*   5 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*   6 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*   7 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*   8 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*   9 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  10 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  11 */ {EA_FORMAT_BCD,     6, EA_FORMAT_FIXED , "Systems Trace Audit Number"}, //
	/*  12 */ {EA_FORMAT_BCD,     6, EA_FORMAT_FIXED , "Date and Time"}, //
	/*  13 */ {EA_FORMAT_BCD,     4, EA_FORMAT_FIXED , "Date, Effective"}, //
	/*  14 */ {EA_FORMAT_BCD,     4, EA_FORMAT_FIXED , "Date, Expiration"}, //
	/*  15 */ {EA_FORMAT_BCD,     4, EA_FORMAT_FIXED , ""}, //
	/*  16 */ {EA_FORMAT_BCD,     4, EA_FORMAT_FIXED , ""}, //
	/*  17 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  18 */ {EA_FORMAT_BCD,     4, EA_FORMAT_FIXED , ""}, //
	/*  19 */ {EA_FORMAT_BCD,    14, EA_FORMAT_FIXED , "Exit Station Time"}, //
	/*  20 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  21 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  22 */ {EA_FORMAT_CNBCD,   3, EA_FORMAT_FIXED , "Point of Service Data Code"}, //
	/*  23 */ {EA_FORMAT_BCD,     3, EA_FORMAT_FIXED , "Card Sequence Number"}, //
	/*  24 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  25 */ {EA_FORMAT_CNBCD,   2, EA_FORMAT_FIXED , "Message Reason Code"}, //
	/*  26 */ {EA_FORMAT_BCD,     2, EA_FORMAT_FIXED , "Card Acceptor Business Code"}, //
	/*  27 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  28 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  29 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  30 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  31 */ {EA_FORMAT_ASC,     0, EA_FORMAT_LLVAR , ""}, //
	/*  32 */ {EA_FORMAT_CNBCD,  11, EA_FORMAT_LLVAR , "Acceptor Identification code"}, //
	/*  33 */ {EA_FORMAT_BCD,     0, EA_FORMAT_LLVAR , ""}, //
	/*  34 */ {EA_FORMAT_ASC,     0, EA_FORMAT_LLVAR , ""}, //
	/*  35 */ {EA_FORMAT_CNBCD,  37, EA_FORMAT_LLVAR , "Track 2 Data"}, //
	/*  36 */ {EA_FORMAT_CNBCD, 104, EA_FORMAT_LLLVAR, "Track 3 Data"}, //
	/*  37 */ {EA_FORMAT_ASC,    12, EA_FORMAT_FIXED , "Retrieval Reference Number"}, //
	/*  38 */ {EA_FORMAT_ASC,     6, EA_FORMAT_FIXED , "Approval Code"}, //
	/*  39 */ {EA_FORMAT_ASC,     2, EA_FORMAT_FIXED , "Action Code"}, //
	/*  40 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  41 */ {EA_FORMAT_ASC,     8, EA_FORMAT_FIXED , "Card Acceptor Terminal Id"}, //
	/*  42 */ {EA_FORMAT_ASC,    15, EA_FORMAT_FIXED , "Card Acceptor Id Code"}, //
	/*  43 */ {EA_FORMAT_ASC,     8, EA_FORMAT_FIXED , "ODA system identification"}, //
	/*  44 */ {EA_FORMAT_ASC,    25, EA_FORMAT_LLVAR , "Additional Response Data"}, //
	/*  45 */ {EA_FORMAT_ASC,     0, EA_FORMAT_LLVAR , ""}, //
	/*  46 */ {EA_FORMAT_ASC,     0, EA_FORMAT_LLLVAR, ""}, //
	/*  47 */ {EA_FORMAT_ASC,     0, EA_FORMAT_LLLVAR, ""}, //
	/*  48 */ {EA_FORMAT_CNBCD, 322, EA_FORMAT_LLLVAR, "Additional Data - Private"}, //
	/*  49 */ {EA_FORMAT_ASC,     3, EA_FORMAT_FIXED , "Currency Code"}, //
	/*  50 */ {EA_FORMAT_ASC,     0, EA_FORMAT_FIXED , ""}, //
	/*  51 */ {EA_FORMAT_ASC,     0, EA_FORMAT_FIXED , ""}, //
	/*  52 */ {EA_FORMAT_BIN,     8, EA_FORMAT_FIXED , "Personal Id Number (PIN) Data"}, //
	/*  53 */ {EA_FORMAT_BCD,    16, EA_FORMAT_FIXED , "Security Related Control Information"}, //
	/*  54 */ {EA_FORMAT_ASC,    20, EA_FORMAT_LLLVAR, "Amounts, Additional"}, //
	/*  55 */ {EA_FORMAT_BIN,   255, EA_FORMAT_LLLVAR, "IC Card System Related Data"}, //
	/*  56 */ {EA_FORMAT_BCD,     0, EA_FORMAT_LLVAR , ""}, //
	/*  57 */ {EA_FORMAT_BCD,     0, EA_FORMAT_FIXED , ""}, //
	/*  58 */ {EA_FORMAT_ASC,   100, EA_FORMAT_LLLVAR, "Authorizing Agent Inst Id Code"}, //
	/*  59 */ {EA_FORMAT_ASC,   999, EA_FORMAT_LLLVAR, "QR Code transaction information"}, //
	/*  60 */ {EA_FORMAT_CNBCD,  17, EA_FORMAT_LLLVAR, "Reserved for National use"}, //
	/*  61 */ {EA_FORMAT_CNBCD, 255, EA_FORMAT_LLLVAR, "Original transaction information"}, //
	/*  62 */ {EA_FORMAT_ASC,   999, EA_FORMAT_LLLVAR, "Reserved for Private use"}, //
	/*  63 */ {EA_FORMAT_ASC,   163, EA_FORMAT_LLLVAR, "Reserved for Private use"}, //
	/*  64 */ {EA_FORMAT_BIN,     8, EA_FORMAT_FIXED , "Message Authentication Code Field"}, //
};



int EA_Set_One_Field_Def_PBOC(int iFieldID, int iFieldType, int iFieldLen, int iFieldLenType)
{
    if ((iFieldID < 0) || (iFieldID > (sizeof (g_pboc_fields_def)/sizeof (g_pboc_fields_def[0]) - 1)))
    {
        return EA_PARAMETER_ERR;
    }

    if ((iFieldType < EA_FORMAT_BIN) || (iFieldID > EA_FORMAT_ASC))
    {
        return EA_PARAMETER_ERR;
    }

    if ((iFieldLenType < EA_FORMAT_FIXED) || (iFieldLenType > EA_FORMAT_LLLLVAR))
    {
        return EA_PARAMETER_ERR;
    }


    g_pboc_fields_def[iFieldID].dataType = iFieldType;
    g_pboc_fields_def[iFieldID].length = iFieldLen;
    g_pboc_fields_def[iFieldID].lengthType = iFieldLenType;


    return EA_OK;
}





