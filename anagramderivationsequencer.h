#ifndef ANAGRAMDERIVATIONSEQUENCER_H
#define ANAGRAMDERIVATIONSEQUENCER_H

#include <QFile>
#include <QHash>
#include <QVector>
#include <QPair>
#include <QtAlgorithms>

class AnagramDerivationSequencer
{
public:
    void findDerivationsFor(QString initialWord);
    void loadWordsFrom(QString wordsDatabasePath);
    QVector<QVector<QString>> getSequences() const;

private:
    void scan(QVector<QString> sequence, QString sorted, int len) const;
    bool containsWord(QString text, QString searchedWord) const;
    bool areDerivations(QString shorter, QString longer) const;
    bool shouldFilterWord(QString wordToCheck) const;

    QString initialWord;
    QString sortedInitialWord;
    int maxWordLength = 0;
    QHash<int, QVector<QPair<QString, QString>>> inputWords;
    mutable QVector<QVector<QString>> results;
};

#endif // ANAGRAMDERIVATIONSEQUENCER_H
