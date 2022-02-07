#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	const int StopWordsCtn = 12;
	string stopWords[] = { "at", "for", "the", "in", "before", "on", "so", "a", "than", "to", "with", "by" };

	const int MaxWords = 10000;
	int* wordRepeats = new int[MaxWords] {};
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
		cutWordsCtn = 0,
		currString = 0;
loop_input:
	if (fin >> word)
	{
		if (fin.get() == '\n')
		{
			currString++;
		}

		bool isNewWord = true;

		int i = 0;
		string fixedWord = "";
	process_word:
		if (word[i] != '\0')
		{
			char c = word[i];
			if ((c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c == '-')) // allowed chars
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

		if (word == "")
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
	check_word_repeats:
		if (i < wordsCtn)
		{
			if (words[i] == word)
			{
				wordRepeats[i]++;
				if (wordRepeats[i] > 100)
				{
					words[i] = "";
					cutWordsCtn++;
				}
				isNewWord = false;
			}

			i++;
			goto check_word_repeats;
		}
		if (isNewWord)
		{
			words[wordsCtn] = word;
			wordRepeats[wordsCtn] = 1;
			wordsCtn++;
		}

		int wordIdx = wordsCtn - 1,
			pageIdx = wordRepeats[wordIdx] - 1;
		int pageNum = currString / 45 + 1;
		wordPages[wordIdx][pageIdx] = pageNum;

		goto loop_input;
	}
	fin.close();
	cout << "done" << endl;

	// TODO: cut off words that enter more than 100 times

	// write to file
	ofstream fout;
	fout.open("output.txt");

	i = 0;
loop_output:
	if (i < wordsCtn)
	{
		fout << words[i] << " - ";

		int j = 0;
		fout << wordPages[i][j];
		j++;
	loop_pages:
		if (wordPages[i][j] != 0)
		{
			fout << ", " << wordPages[i][j];
			j++;
			goto loop_pages;
		}
		fout << endl;
		i++;
		goto loop_output;
	}
	fout.close();

	// free memory
	i = 0;
delete_word_pages:
	if (i < MaxWords)
	{
		delete[] wordPages[i];
		i++;
		goto delete_word_pages;
	}
	delete[] wordRepeats, words, wordPages;

	system("pause");
	return 0;
}