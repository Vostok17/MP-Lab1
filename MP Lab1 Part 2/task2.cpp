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
		wordPages[i] = new int[MaxPages];
		goto init_word_pages;
	}
	
	string word;

	ifstream fin;
	fin.open("input.txt");

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