/*
本著作係採用創用 CC 姓名標示-非商業性 3.0 台灣 授權條款授權.
Contributor: Sheng-Wei Wang
*/

#include <iostream>
#include "block.h"

using namespace std;

block::block(int n,int oID,int cID,char* o,double t)
{
	id=n;
	owner=o;
	chain=cID;
	ownerID=oID;
	timeStamp=t;
}
 
block::block()
{
	id=0;
	owner="Initialization";
}

int block::getBlockID()
{
	return id;
}

int block::getOwnerID()
{
	return ownerID;
}
int block::getChain()
{
	return chain;
}

double block::getBlockTime()
{
	return timeStamp;
}

void block::show()
{
	cout<<"Block "<<id<<" is mined by miner "<<owner<<" at time: "<<timeStamp<< endl;
}

bool block::operator<(const block& b) const
{
    if(timeStamp<b.timeStamp)
        return true;
    else
        return false;
}
