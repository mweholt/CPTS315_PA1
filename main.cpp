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
    double conf = 0;
    double freq = 0;
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

map<string,_pair> pairs2;
map<string,_pair> pairs3;

void addPair(string item1, string item2, map<string, _pair> &pairs, string item3 = "") {
    if (item3 == "") {
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
    
    else {
        _pair temp1 = makePair(item1, item2,item3);
    
        auto it = pairs.find(item1+item2+item3);
        if (it != pairs.end()) {
            it->second.freq++;
            //cout << "found matching pair" << endl;
            return;
        }
        else { pairs.insert(pair<string, _pair>(item1 + item2 + item3, temp1)); }
        //cout << "created new pair" << endl;
    }
}

bool compareByConf(const _pair &a, const _pair &b) {
    return b.conf < a.conf;
}

void Combination(const vector<string>& v, int count)
{
    vector<string> tempStrings;
    assert(count <= v.size());
    vector<bool> bitset(v.size() - count, 0);
    bitset.resize(v.size(), 1);
    
    do {
        for (size_t i = 0; i != v.size(); ++i) {
            if (bitset[i]) {
                tempStrings.push_back(v[i]);
            }
        }
        
        // add to pairs3
        addPair(tempStrings[0], tempStrings[1], pairs3, tempStrings[2]);
        tempStrings.clear();
    } while (next_permutation(bitset.begin(), bitset.end()));
    
}

int main(int argc, const char * argv[]) {
    ifstream inFile;
    inFile.open("browsing-data.txt");
    //inFile.open("test.txt");
    
    vector<string> ftemp;
    map<string, int> frequency;
    vector<vector<string> > allItems;
    
    int line = 0;
    double transactions = 0;
    string str;
    string word;
    
    vector<string> temp;
    
    
    
    // Read every line and place items into vector
    while (getline(inFile, str)) {
        istringstream buffer(str);
        transactions++;
        while (buffer >> word) {
            // Check frequent list and add entry if needed
            auto it = frequency.find(word);
            if (it != frequency.end()) {
                it->second++;
                if (it->second >= support) {
                    if (find(ftemp.begin(),ftemp.end(),word) == ftemp.end()) {
                        ftemp.push_back(word);
                    }
                }
            }
            else { frequency.insert(pair<string, int>(word, 1)); }
            
            //Add to master list
            temp.push_back(word);
        }
        
        sort(temp.begin(), temp.end());
        allItems.push_back(temp);
        temp.clear();
        
        //cout << "Line " << line << " completed" << endl;
        line++;
    }
    
    inFile.close();
    sort(ftemp.begin(), ftemp.end());
    
    // Remove non-frequent items
    for (auto it = allItems.begin(); it != allItems.end(); it++) {
        for (auto n = it->begin(); n != it->end();) {
            if (find(ftemp.begin(),ftemp.end(),*n) == ftemp.end()) {
                n = it->erase(n);
            }
            else {n++;}
        }
    }
    
    // Create pairs and remove pairs that don't have support
    for (auto it = allItems.begin(); it != allItems.end(); it++) {
        for (auto n = it->begin(); n != it->end(); n++) {
            for (auto p = it->begin() + 1; p != it->end(); p++) {
                addPair(*n, *p, pairs2);
            }
        }
    }
    for (auto it = pairs2.begin(); it != pairs2.end();) {
        if (it->second.freq < support) {
            it = pairs2.erase(it);
        }
        else {it++;}
    }
    
    // Make 3 pairs
    for (auto it = allItems.begin(); it != allItems.end(); it++) {
        if (it->size() > 2) {
            vector<string> tempVect = *it;
            Combination(tempVect, 3);
        }
    }
    for (auto it = pairs3.begin(); it != pairs3.end();) {
        if (it->second.freq < support) {
            it = pairs3.erase(it);
        }
        else {
            it++;
        }
    }
    
    // Calculate confidence and sort
    vector<_pair> pair2Final;
    for (auto it = pairs2.begin(); it != pairs2.end(); it++) {
        double tempfreq = it->second.freq;
        it->second.conf = tempfreq/transactions;
        pair2Final.push_back(it->second);
    }
    sort(pair2Final.begin(),pair2Final.end(),compareByConf);
    vector<_pair> pair3Final;
    for (auto it = pairs3.begin(); it != pairs3.end(); it++) {
        double tempfreq = it->second.freq;
        it->second.conf = tempfreq/transactions;
        pair3Final.push_back(it->second);
    }
    sort(pair3Final.begin(),pair3Final.end(),compareByConf);
    
    
    // Print
    ofstream outFile("output.txt");
    outFile << "OUTPUT A" << endl;
    for (int i = 4; i > 0; i--) {
        outFile << pair2Final[i].item1 << " " << pair2Final[i].item2 << " " << pair2Final[4].conf << endl;
    }
    outFile << "OUTPUT B" << endl;
    for (int i = 4; i > 0; i--) {
        outFile << pair3Final[i].item1 << " " << pair3Final[i].item2 << " " << pair3Final[i].item3 << " " << pair3Final[i+4].conf << endl;
    }
    
    
    return 0;
}
