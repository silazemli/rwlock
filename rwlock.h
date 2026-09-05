#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    atomic_bool changing_state;
    int state;
} rwlock_t;

void rwlock_init(rwlock_t *lock);

static inline void lock_state(rwlock_t *lock);
static inline void unlock_state(rwlock_t *lock);

void rwlock_begin_read(rwlock_t *lock);
void rwlock_end_read(rwlock_t *lock);

void rwlock_begin_write(rwlock_t *lock);
void rwlock_end_write(rwlock_t *lock);