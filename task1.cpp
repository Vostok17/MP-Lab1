#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	const int WordsToDisplayCtn = 25;
	string wordsToDisplay[WordsToDisplayCtn];

	const int StopWordsCtn = 12;
	string stopWords[] = { "at", "for", "the", "in", "before", "on", "so", "a", "than", "to", "with", "by" };

	const int MaxWords = 100;
	int* wordRepeats = new int[MaxWords] {};
	string* words = new string[MaxWords];

	string word;

	ifstream fin;
	fin.open("input.txt");

	int wordsCtn = 0;
loop_input:
	if (fin >> word)
	{
		wordsCtn++;
		bool isNewWord = true;
		int i = 0;
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

	ofstream fout;;
	fout.open("output.txt");

	int i = 0;
loop_output:
	if (i < wordsCtn)
	{
		fout << words[i] << " - " << wordRepeats[i] << endl;
		i++;
		goto loop_output;
	}
	fout.close();

	delete[] wordRepeats, words;

	system("pause");
	return 0;
}