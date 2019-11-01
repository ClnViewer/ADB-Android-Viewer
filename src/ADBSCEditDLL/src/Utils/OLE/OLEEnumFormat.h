//
//  Origin:
//  ENUMFORMAT.CPP
//  By J Brown 2004
//  www.catch22.net
//
//  Implementation of the IEnumFORMATETC interface
//  For Win2K and above look at the SHCreateStdEnumFmtEtc API call!!
//  Apparently the order of formats in an IEnumFORMATETC object must be
//  the same as those that were stored in the clipboard
//

#pragma once

namespace GameDev
{
    class OLEEnumFormat final : public IEnumFORMATETC
    {
        public:
            //
            // IUnknown members
            //
            HRESULT __stdcall  QueryInterface(REFIID, void**);
            ULONG   __stdcall  AddRef();
            ULONG   __stdcall  Release();
            //
            // IEnumFormatEtc members
            //
            HRESULT __stdcall  Next(ULONG, FORMATETC*, ULONG*);
            HRESULT __stdcall  Skip(ULONG);
            HRESULT __stdcall  Reset(void);
            HRESULT __stdcall  Clone(IEnumFORMATETC**);
            //
            // Construction / Destruction
            //
            OLEEnumFormat(FORMATETC*, int);
            ~OLEEnumFormat();
            //
            static HRESULT CreateEnumFormatEtc(UINT, FORMATETC*, IEnumFORMATETC**);

        private:
            //
            LONG        m_lRefCount;        // Reference count for this COM interface
            ULONG       m_nIndex;           // current enumerator index
            ULONG       m_nNumFormats;      // number of FORMATETC members
            FORMATETC * m_pFormatEtc;       // array of FORMATETC objects
    };

};
