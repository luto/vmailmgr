// Copyright (C) 1999 Bruce Guenter <bruceg@em.ca>
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
#include <stdlib.h>
#include "fdbuf.h"
#include "mystring.h"
#include "response.h"
#include "configrc.h"
#include "cli.h"
#include "vcommand.h"

const char* cli_program = "vpasswds";
const char* cli_help_prefix =
"Changes the passwords for a list of virtual users\n";
const char* cli_help_suffix = "";
const char* cli_args_usage = "";
const int cli_args_min = 0;
const int cli_args_max = 0;
cli_option cli_options[] = { {0} };

static int errors = 0;

mystring get_word(mystring& line)
{
  unsigned i = 0;
  while(i < line.length() && isspace(line[i]))
    ++i;
  unsigned start = i;
  while(i < line.length() && !isspace(line[i]))
    ++i;
  mystring word = line.sub(start, i-start);
  line = line.right(i);
  return word;
}
  
void change_one(const mystring& line)
{
  mystring str = line;
  mystring user = get_word(str);
  mystring pass = get_word(str);
  if(!user || !pass) {
    ++errors;
    ferr << "vpasswds: invalid line, ignoring:\n  "
	 << line << endl;
    return;
  }
  user = user.lower();

  response resp = domain.chattr(user, vdomain::ATTR_PASS, pass);

  if(!resp) {
    ++errors;
    ferr << "vpasswds: error changing the password for user '"
	 << user << "':\n  " << resp.msg << endl;
    return;
  }
  else
    fout << "vpasswds: password for user '" << user
	 << "' successfully changed.\n";
}

int cli_main(int, char*[])
{
  if(!go_home())
    return 1;
  mystring str;
  while(fin.getline(str))
    change_one(str);
  if(errors) {
    ferr << "vpasswds: " << errors << " errors were encountered." << endl;
    return 1;
  }
  return 0;
}
