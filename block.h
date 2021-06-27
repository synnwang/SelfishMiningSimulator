/*
本著作係採用創用 CC 姓名標示-非商業性 3.0 台灣 授權條款授權.
Contributor: Sheng-Wei Wang
*/

#ifndef block_h
#define block_h

class block{
	public:
		block();
		block(int,int,int,char*,double);
		void show();
		bool operator<(const block&) const; //重定義事件排序
		int getBlockID();
		int getOwnerID();
		int getChain();

		double getBlockTime();
	private:
		int id;
		int ownerID;
		char* owner;
		int chain;
		double timeStamp;
}; 

#endif
