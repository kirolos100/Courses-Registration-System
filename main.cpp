#include<iostream>
#include <map>
#include"Classes_functions.hpp"
#include <vector>
#include <algorithm>
#include <graphics.h>
#include <math.h>
#include <sstream>
using namespace std;

int main(){
    vector<vector<int>> CoursePlusPre;
    vector<int> parents;
    vector<int> fullvec;
    vector<vector<int>> avacourses;
    vector<int> takencourses;
    vector<int> wantcourses;
    vector<vector<int>> fulldvec;
    map<int, string> values;
    map<string, int> keys;
    map<int,int> binarycourses;
    map<int, int> fullmap;
    BST treecourses;
    studentbst streecourses;
    readfile(CoursePlusPre, keys, values); //Values to invert the map in order to print the full course name to the user
    treecourses.addtotree(CoursePlusPre);

    //Converting the CoursePlusPre to a Map
    for(auto i: CoursePlusPre){
        fullmap[i[0]] = i[1];
    }
    /*
    cout << "BST: ";
    treecourses.inorder();
    cout << endl;
    */
    //Map Binary Assignment
    treecourses.binary(binarycourses);


    //Taken courses input
    char course[150];
    int sizee, var;

    cout << "How many courses did you study ?" << endl;
    cin >> sizee;
    cout << "Enter the courses which you already studied: " << endl;
    for(int i = 0; i < sizee; i++){
        getchar();  //Program closes immediately after pasting or left clicking on the cmd screen
        scanf(" %100[^\n]", course);

        takencourses.push_back(keys[course]);
    }
    cout << endl;

    //Assigning the parents from the inputs
    for(auto i: takencourses){
        if((treecourses.searchtree(parents, i, binarycourses)) == -1){
            cout << "No courses found with that code!" << endl;
            break;
        }
    }
    //removing duplicates of parents vector
    sort(parents.begin(), parents.end());
    auto last = unique(parents.begin(), parents.end());
    parents.erase(last, parents.end());


     cout << "How many you want to study ?" << endl;
     cin >> sizee;
     cout << "Enter the courses that you want to study: " << endl;
     for(int i = 0; i < sizee; i++){
        getchar();
        scanf(" %100[^\n]", course);
        wantcourses.push_back(keys[course]);
    }
    //removing duplicates
    sort(wantcourses.begin(), wantcourses.end());
    auto last1 = unique(wantcourses.begin(), wantcourses.end());
    wantcourses.erase(last1, wantcourses.end());
    cout << endl;

    //appending the two vectors

    fullvec.insert(fullvec.end(), parents.begin(), parents.end());
    fullvec.insert(fullvec.end(), wantcourses.begin(), wantcourses.end());


    cout << "Processing..." << endl;

    //Configuring the available courses
    treecourses.detector(parents, wantcourses, avacourses, binarycourses);



    //Printing the ok courses and not ok courses to take
    cout << "The following courses are OK to take:" << endl;
    for(int i = 0; i < avacourses.size(); i++){
            if(avacourses[i][1] == 1){
                cout << values[avacourses[i][0]] << endl;
            }
    }
    cout << endl;
    cout << "The following courses are CANNOT be taken:" << endl;
    for(int i = 0; i < avacourses.size(); i++){
            if(avacourses[i][1] == 0){
                cout << values[avacourses[i][0]] << endl;
            }
    }

    fulldvec.resize(fullvec.size()); // resize co and make it the same size of co
    for(int i = 0; i<fulldvec.size(); i++){ // resize each index in the both vector to contain 2 numbers
        fulldvec[i].resize(2);
    }
    int counttme = 0;
    for(int i = 0; i < fulldvec.size(); i++){
        fulldvec[i][0] = fullvec[counttme]; //Course assignment
        fulldvec[i][1] = fullmap[fullvec[counttme]]; //Prerequistes assignment
        counttme ++;
    }

    streecourses.addtotree(fulldvec);
//-----------------------------------------------------------------------------------------
    // Initialize graphic driver
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "None");
    int indicc;
    cout << endl << endl << "Do you want to display the student's course structure ? 1 = Yes, 0 = No." << endl;
    cin >> indicc;
    if(indicc == 1){
        initgraph(&gd, &gm, "None");
        streecourses.printTree(300, 100, 0, 20);
        getch();

        // closegraph function closes the
        // graphics mode and deallocates
        // all memory allocated by
        // graphics system
        closegraph();
    }
    else{
        cout << "Thank you, bye!";
    }

//-----------------------------------------------------------------------------------------------------------


}
