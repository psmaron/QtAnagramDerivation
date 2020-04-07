#include "commandlinearguments.h"

#include <iostream>
#include <QCoreApplication>

CommandLineArguments::CommandLineArguments(QCoreApplication& app, int argc, char *argv[]) {
    setupParser();
    parseCommandLineArgs(app, argc, argv);
}

void CommandLineArguments::setupParser() {
    parser.setApplicationDescription("Finds the longest anagram derivation from specified 3-letter word");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption initialWordOption(QStringList() << initialWordParameterName.left(1) << initialWordParameterName,
        QCoreApplication::translate("main", "Prints sequence for this <word>"),
        QCoreApplication::translate("main", "word"));
    parser.addOption(initialWordOption);

    QCommandLineOption wordsDatabasePathOption(QStringList() << wordsDatabasePathParameterName.left(1) << wordsDatabasePathParameterName,
        QCoreApplication::translate("main", "Use words loaded from <file>. Use path relative to .exe and / to separate dirs"),
        QCoreApplication::translate("main", "file"));
    parser.addOption(wordsDatabasePathOption);
}

void CommandLineArguments::parseCommandLineArgs(QCoreApplication& app, int argc, char *argv[]) {
    parser.process(app);

    wordsDatabasePath = parser.value(wordsDatabasePathParameterName);
    initialWord = parser.value(initialWordParameterName);

    if (wordsDatabasePath.isEmpty()) {
        std::cerr << "No path to file with words provided, exiting...\n";
        exit(0);
    }
    if (initialWord.isEmpty()) {
        std::cout << "No initial word provided, checking all sequences!\n";
    }
}

const QString CommandLineArguments::getInitialWord() const {
    return initialWord;
}

const QString CommandLineArguments::getWordsDatabasePath() const {
    return wordsDatabasePath;
}
