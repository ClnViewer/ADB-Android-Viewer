//
//  Origin:
//  DROPTARGET.CPP
//  By J Brown 2004
//  www.catch22.net
//

#pragma once

namespace GameDev
{
    //
    //  implements IDropTarget interface
    //
    class OLEDropTarget final : public IDropTarget {
        public:
            // IUnknown implementation
            HRESULT __stdcall QueryInterface(REFIID, void**);
            ULONG   __stdcall AddRef();
            ULONG   __stdcall Release();

            // IDropTarget implementation
            HRESULT __stdcall DragEnter(IDataObject*, DWORD, POINTL, DWORD*);
            HRESULT __stdcall DragOver(DWORD, POINTL, DWORD*);
            HRESULT __stdcall DragLeave();
            HRESULT __stdcall Drop(IDataObject*, DWORD, POINTL, DWORD*);

            // Constructor
            OLEDropTarget(HWND);
            ~OLEDropTarget();
            //
            static void DropData(HWND, IDataObject*);
            static void RegisterDropWindow(HWND, IDropTarget**);
            static void UnregisterDropWindow(HWND, IDropTarget*);

        private:
            // internal helper function
            DWORD DropEffect(DWORD, POINTL pt, DWORD);
            bool  QueryDataObject(IDataObject*);
            // Private member variables
            LONG         m_lRefCount;
            HWND         m_hWnd;
            bool         m_fAllowDrop;
            IDataObject *m_pDataObject;

    };

};
