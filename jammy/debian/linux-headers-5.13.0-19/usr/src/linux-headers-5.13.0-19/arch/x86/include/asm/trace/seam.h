/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM seam

#if !defined(_TRACE_SEAM_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_SEAM_H

#include <linux/tracepoint.h>

#ifdef SEAMLDR_SEAMCALLS
#define SEAMCALLS		\
	SEAMLDR_SEAMCALLS,	\
	TDX_SEAMCALLS
#elif defined(TDX_SEAMCALLS)
#define SEAMCALLS		\
	TDX_SEAMCALLS
#else
#define SEAMCALLS {}
#endif

TRACE_EVENT(seamcall_enter,
	    TP_PROTO(u64 fn, u64 rcx, u64 rdx,
		     u64 r8, u64 r9, u64 r10, u64 r11),
	    TP_ARGS(fn, rcx, rdx, r8, r9, r10, r11),
	    TP_STRUCT__entry(
		__field(u64, fn)
		__field(u64, rcx)
		__field(u64, rdx)
		__field(u64, r8)
		__field(u64, r9)
		__field(u64, r10)
		__field(u64, r11)
	    ),
	    TP_fast_assign(
		__entry->fn = fn;
		__entry->rcx = rcx;
		__entry->rdx = rdx;
		__entry->r8 = r8;
		__entry->r9 = r9;
		__entry->r10 = r10;
		__entry->r11 = r11;
	    ),
	    TP_printk("op: %s %llx %llx %llx %llx %llx %llx",
		__print_symbolic_u64(__entry->fn, SEAMCALLS),
		__entry->rcx,
		__entry->rdx,
		__entry->r8,
		__entry->r9,
		__entry->r10,
		__entry->r11
	    )
);

struct tdx_ex_ret;
TRACE_EVENT(seamcall_exit,
	    TP_PROTO(u64 fn, u64 err,
		     const struct tdx_ex_ret *ex_ret),
	    TP_ARGS(fn, err, ex_ret),
	    TP_STRUCT__entry(
		__field(u64, fn)
		__field(u64, err)
		__field(u64, rcx)
		__field(u64, rdx)
		__field(u64, r8)
		__field(u64, r9)
		__field(u64, r10)
		__field(u64, r11)
	    ),
	    TP_fast_assign(
		__entry->fn = fn;
		__entry->err = err;
		__entry->rcx = ex_ret->regs.rcx;
		__entry->rdx = ex_ret->regs.rdx;
		__entry->r8 = ex_ret->regs.r8;
		__entry->r9 = ex_ret->regs.r9;
		__entry->r10 = ex_ret->regs.r10;
		__entry->r11 = ex_ret->regs.r11;
	    ),
	    TP_printk("op: %s err: %s(%llx) %llx %llx %llx %llx %llx %llx",
		__print_symbolic_u64(__entry->fn, SEAMCALLS),
#ifdef SEAMLDR_ERROR_CODES
		({ ((__entry->err & TDX_SEAMCALL_STATUS_MASK) ==
			  P_SEAMLDR_SEAMCALL_ERROR_CODE) ?
				__print_symbolic_u64(__entry->err,
						     P_SEAMLDR_ERROR_CODES) :
				__print_symbolic_u64(__entry->err &
						     TDX_SEAMCALL_STATUS_MASK,
						     TDX_STATUS_CODES); }),
#else
				__print_symbolic_u64(__entry->err &
						     TDX_SEAMCALL_STATUS_MASK,
						     TDX_STATUS_CODES),
#endif
		__entry->err,
		__entry->rcx,
		__entry->rdx,
		__entry->r8,
		__entry->r9,
		__entry->r10,
		__entry->r11
	    )
);

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH ../../arch/x86/include/asm/trace/
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE seam
#endif /* _TRACE_SEAM_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
