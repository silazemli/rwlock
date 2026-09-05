# RW Lock

## Overview

This project implements an RW lock synchronization primitive that allows concurrent access for read-only operations and requires exclusive access for write operations.

## Usage

```c
rwlock_t lock;
rwlock_init(&lock);

rwlock_begin_read(&lock);
// read shared data
rwlock_end_read(&lock);

rwlock_begin_write(&lock);
// modify shared data
rwlock_end_write(&lock);
```
Multiple readers may hold the lock simultaneously, while writers require exclusive access.