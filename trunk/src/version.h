// -*- C++ -*-
// VisualBoyAdvance - Nintendo Gameboy/GameboyAdvance (TM) emulator.
// Copyright (C) 1999-2003 Forgotten
// Copyright (C) 2004 Forgotten and the VBA development team

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or(at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#define VBA_NAME_W L"VisualBoyAdvance-M"
#define VBA_NAME_A "VisualBoyAdvance-M"
#ifdef UNICODE
#define VBA_NAME VBA_NAME_W
#else
#define VBA_NAME VBA_NAME_A
#endif

#ifdef WIN32
#include "svnrev.h"
#else
#ifdef SVN_REV
#define SVN_REV_STR SVN_REV
#else
#define SVN_REV_STR ""
#endif
#endif

#ifdef DEBUG
#define VBA_SUBVERSION_STRING_W L" debug"
#define VBA_SUBVERSION_STRING_A " debug"
#elif defined(PUBLIC_RELEASE)
#define VBA_SUBVERSION_STRING_W L""
#define VBA_SUBVERSION_STRING_A ""
#else
#define VBA_SUBVERSION_STRING_W L" (SVN" _T(SVN_REV_STR) L")"
#define VBA_SUBVERSION_STRING_A " (SVN" SVN_REV_STR ")"
#endif
#ifdef UNICODE
#define VBA_SUBVERSION_STRING VBA_SUBVERSION_STRING_W
#else
#define VBA_SUBVERSION_STRING VBA_SUBVERSION_STRING_A
#endif

#if defined(_MSC_VER)
#define VBA_COMPILER_DETAIL " msvc " _Py_STRINGIZE(_MSC_VER)
#define _Py_STRINGIZE(X) _Py_STRINGIZE1((X))
#define _Py_STRINGIZE1(X) _Py_STRINGIZE2 ## X
#define _Py_STRINGIZE2(X) #X
//re: http://72.14.203.104/search?q=cache:HG-okth5NGkJ:mail.python.org/pipermail/python-checkins/2002-November/030704.html+_msc_ver+compiler+version+string&hl=en&gl=us&ct=clnk&cd=5
#else
// TODO: make for others compilers
#define VBA_COMPILER ""
#define VBA_COMPILER_DETAIL ""
#endif

#define VBA_VERSION_STRING_A " " "1.8.0" VBA_SUBVERSION_STRING
#define VBA_VERSION_STRING_W L" " L"1.8.0" VBA_SUBVERSION_STRING
#ifdef UNICODE
#define VBA_VERSION_STRING VBA_VERSION_STRING_W
#else
#define VBA_VERSION_STRING VBA_VERSION_STRING_A
#endif

#define VBA_NAME_AND_VERSION " " VBA_NAME VBA_VERSION_STRING
#define VBA_NAME_AND_SUBVERSION_W L" " VBA_NAME VBA_SUBVERSION_STRING
#define VBA_NAME_AND_SUBVERSION_A " " VBA_NAME VBA_SUBVERSION_STRING
#ifdef UNICODE
#define VBA_NAME_AND_SUBVERSION VBA_NAME_AND_SUBVERSION_W
#else
#define VBA_NAME_AND_SUBVERSION VBA_NAME_AND_SUBVERSION_A
#endif