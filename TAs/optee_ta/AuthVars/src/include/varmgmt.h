// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once
#include <varops.h>

 // For cleaner descriptor validation
#define IS_VALID(a)         ((a) != (TEE_HANDLE_NULL))

 // Storage flags
#define TA_STORAGE_FLAGS    (TEE_DATA_FLAG_ACCESS_READ  | \
                             TEE_DATA_FLAG_ACCESS_WRITE | \
                             TEE_DATA_FLAG_ACCESS_WRITE_META)

 // Maximum number of variables we'll track
#define MAX_AUTHVAR_ENTRIES     (256)

 // MUST MATCH TA_DATA_SIZE (user_ta_header_defines.h)
#define NV_AUTHVAR_SIZE         (512 * 1024) // = 0x80000

 // Maximum possible storage (TA_DATA_SIZE) for volatile vars
#define MAX_VOLATILE_STORAGE    (0x40000)   // = NV_AUTHVAR_SIZE / 2

// Update if architected objectID length changes!
#if TEE_OBJECT_ID_MAX_LEN > 64
#error "Unexpected TEE_OBJECT_ID_MAX_LEN!"
#else
typedef struct _AUTHVAR_META
{
    UINT64              ObjectID;       // Storage object identifier
    TEE_ObjectHandle    ObjectHandle;   // Handle to open storage object
    PUEFI_VARIABLE      Var;            // In-memory variable
} AUTHVAR_META, *PAUTHVAR_META;
#endif 

TEE_Result
AuthVarInitStorage(
    VOID
);

TEE_Result
AuthVarCloseStorage(
    VOID
);

VOID
SearchList(
    PCUNICODE_STRING     UnicodeName,   // IN
    PCGUID               VendorGuid,    // IN
    PUEFI_VARIABLE      *Var,           // OUT
    VARTYPE             *VarType        // OUT
);

TEE_Result
CreateVariable(
    PCUNICODE_STRING        UnicodeName,        // IN
    PCGUID                  VendorGuid,         // IN
    ATTRIBUTES              Attributes,         // IN
    PEXTENDED_ATTRIBUTES    ExtAttributes,      // IN
    UINT32                  DataSize,           // IN
    PBYTE                   Data                // IN
);

TEE_Result
RetrieveVariable(
    PUEFI_VARIABLE       Var,           // IN
    VARIABLE_GET_RESULT *ResultBuf,     // OUT
    UINT32               ResultBufLen,  // IN
    UINT32              *BytesWritten   // OUT (optional)
);

TEE_Result
DeleteVariable(
    PUEFI_VARIABLE  Variable    // IN
);

TEE_Result
AppendVariable(
    PUEFI_VARIABLE          Var,            // IN
    ATTRIBUTES              Attributes,     // IN
    PEXTENDED_ATTRIBUTES    ExtAttributes,  // IN
    PBYTE                   Data,           // IN
    UINT32                  DataSize        // IN
);

TEE_Result
ReplaceVariable(
    PUEFI_VARIABLE          Var,            // IN
    ATTRIBUTES              Attributes,     // IN
    PEXTENDED_ATTRIBUTES    ExtAttributes,  // IN
    PBYTE                   Data,           // IN
    UINT32                  DataSize        // IN
);

VOID
QueryByAttribute(
    ATTRIBUTES  Attributes,             // IN
    PUINT64     MaxVarStorage,          // OUT
    PUINT64     RemainingVarStorage,    // OUT
    PUINT64     MaxVarSize              // OUT
);

#ifdef AUTHVAR_DEBUG
VOID
AuthVarDumpVarListImpl(
    VOID
);
#endif