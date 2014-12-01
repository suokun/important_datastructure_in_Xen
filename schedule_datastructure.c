/*
 * Basic constants
 */
#define CSCHED_DEFAULT_WEIGHT       256
#define CSCHED_TICKS_PER_TSLICE     3

/* 
 * Default timeslice: 30ms 
 */
#define CSCHED_DEFAULT_TSLICE_MS    30
#define CSCHED_CREDITS_PER_MSEC     10

/*
 * Priorities
 */
#define CSCHED_PRI_TS_BOOST      0      /* time-share waking up */
#define CSCHED_PRI_TS_UNDER     -1      /* time-share w/ credits */
#define CSCHED_PRI_TS_OVER      -2      /* time-share w/o credits */
#define CSCHED_PRI_IDLE         -64     /* idle */


/*
 * Flags
 */
#define CSCHED_FLAG_VCPU_PARKED    0x0  /* VCPU over capped credits */
#define CSCHED_FLAG_VCPU_YIELD     0x1  /* VCPU yielding */


/*
 * Useful macros
 */
#define CSCHED_PRIV(_ops)   \
    ((struct csched_private *)((_ops)->sched_data))
#define CSCHED_PCPU(_c)     \
    ((struct csched_pcpu *)per_cpu(schedule_data, _c).sched_priv)
#define CSCHED_VCPU(_vcpu)  ((struct csched_vcpu *) (_vcpu)->sched_priv)
#define CSCHED_DOM(_dom)    ((struct csched_dom *) (_dom)->sched_priv)
#define RUNQ(_cpu)          (&(CSCHED_PCPU(_cpu)->runq))
/* Is the first element of _cpu's runq its idle vcpu? */
#define IS_RUNQ_IDLE(_cpu)  (list_empty(RUNQ(_cpu)) || \
                             is_idle_vcpu(__runq_elem(RUNQ(_cpu)->next)->vcpu))


/*
 * Node Balancing
 */
#define CSCHED_BALANCE_NODE_AFFINITY    0
#define CSCHED_BALANCE_CPU_AFFINITY     1


/*
 * Physical CPU
 */
struct csched_pcpu {
    struct list_head runq;
    uint32_t runq_sort_last;
    struct timer ticker;
    unsigned int tick;
    unsigned int idle_bias;
    /* Store this here to avoid having too many cpumask_var_t-s on stack */
    cpumask_var_t balance_mask;
};


/*
 * Virtual CPU
 */
struct csched_vcpu {
    struct list_head runq_elem;
    struct list_head active_vcpu_elem;
    struct csched_dom *sdom;
    struct vcpu *vcpu;
    atomic_t credit;
    unsigned int residual;
    s_time_t start_time;   /* When we were scheduled (used for credit) */
    unsigned flags;
    int16_t pri;
#ifdef CSCHED_STATS
    struct {
        int credit_last;
        uint32_t credit_incr;
        uint32_t state_active;
        uint32_t state_idle;
        uint32_t migrate_q;
        uint32_t migrate_r;
        uint32_t kicked_away;
    } stats;
#endif
};


/*
 * Domain
 */
struct csched_dom {
    struct list_head active_vcpu;
    struct list_head active_sdom_elem;
    struct domain *dom;
    /* cpumask translated from the domain's node-affinity.
     * Basically, the CPUs we prefer to be scheduled on. */
    cpumask_var_t node_affinity_cpumask;
    uint16_t active_vcpu_count;
    uint16_t weight;
    uint16_t cap;
};


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

