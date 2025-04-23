#pragma once

#include <atomic>
#include <Objects.hpp>

namespace gs {

class SpinLock final
{
    DEFAULT_DESTRUCT(SpinLock);
    DELETE_CM(SpinLock);
public:
    SpinLock() noexcept
        : m_Lock(ATOMIC_FLAG_INIT)
    { }

    void Lock() noexcept
    {
        while(m_Lock.test_and_set(::std::memory_order_acquire))
        {
            // Spin
        }
    }

    bool TryLock() noexcept
    {
        return !m_Lock.test_and_set(::std::memory_order_acquire);
    }

    void Unlock() noexcept
    {
        m_Lock.clear(::std::memory_order_release);
    }

    void lock() noexcept
    {
        Lock();
    }

    bool try_lock() noexcept
    {
        return TryLock();
    }

    void unlock() noexcept
    {
        Unlock();
    }
private:
    ::std::atomic_flag m_Lock;
};

}
