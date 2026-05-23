#include <QApplication>
#include <QCoreApplication>
#include <iostream>
#include <string>
#include "terminal_window.h"
#include "tiwut_engine.h"

void runCliMode() {
    TiwutEngine engine;
    std::cout << engine.getWelcomeMessage().toStdString();
    
    std::string input;
    while (true) {
        std::cout << engine.getPrompt().toStdString();
        if (!std::getline(std::cin, input)) break;
        
        QString cmd = QString::fromStdString(input).trimmed();
        if (cmd == "exit" || cmd == "quit") break;
        if (cmd == "clear") {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        
        QString output = engine.executeCommand(cmd);
        std::cout << output.toStdString();
    }
}

int main(int argc, char *argv[]) {
    bool cliMode = false;
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--cli") {
            cliMode = true;
            break;
        }
    }

    if (cliMode) {
        QCoreApplication app(argc, argv);
        runCliMode();
        return 0;
    } else {
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        return app.exec();
    }
}