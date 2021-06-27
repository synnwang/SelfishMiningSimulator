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
