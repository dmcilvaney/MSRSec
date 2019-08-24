/*  The copyright in this software is being made available under the BSD License,
 *  included below. This software may be subject to other third party and
 *  contributor rights, including patent rights, and no such rights are granted
 *  under this license.
 *
 *  Copyright (c) Microsoft Corporation
 *
 *  All rights reserved.
 *
 *  BSD License
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  Redistributions of source code must retain the above copyright notice, this list
 *  of conditions and the following disclaimer.
 *
 *  Redistributions in binary form must reproduce the above copyright notice, this
 *  list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ""AS IS""
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <defaultvars.h>

TEE_Result
SetDefaultVariable(
    WCHAR *Name, UINT32 NameSize, BYTE *Bin, UINT32 BinSize,
    GUID Guid, ATTRIBUTES Attributes
)
/*++

    Routine Description:

        Populates and uses a set variable paramteter structure for the
        pre-compiled default variables. Called from the autogenerated
        file defaultvars_encoding.c (see defaultvars.py).

    Arguments:

            Name - Null terminated WCHAR string with the variable name

            NameSize - Length of the name

            Bin - Pointer to the binary contents of the variable

            BinSize - Size of the binary

            Guid - Variable GUID

            Attributes - Attributes to set the variable with

    Returns:

    TEE_Result

--*/
{
    TEE_Result res;
    UINT32 totalSize = sizeof(VARIABLE_SET_PARAM) + NameSize + BinSize;
    PVARIABLE_SET_PARAM setParam = TEE_Malloc(totalSize, TEE_MALLOC_FILL_ZERO);

    if(!setParam) {
        return TEE_ERROR_OUT_OF_MEMORY;
    }

    setParam->Size = sizeof(VARIABLE_SET_PARAM);
    setParam->NameSize = NameSize;
    setParam->VendorGuid = Guid;
    setParam->Attributes = Attributes;
    setParam->DataSize = BinSize;
    setParam->OffsetName = 0;
    setParam->OffsetData = NameSize;

    memcpy(&setParam->Payload[0], Name, NameSize);
    memcpy(&setParam->Payload[NameSize], Bin, BinSize);

    res = SetVariable(totalSize, setParam);

    TEE_Free(setParam);

    return res;
}