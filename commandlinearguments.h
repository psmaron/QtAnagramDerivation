#ifndef COMMANDLINEPARAMETERS_H
#define COMMANDLINEPARAMETERS_H

#include <QString>
#include <QCommandLineParser>

class CommandLineArguments
{
public:
    CommandLineArguments(QCoreApplication &app, int argc, char *argv[]);

    const QString getWordsDatabasePath() const;
    const QString getInitialWord() const;

private:
    void setupParser();
    void parseCommandLineArgs(QCoreApplication& app, int argc, char *argv[]);

    QCommandLineParser parser;
    QString wordsDatabasePath;
    QString initialWord;
    QString wordsDatabasePathParameterName = "path";
    QString initialWordParameterName = "word";
};

#endif // COMMANDLINEPARAMETERS_H
