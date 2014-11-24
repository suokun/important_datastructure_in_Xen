//this data structure is defined in include/xen/sched-if.h


struct task_slice {
    struct vcpu *task;
    s_time_t     time;
    bool_t       migrated;
};
