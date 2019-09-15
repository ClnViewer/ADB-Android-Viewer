//---------------------------------------------------------
// Name: CWindowDock.cpp
// Desc: Win32 API window docking system. Makes a list of
//       windows snap and dock to the parent window when
//       moved within a specified distance. Docked windows
//       will move with the parent window.
//---------------------------------------------------------
// Copyright © 2005 Nick Bloor
//---------------------------------------------------------

#include "WindowDock.h"

namespace Editor
{

CWDChild::CWDChild(): m_Hwnd(0), m_DockType(0), m_x(0), m_y(0) {}
CWDChild::~CWDChild() {}

bool CWDChild::operator ==(const CWDChild &rhs)
{
	if(m_Hwnd == rhs.GetHandle())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CWDChild::SetHandle(HWND hwnd)
{
	m_Hwnd = hwnd;
}

void CWDChild::SetDockType(unsigned char DockType)
{
	m_DockType = DockType;
}

void CWDChild::SetX(long x)
{
	m_x = x;
}

void CWDChild::SetY(long y)
{
	m_y = y;
}

HWND CWDChild::GetHandle() const
{
	return m_Hwnd;
}

unsigned char CWDChild::GetDockType() const
{
	return m_DockType;
}

long CWDChild::GetX() const
{
	return m_x;
}

long CWDChild::GetY() const
{
	return m_y;
}

CWindowDock::CWindowDock(): m_Parent(0), m_Threshold(CWD_DEF_THRESHOLD), m_Working(false),
							m_AutoDock(false)
{
}

CWindowDock::~CWindowDock()
{
	CWDChild *curChild = 0;

	while(m_ChildList.size() > 0)
	{
		curChild = m_ChildList.back();
		m_ChildList.pop_back();
		delete curChild;
	}
}

void CWindowDock::SetParent(HWND hParent)
{
	if(IsWindow(hParent) == TRUE)
	{
		m_Parent = hParent;
	}
}

void CWindowDock::SetThreshold(long lThreshold)
{
	m_Threshold = lThreshold;
}

void CWindowDock::AddChild(HWND hChild, bool AutoDock)
{
	CWDChild *newChild;

	if(IsWindow(hChild) == false)
	{
		return;
	}

	newChild = new CWDChild;
	if(newChild == 0)
	{
		return;
	}
	newChild->SetDockType(CWD_DT_NONE);
	newChild->SetHandle(hChild);
	newChild->SetX(0);
	newChild->SetY(0);

	m_ChildList.push_back(newChild);

	if(AutoDock == true)
	{
		m_AutoDock = true;
		WindowMoved(hChild);
		m_AutoDock = false;
	}
}

void CWindowDock::RemoveChild(HWND hChild)
{
	CWDChild tmpChild;

	tmpChild.SetHandle(hChild);
	m_ChildList.remove(&tmpChild);
}

CWDChild *CWindowDock::FindChild(HWND hChild)
{
	std::list<CWDChild*>::iterator i;

	for(i = m_ChildList.begin(); i != m_ChildList.end(); i++)
	{
		if((*i)->GetHandle() == hChild)
		{
			return (*i);
		}
	}
	return 0;
}

void CWindowDock::WindowMoved(HWND hwnd)
{
	std::list<CWDChild*>::iterator i;
	CWDChild *child = 0;
	RECT rcParent;
	RECT rcChild;
	RECT rcNew;
	bool bVert = false;
	bool bHorz = false;

	if(m_Working == true)
	{
		return;
	}
	m_Working = true;

	if(hwnd != m_Parent)
	{
		child = FindChild(hwnd);
		if(child == 0)
		{
			m_Working = false;
			return;
		}

		::GetWindowRect(m_Parent, &rcParent);
		::GetWindowRect(hwnd, &rcChild);
		::memcpy(&rcNew, &rcChild, sizeof(RECT));

		if(rcChild.left > rcParent.right - m_Threshold && rcChild.left < rcParent.right + m_Threshold)
		{
			bVert = true;
			rcNew.left = rcParent.right;
			child->SetDockType(CWD_DT_RIGHT);
			child->SetX(0);
			child->SetY(rcChild.top - rcParent.top);
		}
		if(rcChild.right > rcParent.left - m_Threshold && rcChild.right < rcParent.left + m_Threshold)
		{
			bVert = true;
			rcNew.left = rcParent.left - (rcChild.right - rcChild.left);
			child->SetDockType(CWD_DT_LEFT);
			child->SetX(0);
			child->SetY(rcChild.top - rcParent.top);
		}
		if(rcChild.top > rcParent.bottom - m_Threshold && rcChild.top < rcParent.bottom + m_Threshold)
		{
			bHorz = true;
			rcNew.top = rcParent.bottom;
			child->SetDockType(CWD_DT_BOTTOM);
			child->SetX(rcChild.left - rcParent.left);
			child->SetY(0);
		}
		if(rcChild.bottom > rcParent.top - m_Threshold && rcChild.bottom < rcParent.top + m_Threshold)
		{
			bHorz = true;
			rcNew.top = rcParent.top - (rcChild.bottom - rcChild.top);
			child->SetDockType(CWD_DT_TOP);
			child->SetX(rcChild.left - rcParent.left);
			child->SetY(0);
		}
		if(rcChild.left > rcParent.left - m_Threshold && rcChild.left < rcParent.left + m_Threshold)
		{
			if(bHorz == true)
			{
				rcNew.left = rcParent.left;
				if(child->GetDockType() == CWD_DT_TOP)
				{
					child->SetDockType(CWD_DT_TOPLEFT);
					child->SetX(0);
					child->SetY(0);
				}
				if(child->GetDockType() == CWD_DT_BOTTOM)
				{
					child->SetDockType(CWD_DT_BOTTOMLEFT);
					child->SetX(0);
					child->SetY(0);
				}
			}
		}
		if(rcChild.right > rcParent.right - m_Threshold && rcChild.right < rcParent.right + m_Threshold)
		{
			if(bHorz == true)
			{
				rcNew.left = rcParent.right - (rcChild.right - rcChild.left);
				if(child->GetDockType() == CWD_DT_TOP)
				{
					child->SetDockType(CWD_DT_TOPRIGHT);
					child->SetX(0);
					child->SetY(0);
				}
				if(child->GetDockType() == CWD_DT_BOTTOM)
				{
					child->SetDockType(CWD_DT_BOTTOMRIGHT);
					child->SetX(0);
					child->SetY(0);
				}
			}
		}
		if(rcChild.top > rcParent.top - m_Threshold && rcChild.top < rcParent.top + m_Threshold)
		{
			if(bVert == true)
			{
				rcNew.top = rcParent.top;
				if(child->GetDockType() == CWD_DT_LEFT)
				{
					child->SetDockType(CWD_DT_LEFTTOP);
					child->SetX(0);
					child->SetY(0);
				}
				if(child->GetDockType() == CWD_DT_RIGHT)
				{
					child->SetDockType(CWD_DT_RIGHTTOP);
					child->SetX(0);
					child->SetY(0);
				}
			}
		}
		if(rcChild.bottom > rcParent.bottom - m_Threshold && rcChild.bottom < rcParent.bottom + m_Threshold)
		{
			if(bVert == true)
			{
				rcNew.top = rcParent.bottom - (rcChild.bottom - rcChild.top);
				if(child->GetDockType() == CWD_DT_LEFT)
				{
					child->SetDockType(CWD_DT_LEFTBOTTOM);
					child->SetX(0);
					child->SetY(0);
				}
				if(child->GetDockType() == CWD_DT_RIGHT)
				{
					child->SetDockType(CWD_DT_RIGHTBOTTOM);
					child->SetX(0);
					child->SetY(0);
				}
			}
		}
		if(rcNew.left != rcChild.left || rcNew.right != rcChild.right || rcNew.top != rcChild.top || rcNew.bottom != rcChild.bottom || m_AutoDock == true)
		{
			SetWindowPos(hwnd, 0, rcNew.left, rcNew.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		else
		{
			child->SetDockType(CWD_DT_NONE);
			child->SetX(0);
			child->SetY(0);
		}
	}
	else
	{
		GetWindowRect(m_Parent, &rcParent);
		memcpy(&rcNew, &rcParent, sizeof(RECT));

		for(i = m_ChildList.begin(); i != m_ChildList.end(); i++)
		{
			child = *i;
			::memcpy(&rcNew, &rcParent, sizeof(RECT));
			::GetWindowRect(child->GetHandle(), &rcChild);

			switch(child->GetDockType())
			{
			case CWD_DT_NONE:
				rcNew.left = rcChild.left;
				rcNew.top = rcChild.top;
				break;

			case CWD_DT_TOP:
				rcNew.left += child->GetX();
				rcNew.top -= (rcChild.bottom - rcChild.top);
				break;

			case CWD_DT_TOPRIGHT:
				rcNew.left = rcParent.right - (rcChild.right - rcChild.left);
				rcNew.top -= (rcChild.bottom - rcChild.top);
				break;

			case CWD_DT_RIGHTTOP:
				rcNew.left = rcParent.right;
				break;

			case CWD_DT_RIGHT:
				rcNew.left = rcParent.right;
				rcNew.top += child->GetY();
				break;

			case CWD_DT_RIGHTBOTTOM:
				rcNew.left = rcParent.right;
				rcNew.top = rcParent.bottom - (rcChild.bottom - rcChild.top);
				break;

			case CWD_DT_BOTTOMRIGHT:
				rcNew.left = rcParent.right - (rcChild.right - rcChild.left);
				rcNew.top = rcParent.bottom;
				break;

			case CWD_DT_BOTTOM:
				rcNew.left += child->GetX();
				rcNew.top = rcParent.bottom;
				break;

			case CWD_DT_BOTTOMLEFT:
				rcNew.top = rcParent.bottom;
				break;

			case CWD_DT_LEFTBOTTOM:
				rcNew.left -= (rcChild.right - rcChild.left);
				rcNew.top = rcParent.bottom - (rcChild.bottom - rcChild.top);
				break;

			case CWD_DT_LEFT:
				rcNew.left -= (rcChild.right - rcChild.left);
				rcNew.top += child->GetY();
				break;

			case CWD_DT_LEFTTOP:
				rcNew.left -= (rcChild.right - rcChild.left);
				break;

			case CWD_DT_TOPLEFT:
				rcNew.top -= (rcChild.bottom - rcChild.top);
				break;

			default:
				rcNew.left = rcChild.left;
				rcNew.top = rcChild.top;
				break;
			}
			SetWindowPos(child->GetHandle(), 0, rcNew.left, rcNew.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
	}
	m_Working = false;
}

};
