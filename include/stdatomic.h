#pragma once

#define atomic_exchange(object, desired) __atomic_exchange_n(object, desired, __ATOMIC_SEQ_CST)

#define atomic_store(object, desired) __atomic_store_n(object, desired, __ATOMIC_SEQ_CST)