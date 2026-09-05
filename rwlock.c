#include "rwlock.h"

void rwlock_init(rwlock_t *lock) {
    atomic_init(&lock->changing_state, false);
    lock->state = 0;
}

static inline void lock_state(rwlock_t *lock) {
    bool expected = false;
    while (!atomic_compare_exchange_strong_explicit(
        &lock->changing_state, 
        &expected, 
        true,
        memory_order_acquire,
        memory_order_relaxed)) {
        expected = false;
    }
}

static inline void unlock_state(rwlock_t *lock) {
    atomic_store_explicit(&lock->changing_state, false, memory_order_release);
}

void rwlock_begin_read(rwlock_t *lock) {
    while (true) {
        lock_state(lock);
        
        if (lock->state != -1) {
            lock->state++;
            unlock_state(lock);
            return;
        }
        
        unlock_state(lock);
    }
}

void rwlock_end_read(rwlock_t *lock) {
    lock_state(lock);
    
    lock->state--;
    
    unlock_state(lock);
}

void rwlock_begin_write(rwlock_t *lock) {
    while (true) {
        lock_state(lock);
        
        if (lock->state == 0) {
            lock->state = -1;
            unlock_state(lock);
            return;
        }
        
        unlock_state(lock);
    }
}

void rwlock_end_write(rwlock_t *lock) {
    lock_state(lock);
    
    lock->state = 0;
    
    unlock_state(lock);
}