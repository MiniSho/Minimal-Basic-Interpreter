#ifndef BASICWINDOW_H
#define BASICWINDOW_H

#include <QMainWindow>
#include "console.h"
#include "program.h"

/*UI界面：BasicWindow 类*/
class BasicWindow : public QMainWindow {
    Q_OBJECT

   public:
    BasicWindow(QWidget *parent = nullptr);
    ~BasicWindow();

   private:
    Console *console;
    Program *program;

    static const int BASIC_WINDOW_WIDTH = 800;
    static const int BASIC_WINDOW_HEIGHT = 600;

   signals:
    void inputNumSignal(QString num);
   public slots:
    void callFromProgram(QString msg);
    void callFromConsole(QString msg);
};

#endif  // BASICWINDOW_H
