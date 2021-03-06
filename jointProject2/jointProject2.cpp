#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

class Candidate {
public:
	void setName(string name);
	string getName(void);

	void setParty(string party);
	string getParty(void);

	Candidate(string name, string party);


private:
	string canName;
	string canParty;
};

Candidate::Candidate(string name, string party)
{
	cout << "Candidate: " << name << "   Party: " << party << " created." << endl;
	canName = name;
	canParty = party;
}

void Candidate::setName(string name)
{
	cout << canName;
	canName = name;
	cout << " changed her/his name to " << canName << endl;;
}

void Candidate::setParty(string party)
{
	cout << "\nCandidate " << canName << " changed from " << canParty << " to " << party << endl;;
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
	int totalVote[5][1];
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

void getStatus(int totalCands, vector<Candidate> canTable, BallotPaper ballot1)
{
	cout << "-----CURRENT STANDINGS-----" << endl;
	for (int x = 0; x < totalCands; x++)
	{
		if (ballot1.getPreference(x) == -1)
		{
			cout << "Candidate: " << canTable[x].getName() << ". Status: Eliminated." << endl << endl;
		}
		else
		{
			cout << "Candidate: " << canTable[x].getName() << ". Status: " << ballot1.getPreference(x) << " vote(s)." << endl << endl;
		}
	}
}

void VoteCounter(int voteTable[11][5], vector<Candidate> candVector, BallotPaper ballot1, int numCands)
{
	int totalCands = numCands;
	int small = totalCands + 1;
	int indexOfSmall, indexOfNext, x, n, choose;
	bool compare, check;
	srand(time(NULL));

	cout << "*********************************************" << endl << endl;

	getStatus(totalCands, candVector, ballot1);

	for (x = 0; x < totalCands; x++)
	{
		if (ballot1.getPreference(x) == 0)
		{
			cout << "Candidate: " << candVector[x].getName() << " has been eliminated. " << endl;
			cout << "*********************************************" << endl;

			ballot1.setPreference(x, -1);
			numCands--;
			getStatus(totalCands, candVector, ballot1);
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
				check = true; //check ensures this will only be ran once
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

		if (compare == true)
		{
			cout << "Elimination is between " << candVector[indexOfSmall].getName() << " and " << candVector[indexOfNext].getName() << endl;

			if (rand() % 2)
			{
				choose = indexOfSmall;
			}
			else
			{
				choose = indexOfNext;
			}
			ballot1.setPreference(choose, -1);
			cout << candVector[choose].getName() << " has been eliminated." << endl;



			//REDISTRIBUTE
			/*for (n = 0; n <= 11 ; n++)
			{
				if (voteTable[choose][n] == 1)
				{
					for (int x = 0; x <= totalCands; x++)
					{
						if (voteTable[x][n] == 2 && ballot1.getPreference(x) != -1)
						{
							ballot1.setPreference(x, ballot1.getPreference(x) + 1);
							cout << "A vote has been redistributed to " << candVector[x].getName() << endl;
						}
					}
				}
			}
			*/

			cout << "*********************************************" << endl;

			getStatus(totalCands, candVector, ballot1);
		}

		else
		{
			ballot1.setPreference(indexOfSmall, -1);
			cout << candVector[indexOfSmall].getName() << " has been eliminated." << endl;

			//REDISTRIBUTE
			/*for (n = 0; n <= 11 ; n++)
			{
				if (voteTable[indexOfSmall][n] == 1)
				{
					for (int x = 0; x <= totalCands; x++)
					{
						if (voteTable[x][n] == 2 && ballot1.getPreference(x) != -1)
						{
							ballot1.setPreference(x, ballot1.getPreference(x) + 1);
							cout << "A vote has been redistributed to " << candVector[x].getName() << endl;
						}
					}
				}
			}
			*/


			cout << "*********************************************" << endl;

			getStatus(totalCands, candVector, ballot1);
		}
	}
	//at this point there should be only one non -1 value in the array

	for (int z = 0; z < totalCands; z++)
	{
		if (ballot1.getPreference(z) != -1)
		{
			cout << "*********************************************" << endl;
			cout << "Election winner: " << candVector[z].getName() << "." << endl;;
		}

	}
}

int main()
{
	vector<Candidate> nominees;
	string position, name, party, token, delimiter = ",";
	int x = 0, voteCount = 0, pos = 0, vote;
	int row = 0, col = 0;
	int numCands = 5, numVoters = 11;
	bool start = false;
	int voteTable[11][5];

	BallotPaper ballot(0, 0);

	//populate array of candidates
	ifstream candidates("candidates.txt");
	if (candidates.is_open())
	{
		while (getline(candidates, position))
		{
			if (position == ".")
			{
				break;
			}
			if (x == 0)
			{
				name = position;
				x++;
			}
			else
			{
				party = position;
				x = 0;
				Candidate temp(name, party);
				nominees.push_back(temp);
			}

		}
	}

	ifstream votes("candidates.txt");
	while (getline(votes, position))
	{
		col = 0;
		voteCount = 0;
		if (start == true)
		{
			while ((pos = position.find(delimiter)) != string::npos) //end of string
			{
				token = position.substr(0, pos);
				vote = stoi(token);
				voteTable[row][col] = vote;
				if (voteTable[row][col] == 1)
				{
					voteCount++;
				}
				//cout << voteTable[row][col] << " ";
				position.erase(0, pos + delimiter.length());
				col++;

			}
			ballot.setPreference(row, voteCount);
			row++;
		}

		else if (position == ".")
		{
			start = true;
		}

	}

	VoteCounter(voteTable, nominees, ballot, numCands);

	system("pause");
	return 0;
}
