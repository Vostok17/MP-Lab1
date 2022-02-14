#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    const int DisplayedWordsCtn = 25;

    const int StopWordsCtn = 12;
    string stopWords[] = { "at", "for", "the", "in", "before", "on", "so", "a", "than", "to", "with", "by" };

    int totalRecords = 100;

    struct Record
    {
        string word = "";
        int count = 0;
    };

    Record* records = new Record[totalRecords];

    int wordsCtn = 0;
    string word;

    ifstream fin;
    fin.open("input.txt");

loop_input:
    if (fin >> word)
    {
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
    check_word_entries:
        if (i < wordsCtn && isNewWord)
        {
            if (records[i].word == word)
            {
                records[i].count++;
                isNewWord = false;
            }
            i++;
            goto check_word_entries;
        }
        if (isNewWord)
        {
            records[wordsCtn].word = word;
            records[wordsCtn].count = 1;
            wordsCtn++;
        }
        goto loop_input;
    }
    fin.close();

    int i = 1;
loop_i:
    if (i < wordsCtn)
    {
        int j = i;
    loop_j:
        if (j > 0 && records[j - 1].count < records[j].count)
        {
            Record temp = records[j];
            records[j] = records[j - 1];
            records[j - 1] = temp;

            j--;
            goto loop_j;
        }
        i++;
        goto loop_i;
    }

    ofstream fout;;
    fout.open("output.txt");

    i = 0;
loop_output:
    if (i < DisplayedWordsCtn && i < wordsCtn)
    {
        fout << records[i].word << " - " << records[i].count << endl;
        i++;
        goto loop_output;
    }
    fout.close();

    delete[] records;

    return 0;
}