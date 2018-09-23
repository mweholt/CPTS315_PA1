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
using namespace std;

int support = 100;

struct item{
    string name;
    int freq;
    int basket;
    
    item(string n = "", int f = 0, int b = 0): name(n), freq(f), basket(b){}
};

struct _pair{
    string item1;
    string item2;
    string item3;
    int freq = 0;
    int conf;
    
    _pair(string n1="", string n2="", int c=0): item1(n1), item2(n2), conf(c){}
    //_pair(string n1="", string n2="", string n3="", int c=0): item1(n1), item2(n2), item3(n3), conf(c){}
};

bool isSupportMet (string name, vector<item> list) {
    for (int i = 0; i <list.size(); i++) {
        if (list[i].name == name) {
            if (list[i].freq >= support) {
                return true;
            }
            else {break;}
        }
    }
    return false;
}

int findPair (string name1, string name2, vector<_pair> pairs) {
    int a = -1;
    for (int i = 0; i < pairs.size(); i++) {
        if ((pairs[i].item1 == name1 && pairs[i].item2 == name2) || (pairs[i].item1 == name2 && pairs[i].item2 == name1)) {
            a = i;
        }
    }
    return a;
}


int main(int argc, const char * argv[]) {
    ifstream inFile;
    inFile.open("browsing-data.txt");
    //inFile.open("test.txt");
    
    vector<item> freqItems;
    vector<item> allItems;
    vector<_pair> pairs;
    
    int line = 1;
    string str;
    string word;
    
    
    // Read every line and place items into vector
    getline(inFile,str);
    istringstream buffer(str);
    while(buffer >> word) {
        freqItems.push_back(item(word,1,line));
        allItems.push_back(item(word,-1,line));
    }
    
    
    while (getline(inFile, str)) {
        cout << "Reading line" << endl;
        istringstream buffer(str);
        line++;
        while(buffer >> word) {
            for(int i=0; i<freqItems.size();++i) {
                if (freqItems[i].name == word) {
                    freqItems[i].freq += 1;
                    allItems.push_back(item(word,-1,line));
                    cout << "Found match" << endl;
                    break;
                    
                }
                if (i==freqItems.size()-1) {
                    freqItems.push_back(item(word,1,line));
                    allItems.push_back(item(word,-1,line));
                    cout << "New Item added" << endl;
                    break;     
                }
            }
        }
    }
    
    
    
    //Create 2-pairs
    int x = 1;
    bool b = true;
    int loc = -1;
    int baskets = 1;
    for (int i = 0; i<allItems.size();i++) {
        while (b) {
            if (isSupportMet(allItems[i].name, freqItems) && allItems[i+x].basket == allItems[i].basket) {
                if (isSupportMet(allItems[i+x].name, freqItems)) {
                    loc = findPair(allItems[i].name, allItems[i+x].name, pairs);
                    if (loc == -1) {
                        pairs.push_back(_pair(allItems[i].name, allItems[i+x].name));
                    }
                    else {
                        pairs[loc].freq++;
                    }
                }
                x++;
                //cout << "{" << allItems[i].name << ", " << allItems[i+x].name << "}" << endl;
            }
            else {b = false;}
        }
        b = true;
        x = 1;
        
        // Ddebugging
        if (allItems[i].basket + 1 != allItems[i].basket) {
            cout << "Basket #" << baskets << "finished" << endl;
            baskets++;
        }
    }
    
    return 0;
}
