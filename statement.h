#ifndef STATEMENT_H
#define STATEMENT_H

#include <QDebug>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QVector>
#include <QWidget>
#include "evalstate.h"
#include "exp.h"

/**Statement类**/
/** 作用：将接受已去除行数的信息进行分类处理**/
enum statementType { REM, LET, PRINT, INPUT, GOTO, IF, END };

class statement : public QObject {
    Q_OBJECT
   private:
    QMap<QString, statementType> staTable;
    Expression *combineExp(QStringList &exp, int x, int y);  //表达式组合
    QStringList *infixToSuffi(QStringList &exp);             //中缀转后缀

   public:
    Expression *expression;
    EvalState *eva;
    explicit statement(QObject *parent = nullptr);
    ~statement() {}
    QString classify(QString str);  //分析
    void convertToExp(QString str);
   signals:
    void writeSignal(QString msg);
};
#endif  // STATEMENT_H
