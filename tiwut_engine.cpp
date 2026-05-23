#include "tiwut_engine.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDateTime>
#include <QProcess>
#include <QCryptographicHash>
#include <QHostInfo>
#include <QSysInfo>
#include <QTextStream>
#include <QRegularExpression>

TiwutEngine::TiwutEngine() {
    initializeDirectories();
    registerCommands();
}

void TiwutEngine::initializeDirectories() {
    QString docPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    baseDir = QDir(docPath + "/TiwutApps");
    if (!baseDir.exists()) {
        baseDir.mkpath(".");
    }
    currentDir = baseDir;

    execDir = QDir(QCoreApplication::applicationDirPath() + "/exec");
    if (!execDir.exists()) {
        execDir.mkpath(".");
    }
}

void TiwutEngine::registerCommands() {
    commands["help"] = [this](const QStringList& args) { return doHelp(args); };
    commands["pwd"] = [this](const QStringList& args) { return doPwd(args); };
    commands["ls"] = [this](const QStringList& args) { return doLs(args); };
    commands["cd"] = [this](const QStringList& args) { return doCd(args); };
    commands["mkdir"] = [this](const QStringList& args) { return doMkdir(args); };
    commands["rmdir"] = [this](const QStringList& args) { return doRmdir(args); };
    commands["rm"] = [this](const QStringList& args) { return doRm(args); };
    commands["cp"] = [this](const QStringList& args) { return doCp(args); };
    commands["cat"] = [this](const QStringList& args) { return doCat(args); };
    commands["echo"] = [this](const QStringList& args) { return doEcho(args); };
    commands["touch"] = [this](const QStringList& args) { return doTouch(args); };
    commands["mv"] = [this](const QStringList& args) { return doMv(args); };
    commands["date"] = [this](const QStringList& args) { return doDate(args); };
    commands["history"] = [this](const QStringList& args) { return doHistory(args); };
    commands["whoami"] = [this](const QStringList& args) { return doWhoami(args); };
    commands["wc"] = [this](const QStringList& args) { return doWc(args); };
    commands["head"] = [this](const QStringList& args) { return doHead(args); };
    commands["base64"] = [this](const QStringList& args) { return doBase64(args); };
    commands["hash"] = [this](const QStringList& args) { return doHash(args); };
    commands["sysinfo"] = [this](const QStringList& args) { return doSysInfo(args); };
}

QString TiwutEngine::getPrompt() const {
    QString relPath = baseDir.relativeFilePath(currentDir.absolutePath());
    if (relPath == ".") relPath = "~";
    return "Tiwut:[" + relPath + "]$ ";
}

QString TiwutEngine::getWelcomeMessage() const {
    return "Welcome to Tiwut Terminal (C++ Qt Edition)!\n"
           "Root Directory: " + baseDir.absolutePath() + "\n"
           "Type 'help' for built-in commands. Drop executables in '" + execDir.absolutePath() + "' to expand.\n";
}

QString TiwutEngine::executeCommand(const QString& commandLine) {
    if (commandLine.trimmed().isEmpty()) return "";
    history.append(commandLine);

    QStringList parts;
    bool inQuotes = false;
    QString currentPart;
    for (int i = 0; i < commandLine.length(); ++i) {
        QChar c = commandLine[i];
        if (c == '\"') {
            inQuotes = !inQuotes;
        } else if (c == ' ' && !inQuotes) {
            if (!currentPart.isEmpty()) {
                parts.append(currentPart);
                currentPart.clear();
            }
        } else {
            currentPart += c;
        }
    }
    if (!currentPart.isEmpty()) parts.append(currentPart);
    if (parts.isEmpty()) return "";

    QString cmd = parts.takeFirst();
    
    if (commands.contains(cmd)) {
        return commands[cmd](parts);
    }
    
    QStringList execFiles = execDir.entryList(QDir::Files | QDir::Executable);
    for (const QString& file : execFiles) {
        if (file == cmd || file == cmd + ".exe" || file == cmd + ".bat" || file == cmd + ".sh") {
            return executeExternalTool(execDir.absoluteFilePath(file), parts);
        }
    }

    return "Error: Command not found: " + cmd + "\n";
}

QString TiwutEngine::executeExternalTool(const QString& program, const QStringList& args) {
    QProcess process;
    process.setWorkingDirectory(currentDir.absolutePath());
    process.start(program, args);
    if (!process.waitForStarted()) {
        return "Error: Could not start external tool.\n";
    }
    process.waitForFinished();
    return process.readAllStandardOutput() + process.readAllStandardError();
}

QString TiwutEngine::doHelp(const QStringList&) {
    return "Tiwut Built-in Commands:\n"
           "  ls, cd, pwd, mkdir, rmdir, rm, cp, mv, touch, cat, echo\n"
           "  date, history, whoami, wc, head, base64, hash, sysinfo\n"
           "  clear, exit\n"
           "External Tool Directory:\n  " + execDir.absolutePath() + "\n";
}

QString TiwutEngine::doPwd(const QStringList&) {
    return currentDir.absolutePath() + "\n";
}

QString TiwutEngine::doLs(const QStringList& args) {
    QDir targetDir = currentDir;
    if (!args.isEmpty()) {
        targetDir = QDir(currentDir.absoluteFilePath(args.first()));
    }
    if (!targetDir.exists()) return "Error: Directory not found.\n";

    QString result;
    QFileInfoList list = targetDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name | QDir::DirsFirst);
    for (const QFileInfo& info : list) {
        if (info.isDir()) {
            result += "[DIR]  " + info.fileName() + "\n";
        } else {
            result += "[FILE] " + info.fileName() + " (" + QString::number(info.size()) + " bytes)\n";
        }
    }
    return result;
}

