#pragma once

#include <stdexcept>
#include <limits>
#include <sstream>
#include "ICalculator.h"
#include "IHistory.h"
#include "Consts.h"

namespace calc
{

class SimpleCalculator : public ICalculator
{
public:
    explicit SimpleCalculator(IHistory& history);

    void SetHistory(IHistory& history) override;

    int Add(int a, int b) override;
    int Subtract(int a, int b) override;
    int Multiply(int a, int b) override;
    int Divide(int a, int b) override;

private:
    void LogOperation(int a, const char* op, int b, int result);
    bool willAdditionOverflow(int a, int b);
    bool willSubtractionOverflow(int a, int b);
    bool willMultiplicationOverflow(int a, int b);
    bool willDivisionOverflow(int a, int b); 


    IHistory& m_history;
};

} // namespace calc