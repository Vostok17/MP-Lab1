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
		goto init_word_pages;
	}
	
	string word;

	ifstream fin;
	fin.open("input.txt");

	int wordsCtn = 0;
	int currStr = 0;
loop_input:
	if (fin >> word)
	{
		wordsCtn++;
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

		i = 0;
	check_word:
		if (i < wordsCtn)
		{
			int j = 0;
		check_stop_words:
			if (j < StopWordsCtn)
			{
				if (stopWords[j] == word)
				{
					wordsCtn--;
					goto loop_input;
				}
				j++;
				goto check_stop_words;
			}

			if (words[i] == word)
			{
				wordRepeats[i]++;
				isNewWord = false;
			}

			i++;
			goto check_word;
		}
		if (isNewWord)
		{
			int idx = wordsCtn - 1;
			words[idx] = word;
			wordRepeats[idx] = 1;
		}
		else
		{
			wordsCtn--;
		}
		goto loop_input;
	}
	fin.close();

	ofstream fout;
	fout.open("output.txt");

	fout.close();



	i = 0;
init_word_pages:
	if (i < MaxWords)
	{
		delete[] wordPages[i];
		goto init_word_pages;
	}
	delete[] wordRepeats, words, wordPages;

	system("pause");
	return 0;
}