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
#include "misc/autodelete.h"
#include "cdb++/cdb++.h"
#include "vpwtable.h"

void vpwtable::end()
{
  delete (cdb_reader*)getdata;
  getdata = 0;
}

bool vpwtable::start()
{
  end();
  getdata = new cdb_reader(filename);
  if(!*(cdb_reader*)getdata) {
    end();
    return false;
  }
  return true;
}

bool vpwtable::rewind()
{
  return getdata && ((cdb_reader*)getdata)->firstrec();
}

bool vpwtable::get(vpwentry& out)
{
  autodelete<datum> rec = ((cdb_reader*)getdata)->nextrec();
  if(!rec)
    return false;
  if(!out.from_record(rec->key, rec->data))
    return false;
  return true;
}
