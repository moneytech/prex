/*-
 * Copyright (c) 2005-2007, Kohsuke Ohtani
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _KERNEL_H
#define _KERNEL_H

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/list.h>
#include <sys/errno.h>
#include <prex/bootinfo.h>
#include <prex/types.h>
#include <machine/stdarg.h>
#include <arch.h>
#include <platform.h>
#include <debug.h>

typedef struct vm_map	*vm_map_t;

typedef void (*sysfn_t)(void);
typedef void (*dkifn_t)(void);


#include <prex/sysinfo.h>

/*
 * Magic numbers
 */
#define OBJECT_MAGIC	0x4f626a3f	/* 'Obj?' */
#define TASK_MAGIC	0x54736b3f	/* 'Tsk?' */
#define THREAD_MAGIC	0x5468723f	/* 'Thr?' */
#define DEVICE_MAGIC	0x4465763f	/* 'Dev?' */
#define MUTEX_MAGIC	0x4d75783f	/* 'Mux?' */
#define COND_MAGIC	0x436f6e3f	/* 'Con?' */
#define SEM_MAGIC	0x53656d3f	/* 'Sem?' */

/*
 * Global variables in the kernel
 */
extern struct thread	*cur_thread;	/* pointer to the current thread */
extern struct task	kern_task;	/* kernel task */
extern struct bootinfo	*bootinfo;	/* pointer to boot information */
extern volatile int	irq_level;	/* current interrupt level */
extern const dkifn_t	driver_service[];
extern const sysfn_t	syscall_table[];
extern const u_int	nr_syscalls;

__BEGIN_DECLS
long	 atol(const char *nptr);
size_t	 strlcpy(char *, const char *, size_t);
char	*strncpy(char *, const char *, size_t);
int	 strncmp(const char *, const char *, size_t);
size_t	 strnlen(const char *, size_t);
void	*memcpy(void *, const void *, size_t);
void	*memset(void *, int, size_t);
int	 sprintf(char *, const char *, ...);
int	 vsprintf(char *, const char *, va_list);
void	 delay_usec(u_long usec);
void	 calibrate_delay(void);
__BEGIN_DECLS

/* Export symbols for drivers. Place the symbol name in .kstrtab and a
 * struct kernel_symbol in the .ksymtab. The elf loader will use this
 * information to resolve these symbols in driver modules */
struct kernel_symbol
{
	u_long value;
	const char *name;
};

#define __EXPORT_SYMBOL(__n, __v)					\
	static const char __kstrtab_##__n[]				\
	__attribute__((section(".kstrtab")))				\
		= #__n;							\
	static const struct kernel_symbol __ksymtab_##__n		\
	__attribute__((__used__))					\
		__attribute__((section(".ksymtab"), unused))		\
		= { .value = (u_long)&__v, .name = __kstrtab_##__n }
#define EXPORT_SYMBOL(sym) __EXPORT_SYMBOL(sym, sym)

/* useful macros to provide information to optimiser */
#define likely(x) __builtin_expect((!!(x)),1)
#define unlikely(x) __builtin_expect((!!(x)),0)

#endif /* !_KERNEL_H */
