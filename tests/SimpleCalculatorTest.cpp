#include "../SimpleCalculator.h"
#include "../InMemoryHistory.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Мок интерфейса истории
class MockHistory : public calc::IHistory {
public:
    MOCK_METHOD(void, AddEntry, (const std::string&), (override));
    MOCK_METHOD(std::vector<std::string>, GetLastOperations, (size_t), (const, override));
};

// Тесты взаимодействия
// Проверяет, что операция сложения логируется в историю
TEST(SimpleCalculatorTest, AddLogsToHistory) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_CALL(mock, AddEntry("2 + 2 = 4"));

    int result = calc.Add(2, 2);
    EXPECT_EQ(result, 4);
}

// Проверяет, что операция умножения логируется в историю
TEST(SimpleCalculatorTest, MultiplyLogsToHistory) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_CALL(mock, AddEntry("6 * 7 = 42"));

    int result = calc.Multiply(6, 7);
    EXPECT_EQ(result, 42);
}

// Тесты на деление (включая деление на 0)
// Деление положительных чисел возвращает корректный результат
TEST(CalculatorTest, dividePositiveNumbersReturnsCorrectResult) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);
    EXPECT_EQ(calc.Divide(10, 2), 5);
}

// Деление отрицательного на положительное возвращает корректный результат
TEST(CalculatorTest, divideNegativeNumbersReturnsCorrectResult) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);
    EXPECT_EQ(calc.Divide(-10, 2), -5);
}

// Деление на ноль вызывает исключение invalid_argument
TEST(CalculatorTest, divideByZeroThrowsException) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);
    EXPECT_THROW(calc.Divide(5, 0), std::invalid_argument);
}

// Деление с остатком отбрасывает дробную часть (целочисленное деление)
TEST(CalculatorTest, divideRoundingTruncatesResult) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);
    EXPECT_EQ(calc.Divide(5, 2), 2);
}

// Деление логируется в историю корректно
TEST(SimpleCalculatorTest, DivideLogsCorrectly) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_CALL(mock, AddEntry("10 / 2 = 5"));

    int result = calc.Divide(10, 2);
    EXPECT_EQ(result, 5);
}

// Граничные значения
// Сложение с переполнением выбрасывает исключение overflow_error
TEST(SimpleCalculatorOverflowTest, AddOverflowThrows) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_THROW(calc.Add(INT_MAX, 1), std::overflow_error);
}

// Сложение без переполнения выполняется корректно (граничное значение)
TEST(SimpleCalculatorOverflowTest, AddNoOverflow) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_NO_THROW({
        int result = calc.Add(INT_MAX, 0);
        EXPECT_EQ(result, INT_MAX);
    });
}

// Вычитание с переполнением выбрасывает исключение overflow_error
TEST(SimpleCalculatorOverflowTest, SubtractOverflowThrows) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_THROW(calc.Subtract(INT_MIN, 1), std::overflow_error);
}

// Вычитание без переполнения выполняется корректно (граничное значение)
TEST(SimpleCalculatorOverflowTest, SubtractNoOverflow) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_NO_THROW({
        int result = calc.Subtract(INT_MIN, 0);
        EXPECT_EQ(result, INT_MIN);
    });
}

// Умножение с переполнением выбрасывает исключение overflow_error
TEST(SimpleCalculatorOverflowTest, MultiplyOverflowThrows) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_THROW(calc.Multiply(INT_MAX, 2), std::overflow_error);
}

// Умножение без переполнения выполняется корректно (граничное значение)
TEST(SimpleCalculatorOverflowTest, MultiplyNoOverflow) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_NO_THROW({
        int result = calc.Multiply(INT_MAX / 2, 2);
        EXPECT_EQ(result, (INT_MAX / 2) * 2);
    });
}

// Деление, которое приводит к переполнению, выбрасывает исключение overflow_error
TEST(SimpleCalculatorOverflowTest, DivideOverflowThrows) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_THROW(calc.Divide(INT_MIN, -1), std::overflow_error);
}

// Деление без переполнения выполняется корректно (граничное значение)
TEST(SimpleCalculatorOverflowTest, DivideNoOverflow) {
    MockHistory mock;
    calc::SimpleCalculator calc(mock);

    EXPECT_NO_THROW({
        int result = calc.Divide(INT_MIN, 1);
        EXPECT_EQ(result, INT_MIN);
    });
}

// Проверка сохранения истории
// Сохраняет историю операций в порядке добавления
TEST(InMemoryHistoryTest, StoresHistoryCorrectly) {
    calc::InMemoryHistory history;
    history.AddEntry("1 + 1 = 2");
    history.AddEntry("2 + 2 = 4");

    auto result = history.GetLastOperations(2);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "1 + 1 = 2");
    EXPECT_EQ(result[1], "2 + 2 = 4");
}

// Запрашиваемое количество записей не превышает реальное (обрезается до размера)
TEST(InMemoryHistoryTest, GetLastOperationsClampsToSize) {
    calc::InMemoryHistory history;
    history.AddEntry("1 + 1 = 2");

    auto result = history.GetLastOperations(5);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "1 + 1 = 2");
}

// История не превышает лимит
TEST(InMemoryHistoryTest, HistorySizeDoesNotExceedLimit) {
    calc::InMemoryHistory history;

    for (int i = 0; i < history.getMaxHistorySize() + 500; ++i) {
        history.AddEntry("op " + std::to_string(i));
    }

    auto entries = history.GetLastOperations(history.getMaxHistorySize() + 100); // запрашиваем больше, чем MAX

    EXPECT_EQ(entries.size(), history.getMaxHistorySize());  // точно не больше
}

// Тест с мок-калькулятором (обратное взаимодействие)
class MockCalculator {
public:
    MOCK_METHOD(int, Add, (int, int), ());
    MOCK_METHOD(int, Subtract, (int, int), ());
    MOCK_METHOD(int, Multiply, (int, int), ());
    MOCK_METHOD(int, Divide, (int, int), ());
};

// Мок калькулятора определён для возможности тестирования клиентского кода,
// который зависит от калькулятора и вызывает его методы.
// В данном проекте пока нет таких компонентов, которые используют калькулятор как зависимость,
// поэтому тесты с моками калькулятора не пишутся.
// Если появится клиентский класс, использующий калькулятор через интерфейс,
// тогда тесты с мок-калькулятором будут необходимы для проверки взаимодействия.