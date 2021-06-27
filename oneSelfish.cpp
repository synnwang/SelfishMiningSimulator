/*
本著作係採用創用 CC 姓名標示-非商業性 3.0 台灣 授權條款授權.
Contributor: Sheng-Wei Wang

Alice: Selfish Miner
Henry: Honest Miner
執行方式  ./a.out hp     1>=hp>=0
*/

#include <iostream>
#include <iomanip>
#include "block.h"
#include <list>

using namespace std;



list<block> confirmChain;

list<block> AliceChain;
list<block> HenryChain;
list<block> event;

const int maxID=1000000;
double hp=1; //probability of honest miner mines selfish chain when tie

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


int main(int argc, char *argv[])
{
	hp=atof(argv[1]);
	for(double i=0.05;i<0.55;i=i+0.05)
	{

		simulation(i,1-i);
		//cout<<"Final Chain Size: "<<confirmChain.size()<<endl;
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
	numAliceBlock=0;
	numHenryBlock=0;
	currentTimeAlice=0;
	currentTimeHenry=0;
	currentIDAlice=1;
	currentIDHenry=1;
	AliceChain.clear();
	HenryChain.clear();
	confirmChain.clear();
		

	block initHenry(currentIDHenry,0,0,"Henry", exp_random(rateHenry,nextRandom(randomHenry)));
	block initAlice(currentIDAlice,1,1,"Alice", exp_random(rateAlice,nextRandom(randomAlice)));
	event.push_back(initAlice);	
	event.push_back(initHenry);

	event.sort();

}

void processBlock(block b)
{

	//b.show();
	if(b.getOwnerID()==0)  //Honest挖到礦的狀況
	{
		if(b.getChain()==0)
		{
			HenryChain.push_back(b);
			if(HenryChain.size()>AliceChain.size())  //honest先挖到礦
			{
			//	cout<<"Honest 挖到礦。直接上公開鏈。"<<numHenryBlock<<" "<<numAliceBlock<< endl;	
				while(!HenryChain.empty())
				{

					confirmChain.push_back(HenryChain.front());		
				//	cout<<"Determine "<<confirmChain.back().getBlockID()<<endl;
					numHenryBlock++;				
					HenryChain.pop_front();
				}
				endSelfishMining(b);
			}
			else if(AliceChain.size()==1) //Honest挖到礦，SM趕快把他的Block也放出來 (1:1)
			{
			//	cout<<"Honest 挖到礦, SM 放出 Block, 平手狀況。"<<numHenryBlock<<" "<<numAliceBlock<< endl;	
				event.clear();
		

				if(b.getBlockID()<maxID)
				{
					double timeAlice=exp_random(rateAlice,nextRandom(randomAlice));


					double timeHenryOnAlice=exp_random(rateHenry*(hp),nextRandom(randomHenry));
					currentIDHenry=currentIDHenry+1;

					block initHonA(currentIDAlice,0,1,"Henry", b.getBlockTime()+ timeHenryOnAlice);					
					event.push_back(initHonA);

					block initAlice(currentIDAlice,1,1,"Alice", b.getBlockTime()+ timeAlice);	
					event.push_back(initAlice);
					if(hp<1)
					{
						double timeHenry=exp_random(rateHenry*(1-hp),nextRandom(randomHenry));	
						block initHenry(currentIDHenry,0,0,"Henry", b.getBlockTime()+ timeHenry);
						event.push_back(initHenry);										
					}



					event.sort();
				}
			}
			else if(AliceChain.size()==HenryChain.size()+1) //Honest挖到礦，SM把他的Chain放出來
			{
			//	cout<<"Honest 挖到礦, SM 放出 Block, 此時為最長鏈。"<<numHenryBlock<<" "<<numAliceBlock<< endl;		
			
				while(!AliceChain.empty())
				{
					confirmChain.push_back(AliceChain.front());
					//	cout<<"Determine "<<confirmChain.back().getBlockID()<<endl;
					numAliceBlock=numAliceBlock+1;
					//AliceChain.front().show();	
					AliceChain.pop_front();
				}
				endSelfishMining(confirmChain.back()); //SM的最後一個礦

			}
			else //Honest 挖到礦，但SM還贏2個以上，好整以瑕
			{
			//	cout<<"Honest 挖到礦, SM 還贏2個以上, 繼續隱藏。"<<numHenryBlock<<" "<<numAliceBlock<< endl;	
				currentIDHenry++;
				currentTimeHenry=b.getBlockTime();
				block initHenry(currentIDHenry,0,0,"Henry", currentTimeHenry+ exp_random(rateHenry,nextRandom(randomHenry)));
				if(b.getBlockID()<maxID)
				{
					event.push_back(initHenry);
					event.sort();
				}
			}
		}
		else if(b.getChain()==1)
		{
			AliceChain.push_back(b);
			if(AliceChain.size()>HenryChain.size())  //honest在Alice Chain先挖到礦
			{
			//	cout<<"Honest 挖到礦。一個Alice Block，一個Honest Block。"<<numHenryBlock<<" "<<numAliceBlock<< endl;	
				while(!AliceChain.empty())
				{

					confirmChain.push_back(AliceChain.front());		
				//	cout<<"Determine "<<confirmChain.back().getBlockID()<<endl;
					if(AliceChain.front().getOwnerID()==0)
						numHenryBlock++;
					else
						numAliceBlock++;				
					AliceChain.pop_front();
				}
				endSelfishMining(b);
			}

		}


	}
	else if(b.getOwnerID()==1)
	{
		AliceChain.push_back(b);
		if(AliceChain.size()>HenryChain.size()+1 || HenryChain.size()==0) // SM挖到礦，先隱藏起來
		{
			//cout<<"Selfish 挖到礦, 先隱藏起來。"<<numHenryBlock<<" "<<numAliceBlock<< endl;		
		
			currentIDAlice++;
			currentTimeAlice=b.getBlockTime();
			block initAlice(currentIDAlice,1,1,"Alice", currentTimeAlice+ exp_random(rateAlice,nextRandom(randomAlice)));
			if(b.getBlockID()<maxID)
			{
				event.push_back(initAlice);
				event.sort();
			}
		}
		else if(HenryChain.size()==1 && AliceChain.size()==2) //SM挖到礦，趕快放出來
		{
			block latestBlock=AliceChain.front();
			//cout<<"Selfish 挖到礦, 平手時獲勝，趕快放出來。"<<numHenryBlock<<" "<<numAliceBlock<< endl;				
			while(!AliceChain.empty())
			{
				latestBlock=AliceChain.front();
				confirmChain.push_back(AliceChain.front());	
			//	cout<<"Determine "<<confirmChain.back().getBlockID()<<endl;
				//AliceChain.front().show();				
				numAliceBlock++;				
				AliceChain.pop_front();
			}
			endSelfishMining(latestBlock);
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
		block initAlice(currentIDAlice,1,1,"Alice", currentTimeAlice+ timeAlice);	
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
