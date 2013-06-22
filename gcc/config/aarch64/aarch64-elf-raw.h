/* Machine description for AArch64 architecture.
   Copyright (C) 2009, 2010, 2011, 2012 Free Software Foundation, Inc.
   Contributed by ARM Ltd.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* Support for bare-metal builds.  */
#ifndef GCC_AARCH64_ELF_RAW_H
#define GCC_AARCH64_ELF_RAW_H

#define STARTFILE_SPEC " crti%O%s crtbegin%O%s crt0%O%s"
#define ENDFILE_SPEC " crtend%O%s crtn%O%s"

#ifndef LINK_SPEC
#define LINK_SPEC "%{mbig-endian:-EB} %{mlittle-endian:-EL} -X \
%{milp32:-maarch64elf%{mbig-endian:b}32} \
%{mlp64:-maarch64elf%{mbig-endian:b}}"
#endif

#endif /* GCC_AARCH64_ELF_RAW_H */
