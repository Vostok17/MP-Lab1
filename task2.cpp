#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    const int StringsInPage = 45;

    const int StopWordsCtn = 12;
    string stopWords[] = { "at", "for", "the", "in", "before", "on", "so", "a", "than", "to", "with", "by" };

    const int MaxEntries = 100;

    int totalRecords = 1000;

    struct Record
    {
        string word = "";
        int count = 0;
        int* pages = new int[MaxEntries];
    };

    Record* records = new Record[totalRecords];

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
            if (records[i].word == word)
            {
                if (++records[i].count == MaxEntries + 1)
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
            wordsCtn++;
            wordIdx = wordsCtn - 1;

            records[wordIdx].word = word;
            records[wordIdx].count = 1;
        }
        if (wordsCtn == totalRecords)
        {
            totalRecords *= 2;
            Record* newRecords = new Record[totalRecords];

            i = 0;
        copy_records:
            if (i < wordsCtn)
            {
                newRecords[i] = records[i];
                i++;
                goto copy_records;
            }
            delete[] records;

            records = newRecords;
        }
        if (records[wordIdx].count <= 100)
        {
            int pageIdx = records[wordIdx].count - 1,
                pageNum = currStr / StringsInPage + 1;
            records[wordIdx].pages[pageIdx] = pageNum;
        }
        goto loop_input;
    }
    fin.close();

    int size = wordsCtn - cutWordsCtn;

    Record* newRecords = new Record[size];

    int ptr = 0,
        i = 0;
cut_words:
    if (i < wordsCtn)
    {
        if (records[i].count <= 100)
        {
            newRecords[ptr] = records[i];
            ptr++;
        }
        else
        {
            delete[] records[i].pages;
        }
        i++;
        goto cut_words;
    }

    i = wordsCtn;
free_memory:
    if (i < totalRecords)
    {
        delete[] records[i].pages;
        i++;
        goto free_memory;
    }
    
    delete[] records;
    records = newRecords;
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
            if (records[j].word[k] != '\0' &&
                records[j - 1].word[k] != '\0' &&
                records[j].word[k] == records[j - 1].word[k])
            {
                k++;
                goto loop_char;
            }
            if (records[j - 1].word[k] > records[j].word[k] || records[j].word[k] == '\0')
            {
                Record temp = records[j];
                records[j] = records[j - 1];
                records[j - 1] = temp;
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
        fout << records[i].word << " - ";

        int j = 0;
        fout << records[i].pages[j];
        j++;
    loop_pages:
        if (j < records[i].count)
        {
            fout << ", " << records[i].pages[j];
            j++;
            goto loop_pages;
        }
        fout << endl;

        i++;
        goto loop_output;
    }
    fout.close();

    i = 0;
clean_memory:
    if (i < wordsCtn)
    {
        delete[] records[i].pages;
        i++;
        goto clean_memory;
    }
    delete[] records;

    return 0;
}