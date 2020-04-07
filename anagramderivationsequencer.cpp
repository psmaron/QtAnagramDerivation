#include "anagramderivationsequencer.h"

#include <QDebug>
#include <QCoreApplication>

/*!
 * \brief AnagramDerivationSequencer::findDerivationsFor
 * \param initialWord
 *
 * Saves specific 3-letters word, that the solution will be build for, together with its sorted equivalent
 */
void AnagramDerivationSequencer::findDerivationsFor(QString initialWord) {
    this->initialWord = initialWord;
    sortedInitialWord = initialWord;
    qSort(sortedInitialWord.begin(), sortedInitialWord.end());
}

/*!
 * \brief AnagramDerivationSequencer::loadWordsFrom
 * \param wordsDatabasePath
 *
 * Reads word by word from given file:
 * - skips words shorter than 3 chars
 * - skips words that cannot make to final solution
 * Finally loads filtered out words to a structure that:
 * - first groups words by length (in hash container for quick lookup and insertion - lenght is a key)
 * - stores each group in a vector fo strings
 * - each word is stored with its sorted counterpart
 */
void AnagramDerivationSequencer::loadWordsFrom(QString wordsDatabasePath) {
    QFile file(qApp->applicationDirPath() + "/" + wordsDatabasePath);
    assert(file.open(QIODevice::ReadOnly | QIODevice::Text));

    while (!file.atEnd()) {
        QString readWord = file.readLine();
        if (readWord.back() == '\n')
            readWord.remove(readWord.length()-1, 1);
        if (readWord.length() < 3)
            continue;
        maxWordLength = maxWordLength < readWord.length() ? readWord.length() : maxWordLength;
        QString sortedReadWord = readWord;
        qSort(sortedReadWord.begin(), sortedReadWord.end());
        if (shouldFilterWord(sortedReadWord))
            continue;
        inputWords[readWord.length()].push_back(qMakePair(readWord, sortedReadWord));
    }

    file.close();
}

bool AnagramDerivationSequencer::shouldFilterWord(QString wordToCheck) const {
    return !this->initialWord.isEmpty() && !containsWord(wordToCheck, sortedInitialWord);
}

/*!
 * \brief AnagramDerivationSequencer::getSequences
 * \return
 *
 * Starting with the longest words, for each one recursively tries to match derivation anagram within a group
 * of words shorter by 1. Remembers sequence of anagram derivations, pushing back next elems onto vector
 * Stops when all words for given group were checked and any sequence was found - otherwise starts
 * to search in group of words shorter by 1 etc. until only words with 3 letters remain.
 *
 * Side Note:
 * Alogorithm uses dynamic programming technique and it's runtime performance could be improved over existing
 * solution by introducing memorization to skip solving overlapping subproblems. But this comes with a cost
 * of greater memory consumption, so IMHO such decision should depend on input data set characteristics:
 * in order to save memory, as requirement states file with words is big, I assume there are not many
 * anagram derivations sequences having common path
 * Likewise memory complexity could be improved by replacing recursion with iterative approach
 */
QVector<QVector<QString>> AnagramDerivationSequencer::getSequences() const {
    for (int wordLength = maxWordLength; wordLength > 3; wordLength--) {
        for (auto& wordPair : inputWords[wordLength]) {
            QVector<QString> sequence;
            sequence.push_back(wordPair.first);
            scan(sequence, wordPair.second, wordLength-1);
        }

        if (results.size() != 0)
            break;
    }

    return results;
}

/*!
 * \brief AnagramDerivationSequencer::scan
 * \param sequence
 * \param sorted
 * \param len
 *
 * Recursively dives into words structure loaded from file, at each level trying to match
 * derivation anagram. Uses vector as a stack to save current anagram derivations sequence
 */
void AnagramDerivationSequencer::scan(QVector<QString> sequence, QString sorted, int len) const {
    if (len == 2) {
        results.push_back(sequence);
        return;
    }
    for (auto& wordPair : inputWords[len]) {
        if (areDerivations(wordPair.second, sorted)) {
            sequence.push_back(wordPair.first);
            scan(sequence, wordPair.second, len-1);
            sequence.pop_back();
        }
    }
}

/*!
 * \brief AnagramDerivationSequencer::containsWord
 * \param text
 * \param searchedWord
 * \return
 *
 * Checks whether characters in searchedWord can be found within given text
 * Only checks if characters are in original order, not if are present one after another
 *
 * Example: text    searchedWord    result
 *          abcde   ace             true
 *          abcde   acf             false
 */
bool AnagramDerivationSequencer::containsWord(QString text, QString searchedWord) const {
    int searchedWordIter = 0, textIter = 0;
    while (textIter < text.length()) {
        if (searchedWord[searchedWordIter] == text[textIter]) {
            searchedWordIter++;
            textIter++;
            if (searchedWordIter == searchedWord.length())
                return true;
        }
        else {
            textIter++;
        }
    }
    return false;
}

bool AnagramDerivationSequencer::areDerivations(QString shorter, QString longer) const {
    if (abs(shorter.length()-longer.length()) != 1)
        return false;

    return containsWord(longer, shorter);
}
