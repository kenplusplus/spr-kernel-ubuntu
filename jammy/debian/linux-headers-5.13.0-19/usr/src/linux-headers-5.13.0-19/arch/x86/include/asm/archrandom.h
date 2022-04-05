/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * This file is part of the Linux kernel.
 *
 * Copyright (c) 2011-2014, Intel Corporation
 * Authors: Fenghua Yu <fenghua.yu@intel.com>,
 *          H. Peter Anvin <hpa@linux.intel.com>
 */

#ifndef ASM_X86_ARCHRANDOM_H
#define ASM_X86_ARCHRANDOM_H

#include <linux/cc_platform.h>
#include <asm/processor.h>
#include <asm/cpufeature.h>

#define RDRAND_RETRY_LOOPS	10

#if defined(BOOT_COMPRESSED_MISC_H) && defined(CONFIG_INTEL_TDX_GUEST)
extern bool early_is_tdx_guest(void);
#endif

/* Unconditional execution of RDRAND and RDSEED */

static inline bool rd_loop(void)
{
#if defined(BOOT_COMPRESSED_MISC_H) && defined(CONFIG_INTEL_TDX_GUEST)
	/* In decompression code */
	return early_is_tdx_guest();
#else
	return cc_platform_has(CC_ATTR_GUEST_RAND_LOOP);
#endif
}

static inline bool __must_check rdrand_long(unsigned long *v)
{
	bool ok;
	unsigned int retry = RDRAND_RETRY_LOOPS;
	do {
		asm volatile("rdrand %[out]"
			     CC_SET(c)
			     : CC_OUT(c) (ok), [out] "=r" (*v));
		if (ok)
			return true;
	} while (--retry || rd_loop());
	return false;
}

static inline bool __must_check rdrand_int(unsigned int *v)
{
	bool ok;
	unsigned int retry = RDRAND_RETRY_LOOPS;
	do {
		asm volatile("rdrand %[out]"
			     CC_SET(c)
			     : CC_OUT(c) (ok), [out] "=r" (*v));
		if (ok)
			return true;
	} while (--retry || rd_loop());
	return false;
}

static inline bool __must_check rdseed_long(unsigned long *v)
{
	bool ok;
	do {
		asm volatile("rdseed %[out]"
			     CC_SET(c)
			     : CC_OUT(c) (ok), [out] "=r" (*v));
		if (ok)
			return ok;
	} while (rd_loop());
	return false;
}

static inline bool __must_check rdseed_int(unsigned int *v)
{
	bool ok;
	do {
		asm volatile("rdseed %[out]"
			     CC_SET(c)
			     : CC_OUT(c) (ok), [out] "=r" (*v));
		if (ok)
			return ok;
	} while (rd_loop());
	return false;
}

/*
 * These are the generic interfaces; they must not be declared if the
 * stubs in <linux/random.h> are to be invoked,
 * i.e. CONFIG_ARCH_RANDOM is not defined.
 */
#ifdef CONFIG_ARCH_RANDOM

static inline bool __must_check arch_get_random_long(unsigned long *v)
{
	return static_cpu_has(X86_FEATURE_RDRAND) ? rdrand_long(v) : false;
}

static inline bool __must_check arch_get_random_int(unsigned int *v)
{
	return static_cpu_has(X86_FEATURE_RDRAND) ? rdrand_int(v) : false;
}

static inline bool __must_check arch_get_random_seed_long(unsigned long *v)
{
	return static_cpu_has(X86_FEATURE_RDSEED) ? rdseed_long(v) : false;
}

static inline bool __must_check arch_get_random_seed_int(unsigned int *v)
{
	return static_cpu_has(X86_FEATURE_RDSEED) ? rdseed_int(v) : false;
}

extern void x86_init_rdrand(struct cpuinfo_x86 *c);

#else  /* !CONFIG_ARCH_RANDOM */

static inline void x86_init_rdrand(struct cpuinfo_x86 *c) { }

#endif  /* !CONFIG_ARCH_RANDOM */

#endif /* ASM_X86_ARCHRANDOM_H */
