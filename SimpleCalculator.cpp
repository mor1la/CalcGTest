#include "SimpleCalculator.h"

namespace calc
{

SimpleCalculator::SimpleCalculator(IHistory& history)
    : m_history(history)
{}

void SimpleCalculator::SetHistory(IHistory& history)
{
    m_history = history;
}

int SimpleCalculator::Add(int a, int b)
{
    if (willAdditionOverflow(a, b))
        throw std::overflow_error(Consts::ADDITION_OVERFLOW_MSG);
    int result = a + b;
    LogOperation(a, "+", b, result);
    return result;
}

int SimpleCalculator::Subtract(int a, int b)
{
    if (willSubtractionOverflow(a, b))
        throw std::overflow_error(Consts::SUBTRACTION_OVERFLOW_MSG);
    int result = a - b;
    LogOperation(a, "-", b, result);
    return result;
}

int SimpleCalculator::Multiply(int a, int b)
{
    if (willMultiplicationOverflow(a, b))
        throw std::overflow_error(Consts::MULTIPLICATION_OVERFLOW_MSG);
    int result = a * b;
    LogOperation(a, "*", b, result);
    return result;
}

int SimpleCalculator::Divide(int a, int b)
{
    if (b == 0)
        throw std::invalid_argument(Consts::DIVISION_BY_ZERO_MSG);
    if (willDivisionOverflow(a, b))
        throw std::overflow_error(Consts::DIVISION_OVERFLOW_MSG);
    int result = a / b;
    LogOperation(a, "/", b, result);
    return result;
}

void SimpleCalculator::LogOperation(int a, const char* op, int b, int result)
{
    std::ostringstream oss;
    oss << a << " " << op << " " << b << " = " << result;
    m_history.AddEntry(oss.str());
}

bool SimpleCalculator::willAdditionOverflow(int a, int b)
{
    if ((b > 0 && a > std::numeric_limits<int>::max() - b) ||
        (b < 0 && a < std::numeric_limits<int>::min() - b)) {
        return true;
    }
    return false;
}

bool SimpleCalculator::willSubtractionOverflow(int a, int b)
{
    if ((b < 0 && a > std::numeric_limits<int>::max() + b) ||
        (b > 0 && a < std::numeric_limits<int>::min() + b)) {
        return true;
    }
    return false;
}

bool SimpleCalculator::willMultiplicationOverflow(int a, int b)
{
    if (a == 0 || b == 0) return false;
    
    if (a == -1 && b == std::numeric_limits<int>::min()) return true;
    if (b == -1 && a == std::numeric_limits<int>::min()) return true;

    int result = a * b;
    return result / b != a;
}

bool SimpleCalculator::willDivisionOverflow(int a, int b)
{
    return (a == std::numeric_limits<int>::min() && b == -1);
}


} // namespace calc
