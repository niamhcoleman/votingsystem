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

void getStatus(int totalCands, string canTable[5][2], BallotPaper ballot1)
{
	cout << "--CURRENT STANDINGS--" << endl;
	for (int x = 0; x < totalCands; x++)
	{
		if (ballot1.getPreference(x) == -1)
		{
			cout << "Candidate: " << canTable[x][0] << ". Status: Eliminated." << endl << endl;
		}
		else
		{
			cout << "Candidate: " << canTable[x][0] << ". Status: " << ballot1.getPreference(x) << " votes." << endl << endl;
		}
	}
}


void VoteCounter(int voteTable[5][5], string canTable[5][2], BallotPaper ballot1, int numCands)
{
	int x;
	int totalCands = numCands;
	int small = 5;
	int indexOfSmall;
	int indexOfNext;
	bool compare = false, check = false; //need to compare two vote totals
	srand(time(NULL));
	cout << "*****START OF VOTE COUNTING******" << endl << endl;

	getStatus(totalCands, canTable, ballot1);

	//set zeroes to -1/ (candidates eliminated)
	for (x = 0; x < totalCands; x++)
	{
		if (ballot1.getPreference(x) == 0)
		{
			cout << "Candidate: " << canTable[x][0] << " has been eliminated. " << endl;
			cout << "*********************************************" << endl;

			ballot1.setPreference(x, -1);
			numCands--;
			getStatus(totalCands, canTable, ballot1);

		}
	}
	//run until there is only one candidate left (the winner)
	for (int j = 0; j < numCands - 1; j++)
	{
		compare = false;
		check = false;

		//get smallest in array
		for (x = 0; x < totalCands; x++)
		{
			if (!check && (ballot1.getPreference(x) != -1))
			{
				small = ballot1.getPreference(x);
				indexOfSmall = x;
				check = true;
			}

			else if (ballot1.getPreference(x) < small && ballot1.getPreference(x) != -1)
			{
				small = ballot1.getPreference(x);
				indexOfSmall = x;
				compare = false;
			}
			else if (ballot1.getPreference(x) == small && ballot1.getPreference(x) != -1)
			{
				indexOfNext = x;
				compare = true;
			}
		}

		//if there is a matching smallest: choose one at random, redistribute votes and delete
		int choose;
		if (compare == true)
		{
			cout << "Elimination is between " << canTable[indexOfSmall][0] << " and " << canTable[indexOfNext][0] << endl;

			if (rand() % 2)
			{
				choose = indexOfSmall;
			}
			else
			{
				choose = indexOfNext;
			}
			ballot1.setPreference(choose, -1);

			//REDISTRIBUTE
			for (int r = 0; r < totalCands; r++)
			{
				if (voteTable[r][choose] == 2) //second choice
				{

					if (ballot1.getPreference(r) >= 0)
					{
						ballot1.setPreference(r, ballot1.getPreference(r) + 1);
					}
				}
			}


			cout << canTable[choose][0] << " has been eliminated, and his votes redistributed." << endl;
			cout << "*********************************************" << endl;

			getStatus(totalCands, canTable, ballot1);
		}

		else //if there is no two matching votes to choose from, find next smallest and compare.
		{
			ballot1.setPreference(indexOfSmall, -1);
			//REDISTRIBUTE 
			for (int r = 0; r < totalCands; r++)
			{
				if (voteTable[r][indexOfSmall] == 2) //second choice
				{

					if (ballot1.getPreference(r) >= 0)
					{
						ballot1.setPreference(r, ballot1.getPreference(r) + 1);
					}
				}
			}

			cout << canTable[indexOfSmall][0] << " has been eliminated, and his votes redistributed." << endl;
			cout << "*********************************************" << endl;

			getStatus(totalCands, canTable, ballot1);
		}
	}
	//at this point there should be only one non -1 value in the array

	for (int z = 0; z < totalCands; z++)
	{
		if (ballot1.getPreference(z) != -1)
		{
			cout << "*********************************************" << endl;
			cout << "Election winner: " << canTable[z][0] << "." << endl;;
		}


	}
}

int main()
{
	string canTable[5][2];
	int voteTable[5][5];

	ifstream candidates("candidates.txt");
	string position;
	int x = 0, y = 0;
	int i = 0, j = 0;
	int count = 1;
	int num;
	int vote, voteCount = 0;
	bool start = false;
	int numCands = 5;

	//Reads Candidates and Party to array
	if (candidates.is_open())
	{
		while (getline(candidates, position))
		{
			//Populate candidate names.
			if (count <= numCands)
			{
				if (x < numCands)
				{
					canTable[i][0] = position;
					i++;
				}
			}

			//Populate party names
			if (count >= numCands)
			{
				if ((x >= numCands) && (x < 10))
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

			for (y = numCands - 1; y >= 0; y--)
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

	BallotPaper ballot1(0, 0);

	for (x = 0; x < numCands; x++)
	{
		voteCount = 0;
		for (y = 0; y < numCands; y++)
		{
			if (voteTable[x][y] == 1)
			{
				voteCount++;
			}
		}
		ballot1.setPreference(x, voteCount);
	}

	Candidate cand1(canTable[0][0], canTable[0][1]);
	Candidate cand2(canTable[1][0], canTable[1][1]);
	Candidate cand3(canTable[2][0], canTable[2][1]);
	Candidate cand4(canTable[3][0], canTable[3][1]);
	Candidate cand5(canTable[4][0], canTable[4][1]);

	cout << endl << endl;

	VoteCounter(voteTable, canTable, ballot1, numCands);
	system("pause");
	return 0;
}