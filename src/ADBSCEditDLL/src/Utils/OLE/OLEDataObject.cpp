/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer/ADBSCEditDLL/ADBSCEdit

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

//
//  Origin:
//  DATAOBJECT.CPP
//  Implementation of the IDataObject COM interface
//  By J Brown 2004
//  www.catch22.net
//

#include <windows.h>
#include <stdint.h>
#include <string>
#include "OLEInternal.h"

namespace GameDev
{
    //
    //  Constructor
    //
    OLEDataObject::OLEDataObject(FORMATETC *fmtetc, STGMEDIUM *stgmed, int32_t cnt)
    {
        m_lRefCount  = 1;
        m_nNumFormats = cnt;

        m_pFormatEtc  = new FORMATETC[cnt];
        m_pStgMedium  = new STGMEDIUM[cnt];

        for(int i = 0; i < cnt; i++)
        {
            m_pFormatEtc[i] = fmtetc[i];
            m_pStgMedium[i] = stgmed[i];
        }
    }

    //
    //  Destructor
    //
    OLEDataObject::~OLEDataObject()
    {
        // cleanup
        if(m_pFormatEtc) delete[] m_pFormatEtc;
        if(m_pStgMedium) delete[] m_pStgMedium;

        // ::OutputDebugString("OLEDataObject destroy\n");
    }

    //
    //  IUnknown::AddRef
    //
    ULONG __stdcall OLEDataObject::AddRef()
    {
        // increment object reference count
        return InterlockedIncrement(&m_lRefCount);
    }

    //
    //  IUnknown::Release
    //
    ULONG __stdcall OLEDataObject::Release()
    {
        // decrement object reference count
        LONG cnt = InterlockedDecrement(&m_lRefCount);

        if(cnt == 0)
        {
            delete this;
            return 0;
        }
        else
            return cnt;
    }

    //
    //  IUnknown::QueryInterface
    //
    HRESULT __stdcall OLEDataObject::QueryInterface(REFIID iid, void **ppvObject)
    {
        // check to see what interface has been requested
        if(iid == IID_IDataObject || iid == IID_IUnknown)
        {
            AddRef();
            *ppvObject = this;
            return S_OK;
        }
        else
        {
            *ppvObject = 0;
            return E_NOINTERFACE;
        }
    }

    HGLOBAL DupMem(HGLOBAL hMem)
    {
        // lock the source memory object
        DWORD   len    = GlobalSize(hMem);
        PVOID   source = GlobalLock(hMem);

        // create a fixed "global" block - i.e. just
        // a regular lump of our process heap
        PVOID   dest   = GlobalAlloc(GMEM_FIXED, len);
        ::memcpy(dest, source, len);
        ::GlobalUnlock(hMem);
        return dest;
    }

    int OLEDataObject::LookupFormatEtc(FORMATETC *pFormatEtc)
    {
        for(int i = 0; i < m_nNumFormats; i++)
        {
            if((pFormatEtc->tymed    &  m_pFormatEtc[i].tymed)   &&
                    pFormatEtc->cfFormat == m_pFormatEtc[i].cfFormat &&
                    pFormatEtc->dwAspect == m_pFormatEtc[i].dwAspect)
                return i;
        }
        return -1;
    }

    //
    //  IDataObject::GetData
    //
    HRESULT __stdcall OLEDataObject::GetData (FORMATETC *pFormatEtc, STGMEDIUM *pMedium)
    {
        int idx;

        //
        // try to match the requested FORMATETC with one of our supported formats
        //
        if((idx = LookupFormatEtc(pFormatEtc)) == -1)
            return DV_E_FORMATETC;

        //
        // found a match! transfer the data into the supplied storage-medium
        //
        pMedium->tymed           = m_pFormatEtc[idx].tymed;
        pMedium->pUnkForRelease  = 0;

        switch(m_pFormatEtc[idx].tymed)
        {
            case TYMED_HGLOBAL:

                pMedium->hGlobal = DupMem(m_pStgMedium[idx].hGlobal);
                //return S_OK;
                break;

            default:
                return DV_E_FORMATETC;
        }
        return S_OK;
    }

    //
    //  IDataObject::GetDataHere
    //
    HRESULT __stdcall OLEDataObject::GetDataHere (FORMATETC*, STGMEDIUM*)
    {
        // GetDataHere is only required for IStream and IStorage mediums
        // It is an error to call GetDataHere for things like HGLOBAL and other clipboard formats
        //
        //  OleFlushClipboard
        //
        return DATA_E_FORMATETC;
    }

    //
    //  IDataObject::QueryGetData
    //
    //  Called to see if the IDataObject supports the specified format of data
    //
    HRESULT __stdcall OLEDataObject::QueryGetData (FORMATETC *pFormatEtc)
    {
        return (LookupFormatEtc(pFormatEtc) == -1) ? DV_E_FORMATETC : S_OK;
    }

    //
    //  IDataObject::GetCanonicalFormatEtc
    //
    HRESULT __stdcall OLEDataObject::GetCanonicalFormatEtc (FORMATETC*, FORMATETC *pFormatEtcOut)
    {
        // Apparently we have to set this field to NULL even though we don't do anything else
        pFormatEtcOut->ptd = NULL;
        return E_NOTIMPL;
    }

    //
    //  IDataObject::SetData
    //
    HRESULT __stdcall OLEDataObject::SetData (FORMATETC*, STGMEDIUM*, BOOL)
    {
        return E_NOTIMPL;
    }

    //
    //  IDataObject::EnumFormatEtc
    //
    HRESULT __stdcall OLEDataObject::EnumFormatEtc (DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc)
    {
        if(dwDirection == DATADIR_GET)
        {
            // for Win2k+ you can use the SHCreateStdEnumFmtEtc API call, however
            // to support all Windows platforms we need to implement IEnumFormatEtc ourselves.
            return OLEEnumFormat::CreateEnumFormatEtc(m_nNumFormats, m_pFormatEtc, ppEnumFormatEtc);
        }
        else
        {
            // the direction specified is not support for drag+drop
            return E_NOTIMPL;
        }
    }

    //
    //  IDataObject::DAdvise
    //
    HRESULT __stdcall OLEDataObject::DAdvise (FORMATETC*, DWORD, IAdviseSink*, DWORD*)
    {
        return OLE_E_ADVISENOTSUPPORTED;
    }

    //
    //  IDataObject::DUnadvise
    //
    HRESULT __stdcall OLEDataObject::DUnadvise (DWORD)
    {
        return OLE_E_ADVISENOTSUPPORTED;
    }

    //
    //  IDataObject::EnumDAdvise
    //
    HRESULT __stdcall OLEDataObject::EnumDAdvise (IEnumSTATDATA**)
    {
        return OLE_E_ADVISENOTSUPPORTED;
    }

    //
    //  Helper function
    //
    HRESULT OLEDataObject::CreateDataObject (FORMATETC *fmtetc, STGMEDIUM *stgmeds, UINT cnt, IDataObject **ppDataObject)
    {
        if(ppDataObject == 0)
            return E_INVALIDARG;

        *ppDataObject = new OLEDataObject(fmtetc, stgmeds, cnt);
        return (*ppDataObject) ? S_OK : E_OUTOFMEMORY;
    }

};
