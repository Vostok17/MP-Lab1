#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    const int StringsInPage = 45;

    const int StopWordsCtn = 12;
    string stopWords[] = { "at", "for", "the", "in", "before", "on", "so", "a", "than", "to", "with", "by" };

    int maxWords = 1000;
    int* wordEntries = new int[maxWords] {};
    string* words = new string[maxWords];

    int** wordPages = new int* [maxWords];
    const int MaxPages = 100;

    int i = 0;
init_word_pages:
    if (i < maxWords)
    {
        wordPages[i] = new int[MaxPages] {};
        i++;
        goto init_word_pages;
    }

    string word;

    ifstream fin;
    fin.open("input.txt");

    int wordsCtn = 0,
        cutWordsCtn = 0,
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

        if (word == "" || word == "-")
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
        if (i < wordsCtn && isNewWord)
        {
            if (words[i] == word)
            {
                if (++wordEntries[i] == 101)
                {
                    cutWordsCtn++;
                }
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
        if (wordsCtn == maxWords)
        {
            maxWords *= 2;
            string* newWords = new string[maxWords];
            int* newWordEntries = new int[maxWords];
            int** newWordPages = new int* [maxWords];

            i = 0;
        resize_words:
            if (i < wordsCtn)
            {
                newWords[i] = words[i];
                newWordEntries[i] = wordEntries[i];
                newWordPages[i] = wordPages[i];
                i++;
                goto resize_words;
            }
            delete[] words, wordEntries, wordPages;

            i = wordsCtn;
        init_second_part:
            if (i < maxWords)
            {
                newWordPages[i] = new int[MaxPages] {};
                i++;
                goto init_second_part;
            }

            words = newWords;
            wordEntries = newWordEntries;
            wordPages = newWordPages;
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

    int size = wordsCtn - cutWordsCtn;
    string* newWords = new string[size];
    int* newWordEntries = new int[size];
    int** newWordPages = new int* [size];
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
    if (i < maxWords)
    {
        delete[] wordPages[i];
        i++;
        goto free_memory;
    }
    delete[] words, wordEntries, wordPages;

    words = newWords;
    wordEntries = newWordEntries;
    wordPages = newWordPages;
    wordsCtn = size;

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

    cout << size << endl;
    system("pause");
    return 0;
}