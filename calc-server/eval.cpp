#include "eval.h"
#include <QRegularExpression>

double Eval::exec(double value)
{
    if(stack.empty()) throw std::logic_error("Internal error");
    Operation op = stack.takeLast();
    switch(op.op)
    {
    case '+': return op.value + value;
    case '-': return op.value - value;
    case '*': return op.value * value;
    case '/': if(!value) throw std::logic_error("Division by zero");
        return op.value / value;
    default: throw std::logic_error("Unknown operation");
    }
}

Eval::Eval(const QString &expr)
{
    const static QRegularExpression regular("^\\d*\\.?\\d+([eE][-+]?\\d+)?");
    bool neg = false, valuePresent = false;
    double value;
    for(int idx = 0, end = expr.length(); idx < end; ++idx)
        switch(char c = expr[idx].toLatin1()) // Проходим по строке
        {
        case '-':
            if(!valuePresent) { neg = !neg; break; } // Унарная операция, меняем знак
        case '+':
            if(!valuePresent) throw std::logic_error("Syntax error");
            if(!stack.empty()) // '+' и '-' имеют низкий приоритет, поэтому выполняем предыдущую операцию
                value = exec(value);
            stack.push_back({value, c});
            valuePresent = false;
            break;
        case '*':
        case '/':
            if(!valuePresent) throw std::logic_error("Syntax error");
            stack.push_back({value, c});
            valuePresent = false;
            break;
        default:
            if((c >= '0' && c <= '9') || c == '.')
            {
                if(valuePresent)
                    throw std::logic_error("Syntax error");
                QRegularExpressionMatch match = regular.match(QStringRef(&expr, idx, end - idx));
                if(!match.hasMatch())
                    throw std::logic_error("Value parsing error");
                bool ok = false;
                value = match.capturedRef().toDouble(&ok);
                if(!ok)
                    throw std::logic_error("Value parsing error");
                if(neg) { neg = false; value = -value; }
                valuePresent = true;
                if(!stack.empty() && (stack.back().op == '*' || stack.back().op == '/'))
                    value = exec(value);
                idx += match.capturedLength() - 1;
            }
            else throw std::logic_error("Unknown symbol");
        }
    if(!valuePresent) throw std::logic_error("Syntax error");
    while(!stack.empty()) // Выполняем всё оставшиеся операции
        value = exec(value);
    result = value;
}

QString Eval::toString() const
{
    return QString("%1").arg(result);
}
