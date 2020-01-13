#include "basicwindow.h"

/*UI界面*/
BasicWindow::BasicWindow(QWidget *parent) : QMainWindow(parent) {
    /**初始化UI**/
    this->setWindowTitle("Basic Interpreter");
    this->setFixedSize(BASIC_WINDOW_WIDTH, BASIC_WINDOW_HEIGHT);

    console = new Console(this);  //控制台
    program = new Program(this);  //处理

    console->setGeometry(0, 0, 800, 600);
    console->write("Minimal Basic Interpreter\n");
    console->write("(Type HELP for help.)\n");

    console->write(
        "----------------------------------------------------------------------"
        "-----------------");
    console->write("\n");

    /** connect **/
    connect(console, &Console::newLineWritten, program,
            &Program::parseCommand);  // console接受str，送到parse
    connect(program, &Program::printSignal, console, &Console::write);  // print
    connect(program, &Program::inputSignal, this,
            &BasicWindow::callFromProgram);  // input
    connect(this, &BasicWindow::inputNumSignal, program,
            &Program::reserveNumber);
    connect(program, &Program::spaceSignal, console, &Console::write);
}

BasicWindow::~BasicWindow() {}

void BasicWindow::callFromProgram(QString msg) {
    disconnect(console, &Console::newLineWritten, program,
               &Program::parseCommand);
    console->write("?");
    connect(console, &Console::newLineWritten, this,
            &BasicWindow::callFromConsole);
}

void BasicWindow::callFromConsole(QString msg) {
    connect(console, &Console::newLineWritten, program, &Program::parseCommand);
    disconnect(console, &Console::newLineWritten, this,
               &BasicWindow::callFromConsole);

    emit inputNumSignal(msg);
}
