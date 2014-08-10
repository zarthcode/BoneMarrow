// OTP.h
// One-Time Programming support methods and defines.
/// @brief These items, once programmed, are FOREVER.  No version bumping, no changes. Ever. Forever.

// OTP Configuration items
typedef enum
{
	OTP_PRODUCTID,		// Used to identify the product, production date, batch information.
	OTP_SERIALNUM,		// Assigned device unique serial number 
	OTP_DIAGNOSTIC,		// Diagnostic/QA results
	
} OTP_ItemType;