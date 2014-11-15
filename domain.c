important_datastructure_in_Xen
==============================

//in include/xen/sched.h

struct domain
{
    domid_t          domain_id;

    shared_info_t   *shared_info;     /* shared data area */

    spinlock_t       domain_lock;

    spinlock_t       page_alloc_lock; /* protects all the following fields  */
    struct page_list_head page_list;  /* linked list */
    struct page_list_head xenpage_list; /* linked list (size xenheap_pages) */
    unsigned int     tot_pages;       /* number of pages currently possesed */
    unsigned int     outstanding_pages; /* pages claimed but not possessed  */
    unsigned int     max_pages;       /* maximum value for tot_pages        */
    atomic_t         shr_pages;       /* number of shared pages             */
    atomic_t         paged_pages;     /* number of paged-out pages          */
    unsigned int     xenheap_pages;   /* # pages allocated from Xen heap    */

    unsigned int     max_vcpus;

    /* Scheduling. */
    void            *sched_priv;    /* scheduler-specific data */
    struct cpupool  *cpupool;

    struct domain   *next_in_list;
    struct domain   *next_in_hashbucket;

    struct list_head rangesets;
    spinlock_t       rangesets_lock;

    /* Event channel information. */
    struct evtchn   *evtchn[NR_EVTCHN_BUCKETS];
    spinlock_t       event_lock;

    struct grant_table *grant_table;

    /*
     * Interrupt to event-channel mappings and other per-guest-pirq data.
     * Protected by the domain's event-channel spinlock.
     */
    unsigned int     nr_pirqs;
    struct radix_tree_root pirq_tree;

    /* I/O capabilities (access to IRQs and memory-mapped I/O). */
    struct rangeset *iomem_caps;
    struct rangeset *irq_caps;

    /* Is this an HVM guest? */
    bool_t           is_hvm;
#ifdef HAS_PASSTHROUGH
    /* Does this guest need iommu mappings? */
    bool_t           need_iommu;
#endif
    /* is node-affinity automatically computed? */
    bool_t           auto_node_affinity;
    /* Is this guest fully privileged (aka dom0)? */
    bool_t           is_privileged;
    /* Which guest this guest has privileges on */
    struct domain   *target;
    /* Is this guest being debugged by dom0? */
    bool_t           debugger_attached;
    /* Is this guest dying (i.e., a zombie)? */
    enum { DOMDYING_alive, DOMDYING_dying, DOMDYING_dead } is_dying;
    /* Domain is paused by controller software? */
    bool_t           is_paused_by_controller;
    /* Domain's VCPUs are pinned 1:1 to physical CPUs? */
    bool_t           is_pinned;

    /* Are any VCPUs polling event channels (SCHEDOP_poll)? */
#if MAX_VIRT_CPUS <= BITS_PER_LONG
    DECLARE_BITMAP(poll_mask, MAX_VIRT_CPUS);
#else
    unsigned long   *poll_mask;
#endif

    /* Guest has shut down (inc. reason code)? */
    spinlock_t       shutdown_lock;
    bool_t           is_shutting_down; /* in process of shutting down? */
    bool_t           is_shut_down;     /* fully shut down? */
    int              shutdown_code;

    /* If this is not 0, send suspend notification here instead of
     * raising DOM_EXC */
    int              suspend_evtchn;

    atomic_t         pause_count;

    unsigned long    vm_assist;

    atomic_t         refcnt;

    struct vcpu    **vcpu;

    /* Bitmask of CPUs which are holding onto this domain's state. */
    cpumask_var_t    domain_dirty_cpumask;

    struct arch_domain arch;

    void *ssid; /* sHype security subject identifier */

    /* Control-plane tools handle for this domain. */
    xen_domain_handle_t handle;

    /* OProfile support. */
    struct xenoprof *xenoprof;
    int32_t time_offset_seconds;

    /* Domain watchdog. */
#define NR_DOMAIN_WATCHDOG_TIMERS 2
    spinlock_t watchdog_lock;
    uint32_t watchdog_inuse_map;
    struct timer watchdog_timer[NR_DOMAIN_WATCHDOG_TIMERS];

    struct rcu_head rcu;

    /*
     * Hypercall deadlock avoidance lock. Used if a hypercall might
     * cause a deadlock. Acquirers don't spin waiting; they preempt.
     */
    spinlock_t hypercall_deadlock_mutex;

    /* transcendent memory, auto-allocated on first tmem op by each domain */
    void *tmem;

    struct lock_profile_qhead profile_head;

    /* Non-migratable and non-restoreable? */
    bool_t disable_migrate;

    /* Various mem_events */
    struct mem_event_per_domain *mem_event;

    /*
     * Can be specified by the user. If that is not the case, it is
     * computed from the union of all the vcpu cpu-affinity masks.
     */
    nodemask_t node_affinity;
    unsigned int last_alloc_node;
    spinlock_t node_affinity_lock;
};


