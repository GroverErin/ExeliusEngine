#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    // Simple Thread safe ring buffer from:
    // https://wickedengine.net/2018/11/24/simple-job-system-using-standard-c/

    template <typename T, size_t size>
    class RingBufferMT
    {
        T m_buffer[size];
        size_t m_head = 0;
        size_t m_tail = 0;
        std::mutex m_bufferLock;

    public:
        inline bool PushBack(const T& elementToPush)
        {
            bool result = false;
            m_bufferLock.lock();
            size_t next = (m_head + 1) % size;
            if (next != m_tail)
            {
                m_buffer[m_head] = elementToPush;
                m_head = next;
                result = true;
            }
            m_bufferLock.unlock();
            return result;
        }

        inline bool PopFront(T& elementToPop)
        {
            bool result = false;
            m_bufferLock.lock();
            if (m_tail != m_head)
            {
                elementToPop = m_buffer[m_tail];
                m_tail = (m_tail + 1) % size;
                result = true;
            }
            m_bufferLock.unlock();
            return result;
        }
    };

    template <typename T, size_t size>
    class RingBuffer
    {
        T m_buffer[size];
        size_t m_head = 0;
        size_t m_tail = 0;

    public:
        inline bool PushBack(const T& elementToPush)
        {
            bool result = false;
            size_t next = (m_head + 1) % size;
            if (next != m_tail)
            {
                m_buffer[m_head] = elementToPush;
                m_head = next;
                result = true;
            }
            return result;
        }

        inline bool PopFront(T& elementToPop)
        {
            bool result = false;
            if (m_tail != m_head)
            {
                elementToPop = m_buffer[m_tail];
                m_tail = (m_tail + 1) % size;
                result = true;
            }
            return result;
        }
    };
}