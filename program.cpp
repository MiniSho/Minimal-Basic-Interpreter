#include "program.h"
#include <QDir>
#include <QThread>
#include <fstream>
#include <iterator>
Program::Program(QObject *parent) : QObject(parent) {
    state = nullptr;
    instantState = new statement();
}

void Program::parseCommand(QString msg) {
    firstWord = msg.section(' ', 0, 0);
    bool flag = false;
    bool flag2 = false;
    firstWord.toInt(&flag);  //用于判断 第一部分是否是行数
    msg.section(' ', 1, 1).toInt(&flag2);
    if (flag && firstWord.toInt() > 0) {
        store(msg);
    }

    else if (msg == "") {
        return;
    }

    else if (firstWord == "LIST") {
        QMap<int, QString>::iterator it = allLine.begin();
        emit spaceSignal("");
        for (; it != allLine.end(); it++) {
            emit printSignal(QString::number(it.key()) + " " + it.value());
        }
    }

    else if (firstWord == "RUN") {
        int num = -1;
        if (!allLine.empty()) num = allLine.begin().key();
        delete instantState;  //清除即时显示的statement
        instantState = new statement();

        runFunction(num);
    }

    else if (firstWord == "PRINT") {
        QString cmd = instantState->classify(msg);
        if (cmd.section(' ', 0, 0) == "WRONG") {
            emit printSignal(cmd);
        } else
            emit printSignal(cmd.section(' ', 1, 1));
    }

    else if (firstWord == "LET") {
        if (!flag2) {
            QString cmd = instantState->classify(msg);
        } else {
            emit printSignal("Wrong Command! Can't be Number!");
        }
    }

    else if (firstWord == "INPUT") {
        instantState->eva->setValue(msg.section(' ', 1, 1), -1);
        emit inputSignal(msg.section(' ', 1, 1) + " p");
    }

    else if (firstWord == "CLEAR") {
        delete instantState;
        instantState = new statement();
        allLine.clear();
    }

    else if (firstWord == "QUIT") {
        exit(0);
    }

    else if (firstWord == "HELP") {
        QDir d;
        QFile file(d.absoluteFilePath(":/help.txt"));
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
        }
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QString str(line);
            str = str.trimmed();
            str.replace(QRegExp("[\\s]+"), " ");
            emit printSignal(str);
        }
    }
    /*其他wrong情况*/
    else {
        emit printSignal("Wrong Command! Please type \"HELP\" to get help.");
    }
}

QString Program::runFunction(int lineNumber) {
    if (state == nullptr) {
        state = new statement();
    }
    QString cmd;
    static QMap<int, QString>::iterator it ;//工具人
    it = allLine.find(lineNumber);

    if (!allLine.empty()) {
        int line = lineNumber;
        cmd = state->classify(allLine[lineNumber]);  // LineNumber的value

        while (cmd != "END") {
            bool flag(false);
            cmd.toInt(&flag);

            if (cmd == "WRONG") {
                emit printSignal("WRONG:line " + QString::number(line) +
                                 " : Wrong Command!");
                break;
            }

            /** GOTO **/
            else if (flag) {                 // GOTO
                int gotoLine = cmd.toInt();  //获得行数
                it = allLine.find(gotoLine);
                if (it == allLine.end()) {  //不存在行数时
                    emit printSignal("WRONG:line " + QString::number(gotoLine) +
                                     " : NO such LINE to GOTO!");
                    break;
                }
                line = gotoLine;
            }

            /** PRINT **/
            else if (cmd.section(' ', 0, 0) == "PRINT") {
                emit printSignal(cmd.section(' ', 1));
                it++;
                if (it == allLine.end()) break;
                line = it.key();
            }

            /** INPUT **/
            else if (cmd.section(' ', 0, 0) == "INPUT") {
                it++;
                inputLineNum = it.key();
                /*判断变量是否合法*/
                if (cmd.section(' ', 1, -1).contains(' ')) {
                    emit printSignal("WRONG:line " + QString::number(it.key()) +
                                     "invalid variable");
                    break;
                }
                state->eva->setValue(cmd.section(' ', 1, 1), -114514);
                emit inputSignal(cmd.section(' ', 1, 1));
                return "INPUT";
            }

            /** 非法字符 **/
            else if (cmd.section(' ', 0, 0) == "WRONG") {
                emit printSignal("WRONG:line " + QString::number(line) +
                                 cmd.section(' ', 1));
                break;
            }

            else {
                if ((++it) == allLine.end()) break;
                line = it.key();
            }

            it = allLine.find(line);
            if (it != allLine.end()) {
                cmd = state->classify(it.value());
            }
        }
    }
    if (it == allLine.end()) {
        delete state;
        state = nullptr;
    }
    return "";
}

void Program::reserveNumber(QString num) {
    bool flag = false;
    int number = num.toInt(&flag);
    if (!flag) {
        emit printSignal("Wrong Input : " + num + " !");
        return;
    }

    statement *st;
    if (firstWord == "INPUT")
        st = instantState;
    else
        st = state;
    QMap<QString, int>::iterator iter;
    QMap<QString, int> *table = st->eva->getSymbolTable();
    for (iter = table->begin(); iter != table->end(); iter++) {
        if (iter.value() == -114514) {
            st->eva->setValue(iter.key(), number);
        }
    }
    if (firstWord != "INPUT") {
        this->runFunction(inputLineNum);
    }
}

void Program::store(QString msg) {
    QString str = msg.section(' ', 0, 0);
    bool numFlag;
    int num = str.toInt(&numFlag);
    msg = msg.section(' ', 1, -1);
    allLine.insert(num, msg);
}
