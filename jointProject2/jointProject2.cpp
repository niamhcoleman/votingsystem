#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Candidate {
public:
	void setName(string name);
	string getName(void);

	void setParty(string party);
	string getParty(void);

	Candidate(string name, string party); //calls constructor


private:
	string canName;
	string canParty;
};

Candidate::Candidate(string name, string party)
{
	cout << name << " of the " << party << " created." << endl;
	canName = name;
	canParty = party;
}

void Candidate::setName(string name)
{
	cout << "\nCandidate " << canName << " changed to " << name;
	canName = name;
}

void Candidate::setParty(string party)
{
	cout << "\nCandidate " << canName << " changed from " << canParty << " to " << party;
	canParty = party;
}

string Candidate::getName(void)
{
	return canName;
}

string Candidate::getParty(void)
{
	return canParty;
}

class BallotPaper
{
public:
	int totalVote[4][1];
	int totalInput = 0;
	int totalIndex = 0;
	void setPreference(int index, int input);
	int getPreference(int index);

	BallotPaper(int index, int input);
};

BallotPaper::BallotPaper(int index, int input)
{
	totalIndex = index;
	totalInput = input;
	totalVote[totalIndex][0] = totalInput;
}

void BallotPaper::setPreference(int index, int input)
{
	totalVote[index][0] = input;
}

int BallotPaper::getPreference(int index)
{
	int value = totalVote[index][0];
	return value;
}

int random(int a, int b)
{
	srand(time(NULL));

	int r = rand() % 2;
	if (r == 0)
		return a;
	else
		return b;
}
int largest(BallotPaper ballot1, int totalCands)
{
	int temp = ballot1.getPreference(0);

	for (int i = 0; i < totalCands; i++)
	{
		if (ballot1.getPreference(i) > temp)
		{
			temp = ballot1.getPreference(i);
		}

	}
		return temp;
}

int nextLargest(BallotPaper ballot1, int totalCands, int largest, int comp)
{
	int nextLargest;
	for (int i = 0; i < totalCands; i++)
	{
		if (ballot1.getPreference(i) > comp && ballot1.getPreference(i) <= largest)
		{
			nextLargest = i;
		}
	}
	return nextLargest;
}
void VoteCounter(int voteTable[4][4], string canTable[4][2], BallotPaper ballot1, int numCands)
{
	int x;
	int totalCands = numCands;
	int small = 5;
	int indexOfSmall;
	int indexOfNext;
	bool compare = false; //need to compare two vote totals
	int comp;
	cout << "*****START OF VOTE COUNTING******" << endl << endl;

	//set zeroes to -1/Candidates Eliminated
	for (x = 0; x < totalCands; x++)
	{
		if (ballot1.getPreference(x) == 0)
		{
			cout << "Candidate: " << canTable[x][0] << " has been eliminated." << endl << "************" << endl;
			ballot1.setPreference(x, -1);
			numCands--;
		}
	}
	//run until there is only one candidate left (the winner)
	while (numCands > 1)
	{
		compare = false;
		//get smallest in array
		for (x = 0; x < totalCands; x++)
		{
			if ((ballot1.getPreference(x) < small) && (ballot1.getPreference(x) != -1))
			{
				small = ballot1.getPreference(x);
				indexOfSmall = x; 
			}
		}
		//search for another matching smallest
		for (int i = 0; i < totalCands; i++)
		{
			if ((ballot1.getPreference(i) == small) && (i != indexOfSmall))
			{
				indexOfNext = i;
				compare = true;
			}
			
		}
		//if there is a matching smallest: choose one at random, redistribute votes and delete
		
		if (compare == true)
		{			
			cout << "Elimination is between " << canTable[indexOfSmall][0] << " and " << canTable[indexOfNext][0] << endl << "*******" << endl;
			int choose = random(indexOfSmall, indexOfNext);
			ballot1.setPreference(choose, -1);
			numCands--;
			cout << canTable[choose][0] << " has been eliminated!" << endl << "*******" << endl;;
			
			if (choose == indexOfSmall)
			{
				comp = indexOfNext;
			}
			else
			{
				comp = indexOfSmall;
			}
		}
		
		else//if there is no two matching votes to choose from, find next smallest and compare.
		{
			comp = small;
			int temp = largest(ballot1, totalCands);
			int next = nextLargest(ballot1, totalCands, temp, comp);
			cout << "Elimination is between " << canTable[next][0] << " and " << canTable[indexOfSmall][0] << endl << "*******" << endl;;
			ballot1.setPreference(next, -1);
			numCands--;

		}
	}
	//at this point there should be only one non -1 value in the array
	
	for (int z = 0; z < totalCands; z++)
	{
		if (ballot1.getPreference(z) != -1)
		{
			cout << "aaaaand the winner is.... " << canTable[z][0] << "!" << endl;;
		}
		
	}
}

int main()
{
	string canTable[4][2];
	int voteTable[4][4];

	ifstream candidates("candidates.txt");
	string position;
	int x = 0, y = 0;
	int i = 0, j = 0;
	int count = 1;
	int num;
	int vote, voteCount = 0;
	bool start = false;
	int numCands = 4;

	//Reads Candidates and Party to array
	if (candidates.is_open())
	{
		while (getline(candidates, position))
		{
			//Populate candidate names.
			if (count <= 4)
			{
				if (x < 4)
				{
					canTable[i][0] = position;
					i++;
				}
			}

			//Populate party names
			if (count >= 5)
			{
				if ((x >= 4) && (x < 8))
				{
					canTable[j][1] = position;
					j++;
				}
			}

			count++;
			x++;
		}
	}

	ifstream votes("candidates.txt");
	//Populate votes array
	count = 0;
	while (getline(votes, position))
	{
		if (start == true)
		{
			int num = stoi(position);

			for (y = 3; y >= 0; y--)
			{
				vote = num % 10;
				num = num / 10;
				voteTable[count][y] = vote;
			}
			count++;
		}

		else if (position == ".")
		{
			start = true;
		}
	}
	//Counting number of 1's and adding to totalTable
	BallotPaper ballot1(0, 0);

	for (x = 0; x < 4; x++)
	{
		voteCount = 0;
		for (y = 0; y < 4; y++)
		{
			if (voteTable[x][y] == 1)
			{
				voteCount++;
			}
		}
		ballot1.setPreference(x, voteCount);
		cout << ballot1.getPreference(x) << endl << endl;
	}

	//Declare Candidates
	Candidate cand1(canTable[0][0], canTable[0][1]);
	Candidate cand2(canTable[1][0], canTable[1][1]);
	Candidate cand3(canTable[2][0], canTable[2][1]);
	Candidate cand4(canTable[3][0], canTable[3][1]);

	/*	//Display Candidates
	cout << endl << endl;
	cout << "Candidate 1: " << cand1.getName() << endl << "Party: " << cand1.getParty() << endl << endl;
	cout << "Candidate 2: " << cand2.getName() << endl << "Party: " << cand2.getParty() << endl << endl;
	cout << "Candidate 3: " << cand3.getName() << endl << "Party: " << cand3.getParty() << endl << endl;
	cout << "Candidate 4: " << cand4.getName() << endl << "Party: " << cand4.getParty() << endl << endl;
	*/
	//Candidate setName and setParty

	VoteCounter(voteTable, canTable, ballot1, numCands);
	system("pause");
	return 0;
}