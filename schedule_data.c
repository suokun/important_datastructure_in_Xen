//defined in include/xen/sched-if.h

/*
 * In order to allow a scheduler to remap the lock->cpu mapping,
 * we have a per-cpu pointer, along with a pre-allocated set of
 * locks.  The generic schedule init code will point each schedule lock
 * pointer to the schedule lock; if the scheduler wants to remap them,
 * it can simply modify the schedule locks.
 * 
 * For cache betterness, keep the actual lock in the same cache area
 * as the rest of the struct.  Just have the scheduler point to the
 * one it wants (This may be the one right in front of it).*/
struct schedule_data {
    spinlock_t         *schedule_lock,
                       _lock;
    struct vcpu        *curr;           /* current task                    */
    void               *sched_priv;
    struct timer        s_timer;        /* scheduling timer                */
    atomic_t            urgent_count;   /* how many urgent vcpus           */
};
