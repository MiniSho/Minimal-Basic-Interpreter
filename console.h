#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

/*控制台类*/
/**用于接受，反馈用户信息**/
class Console : public QTextEdit {
    Q_OBJECT
   public:
    explicit Console(QWidget *parent = nullptr);

   signals:
    void newLineWritten(QString newline);

   public slots:
    void write(QString msg);
    void inputWaitFunction(QString msg);

   protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif  // CONSOLE_H
