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
//  DROPTARGET.CPP
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
    //  Constructor for the OLEDropTarget class
    //
    OLEDropTarget::OLEDropTarget(HWND hwnd)
    {
        m_lRefCount  = 1;
        m_hWnd       = hwnd;
        m_fAllowDrop = false;
    }

    //
    //  Destructor for the OLEDropTarget class
    //
    OLEDropTarget::~OLEDropTarget()
    {
    }

    //
    //  Position the edit control's caret under the mouse
    //
    void PositionCursor(HWND hwndEdit, POINTL pt)
    {
        DWORD curpos;
        // get the character position of mouse
        ScreenToClient(hwndEdit, (POINT *)&pt);
        curpos = SendMessage(hwndEdit, EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
        // set cursor position
        SendMessage(hwndEdit, EM_SETSEL, LOWORD(curpos), LOWORD(curpos));
    }

    //
    //  IUnknown::QueryInterface
    //
    HRESULT __stdcall OLEDropTarget::QueryInterface (REFIID iid, void ** ppvObject)
    {
        if(iid == IID_IDropTarget || iid == IID_IUnknown)
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
    //  IUnknown::AddRef
    //
    ULONG __stdcall OLEDropTarget::AddRef()
    {
        return InterlockedIncrement(&m_lRefCount);
    }

    //
    //  IUnknown::Release
    //
    ULONG __stdcall OLEDropTarget::Release()
    {
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
    //  QueryDataObject private helper routine
    //
    bool OLEDropTarget::QueryDataObject(IDataObject *pDataObject)
    {
        FORMATETC fmtetc = { CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        // does the data object support CF_TEXT using a HGLOBAL?
        return pDataObject->QueryGetData(&fmtetc) == S_OK ? true : false;
    }

    //
    //  DropEffect private helper routine
    //
    DWORD OLEDropTarget::DropEffect(DWORD grfKeyState, POINTL, DWORD dwAllowed)
    {
        DWORD dwEffect = 0;
        // 1. check "pt" -> do we allow a drop at the specified coordinates?
        // 2. work out that the drop-effect should be based on grfKeyState
        if(grfKeyState & MK_CONTROL)
            dwEffect = dwAllowed & DROPEFFECT_COPY;
        else
            if(grfKeyState & MK_SHIFT)
                dwEffect = dwAllowed & DROPEFFECT_MOVE;

        // 3. no key-modifiers were specified (or drop effect not allowed), so
        //    base the effect on those allowed by the dropsource
        if(dwEffect == 0)
        {
            if(dwAllowed & DROPEFFECT_COPY) dwEffect = DROPEFFECT_COPY;
            if(dwAllowed & DROPEFFECT_MOVE) dwEffect = DROPEFFECT_MOVE;
        }

        return dwEffect;
    }

    //
    //  IDropTarget::DragEnter
    //
    //
    //
    HRESULT __stdcall OLEDropTarget::DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
    {
        // does the dataobject contain data we want?
        if((m_fAllowDrop = QueryDataObject(pDataObject)))
        {
            // get the dropeffect based on keyboard state
            *pdwEffect = DropEffect(grfKeyState, pt, *pdwEffect);
            SetFocus(m_hWnd);
            PositionCursor(m_hWnd, pt);
        }
        else
            *pdwEffect = DROPEFFECT_NONE;
        return S_OK;
    }

    //
    //  IDropTarget::DragOver
    //
    //
    //
    HRESULT __stdcall OLEDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
    {
        if(m_fAllowDrop)
        {
            *pdwEffect = DropEffect(grfKeyState, pt, *pdwEffect);
            PositionCursor(m_hWnd, pt);
        }
        else
            *pdwEffect = DROPEFFECT_NONE;
        return S_OK;
    }

    //
    //  IDropTarget::DragLeave
    //
    HRESULT __stdcall OLEDropTarget::DragLeave()
    {
        return S_OK;
    }

    //
    //  IDropTarget::Drop
    //
    //
    HRESULT __stdcall OLEDropTarget::Drop(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
    {
        PositionCursor(m_hWnd, pt);

        if(m_fAllowDrop)
        {
            OLEDropTarget::DropData(m_hWnd, pDataObject);
            *pdwEffect = DropEffect(grfKeyState, pt, *pdwEffect);
        }
        else
            *pdwEffect = DROPEFFECT_NONE;
        return S_OK;
    }

    // static

    void OLEDropTarget::DropData(HWND hwnd, IDataObject *pDataObject)
    {
        // construct a FORMATETC object
        FORMATETC fmtetc = { CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        STGMEDIUM stgmed;

        // See if the dataobject contains any TEXT stored as a HGLOBAL
        if(pDataObject->QueryGetData(&fmtetc) == S_OK)
        {
            // Yippie! the data is there, so go get it!
            if(pDataObject->GetData(&fmtetc, &stgmed) == S_OK)
            {
                // we asked for the data as a HGLOBAL, so access it appropriately
                PVOID data = GlobalLock(stgmed.hGlobal);
                SetWindowText(hwnd, (char *)data);
                GlobalUnlock(stgmed.hGlobal);
                // release the data using the COM API
                ReleaseStgMedium(&stgmed);
            }
        }
    }

    void OLEDropTarget::RegisterDropWindow(HWND hwnd, IDropTarget **ppDropTarget)
    {
        OLEDropTarget *pDropTarget = new OLEDropTarget(hwnd);
        // acquire a strong lock
        CoLockObjectExternal(pDropTarget, TRUE, FALSE);
        // tell OLE that the window is a drop target
        RegisterDragDrop(hwnd, pDropTarget);
        *ppDropTarget = pDropTarget;
    }

    void OLEDropTarget::UnregisterDropWindow(HWND hwnd, IDropTarget *pDropTarget)
    {
        // remove drag+drop
        RevokeDragDrop(hwnd);
        // remove the strong lock
        CoLockObjectExternal(pDropTarget, FALSE, TRUE);
        // release our own reference
        pDropTarget->Release();
    }

};
