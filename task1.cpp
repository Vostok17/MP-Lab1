#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	const int DisplayedWordsCtn = 25;

	const int StopWordsCtn = 12;
	string stopWords[] = { "at", "for", "the", "in", "before", "on", "so", "a", "than", "to", "with", "by" };

	const int MaxWords = 10000;
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

	// Insertion sort
	int i = 1;
loop_i:
	if (i < wordsCtn)
	{
		int j = i;
	loop_j:
		if (j > 0 && wordRepeats[j - 1] < wordRepeats[j])
		{
			int temp = wordRepeats[j];
			wordRepeats[j] = wordRepeats[j - 1];
			wordRepeats[j - 1] = temp;

			string tempWord = words[j];
			words[j] = words[j - 1];
			words[j - 1] = tempWord;

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
	if (i < DisplayedWordsCtn)
	{
		if (i < wordsCtn)
		{
			fout << words[i] << " - " << wordRepeats[i] << endl;
			i++;
			goto loop_output;
		}	
	}
	fout.close();

	delete[] wordRepeats, words;

	system("pause");
	return 0;
}