//this data structure is defined in xen/common/sched_credit.c


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

//#define CSCHED_PRI_TS_BOOST      0      /* time-share waking up */
//#define CSCHED_PRI_TS_UNDER     -1      /* time-share w/ credits */
//#define CSCHED_PRI_TS_OVER      -2      /* time-share w/o credits */
//#define CSCHED_PRI_IDLE         -64     /* idle */    
    
    
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
