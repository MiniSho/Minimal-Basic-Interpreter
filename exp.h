#ifndef EXP_H
#define EXP_H
#include <QMap>
#include <QString>
#include <string>
#include "evalstate.h"

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

class Expression {
   public:
    Expression();
    virtual ~Expression();
    virtual int eval(EvalState &state) = 0;
    virtual QString toQString() = 0;
    virtual ExpressionType getType() = 0;
    virtual QString getName() { return ""; }
};

class ConstantExp : public Expression {
   public:
    ConstantExp(int val);
    virtual int eval(EvalState &state);
    virtual QString toQString();
    virtual ExpressionType getType();

    int getValue();

   private:
    int value;
};

class IdentifierExp : public Expression {
   public:
    IdentifierExp(QString name);
    virtual int eval(EvalState &state);
    virtual QString toQString();
    virtual ExpressionType getType();

    QString getName();

   private:
    QString name;
};

class CompoundExp : public Expression {
   public:
    CompoundExp(QString op, Expression *lhs, Expression *rhs);
    ~CompoundExp();
    virtual int eval(EvalState &state);
    virtual QString toQString();
    virtual ExpressionType getType();

    QString getOp();
    Expression *getLHS();
    Expression *getRHS();

   private:
    QString op;
    Expression *lhs;
    Expression *rhs;
};

#endif  // EXP_H
