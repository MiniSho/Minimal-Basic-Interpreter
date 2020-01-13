#include "statement.h"
#include <QMap>
#include <QStack>
#include <QStringList>
statement::statement(QObject *parent) : QObject(parent) {
    staTable["REM"] = REM;
    staTable["LET"] = LET;
    staTable["PRINT"] = PRINT;
    staTable["INPUT"] = INPUT;
    staTable["GOTO"] = GOTO;
    staTable["IF"] = IF;
    staTable["END"] = END;
    eva = new EvalState();
}

/**区分不同command**/
QString statement::classify(QString s) {
    QString firstWord;                  // 接受lineNumber
    QString otherWord;                  // statement
    firstWord = s.section(' ', 0, 0);   //接受字符串的第一段
    otherWord = s.section(' ', 1, -1);  //接受字符串的第二段
    bool flag = false;
    otherWord.toInt(&flag);
    if (!staTable.contains(firstWord)) {  //若不包含 返回wrong
        return "WRONG";
    } else {
        try {
            switch (staTable[firstWord]) {  //识别命令
                case REM:
                    return "";

                case LET:
                    convertToExp(otherWord);
                    if (expression == nullptr)
                        return QString("WRONG EXPRESSION : ") + s;
                    expression->eval(*eva);
                    delete expression;
                    break;

                case PRINT: {
                    convertToExp(otherWord);
                    if (expression == nullptr)
                        return QString("WRONG EXPRESSION : ") + s;
                    int num = expression->eval(*eva);
                    delete expression;
                    return "PRINT " + QString::number(num);
                }

                case INPUT:
                    if (flag) throw QString(" can't be a number.");
                    return (QString("INPUT ") + otherWord);

                case GOTO:
                    if (!flag) throw QString(" no such number.");
                    return s.section(' ', 1, 1);
                case IF: {
                    bool _flag(false);
                    s.section(' ', -1, -1).toInt(&_flag);

                    // THEN 之后不是数字的情况
                    if (!_flag) {
                        return "WRONG";
                    }
                    //没有THEN的情况
                    if (s.section(' ', -2, -2) != "THEN") {
                        return "WRONG";
                    }

                    int exp1;
                    int exp2;
                    QString exp1_str;
                    QString exp2_str;
                    QStringList l = s.split(' ');
                    int i = 0;
                    for (; i < l.size(); i++)
                        if (l[i] == "<" || l[i] == ">" || l[i] == "<=" ||
                            l[i] == ">=" || l[i] == "=" || l[i] == "!=")  //=
                            break;
                    exp1_str = s.section(' ', 1, i - 1);
                    exp2_str = s.section(' ', i + 1, -3);

                    convertToExp(exp1_str);
                    exp1 = expression->eval(*eva);
                    delete expression;

                    convertToExp(exp2_str);
                    exp2 = expression->eval(*eva);
                    delete expression;

                    if (l[i] == "<" && exp1 < exp2)
                        return s.section(' ', -1, -1);
                    else if (l[i] == ">" && exp1 > exp2)
                        return s.section(' ', -1, -1);
                    else if (l[i] == "<=" && exp1 <= exp2)
                        return s.section(' ', -1, -1);
                    else if (l[i] == ">=" && exp1 >= exp2)
                        return s.section(' ', -1, -1);
                    else if (l[i] == "=" && exp1 == exp2)
                        return s.section(' ', -1, -1);
                    else if (l[i] == "!=" && exp1 != exp2)
                        return s.section(' ', -1, -1);
                    else
                        break;
                }

                case END:
                    return "END";
            }
        } catch (QString s) {
            return QString("WRONG : ") + s;
        }
    }
    return "";
}

//将expression转化
void statement::convertToExp(QString s) {
    QStringList wholeExpression;
    QVector<QString> operators;
    QVector<QString> operands;

    // 将输入进来的字符串放在wholeExpression里
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '*' && s[i + 1] == '*') {
            s.insert(i, ' ');
            s.insert(i += 3, ' ');
            s.insert(i + 1, ' ');
        }
        if (s[i] == '=' || s[i] == '+' || s[i] == '-' || s[i] == '*' ||
            s[i] == '/' || s[i] == '(' || s[i] == ')') {
            s.insert(i++, ' ');
            s.insert(i + 1, ' ');
        }
    }
    s = s.trimmed();
    s.replace(QRegExp("[\\s]+"), " ");
    wholeExpression = s.split(' ');
    QStringList *tmp = infixToSuffi(wholeExpression);
    if (tmp == nullptr) {
        expression = nullptr;
    } else
        expression = combineExp(*tmp, 0, (*tmp).size());
}
//工具函数， 中缀转后缀时使用

/*判断是否是数字*/
bool isDigit(QString str) {
    bool flag(false);
    str.toInt(&flag);
    return flag;
}

/*返回该运算符的优先级*/
int getPrioraty(QString op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "**") return 3;
    return 0;
}

/*判断是否是运算符*/
bool isOperator(QString op) {
    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "**" ||
        op == "=")
        return true;
    return false;
}

/*将表达式组合*/
Expression *statement::combineExp(QStringList &exp, int x, int y) {
    QStack<Expression *> elem;
    for (; x < y; x++) {
        if (isDigit(exp[x])) {
            Expression *e = new ConstantExp(exp[x].toInt());
            elem.push(e);
        } else if (isOperator(exp[x])) {
            if (elem.empty()) {
                return nullptr;
            }
            Expression *r = elem.pop();
            if (elem.empty()) {
                return nullptr;
            }
            Expression *l = elem.pop();
            Expression *c = new CompoundExp(exp[x], l, r);
            elem.push(c);
        } else {
            Expression *e = new IdentifierExp(exp[x]);
            elem.push(e);
        }
    }
    if (y == 0) {
        Expression *e = new IdentifierExp(exp[x]);
        return e;
    }
    return elem.pop();
}

/*中缀转后缀*/
QStringList *statement::infixToSuffi(QStringList &exp) {
    QVector<QString> num;
    QStack<QString> op;
    QString tmp;
    QStringList *suffi;
    suffi = new QStringList;
    int index = 0;
    int count = 0;
    bool flag(false);
    if (exp.size() == 1) return &exp;
    if (exp.contains("=")) {
        flag = true;
        index = 2;
    }

    while (index < exp.size()) {
        if (exp[index] == "(") {
            count++;
            op.push(exp[index]);
            index++;
        } else if (isOperator(exp[index])) {
            if (op.empty()) {
                op.push(exp[index]);
                index++;
            } else {
                while (!op.empty()) {
                    tmp = op.top();
                    if (getPrioraty(exp[index]) <= getPrioraty(tmp)) {
                        suffi->push_back(tmp);
                        op.pop();
                    } else
                        break;
                }
                op.push(exp[index]);
                index++;
            }
        } else if (exp[index] == ")") {
            count--;
            while (op.top() != "(") {
                suffi->push_back(op.top());
                op.pop();
            }
            op.pop();
            index++;
        } else {
            suffi->push_back(exp[index]);
            index++;
        }
    }
    while (!op.empty()) {
        suffi->push_back(op.top());
        op.pop();
    }
    if (count != 0) {
        return nullptr;
    }
    if (flag) {
        suffi->push_front(exp[0]);
        suffi->push_back(exp[1]);
    }
    return suffi;
}
