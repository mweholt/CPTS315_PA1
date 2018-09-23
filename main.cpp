//
//  main.cpp
//  PA1
//
//  Created by Mitchell on 9/19/18.
//  Copyright © 2018 Mitchell. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>
using namespace std;

int support = 100;

struct item{
    string name;
    int basket;
    
    item(string n = "", int b = 0): name(n), basket(b){}
};

struct _pair{
    string item1 = "";
    string item2 = "";
	string item3 = "";
	int conf = 0;
	int freq = 0;
};

_pair makePair(string item1, string item2, string item3 = "", int conf = 0, int freq = 1) {
	_pair temp;
	temp.item1 = item1;
	temp.item2 = item2;
	temp.item3 = item3;
	temp.conf = conf;
	temp.freq = 1;

	return temp;
}

void addPair(string item1, string item2, map<string, _pair> &pairs) {
	_pair temp1 = makePair(item1, item2);

	auto it = pairs.find(item1+item2);
	if (it != pairs.end()) {
		it->second.freq++;
		//cout << "found matching pair" << endl;
		return;
	}
	else { pairs.insert(pair<string, _pair>(item1 + item2, temp1)); }
	//cout << "created new pair" << endl;
}

bool isFrequent(string item, map<string, int> freq) {
	bool out = true;
	if (freq.find(item)->second < 100) { return false; }
	return out;
}



class apriori {
private:
	// Baslet #, 
	vector<vector<string>> items;



};


int main(int argc, const char * argv[]) {
    ifstream inFile;
    inFile.open("browsing-data.txt");
    //inFile.open("test.txt");
    
	map<string, int> frequency;
	vector<vector<string> > allItems;
	map<string, _pair> pairs;
    
    int line = 0;
    string str;
    string word;
    
	vector<string> temp;
    
    // Read every line and place items into vector
	while (getline(inFile, str)) {
		istringstream buffer(str);
		while (buffer >> word) {
			// Check frequent list and add entry if needed
			auto it = frequency.find(word);
			if (it != frequency.end()) { it->second++; }
			else { frequency.insert(pair<string, int>(word, 1)); }
			
			//Add to master list
			temp.push_back(word);
		}
		
		sort(temp.begin(), temp.end());
		allItems.push_back(temp);
		temp.clear();
		cout << "Line " << line << " completed" << endl;
		line++;
	}

	


    
    return 0;
}
