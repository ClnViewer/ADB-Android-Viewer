//
//  Origin:
//  DROPSOURCE.CPP
//  Implementation of the IDropSource COM interface
//  By J Brown 2004
//  www.catch22.net
//

#pragma once

namespace GameDev
{
    class OLEDropSource final : public IDropSource
    {
        public:
            //
            // IUnknown members
            //
            HRESULT __stdcall QueryInterface(REFIID, void**);
            ULONG   __stdcall AddRef();
            ULONG   __stdcall Release();
            //
            // IDropSource members
            //
            HRESULT __stdcall QueryContinueDrag(BOOL, DWORD);
            HRESULT __stdcall GiveFeedback(DWORD);
            //
            // Constructor / Destructor
            //
            OLEDropSource();
            ~OLEDropSource();
            //
            static HRESULT CreateDropSource(IDropSource**);
            static void    BeginDrag(std::string const&);

        private:
            //
            // private members and functions
            //
            LONG       m_lRefCount;
    };

};
