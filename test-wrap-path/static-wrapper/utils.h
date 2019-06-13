#ifndef librabbitmq_examples_utils_h
#define librabbitmq_examples_utils_h

int die(const char *fmt, ...);
extern int die_on_error(int x, char const *context);
extern int die_on_amqp_error(amqp_rpc_reply_t x, char const *context);

extern void amqp_dump(void const *buffer, size_t len);

extern uint64_t now_microseconds(void);
extern void microsleep(int usec);

#endif
