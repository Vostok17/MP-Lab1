#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    const int StringsInPage = 45;

    const int StopWordsCtn = 12;
    string stopWords[] = { "at", "for", "the", "in", "before", "on", "so", "a", "than", "to", "with", "by" };

    const int MaxWords = 10000;
    int* wordEntries = new int[MaxWords] {};
    string* words = new string[MaxWords];

    int** wordPages = new int* [MaxWords];
    const int MaxPages = 100;

    int i = 0;
init_word_pages:
    if (i < MaxWords)
    {
        wordPages[i] = new int[MaxPages] {};
        i++;
        goto init_word_pages;
    }

    string word;

    ifstream fin;
    fin.open("input.txt");

    int wordsCtn = 0,
        wordIdx = 0,
        currStr = 0;
loop_input:
    if (fin >> word)
    {
    loop_empty_lines:
        if (fin.peek() == '\n')
        {
            fin.get();
            currStr++;
            goto loop_empty_lines;
        }

        bool isNewWord = true;

        string fixedWord = "";
        int i = 0;
    process_word:
        if (word[i] != '\0')
        {
            char c = word[i];
            if ((c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z') ||
                (c == '-'))
            {
                if (c >= 'A' && c <= 'Z')
                {
                    c += 'a' - 'A';
                }
                fixedWord += c;
            }
            i++;
            goto process_word;
        }
        word = fixedWord;

        if (word == "" or word == "-")
        {
            goto loop_input;
        }

        i = 0;
    check_stop_words:
        if (i < StopWordsCtn)
        {
            if (stopWords[i] == word)
            {
                goto loop_input;
            }
            i++;
            goto check_stop_words;
        }

        i = 0;
    check_word_Entries:
        if (i < wordsCtn)
        {
            if (words[i] == word)
            {
                wordEntries[i]++;
                isNewWord = false;
                wordIdx = i;
            }
            i++;
            goto check_word_Entries;
        }
        if (isNewWord)
        {
            wordIdx = wordsCtn;
            words[wordIdx] = word;
            wordEntries[wordIdx] = 1;
            wordsCtn++;
        }
        if (wordEntries[wordIdx] <= 100)
        {
            int pageIdx = wordEntries[wordIdx] - 1,
                pageNum = currStr / StringsInPage + 1;
            wordPages[wordIdx][pageIdx] = pageNum;
        }
        goto loop_input;
    }
    fin.close();

    string* newWords = new string[wordsCtn];
    int* newWordEntries = new int[wordsCtn];
    int** newWordPages = new int* [wordsCtn];
    int ptr = 0;

    i = 0;
rewrite_words:
    if (i < wordsCtn)
    {
        if (wordEntries[i] <= 100)
        {
            newWords[ptr] = words[i];
            newWordEntries[ptr] = wordEntries[i];
            newWordPages[ptr] = wordPages[i];
            ptr++;
        }
        else
        {
            delete[] wordPages[i];
        }
        i++;
        goto rewrite_words;
    }
    i = wordsCtn;
free_memory:
    if (i < MaxWords)
    {
        delete[] wordPages[i];
        i++;
        goto free_memory;
    }
    delete[] words, wordEntries, wordPages;

    words = newWords;
    wordEntries = newWordEntries;
    wordPages = newWordPages;
    wordsCtn = ptr;

    i = 1;
loop_i:
    if (i < wordsCtn)
    {
        int j = i;
    loop_j:
        if (j > 0)
        {
            int k = 0;
        loop_char:
            if (words[j][k] != '\0' && 
                words[j - 1][k] != '\0' && 
                words[j][k] == words[j - 1][k])
            {
                k++;
                goto loop_char;
            }
            if (words[j - 1][k] > words[j][k] || words[j][k] == '\0')
            {
                int temp = wordEntries[j];
                wordEntries[j] = wordEntries[j - 1];
                wordEntries[j - 1] = temp;

                string tempWord = words[j];
                words[j] = words[j - 1];
                words[j - 1] = tempWord;

                int* tempWordPages = wordPages[j];
                wordPages[j] = wordPages[j - 1];
                wordPages[j - 1] = tempWordPages;
            }
            j--;
            goto loop_j;
        }
        i++;
        goto loop_i;
    }

    ofstream fout;
    fout.open("output.txt");

    i = 0;
loop_output:
    if (i < wordsCtn)
    {
        if (wordEntries[i] <= 100)
        {
            fout << words[i] << " - ";

            int j = 0;
            fout << wordPages[i][j];
            j++;
        loop_pages:
            if (j < wordEntries[i])
            {
                fout << ", " << wordPages[i][j];
                j++;
                goto loop_pages;
            }
            fout << endl;
        }
        i++;
        goto loop_output;
    }
    fout.close();

    i = 0;
delete_word_pages:
    if (i < wordsCtn)
    {
        delete[] wordPages[i];
        i++;
        goto delete_word_pages;
    }
    delete[] wordEntries, words, wordPages;

    return 0;
}