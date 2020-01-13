#ifndef EVALSTATE_H
#define EVALSTATE_H

#include <QMap>
#include <QObject>
#include <QString>

class EvalState : QObject {
    Q_OBJECT

   public:
    EvalState();
    ~EvalState();

    int Line;
    void setValue(QString var, int value);
    int getValue(QString var);
    bool isDefined(QString var);
    void clear();
    QMap<QString, int> *getSymbolTable();

   private:
    QMap<QString, int> symbolTable;
};
#endif  // EVALSTATE_H
