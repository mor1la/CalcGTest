#include "InMemoryHistory.h"

namespace calc
{

void InMemoryHistory::AddEntry(const std::string& operation)
{
    if (m_operations.size() >= maxHistorySize) {
        m_operations.erase(m_operations.begin()); 
    }
    m_operations.push_back(operation);
}

std::vector<std::string> InMemoryHistory::GetLastOperations(size_t count) const
{
    auto start = m_operations.end() - std::min(count, m_operations.size());
    return {start, m_operations.end()};
}

size_t InMemoryHistory::getMaxHistorySize()
{
    return maxHistorySize;
}

} // namespace calc