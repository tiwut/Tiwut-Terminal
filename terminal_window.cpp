#include "terminal_window.h"
#include <QApplication>
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextCursor>
#include <QClipboard>

TerminalWindow::TerminalWindow(QWidget* parent) : QPlainTextEdit(parent) {
    setStyleSheet("QPlainTextEdit { background-color: #1e1e1e; color: #d4d4d4; font-family: 'Consolas', 'Courier New', monospace; font-size: 14px; }");
    setWordWrapMode(QTextOption::WrapAnywhere);
    setUndoRedoEnabled(false);

    appendPlainText(engine.getWelcomeMessage());
    appendPrompt();
}

void TerminalWindow::appendPrompt() {
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
    insertPlainText(engine.getPrompt());
    promptPosition = textCursor().position();
}

void TerminalWindow::processCommand() {
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);

    QString command = toPlainText().mid(promptPosition).trimmed();
    insertPlainText("\n");

    if (command == "clear") {
        clear();
        appendPrompt();
        return;
    } else if (command == "exit" || command == "quit") {
        QApplication::quit();
        return;
    }

    if (!command.isEmpty()) {
        QString output = engine.executeCommand(command);
        if (!output.isEmpty()) {
            insertPlainText(output);
            if (!output.endsWith("\n")) {
                insertPlainText("\n");
            }
        }
    }
    appendPrompt();
}

void TerminalWindow::insertInterrupt() {
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
    insertPlainText("^C\n");
    appendPrompt();
}

void TerminalWindow::keyPressEvent(QKeyEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_C) {
            if (textCursor().hasSelection()) {
                copy();
            } else {
                insertInterrupt();
            }
            return;
        } else if (event->key() == Qt::Key_V) {
            QTextCursor cursor = textCursor();
            if (cursor.position() < promptPosition) {
                cursor.movePosition(QTextCursor::End);
                setTextCursor(cursor);
            }
            paste();
            return;
        }
    }

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        processCommand();
        return;
    }

    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Left) {
        if (textCursor().position() <= promptPosition) {
            return;
        }
    }

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        return;
    }

    QTextCursor cursor = textCursor();
    if (cursor.position() < promptPosition) {
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
    }

    QPlainTextEdit::keyPressEvent(event);
}

void TerminalWindow::mousePressEvent(QMouseEvent* event) {
    QPlainTextEdit::mousePressEvent(event);
}

void TerminalWindow::mouseDoubleClickEvent(QMouseEvent* event) {
    QPlainTextEdit::mouseDoubleClickEvent(event);
}

void TerminalWindow::contextMenuEvent(QContextMenuEvent*) {
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    terminal = new TerminalWindow(this);
    setCentralWidget(terminal);
    resize(1000, 600);
    setWindowTitle("Tiwut Terminal");
}