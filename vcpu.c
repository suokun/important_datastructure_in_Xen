important_datastructure_in_Xen
==============================

//in include/xen/sched.h


struct vcpu
{
    int              vcpu_id;

    int              processor;

    vcpu_info_t     *vcpu_info;

    struct domain   *domain;

    struct vcpu     *next_in_list;

    s_time_t         periodic_period;
    s_time_t         periodic_last_event;
    struct timer     periodic_timer;
    struct timer     singleshot_timer;

    struct timer     poll_timer;    /* timeout for SCHEDOP_poll */

    void            *sched_priv;    /* scheduler-specific data */

    struct vcpu_runstate_info runstate;
    
#ifndef CONFIG_COMPAT
# define runstate_guest(v) ((v)->runstate_guest)
    XEN_GUEST_HANDLE(vcpu_runstate_info_t) runstate_guest; /* guest address */
#else
# define runstate_guest(v) ((v)->runstate_guest.native)
    union {
        XEN_GUEST_HANDLE(vcpu_runstate_info_t) native;
        XEN_GUEST_HANDLE(vcpu_runstate_info_compat_t) compat;
    } runstate_guest; /* guest address */
#endif

    /* last time when vCPU is scheduled out */
    uint64_t last_run_time;

    /* Has the FPU been initialised? */
    bool_t           fpu_initialised;
    /* Has the FPU been used since it was last saved? */
    bool_t           fpu_dirtied;
    /* Initialization completed for this VCPU? */
    bool_t           is_initialised;
    /* Currently running on a CPU? */
    bool_t           is_running;
    /* VCPU should wake fast (do not deep sleep the CPU). */
    bool_t           is_urgent;

#ifdef VCPU_TRAP_LAST
#define VCPU_TRAP_NONE    0
    struct {
        bool_t           pending;
        uint8_t          old_mask;
    }                async_exception_state[VCPU_TRAP_LAST];
#define async_exception_state(t) async_exception_state[(t)-1]
    uint8_t          async_exception_mask;
#endif

    /* Require shutdown to be deferred for some asynchronous operation? */
    bool_t           defer_shutdown;
    /* VCPU is paused following shutdown request (d->is_shutting_down)? */
    bool_t           paused_for_shutdown;
    /* VCPU need affinity restored */
    bool_t           affinity_broken;

    /*
     * > 0: a single port is being polled;
     * = 0: nothing is being polled (vcpu should be clear in d->poll_mask);
     * < 0: multiple ports may be being polled.
     */
    int              poll_evtchn;

    /* (over-)protected by ->domain->event_lock */
    int              pirq_evtchn_head;

    unsigned long    pause_flags;
    atomic_t         pause_count;

    /* IRQ-safe virq_lock protects against delivering VIRQ to stale evtchn. */
    u16              virq_to_evtchn[NR_VIRQS];
    spinlock_t       virq_lock;

    /* Bitmask of CPUs on which this VCPU may run. */
    cpumask_var_t    cpu_affinity;
    /* Used to change affinity temporarily. */
    cpumask_var_t    cpu_affinity_tmp;
    /* Used to restore affinity across S3. */
    cpumask_var_t    cpu_affinity_saved;

    /* Bitmask of CPUs which are holding onto this VCPU's state. */
    cpumask_var_t    vcpu_dirty_cpumask;

    /* Tasklet for continue_hypercall_on_cpu(). */
    struct tasklet   continue_hypercall_tasklet;

    /* Multicall information. */
    struct mc_state  mc_state;

    struct waitqueue_vcpu *waitqueue_vcpu;

    /* Guest-specified relocation of vcpu_info. */
    unsigned long vcpu_info_mfn;

    struct arch_vcpu arch;
};


