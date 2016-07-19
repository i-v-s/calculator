#include "eval.h"

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
    bool neg = false, valuePresent = false;
    double value;
    for(auto i = expr.begin(); i != expr.end(); ++i)
        switch(char c = i->toLatin1()) // Проходим по строке
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
                auto start = i;
                for( ; i != expr.end(); ++i)
                {
                    char d = i->toLatin1();
                    if(!((d >= '0' && d <= '9') || d == '.'))
                        break;
                }
                bool ok = false;
                value = QString::fromRawData(start, i - start).toDouble(&ok);
                if(!ok) throw std::logic_error("Value parsing error");
                if(neg) { neg = false; value = -value; }
                valuePresent = true;
                if(!stack.empty() && (stack.back().op == '*' || stack.back().op == '/'))
                    value = exec(value);
                --i;
            }
            else throw std::logic_error("Unknown symbol");
        }
    if(!valuePresent) throw std::logic_error("Syntax error");
    while(!stack.empty()) // Выполняем всё оставшиеся операции
        value = exec(value);
    result = value;
}

QString Eval::toString()
{
    return QString("%1").arg(result);
}
