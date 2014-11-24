


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
