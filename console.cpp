#include "console.h"
#include <QFont>
#include <QKeyEvent>
#include <QPalette>
#include <QTextCursor>
#include <QTextLine>

Console::Console(QWidget *parent) : QTextEdit(parent) {
    this->setStyleSheet(QString("background-color:black"));
    this->setTextBackgroundColor(QColor(QString("black")));
    this->setTextColor(QColor(QString("lightgreen")));
    QFont font("Consolas", 10, 70);
    this->setFont(QFont(font));
}

void Console::keyPressEvent(QKeyEvent *event) {
    this->setTextColor(QColor(QString("white")));
    if (event->key() == Qt::Key_Backspace) this->textCursor().clearSelection();
    if (event->key() == Qt::Key_Delete) {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
    }
    if (this->textCursor().hasSelection()) return;
    if (event->key() == Qt::Key_Return) {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        emit newLineWritten(lastLine);
    }
    QTextEdit::keyPressEvent(event);
}

void Console::write(QString msg) {
    this->setTextColor(QColor(QString("lightgreen")));
    this->append(msg);
}

void Console::inputWaitFunction(QString msg) { this->write("input " + msg); }
