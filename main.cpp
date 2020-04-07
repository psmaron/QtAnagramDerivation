#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include "commandlinearguments.h"
#include "anagramderivationsequencer.h"

void printAnagramDerivationSequences(QVector<QVector<QString>> sequences);
QVector<QVector<QString>> computeSequencesFor(QString wordsDatabasePath, QString initialWord);

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("AnagramDerivation");
    QCoreApplication::setApplicationVersion("1.0");

    QCoreApplication app(argc, argv);
    CommandLineArguments commandLineArgs(app, argc, argv);

    QVector<QVector<QString>> sequences =
            computeSequencesFor(commandLineArgs.getWordsDatabasePath(), commandLineArgs.getInitialWord());
    printAnagramDerivationSequences(sequences);
}

QVector<QVector<QString>> computeSequencesFor(QString wordsDatabasePath, QString initialWord) {
    AnagramDerivationSequencer sequencer;
    sequencer.findDerivationsFor(initialWord);
    sequencer.loadWordsFrom(wordsDatabasePath);
    return sequencer.getSequences();
}

void printAnagramDerivationSequences(QVector<QVector<QString>> sequences) {
    for (const auto& sequence : sequences) {
        for (auto rIter = sequence.rbegin(); rIter != sequence.rend(); rIter++) {
            std::cout << rIter->toStdString();
            if ((rIter+1) != sequence.rend())
                std::cout << " -> ";
        }
        std::cout << std::endl;
    }
}
