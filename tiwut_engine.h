#ifndef TIWUT_ENGINE_H
#define TIWUT_ENGINE_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QMap>
#include <functional>

class TiwutEngine {
public:
    TiwutEngine();
    QString executeCommand(const QString& commandLine);
    QString getPrompt() const;
    QString getWelcomeMessage() const;

private:
    QDir baseDir;
    QDir currentDir;
    QDir execDir;
    QStringList history;

    QMap<QString, std::function<QString(const QStringList&)>> commands;

    void initializeDirectories();
    void registerCommands();

    QString doHelp(const QStringList& args);
    QString doPwd(const QStringList& args);
    QString doLs(const QStringList& args);
    QString doCd(const QStringList& args);
    QString doMkdir(const QStringList& args);
    QString doRmdir(const QStringList& args);
    QString doRm(const QStringList& args);
    QString doCp(const QStringList& args);
    QString doCat(const QStringList& args);
    QString doEcho(const QStringList& args);
    QString doTouch(const QStringList& args);
    QString doMv(const QStringList& args);
    QString doDate(const QStringList& args);
    QString doHistory(const QStringList& args);
    QString doWhoami(const QStringList& args);
    QString doWc(const QStringList& args);
    QString doHead(const QStringList& args);
    QString doBase64(const QStringList& args);
    QString doHash(const QStringList& args);
    QString doSysInfo(const QStringList& args);

    QString executeExternalTool(const QString& program, const QStringList& args);
    QString formatPath(const QDir& dir) const;
};

#endif