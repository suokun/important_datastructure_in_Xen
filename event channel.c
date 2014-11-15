important_datastructure_in_Xen
==============================

//in include/xen/sched.h


struct evtchn
{
#define ECS_FREE         0 /* Channel is available for use.                  */
#define ECS_RESERVED     1 /* Channel is reserved.                           */
#define ECS_UNBOUND      2 /* Channel is waiting to bind to a remote domain. */
#define ECS_INTERDOMAIN  3 /* Channel is bound to another domain.            */
#define ECS_PIRQ         4 /* Channel is bound to a physical IRQ line.       */
#define ECS_VIRQ         5 /* Channel is bound to a virtual IRQ line.        */
#define ECS_IPI          6 /* Channel is bound to a virtual IPI line.        */
    u8  state;             /* ECS_* */
    u8  xen_consumer;      /* Consumer in Xen, if any? (0 = send to guest) */
    u16 notify_vcpu_id;    /* VCPU for local delivery notification */
    union {
        struct {
            domid_t remote_domid;
        } unbound;     /* state == ECS_UNBOUND */
        struct {
            u16            remote_port;
            struct domain *remote_dom;
        } interdomain; /* state == ECS_INTERDOMAIN */
        struct {
            u16            irq;
            u16            next_port;
            u16            prev_port;
        } pirq;        /* state == ECS_PIRQ */
        u16 virq;      /* state == ECS_VIRQ */
    } u;
#ifdef FLASK_ENABLE
    void *ssid;
#endif
};
