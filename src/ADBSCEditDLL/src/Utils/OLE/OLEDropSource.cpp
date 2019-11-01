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
//  DROPSOURCE.CPP
//  Implementation of the IDropSource COM interface
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
    OLEDropSource::OLEDropSource()
    {
        m_lRefCount = 1;
    }

    //
    //  Destructor
    //
    OLEDropSource::~OLEDropSource()
    {
    }

    //
    //  IUnknown::AddRef
    //
    ULONG __stdcall OLEDropSource::AddRef()
    {
        // increment object reference count
        return InterlockedIncrement(&m_lRefCount);
    }

    //
    //  IUnknown::Release
    //
    ULONG __stdcall OLEDropSource::Release()
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
    HRESULT __stdcall OLEDropSource::QueryInterface(REFIID iid, void **ppvObject)
    {
        // check to see what interface has been requested
        if(iid == IID_IDropSource || iid == IID_IUnknown)
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
    //  OLEDropSource::QueryContinueDrag
    //
    //  Called by OLE whenever Escape/Control/Shift/Mouse buttons have changed
    //
    HRESULT __stdcall OLEDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
    {
        // if the <Escape> key has been pressed since the last call, cancel the drop
        if(fEscapePressed == TRUE)
            return DRAGDROP_S_CANCEL;

        // if the <LeftMouse> button has been released, then do the drop!
        if((grfKeyState & MK_LBUTTON) == 0)
            return DRAGDROP_S_DROP;

        // continue with the drag-drop
        return S_OK;
    }

    //
    //  OLEDropSource::GiveFeedback
    //
    //  Return either S_OK, or DRAGDROP_S_USEDEFAULTCURSORS to instruct OLE to use the
    //  default mouse cursor images
    //
    HRESULT __stdcall OLEDropSource::GiveFeedback(DWORD)
    {
        return DRAGDROP_S_USEDEFAULTCURSORS;
    }

    //
    //  Helper routine to create an IDropSource object
    //
    HRESULT OLEDropSource::CreateDropSource(IDropSource **ppDropSource)
    {
        if(ppDropSource == 0)
            return E_INVALIDARG;

        *ppDropSource = new OLEDropSource();

        return (*ppDropSource) ? S_OK : E_OUTOFMEMORY;

    }

    void OLEDropSource::BeginDrag(std::string const & s)
    {
        IDataObject *pDataObject = nullptr;
        IDropSource *pDropSource = nullptr;
        do
        {
            FORMATETC fmtetc = { CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
            STGMEDIUM stgmed = { TYMED_HGLOBAL, { 0 }, 0 };
            //
            stgmed.hGlobal = ::GlobalAlloc(GHND, s.length() + 1);
            if (!stgmed.hGlobal)
                break;
            //
            {
                char *ptr  = (char*)::GlobalLock(stgmed.hGlobal);
                ::memcpy(ptr, s.data(), s.length());
                ptr[s.length()] = '\0';
            }
            ::GlobalUnlock(stgmed.hGlobal);

            OLEDataObject::CreateDataObject(&fmtetc, &stgmed, 1, &pDataObject);
            if (!pDataObject)
                break;

            OLEDropSource::CreateDropSource(&pDropSource);
            if (!pDropSource)
                break;

            DWORD dwe;
            (void) DoDragDrop(pDataObject, pDropSource, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwe);
        }
        while (0);
        //
        if (pDataObject)
            pDataObject->Release();
        if (pDropSource)
            pDropSource->Release();
    }

};
