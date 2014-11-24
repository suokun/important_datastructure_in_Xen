//this data structure is defined in xen/common/sched_credit.c

/*
 * System-wide private data
 */
struct csched_private {
    /* lock for the whole pluggable scheduler, nests inside cpupool_lock */
    spinlock_t lock;
    struct list_head active_sdom;
    uint32_t ncpus;
    struct timer  master_ticker;
    unsigned int master;
    cpumask_var_t idlers;
    cpumask_var_t cpus;
    uint32_t weight;
    uint32_t credit;
    int credit_balance;
    uint32_t runq_sort;
    unsigned ratelimit_us;
    /* Period of master and tick in milliseconds */
    unsigned tslice_ms, tick_period_us, ticks_per_tslice;
    unsigned credits_per_tslice;
};