QString TiwutEngine::doCd(const QStringList& args) {
    if (args.isEmpty()) {
        currentDir = baseDir;
        return "";
    }
    QString target = args.first();
    if (target == "..") {
        if (currentDir != baseDir) currentDir.cdUp();
        return "";
    }
    QDir newDir(currentDir.absoluteFilePath(target));
    if (newDir.exists() && newDir.absolutePath().startsWith(baseDir.absolutePath())) {
        currentDir = newDir;
        return "";
    }
    return "Error: Invalid directory or access denied.\n";
}

QString TiwutEngine::doMkdir(const QStringList& args) {
    if (args.isEmpty()) return "Usage: mkdir <dir>\n";
    if (currentDir.mkdir(args.first())) return "Directory created.\n";
    return "Error: Could not create directory.\n";
}

QString TiwutEngine::doRmdir(const QStringList& args) {
    if (args.isEmpty()) return "Usage: rmdir <dir>\n";
    if (currentDir.rmdir(args.first())) return "Directory removed.\n";
    return "Error: Could not remove directory.\n";
}

QString TiwutEngine::doRm(const QStringList& args) {
    if (args.isEmpty()) return "Usage: rm [-r] <file/dir>\n";
    bool recursive = args.contains("-r");
    QString target = args.last();
    QFileInfo info(currentDir.absoluteFilePath(target));
    if (!info.exists()) return "Error: Target not found.\n";
    if (info.isFile()) {
        QFile::remove(info.absoluteFilePath());
        return "File removed.\n";
    } else if (info.isDir() && recursive) {
        QDir(info.absoluteFilePath()).removeRecursively();
        return "Directory recursively removed.\n";
    }
    return "Error: Cannot remove directory without -r flag.\n";
}

QString TiwutEngine::doCp(const QStringList& args) {
    if (args.size() < 2) return "Usage: cp <source> <dest>\n";
    QString src = currentDir.absoluteFilePath(args[0]);
    QString dst = currentDir.absoluteFilePath(args[1]);
    if (QFile::copy(src, dst)) return "File copied.\n";
    return "Error: Could not copy file.\n";
}

QString TiwutEngine::doMv(const QStringList& args) {
    if (args.size() < 2) return "Usage: mv <source> <dest>\n";
    QString src = currentDir.absoluteFilePath(args[0]);
    QString dst = currentDir.absoluteFilePath(args[1]);
    if (QFile::rename(src, dst)) return "File moved.\n";
    return "Error: Could not move file.\n";
}

QString TiwutEngine::doTouch(const QStringList& args) {
    if (args.isEmpty()) return "Usage: touch <file>\n";
    QFile file(currentDir.absoluteFilePath(args.first()));
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        file.close();
        return "";
    }
    return "Error: Could not touch file.\n";
}

QString TiwutEngine::doCat(const QStringList& args) {
    if (args.isEmpty()) return "Usage: cat <file>\n";
    QFile file(currentDir.absoluteFilePath(args.first()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "Error: Could not read file.\n";
    return QString(file.readAll()) + "\n";
}

QString TiwutEngine::doEcho(const QStringList& args) {
    return args.join(" ") + "\n";
}

QString TiwutEngine::doDate(const QStringList&) {
    return QDateTime::currentDateTime().toString() + "\n";
}

QString TiwutEngine::doHistory(const QStringList&) {
    QString res;
    for (int i = 0; i < history.size(); ++i) res += QString::number(i + 1) + "  " + history[i] + "\n";
    return res;
}

QString TiwutEngine::doWhoami(const QStringList&) {
    return QDir::home().dirName() + "\n";
}

QString TiwutEngine::doWc(const QStringList& args) {
    if (args.isEmpty()) return "Usage: wc <file>\n";
    QFile file(currentDir.absoluteFilePath(args.first()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "Error: Could not read file.\n";
    QString content = file.readAll();
    int lines = content.count('\n');
    int words = content.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).size();
    int chars = content.length();
    return QString("%1 lines, %2 words, %3 chars\n").arg(lines).arg(words).arg(chars);
}

QString TiwutEngine::doHead(const QStringList& args) {
    if (args.isEmpty()) return "Usage: head <file>\n";
    QFile file(currentDir.absoluteFilePath(args.first()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "Error: Could not read file.\n";
    QString res;
    QTextStream in(&file);
    for (int i = 0; i < 10 && !in.atEnd(); ++i) res += in.readLine() + "\n";
    return res;
}

QString TiwutEngine::doBase64(const QStringList& args) {
    if (args.size() < 2) return "Usage: base64 -e/-d <string>\n";
    if (args[0] == "-e") return args[1].toUtf8().toBase64() + "\n";
    if (args[0] == "-d") return QByteArray::fromBase64(args[1].toUtf8()) + "\n";
    return "Error: Invalid flag.\n";
}

QString TiwutEngine::doHash(const QStringList& args) {
    if (args.size() < 2) return "Usage: hash <md5|sha1|sha256> <string>\n";
    QCryptographicHash::Algorithm algo = QCryptographicHash::Md5;
    if (args[0] == "sha1") algo = QCryptographicHash::Sha1;
    else if (args[0] == "sha256") algo = QCryptographicHash::Sha256;
    return QCryptographicHash::hash(args[1].toUtf8(), algo).toHex() + "\n";
}

QString TiwutEngine::doSysInfo(const QStringList&) {
    return "OS: " + QSysInfo::prettyProductName() + "\n" +
           "CPU Arch: " + QSysInfo::currentCpuArchitecture() + "\n" +
           "Host: " + QHostInfo::localHostName() + "\n";
}