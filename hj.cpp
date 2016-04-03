#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm> // for copy
#include <iterator>
#include <cmath>
using namespace std;

string r1f="r1.txt",r2f="r2.txt";
int rcsz1=100,rcsz2=120,pgsz=400,pgno=5,roundno=3,buckno,recinpage1,recinpage2;


int hashfn(int arg,int round){
	for (size_t i = 1; i < round; i++) {
		arg/=buckno;	}
	return arg%buckno;}

int fsz(string name){
	int i=0;string line;
	ifstream in(name.c_str());
	while (getline(in,line)) if (line.size()>0)i++;
	in.close();
	return i;}

void join_inmem(std::vector<int> bucket1,std::vector<int> bucket2){

	for (int i =0;i<bucket1.size();i++)
		for (int j = 0; j < bucket2.size(); j++)
			if (bucket2[j]== bucket1[i])
				cout << "\t\t\t\t Match found with " <<bucket1[i]<<endl;}


void join_recur(int round,std::vector<int> bucket1,std::vector<int> bucket2){
	if (round>roundno) {cout << "ERROR - Hashing limit exceeded, terminating"; exit(1);}
	vector<vector<int> > hash1(buckno),hash2(buckno);
	int index,s1,s2;																		cout << "Bucket 1 in progress \n";

	for (int i = 0; i < bucket1.size(); ++i)
	{
		index=hashfn(bucket1[i],round);
		hash1[index].push_back(bucket1[i]);													cout <<bucket1[i]<<" goes into bucket "<<index<<endl;
		if (hash1[index].size()%recinpage1==1&&hash1[index].size()!=1) 						cout << "bucket "<<index<<" filled, spilling into disk\n";}
																							cout << "Bucket 2 in progress  \n";
for (int i = 0; i < bucket2.size(); ++i)
	{
		index=hashfn(bucket2[i],round);
		hash2[index].push_back(bucket2[i]);													cout <<bucket1[i]<<" goes into bucket "<<index<<endl;
		if (hash2[index].size()%recinpage2==1&&hash2[index].size()!=1) 						cout << "bucket "<<index<<" filled, spilling into disk\n";}


for ( int i = 0; i < buckno; ++i)
	{
		s1=ceil( (float) hash1[i].size()/recinpage1),s2=ceil( (float) hash2[i].size()/recinpage2);
		if(s1+s2<pgno) {
			cout<<"In memory join in bucket "<<i<<endl;
			join_inmem(hash1[i],hash2[i]);}
		else{
			cout<<"Round "<<round+1<<" hashing in bucket "<<i<<endl;
			join_recur(round +1 ,hash1[i],hash2[i]);}
	}



}


void join_init(){
	int s1=ceil( (float)fsz(r1f)/recinpage1),s2=ceil( (float) fsz(r2f)/recinpage2);
																							std::cout <<"page size of rln 1 = "<< s1<< std::endl;
																							std::cout <<"page size of rln 2 = "<< s2<< std::endl;
	int i=0,index;
	string line;
	ifstream in(r1f.c_str());
	ifstream in2(r2f.c_str());
	

	if(s1+s2<pgno) {		
		std::vector<int> r1,r2;																cout<<"Both rln in mem, performing join\n";
																							cout << "Relation 1 in progress - <3 \n";

		while (getline(in,line)) if (line.size()>0){
			i=atoi(line.c_str());
			r1.push_back(i);																cout << "Found " <<i<<endl;
		}
																							cout << "Relation 1 ended <|3 , Relation 2  in progress - <3 \n";
		while (getline(in2,line)) if (line.size()>0){
			i=atoi(line.c_str());
			r2.push_back(i);																cout << "Found " <<i<<endl;
		}																					cout << "Relation 2 ended <|3 \n";

		join_inmem(r1,r2);
		in.close(); in2.close(); return;}
																							

	vector<vector<int> > hash1(buckno),hash2(buckno);										cout << "Relation 1 in progress - <3 \n";

	while (getline(in,line)) if (line.size()>0){
		i=atoi(line.c_str());
		index=hashfn(i,1);
		hash1[index].push_back(i);
																							cout <<i<<" goes into bucket "<<index<<endl;
		if (hash1[index].size()%recinpage1==1&&hash1[index].size()!=1) 						cout << "bucket "<<index<<" filled, spilling into disk\n";}	
																							cout << "Relation 1 ended <|3 , Relation 2  in progress - <3 \n";
	while (getline(in2,line)) if (line.size()>0){
		i=atoi(line.c_str());
		index=hashfn(i,1);
		hash2[index].push_back(i);
		cout <<i<<" goes into bucket "<<index<<endl;
		if (hash2[index].size()%recinpage2==1&&hash2[index].size()!=1) cout << "bucket "<<index<<" filled, spilling into disk\n";}
																							cout << "Relation 2 ended <|3 \n";

	in.close(); in2.close();

	for ( i = 0; i < buckno; ++i)
	{
		s1=ceil( (float) hash1[i].size()/recinpage1),s2=ceil( (float) hash2[i].size()/recinpage2);
		if(s1+s2<pgno) {
			cout<<"In memory join in bucket "<<i<<endl;
			join_inmem(hash1[i],hash2[i]);}
		else{
			cout<<"Round 2 hashing in bucket "<<i<<endl;
			join_recur(2,hash1[i],hash2[i]);}
	}

}





int main(int argc, char const *argv[]){
	if(argc==8){r1f=argv[1];r2f=argv[2];rcsz1=atoi(argv[3]);rcsz2=atoi(argv[4]);pgsz=atoi(argv[5]);pgno=atoi(argv[6]);roundno=atoi(argv[7]);}
	buckno=roundno-1;
	recinpage1=pgsz/rcsz1;
	recinpage2=pgsz/rcsz2;
	join_init();
	return 0;
}
