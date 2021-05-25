#pragma once

#include <assert.h>
#include <vector>
#include <utility>

namespace mimax {
namespace common {

template<typename TObject>
class CObjectPool
{
public:
    inline void ResetPool()
    {
        m_objects.clear();
    }
    inline void ResetPool(size_t const poolSize)
    {
        m_objects.clear();
        m_objects.reserve(poolSize);
    }
    inline bool IsEmpty() { return m_objects.size() == m_objects.capacity(); }
    inline size_t GetAllocatedSize() const { return m_objects.size(); }

    template<typename ...TArgs>
    inline TObject* AllocateObject(TArgs&& ...args)
    {
        assert(m_objects.size() < m_objects.capacity());
        m_objects.emplace_back(std::forward<TArgs>(args)...);
        return &m_objects.back();
    }
private:
    std::vector<TObject> m_objects;
};

}
}