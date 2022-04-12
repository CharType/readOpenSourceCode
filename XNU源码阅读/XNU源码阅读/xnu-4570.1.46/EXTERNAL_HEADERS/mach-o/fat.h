/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#ifndef _MACH_O_FAT_H_
#define _MACH_O_FAT_H_
/*
 * This header file describes the structures of the file format for "fat"
 * architecture specific file (wrapper design).  At the begining of the file
 * there is one fat_header structure followed by a number of fat_arch
 * structures.  For each architecture in the file, specified by a pair of
 * cputype and cpusubtype, the fat_header describes the file offset, file
 * size and alignment in the file of the architecture specific member.
 * The padded bytes in the file to place each member on it's specific alignment
 * are defined to be read as zeros and can be left as "holes" if the file system
 * can support them as long as they read as zeros.
 *
 * All structures defined here are always written and read to/from disk
 * in big-endian order.
 */

/*
 * <mach/machine.h> is needed here for the cpu_type_t and cpu_subtype_t types
 * and contains the constants for the possible values of these types.
 */
#include <stdint.h>
#include <mach/machine.h>
#include <architecture/byte_order.h>

#define FAT_MAGIC	0xcafebabe
#define FAT_CIGAM	0xbebafeca	/* NXSwapLong(FAT_MAGIC) */

// fatHeader描述
struct fat_header {
    // 是一个常量 FAT_MAGIC 加载器通过这个常量值判断是什么类型的文件,通用二进制格式还是胖二进制格式
	uint32_t	magic;		/* FAT_MAGIC */
    // 指当前通用的二进制文件包含了多少个不同架构的Mac-O文件
	uint32_t	nfat_arch;	/* number of structs that follow */
};

// nfat_arch 有多少个二进制文件就会有多少个 fat_arch 结构体
struct fat_arch {
    // 标识CPU架构 arm，X86_64
	cpu_type_t	cputype;	/* cpu specifier (int) */
    // 标识具体的CPU类型，区分不同版本的处理器 armv7 类型或者是arm64架构类型
	cpu_subtype_t	cpusubtype;	/* machine specifier (int) */
    // 指定该CPU架构数据相对于文件开头的偏移量
	uint32_t	offset;		/* file offset to this object file */
    // 指定该CPU架构的数据大小
	uint32_t	size;		/* size of this object file */
    // 指定数据的内存对齐边界，取值必须是2的N次方，可以使用otool工具 -f 查看fat_header结构 -v参数显示详细的参数
	uint32_t	align;		/* alignment as a power of 2 */
};

#endif /* _MACH_O_FAT_H_ */
