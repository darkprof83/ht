/* 
 *	HT Editor
 *	htlehead.cc
 *
 *	Copyright (C) 1999, 2000, 2001 Stefan Weyergraf (stefan@weyergraf.de)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "htatom.h"
#include "htnewexe.h"
#include "htle.h"
#include "htlehead.h"
#include "httag.h"
#include "formats.h"

#include "lestruct.h"

int_hash le_machines[] =
{
	{ IMAGE_LE_CPU_286,	"Intel 286" },
	{ IMAGE_LE_CPU_386,	"Intel 386" },
	{ IMAGE_LE_CPU_486,	"Intel 486" },
	{ IMAGE_LE_CPU_586,	"Intel 586" },
	{ IMAGE_LE_CPU_N10,	"Intel i860(N10)" },
	{ IMAGE_LE_CPU_N11,	"Intel N11" },
	{ IMAGE_LE_CPU_R2000, "MIPS R2000"	},
	{ IMAGE_LE_CPU_R6000, "MIPS R6000"	},
	{ IMAGE_LE_CPU_R4000, "MIPS R4000"	},
	{0, 0}
};

int_hash le_os[] =
{
	{ IMAGE_LE_OS_OS2, "OS/2"},
	{ IMAGE_LE_OS_WIN, "Windows"},
	{ IMAGE_LE_OS_DOS4,	"DOS 4.x"},
	{ IMAGE_LE_OS_WIN386, "Windows 386" },
	{0, 0}
};

ht_mask_ptable leheader[]=
{
	{"magic",							STATICTAG_EDIT_WORD_LE("00000000")},
	{"byte order",						STATICTAG_EDIT_BYTE("00000002")},
	{"word order",						STATICTAG_EDIT_BYTE("00000003")},
	{"level",							STATICTAG_EDIT_DWORD_LE("00000004")},
	{"cpu type",						STATICTAG_EDIT_WORD_LE("00000008")" "STATICTAG_DESC_WORD_LE("00000008", ATOM_LE_MACHINE_STR)},
	{"os type",						STATICTAG_EDIT_WORD_LE("0000000a")" "STATICTAG_DESC_WORD_LE("0000000a", ATOM_LE_OS_STR)},
	{"module version",					STATICTAG_EDIT_DWORD_LE("0000000c")},
	{"module flags",					STATICTAG_EDIT_DWORD_LE("00000010")},
	{"number of pages",					STATICTAG_EDIT_DWORD_LE("00000014")},
	{"logical cs",						STATICTAG_EDIT_DWORD_LE("00000018")},
	{"eip",							STATICTAG_EDIT_DWORD_LE("0000001c")},
	{"logical ss",						STATICTAG_EDIT_DWORD_LE("00000020")},
	{"esp",							STATICTAG_EDIT_DWORD_LE("00000024")},
	{"memory page size",				STATICTAG_EDIT_DWORD_LE("00000028")},
	{"last page size",					STATICTAG_EDIT_DWORD_LE("0000002c")},
	{"fixup section size",				STATICTAG_EDIT_DWORD_LE("00000030")},
	{"fixup section checksum",			STATICTAG_EDIT_DWORD_LE("00000034")},
	{"loader section size",				STATICTAG_EDIT_DWORD_LE("00000038")},
	{"loader section checksum",			STATICTAG_EDIT_DWORD_LE("0000003c")},
	{"offset of object table",			STATICTAG_EDIT_DWORD_LE("00000040")},
	{"number of objects",				STATICTAG_EDIT_DWORD_LE("00000044")},
	{"offset page map",					STATICTAG_EDIT_DWORD_LE("00000048")},
	{"offset iterated data",				STATICTAG_EDIT_DWORD_LE("0000004c")},
	{"offset of resource table",			STATICTAG_EDIT_DWORD_LE("00000050")},
	{"number of resource entries",		STATICTAG_EDIT_DWORD_LE("00000054")},
	{"offset of resident name table",		STATICTAG_EDIT_DWORD_LE("00000058")},
	{"offset of entry table",			STATICTAG_EDIT_DWORD_LE("0000005c")},
	{"offset of module directive table",	STATICTAG_EDIT_DWORD_LE("00000060")},
	{"number of module directives",		STATICTAG_EDIT_DWORD_LE("00000064")},
	{"offset of fixup page table",		STATICTAG_EDIT_DWORD_LE("00000068")},
	{"offset of fixup record table",		STATICTAG_EDIT_DWORD_LE("0000006c")},
	{"offset of import module name table",	STATICTAG_EDIT_DWORD_LE("00000070")},
	{"number of imported modules",		STATICTAG_EDIT_DWORD_LE("00000074")},
	{"offset of import procedure name table", STATICTAG_EDIT_DWORD_LE("00000078")},
	{"offset of per-page checksum table",	STATICTAG_EDIT_DWORD_LE("0000007c")},
	{"offset of enumerated data pages",	STATICTAG_EDIT_DWORD_LE("00000080")},
	{"number of preload pages",			STATICTAG_EDIT_DWORD_LE("00000084")},
	{"offset of non-resident names table",	STATICTAG_EDIT_DWORD_LE("00000088")},
	{"size of non-resident names table",	STATICTAG_EDIT_DWORD_LE("0000008c")},
	{"non-resident name table checksum",	STATICTAG_EDIT_DWORD_LE("00000090")},
	{"auto data object number",			STATICTAG_EDIT_DWORD_LE("00000094")},
	{"offset of debug info",				STATICTAG_EDIT_DWORD_LE("00000098")},
	{"size of debug info",				STATICTAG_EDIT_DWORD_LE("0000009c")},
	{"pages in preload section", 			STATICTAG_EDIT_DWORD_LE("000000a0")},
	{"pages in demand load section", 		STATICTAG_EDIT_DWORD_LE("000000a4")},
	{"size of heap (16-bit)", 			STATICTAG_EDIT_DWORD_LE("000000a8")},
	{"reserved (12 bytes)", 				STATICTAG_EDIT_BYTE("000000ac")" "STATICTAG_EDIT_BYTE("000000ad")" "STATICTAG_EDIT_BYTE("000000ae")" "STATICTAG_EDIT_BYTE("000000af")},
	{0, 								STATICTAG_EDIT_BYTE("000000b0")" "STATICTAG_EDIT_BYTE("000000b1")" "STATICTAG_EDIT_BYTE("000000b2")" "STATICTAG_EDIT_BYTE("000000b3")},
	{0, 								STATICTAG_EDIT_BYTE("000000b4")" "STATICTAG_EDIT_BYTE("000000b5")" "STATICTAG_EDIT_BYTE("000000b6")" "STATICTAG_EDIT_BYTE("000000b7")},
	{"--- VxD only ---", 0},
	{"offset of win-resource data",		STATICTAG_EDIT_DWORD_LE("000000b8")},
	{"size of win-resource data",			STATICTAG_EDIT_DWORD_LE("000000bc")},
	{"device id",						STATICTAG_EDIT_WORD_LE("000000c0")},
	{"ddk version",					STATICTAG_EDIT_WORD_LE("000000c2")},
	{0, 0}
};

ht_view *htleheader_init(bounds *b, ht_streamfile *file, ht_format_group *group)
{
	ht_le_shared_data *le_shared=(ht_le_shared_data *)group->get_shared_data();

/* FIXME: */
	bool le_bigendian = false;
	int h=le_shared->hdr_ofs;
	ht_uformat_viewer *v=new ht_uformat_viewer();
	v->init(b, DESC_LE_HEADER, VC_EDIT | VC_SEARCH, file, group);
	ht_mask_sub *m=new ht_mask_sub();
	m->init(file, 0);
	register_atom(ATOM_LE_MACHINE, le_machines);
	register_atom(ATOM_LE_OS, le_os);
	char info[128];
	sprintf(info, "* LE header at offset %08x", le_shared->hdr_ofs);
	m->add_mask(info);
	m->add_staticmask_ptable(leheader, h, le_bigendian);
	v->insertsub(m);

	le_shared->v_header=v;
	return v;
}

format_viewer_if htleheader_if = {
	htleheader_init,
	0
};
