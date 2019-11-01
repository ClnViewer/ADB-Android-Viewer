//
//  Origin:
//  DATAOBJECT.CPP
//  Implementation of the IDataObject COM interface
//  By J Brown 2004
//  www.catch22.net
//

#pragma once

namespace GameDev
{
    class OLEDataObject final : public IDataObject
    {
        public:
            //
            // IUnknown members
            //
            HRESULT __stdcall QueryInterface (REFIID, void**);
            ULONG   __stdcall AddRef();
            ULONG   __stdcall Release();

            //
            // IDataObject members
            //
            HRESULT __stdcall GetData(FORMATETC*,  STGMEDIUM*);
            HRESULT __stdcall GetDataHere(FORMATETC*,  STGMEDIUM*);
            HRESULT __stdcall QueryGetData(FORMATETC*);
            HRESULT __stdcall GetCanonicalFormatEtc(FORMATETC*,  FORMATETC*);
            HRESULT __stdcall SetData(FORMATETC*,  STGMEDIUM*, BOOL);
            HRESULT __stdcall EnumFormatEtc(DWORD, IEnumFORMATETC**);
            HRESULT __stdcall DAdvise(FORMATETC*,  DWORD, IAdviseSink*, DWORD*);
            HRESULT __stdcall DUnadvise(DWORD);
            HRESULT __stdcall EnumDAdvise(IEnumSTATDATA**);
            //
            // Constructor / Destructor
            //
            OLEDataObject(FORMATETC*, STGMEDIUM*, int);
            ~OLEDataObject();
            //
            static HRESULT CreateDataObject(FORMATETC*, STGMEDIUM*, UINT, IDataObject**);

        private:
            //
            int LookupFormatEtc(FORMATETC*);

            //
            // any private members and functions
            //
            LONG       m_lRefCount;
            FORMATETC *m_pFormatEtc;
            STGMEDIUM *m_pStgMedium;
            LONG       m_nNumFormats;

    };

};
