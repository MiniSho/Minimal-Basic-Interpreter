#ifndef PROGRAM_H
#define PROGRAM_H
#include <QFile>
#include <QObject>
#include <QWidget>
#include "evalstate.h"
#include "statement.h"

class Program : public QObject {
    Q_OBJECT
   private:
    QString firstWord;
    statement *state;         //正常输入行数的情况
    statement *instantState;  //即时显示（不输入行数）

    QMap<int, QString> allLine;

   public:
    int inputLineNum;
    explicit Program(QObject *parent = nullptr);
    void store(QString str);
   signals:
    void stateCommand(QString s);
    void printSignal(QString s);
    void runFunctionSignal();
    void inputSignal(QString s);
    void spaceSignal(QString s);
   public slots:
    void parseCommand(QString msg);
    QString runFunction(int lineNumber);
    void reserveNumber(QString num);
};
#endif  // PROGRAM_H
