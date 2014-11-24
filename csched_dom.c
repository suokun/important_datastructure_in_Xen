//this data structure is defined in xen/common/sched_credit.c

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
