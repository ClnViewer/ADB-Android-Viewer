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
//  ENUMFORMAT.CPP
//  By J Brown 2004
//  www.catch22.net
//
//  Implementation of the IEnumFORMATETC interface
//  For Win2K and above look at the SHCreateStdEnumFmtEtc API call!!
//  Apparently the order of formats in an IEnumFORMATETC object must be
//      the same as those that were stored in the clipboard
//  www.catch22.net
//

#include <windows.h>
#include <stdint.h>
#include <string>
#include "OLEInternal.h"

namespace GameDev
{
    //
    //  Helper function to perform a "deep" copy of a FORMATETC
    //
    static void DeepCopyFormatEtc(FORMATETC *dest, FORMATETC *source)
    {
        // copy the source FORMATETC into dest
        *dest = *source;

        if(source->ptd)
        {
            // allocate memory for the DVTARGETDEVICE if necessary
            dest->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));

            // copy the contents of the source DVTARGETDEVICE into dest->ptd
            *(dest->ptd) = *(source->ptd);
        }
    }

    //
    //  "Drop-in" replacement for SHCreateStdEnumFmtEtc. Called by CDataObject::EnumFormatEtc
    //
    HRESULT OLEEnumFormat::CreateEnumFormatEtc(UINT nNumFormats, FORMATETC *pFormatEtc, IEnumFORMATETC **ppEnumFormatEtc)
    {
        if(nNumFormats == 0 || pFormatEtc == 0 || ppEnumFormatEtc == 0)
            return E_INVALIDARG;

        *ppEnumFormatEtc = new OLEEnumFormat(pFormatEtc, nNumFormats);

        return (*ppEnumFormatEtc) ? S_OK : E_OUTOFMEMORY;
    }

    //
    //  Constructor
    //
    OLEEnumFormat::OLEEnumFormat(FORMATETC *pFormatEtc, int nNumFormats)
    {
        m_lRefCount   = 1;
        m_nIndex      = 0;
        m_nNumFormats = nNumFormats;
        m_pFormatEtc  = new FORMATETC[nNumFormats];

        // copy the FORMATETC structures
        for(int i = 0; i < nNumFormats; i++)
            DeepCopyFormatEtc(&m_pFormatEtc[i], &pFormatEtc[i]);
    }

    //
    //  Destructor
    //
    OLEEnumFormat::~OLEEnumFormat()
    {
        if(m_pFormatEtc)
        {
            for(ULONG i = 0; i < m_nNumFormats; i++)
            {
                if(m_pFormatEtc[i].ptd)
                    CoTaskMemFree(m_pFormatEtc[i].ptd);
            }

            delete[] m_pFormatEtc;
        }
    }

    //
    //  IUnknown::AddRef
    //
    ULONG __stdcall OLEEnumFormat::AddRef()
    {
        // increment object reference count
        return InterlockedIncrement(&m_lRefCount);
    }

    //
    //  IUnknown::Release
    //
    ULONG __stdcall OLEEnumFormat::Release()
    {
        // decrement object reference count
        LONG count = InterlockedDecrement(&m_lRefCount);

        if(count == 0)
        {
            delete this;
            return 0;
        }
        else
            return count;
    }

    //
    //  IUnknown::QueryInterface
    //
    HRESULT __stdcall OLEEnumFormat::QueryInterface(REFIID iid, void **ppvObject)
    {
        // check to see what interface has been requested
        if(iid == IID_IEnumFORMATETC || iid == IID_IUnknown)
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

    //
    //  IEnumFORMATETC::Next
    //
    //  If the returned FORMATETC structure contains a non-null "ptd" member, then
    //  the caller must free this using CoTaskMemFree (stated in the COM documentation)
    //
    HRESULT __stdcall OLEEnumFormat::Next(ULONG celt, FORMATETC *pFormatEtc, ULONG * pceltFetched)
    {
        ULONG copied  = 0;

        // validate arguments
        if(celt == 0 || pFormatEtc == 0)
            return E_INVALIDARG;

        // copy FORMATETC structures into caller's buffer
        while(m_nIndex < m_nNumFormats && copied < celt)
        {
            DeepCopyFormatEtc(&pFormatEtc[copied], &m_pFormatEtc[m_nIndex]);
            copied++;
            m_nIndex++;
        }

        // store result
        if(pceltFetched != 0)
            *pceltFetched = copied;

        // did we copy all that was requested?
        return (copied == celt) ? S_OK : S_FALSE;
    }

    //
    //  IEnumFORMATETC::Skip
    //
    HRESULT __stdcall OLEEnumFormat::Skip(ULONG celt)
    {
        m_nIndex += celt;
        return (m_nIndex <= m_nNumFormats) ? S_OK : S_FALSE;
    }

    //
    //  IEnumFORMATETC::Reset
    //
    HRESULT __stdcall OLEEnumFormat::Reset()
    {
        m_nIndex = 0;
        return S_OK;
    }

    //
    //  IEnumFORMATETC::Clone
    //
    HRESULT __stdcall OLEEnumFormat::Clone(IEnumFORMATETC ** ppEnumFormatEtc)
    {
        HRESULT hResult;

        // make a duplicate enumerator
        hResult = OLEEnumFormat::CreateEnumFormatEtc(m_nNumFormats, m_pFormatEtc, ppEnumFormatEtc);

        if(hResult == S_OK)
        {
            // manually set the index state
            ((OLEEnumFormat*) *ppEnumFormatEtc)->m_nIndex = m_nIndex;
        }

        return hResult;
    }

};
