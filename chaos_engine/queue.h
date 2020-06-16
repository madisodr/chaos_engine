#ifndef QUEUE_H
#define QUEUE_H

template<class T>
class Queue
{
    public:
        Queue(int _maxitems = 256) {
            m_front = 0;
            m_back = 0;
            m_count = 0;
            m_max_items = _maxitems;
            m_data = new T[_maxitems + 1];
        }

        ~Queue() {
            delete[] m_data;
        }

        inline int Count();
        inline int Front();
        inline int back();
        void Push(const T &item);
        T Peek();
        T Pop();
        void Clear();
        T At(uint8_t at);
    private:
        int m_front, m_back, m_count;
        T *m_data;
        int m_max_items;
};

template<class T>
inline int Queue<T>::Count()
{
    return m_count;
}

template<class T>
inline int Queue<T>::Front()
{
    return m_front;
}

template<class T>
inline int Queue<T>::back()
{
    return m_back;
}

    template<class T>
void Queue<T>::Push(const T &item)
{
    if (m_count < m_max_items) { // Drops out when full
        m_data[m_back++] = item;
        ++m_count;
        // Check wrap around
        if (m_back > m_max_items) {
            m_back -= (m_max_items + 1);
        }
    }
}

    template<class T>
T Queue<T>::Pop()
{
    if (m_count <= 0) {
        return T();    // Returns empty
    } else {
        T result = m_data[m_front];
        m_front++;
        --m_count;

        // Check wrap around
        if (m_front > m_max_items) {
            m_front -= (m_max_items + 1);
        }

        return result;
    }
}

template<class T>
T Queue<T>::Peek()
{
    if (m_count <= 0) {
        return T();    // Returns empty
    } else {
        return m_data[m_front];
    }
}

template<class T>
void Queue<T>::Clear()
{
    m_front = m_back;
    m_count = 0;
}

template<class T>
T Queue<T>::At(uint8_t at)
{
    if (at > m_count || m_count <= 0) {
        return NULL; // returns empty
    } else {
        return m_data[at];
    }
}

#endif // QUEUE_H
