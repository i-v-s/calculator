#ifndef EVAL_H
#define EVAL_H
#include <QString>
#include <QVector>

//! Класс расчёта строкового выражения
class Eval
{
    double result;
    struct Operation //!< Структура, содержащая фрагмент выражения вида [value][op], например '3.14+'
    {
        double value;
        char op;
    };
    QVector<Operation> stack; //!< Стек отложенных фрагментов выражений
    double exec(double value); //!< Метод выполняет операцию
public:
    Eval(const QString &expr); //!< Конструктор принимает выражение и рассчитывает его
    QString toString() const; //!< Метод возвращает результат вычисления в виде строки
};

#endif // EVAL_H
