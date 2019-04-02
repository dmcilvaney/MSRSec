// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#ifndef AUTHVARS_TA_H
#define AUTHVARS_TA_H

#include <varops.h>

//
// This UUID is generated with uuidgen
//
#define TA_AUTHVARS_UUID { 0x2d57c0f7, 0xbddf, 0x48ea, \
    {0x83, 0x2f, 0xd8, 0x4a, 0x1a, 0x21, 0x93, 0x01}}

//
// The TAFs ID implemented in this TA
//
#define TA_AUTHVAR_GET_VARIABLE         (0) // Get authenticated variable
#define TA_AUTHVAR_GET_NEXT_VARIABLE    (1) // Get next autheiticated variable
#define TA_AUTHVAR_SET_VARIABLE         (2) // Set authenticated variable
#define TA_AUTHVAR_QUERY_VARINFO        (3) // Query authenticated variable info
#define TA_AUTHVAR_EXIT_BOOT_SERVICES   (4) // Used to signal ExitBootServices()

//
// Macro for intentionally unreferenced parameters
//
#define UNREFERENCED_PARAMETER(_Parameter_) (void)(_Parameter_)

//
// Shorthand for TA functions taking uniform arg types
//
#define TA_ALL_PARAM_TYPE(a) TEE_PARAM_TYPES((a), (a), (a), (a))

//
// External functions supporting initialization
//
extern int  AuthVarInitStorage();
extern void AuthVarCloseStorage();

#endif /* AUTHVARS_TA_H */