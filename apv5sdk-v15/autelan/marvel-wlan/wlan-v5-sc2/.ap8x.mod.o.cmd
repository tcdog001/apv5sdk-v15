cmd_/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/ap8x.mod.o := /home/shark/apv7_marvell/apv7sdk/build/../build/armv7-marvell-linux-gnueabi-softfp/bin/arm-marvell-linux-gnueabi-gcc -Wp,-MD,/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/.ap8x.mod.o.d  -nostdinc -isystem /home/shark/apv7_marvell/apv7sdk/build/armv7-marvell-linux-gnueabi-softfp/bin/../lib/gcc/arm-marvell-linux-gnueabi/4.4.5/include -I/home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-armada370/include -Iarch/arm/plat-armada/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -ffunction-sections -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -I/home/shark/apv7_marvell/apv7sdk/build/../linux/kernels/marvell-linux-2.6.35.9 -DWL_KERNEL_26 -DUSE_TASKLET -O2 -funroll-loops -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2 -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2 -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/driver/linux -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/api -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/incl -I/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/core/mgt/STA/incl -IW8764 -IW8764/Common -IW8764/Soc/idma -IW8764/Soc/ctrlEnv -IW8764/mv88f5181 -IW8764/osServices -IW8764/osServices/linux/ArchARM -IW8764/Board/boardEnv -IW8764/Board/boardEnv/DB_88FXX81 -IW8764/Soc/ctrlEnv/MV_88FXX81 -IW8764/osServices/linux -DMV_CPU_LE -DMV_LINUX -DMV_ARM -DUDP_API -DAP_MAC_LINUX -DIEEE80211H -DWPA -DAP_WPA2 -DQOS_FEATURE -DSTA_INFO_DB -DBT_COEXISTENCE -DAVOID_MIS_ACC -DQOS_WSM_FEATURE -DEXPORT_SYMTAB -DRELEASE_11N -DAGG_QUE -DAP8X_STATISTICS -DENABLE_WLSNDEVT -DAP8X_DUMP -DAUTOCHANNEL -DENABLE_WLSYSLOG -DUAPSD_SUPPORT -DAMPDU_SUPPORT -DMRV_8021X -DINTEROP -DAMSDU_BYTE_REORDER -DIEEE_HTIE -DWDS_FEATURE -DBRS_SUPPORT -DIEEE80211_DH -DMFG_SUPPORT -DPWRFRAC -DMRVL_WSC -DMRVL_DFS -DIEEE80211N_MIMOPSD110 -DCOEXIST_20_40_SUPPORT -DGENERIC_GETIE -DMRVL_WPS2 -DEXPLICIT_BF -DEWB -DWLAN_INCLUDE_TSO -DMBSS -DCOMMON_PHYDSSS -DMIMO_PS_HT -DNPROTECTION -DCLIENT_SUPPORT -DWPA_STA -DWPA2 -DRSN_RESOLVE -DAMPDU_SUPPORT_TX_CLIENT -DSOC_W8764 -DWLMEM_DISABLED -DMPRXY -DMPRXY_SNOOP -DMPRXY_IGMP_QUERY -DNUMOFAPS=8 -DMRVL_WPS_CLIENT -DDFS_PASSIVE_SCAN -DDFS_CHNL_SUPPORT -DFCC_15E_INTERIM_PLAN -DBA_WATCHDOG -DV6FW -DATTN_RSSI -DAMPDU_SUPPORT_SBA -DDYNAMIC_BA_SUPPORT -DAMSDUOVERAMPDU -DWFA_TKIP_NEGATIVE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ap8x.mod)"  -D"KBUILD_MODNAME=KBUILD_STR(ap8x)"  -DMODULE -c -o /home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/ap8x.mod.o /home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/ap8x.mod.c

deps_/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/ap8x.mod.o := \
  /home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/ap8x.mod.c \
    $(wildcard include/config/module/unload.h) \
  include/linux/module.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/prefetch.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/posix_types.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/processor.h \
    $(wildcard include/config/mmu.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/hwcap.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/32v6k.h) \
  include/linux/linkage.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/linkage.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/irqflags.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/asm-generic/cmpxchg-local.h \
  include/linux/stat.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/numa.h) \
  /home/shark/apv7_marvell/apv7sdk/build/armv7-marvell-linux-gnueabi-softfp/bin/../lib/gcc/arm-marvell-linux-gnueabi/4.4.5/include/stdarg.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/bitops.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/dynamic_debug.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/div64.h \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
  include/asm-generic/atomic-long.h \
  include/linux/math64.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/current.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  include/linux/nodemask.h \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/string.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/page.h \
    $(wildcard include/config/mv/support/64kb/page/size.h) \
    $(wildcard include/config/cpu/copy/v3.h) \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/feroceon.h) \
    $(wildcard include/config/cpu/copy/fa.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/glue.h \
    $(wildcard include/config/cpu/arm610.h) \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/cpu/abrt/lv4t.h) \
    $(wildcard include/config/cpu/abrt/ev4.h) \
    $(wildcard include/config/cpu/abrt/ev4t.h) \
    $(wildcard include/config/cpu/abrt/ev5tj.h) \
    $(wildcard include/config/cpu/abrt/ev5t.h) \
    $(wildcard include/config/cpu/abrt/ev6.h) \
    $(wildcard include/config/cpu/abrt/ev7.h) \
    $(wildcard include/config/cpu/pabrt/legacy.h) \
    $(wildcard include/config/cpu/pabrt/v6.h) \
    $(wildcard include/config/cpu/pabrt/v7.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/memory.h \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
  include/linux/const.h \
  arch/arm/mach-armada370/include/mach/memory.h \
    $(wildcard include/config/fb/aurora/consistent/dma/size.h) \
    $(wildcard include/config/aurora/io/cache/coherency.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/errno.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/pfn.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/linux/param.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/timex.h \
  arch/arm/mach-armada370/include/mach/timex.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/elf.h \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/user.h \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
  include/linux/completion.h \
  include/linux/rcutree.h \
    $(wildcard include/config/no/hz.h) \
  /home/shark/apv7_marvell/apv7sdk/linux/kernels/marvell-linux-2.6.35.9/arch/arm/include/asm/module.h \
    $(wildcard include/config/arm/unwind.h) \
  include/trace/events/module.h \
  include/trace/define_trace.h \
  include/linux/vermagic.h \
  include/generated/utsrelease.h \

/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/ap8x.mod.o: $(deps_/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/ap8x.mod.o)

$(deps_/home/shark/apv7_marvell/apv7sdk/build/../autelan/marvel-wlan/wlan-v5-sc2/ap8x.mod.o):
