#! %PYTHON%
#
# Copyright (C) 1999 Bruce Guenter <bruceg@em.ca>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import vmailmgr

def main(form):
    command = vmailmgr.Command(form.get('command'),
                               form.fetch_args())
    response = vmailmgr.Daemon().execute(command)
    data = {
        'command': command.name,
        'response_code': response.code,
        'response_msg': response.msg
        }
    for i in range(len(command.args)):
        data['arg%d'%i] = command.args[i]
    basename = form.get('filename', command.name)
    vmailmgr.print_html(basename, data)

if __name__ == '__main__': vmailmgr.cgi_main(main)
