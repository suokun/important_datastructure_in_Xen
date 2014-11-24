

struct scheduler {
    char *name;             /* full name for this scheduler      */
    char *opt_name;         /* option name for this scheduler    */
    unsigned int sched_id;  /* ID for this scheduler             */
    void *sched_data;       /* global data pointer               */

    int          (*global_init)    (void);

    int          (*init)           (struct scheduler *);
    void         (*deinit)         (const struct scheduler *);

    void         (*free_vdata)     (const struct scheduler *, void *);
    void *       (*alloc_vdata)    (const struct scheduler *, struct vcpu *,
                                    void *);
    void         (*free_pdata)     (const struct scheduler *, void *, int);
    void *       (*alloc_pdata)    (const struct scheduler *, int);
    void         (*free_domdata)   (const struct scheduler *, void *);
    void *       (*alloc_domdata)  (const struct scheduler *, struct domain *);

    int          (*init_domain)    (const struct scheduler *, struct domain *);
    void         (*destroy_domain) (const struct scheduler *, struct domain *);

    /* Activate / deactivate vcpus in a cpu pool */
    void         (*insert_vcpu)    (const struct scheduler *, struct vcpu *);
    void         (*remove_vcpu)    (const struct scheduler *, struct vcpu *);

    void         (*sleep)          (const struct scheduler *, struct vcpu *);
    void         (*wake)           (const struct scheduler *, struct vcpu *);
    void         (*yield)          (const struct scheduler *, struct vcpu *);
    void         (*context_saved)  (const struct scheduler *, struct vcpu *);

    struct task_slice (*do_schedule) (const struct scheduler *, s_time_t,
                                      bool_t tasklet_work_scheduled);

    int          (*pick_cpu)       (const struct scheduler *, struct vcpu *);
    void         (*migrate)        (const struct scheduler *, struct vcpu *,
                                    unsigned int);
    int          (*adjust)         (const struct scheduler *, struct domain *,
                                    struct xen_domctl_scheduler_op *);
    int          (*adjust_global)  (const struct scheduler *,
                                    struct xen_sysctl_scheduler_op *);
    void         (*set_node_affinity) (const struct scheduler *,
                                       struct domain *, nodemask_t *);
    void         (*dump_settings)  (const struct scheduler *);
    void         (*dump_cpu_state) (const struct scheduler *, int);

    void         (*tick_suspend)    (const struct scheduler *, unsigned int);
    void         (*tick_resume)     (const struct scheduler *, unsigned int);
};
