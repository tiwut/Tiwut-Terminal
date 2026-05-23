#ifndef TERMINAL_WINDOW_H
#define TERMINAL_WINDOW_H

#include <QPlainTextEdit>
#include <QMainWindow>
#include "tiwut_engine.h"

class TerminalWindow : public QPlainTextEdit {
    Q_OBJECT

public:
    TerminalWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    TiwutEngine engine;
    int promptPosition;

    void appendPrompt();
    void processCommand();
    void insertInterrupt();
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    TerminalWindow* terminal;
};

#endif