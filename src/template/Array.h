#ifndef TEMPLATE_ARRAY_H_
#define TEMPLATE_ARRAY_H_

#include <utility>

namespace kdl
{

///
/// PURPOSE
///     Generic Array 
/// 
template <class T>
class Array
{
public:
    Array() : m_array(nullptr), m_size(0), m_maxSize(0)
    {
        resize(2);
    }
    Array(int size) : m_array(nullptr), m_size(0), m_maxSize(size)
    {
        resize(size);
    }
    ~Array()
    {
        for (int i = 0; i < m_size; i++)
            m_array[i].~T();
        if (m_array)
            ::free(m_array);
    }

    ///
    /// PURPOSE
    ///     Get object at index
    /// 
    T* operator[](int index)
    {
        return &m_array[index];
    }


    ///
    /// PURPOSE
    ///     Add a new T to the array
    ///     Will construct in place
    ///     
    template<class...Args>
    T* add(Args...args)
    {
        if (m_size == m_maxSize)
            resize(m_size + (int)(m_size / 2));
        T* nobject = new (&m_array[m_size]) T(std::forward<Args>(args)...);
        ++m_size;
        return nobject;
    }

    ///
    /// PURPOSE
    ///     Resize the array to n * T bytes
    ///     Copies any data to the new array
    /// 
    void resize(int n)
    {
        T* new_arr = (T*)malloc(sizeof(T) * n);

        // only copy data if there is data to copy.
        if (m_array)
        {
            for (int i = 0; i < m_size; i++)
                new_arr[i] = m_array[i];

            if (m_array)
                free(m_array);
        }

        m_array = (T*)new_arr;
        m_maxSize = n;
    }

    T* data()
    {
        return m_array;
    }


    const T* data() const
    {
        return m_array;
    }

    int size() const { return m_size; }
    int maxSize() const { return m_maxSize; }
private:
    T* m_array;
    int m_size;
    int m_maxSize;

};


} // kdl


#endif // TEMPLATE_ARRAY_H_