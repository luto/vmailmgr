// Copyright (C) 1999,2000 Bruce Guenter <bruceg@em.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <config.h>
#include <ctype.h>
#include <string.h>
#include "vpwentry.h"

const char* from_uint(const char* ptr, unsigned& uint)
{
  if(*ptr == '-') {
    uint = (unsigned)-1;
    ++ptr;
    while(isdigit(*ptr))
      ++ptr;
  }
  else {
    uint = 0;
    while(isdigit(*ptr))
      uint = (uint * 10) + (*ptr++ - '0');
  }
  if(*ptr != 0)
    return 0;
  return ++ptr;
}
  
bool vpwentry::from_ver1_record(vpwentry& vpw, const mystring& text)
{
  // Sanity check to ensure that text conversions can't
  // fall off the end of the string.
  if(text[text.length()-1] != 0)
    return false;

  const char* ptr = text.c_str();
  const char* const end = ptr++ + text.length();

  vpw.pass = ptr;
  ptr += vpw.pass.length() + 1;
  if(ptr >= end) return false;
  
  vpw.mailbox = ptr;
  ptr += vpw.mailbox.length() + 1;
  if(ptr >= end) return false;
  
  const char* start = ptr;
  while(ptr < end && *ptr != 0)
    ptr += strlen(ptr) + 1;
  if(ptr == start)
    vpw.forwards = "";
  else
    vpw.forwards = mystring(start, ptr-start-1);
  if(ptr++ >= end) return false;
  
  vpw.personal = ptr;
  ptr += vpw.personal.length() + 1;
  if(ptr >= end) return false;
  
  if((ptr = from_uint(ptr, vpw.hardquota)) == 0 || ptr >= end) return false;
  if((ptr = from_uint(ptr, vpw.softquota)) == 0 || ptr >= end) return false;
  if((ptr = from_uint(ptr, vpw.msgsize)) == 0 || ptr >= end) return false;
  if((ptr = from_uint(ptr, vpw.msgcount)) == 0 || ptr >= end) return false;
  if((ptr = from_uint(ptr, vpw.ctime)) == 0 || ptr >= end) return false;
  if((ptr = from_uint(ptr, vpw.expiry)) == 0 || ptr > end) return false;
  
  //vpw.data = ptr;
  
  return ptr == end;
}
