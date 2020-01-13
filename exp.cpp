#include "exp.h"
#include <math.h>
Expression::Expression() {}

Expression::~Expression() {}

/* Implementation notes: the ConstantExp subclass*/
ConstantExp::ConstantExp(int value) { this->value = value; }

int ConstantExp::eval(EvalState &state) { return value; }

QString ConstantExp::toQString() { return QString::number(value); }

ExpressionType ConstantExp::getType() { return CONSTANT; }

int ConstantExp::getValue() { return value; }

/* Implementation notes: the IdentifierExp subclass*/
IdentifierExp::IdentifierExp(QString name) {
    if (name == "LET") throw("SYNTAX ERROR");
    this->name = name;
}

int IdentifierExp::eval(EvalState &state) {
    if (!state.isDefined(name)) throw("VARIABLE NOT DEFINED");
    return state.getValue(name);
}

QString IdentifierExp::toQString() { return name; }

ExpressionType IdentifierExp::getType() { return IDENTIFIER; }

QString IdentifierExp::getName() { return name; }

/* Implementation notes: the CompoundExp subclass*/
CompoundExp::CompoundExp(QString op, Expression *lhs, Expression *rhs) {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

CompoundExp::~CompoundExp() {
    delete lhs;
    delete rhs;
}

int CompoundExp::eval(EvalState &state) {
    if (op == "=") {
        if (lhs->getType() != IDENTIFIER) {
            throw("Illegal variable");
        }
        int val = rhs->eval(state);
        state.setValue(((IdentifierExp *)lhs)->getName(), val);
        return val;
    }
    int left = lhs->eval(state);
    int right = rhs->eval(state);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) {
            throw("DIVIDE BY ZERO");
        }
        return left / right;
    }
    if (op == "**") return (int)pow(left, right);
    throw("Illegal operator in expression");
}

QString CompoundExp::toQString() {
    return '(' + lhs->toQString() + ' ' + op + ' ' + rhs->toQString() + ')';
}

ExpressionType CompoundExp::getType() { return COMPOUND; }

QString CompoundExp::getOp() { return op; }

Expression *CompoundExp::getLHS() { return lhs; }

Expression *CompoundExp::getRHS() { return rhs; }
