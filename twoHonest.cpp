/*
本著作係採用創用 CC 姓名標示-非商業性 3.0 台灣 授權條款授權.
Contributor: Sheng-Wei Wang

Alice 和 Henry 都是Honest Miner
*/

#include <iostream>
#include <iomanip>
#include "block.h"
#include <list>

using namespace std;

const int MINERS=3;


list<block> confirmChain;

list<block> AliceChain;
list<block> HenryChain;
list<block> event;

const int maxID=1000000;
int numHenryBlock=0;
int numAliceBlock=0;

long long randomAlice=231498731;
long long randomHenry=904539021;



double currentTimeAlice=0;
double currentTimeHenry=0;

double currentIDAlice=1;
double currentIDHenry=1;

double rateHenry;
double rateAlice;

void initialization();
double exp_random(double,long long);
long long nextRandom(long long&);
void simulation(double,double);
void processBlock(block);
void endSelfishMining(block);


int main()
{

	for(double i=0.05;i<0.5;i=i+0.05)
	{
		numAliceBlock=0;
		numHenryBlock=0;
		currentTimeAlice=0;
		currentTimeHenry=0;
		currentIDAlice=1;
		currentIDHenry=1;
		simulation(i,1-i);
//		cout<<"SM Rate: "<<setw(2)<<i<<" Henry: "<<setw(5)<<numHenryBlock<<"/ Alice: "<<setw(5)<<numAliceBlock<<" R: "<<(double)numAliceBlock/maxID<<endl;
		cout<<i<<" "<<(double)numAliceBlock/maxID<<endl;
	}

	
	return 0;
}



void simulation(double rateA, double rateH)
{

	rateHenry=rateH;
	rateAlice=rateA;

	initialization();
	while(!event.empty())
	{

		block eventBlock=event.front();
		event.pop_front();
		processBlock(eventBlock);

	}
	
}



void  initialization()
{
	block initHenry(currentIDHenry,0,0,"Henry", exp_random(rateHenry,nextRandom(randomHenry)));
	block initAlice(currentIDAlice,1,0,"Alice", exp_random(rateAlice,nextRandom(randomAlice)));
	event.push_back(initAlice);	
	event.push_back(initHenry);

	event.sort();

}

void processBlock(block b)
{
	if(b.getOwnerID()==0)
	{
		HenryChain.push_back(b);
		if(AliceChain.size()==0)  //honest先挖到礦
		{
			confirmChain.push_back(b);
			numHenryBlock++;
			HenryChain.clear();
		//	b.show();
			endSelfishMining(b);
		}

	}
	else if(b.getOwnerID()==1)
	{
		AliceChain.push_back(b);
		if(HenryChain.size()==0)
		{
			confirmChain.push_back(b);
			numAliceBlock++;
			AliceChain.clear();
		//	b.show();
			endSelfishMining(b);
		}
	}
}

void endSelfishMining(block b)
{

	currentIDAlice=b.getBlockID()+1;
	currentIDHenry=b.getBlockID()+1;

	currentTimeAlice=b.getBlockTime();
	currentTimeHenry=b.getBlockTime();
	HenryChain.clear();
	AliceChain.clear();
	event.clear();

	double timeHenry=exp_random(rateHenry,nextRandom(randomHenry));
	double timeAlice=exp_random(rateAlice,nextRandom(randomAlice));

	if(b.getBlockID()<maxID)
	{
		block initAlice(currentIDAlice,1,0,"Alice", currentTimeAlice+ timeAlice);	
		block initHenry(currentIDHenry,0,0,"Henry", currentTimeHenry+ timeHenry);

		event.push_back(initHenry);
		event.push_back(initAlice);
		event.sort();
	}


}

double exp_random(double rate, long long random)
{
    double random_exp=0-(1/rate)*log(1.0-(double)random/2147483647.0);
    return random_exp;
}

long long nextRandom(long long& current)
{
	current=(16807*current)%2147483647;
	return current;
}
