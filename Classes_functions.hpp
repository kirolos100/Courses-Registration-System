#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<stdio.h>
#include <graphics.h>
#include <math.h>
using namespace std;
static int countt = 0;


class Node{
public:
    Node(){
        right = left = NULL;
    }
    int coursecode = -1;
    Node* right;
    Node* left;
};

class BST{
private:
   Node* root;
public:
    BST(){root = NULL;}
    void inorder(Node * looper);// prints the tree from the far left to the far right
    int searchcourses_right(int pre, vector<vector<int>> courses); // searches for the suitable course for the right child
    int searchcourses_left(int pre, vector<vector<int>> courses);// searches for the suitable course for the left child
    void addtotree(vector<vector<int>> courses, Node *parent, Node *thisnode); // adds all the courses in the vector in the tree
                                                                            // with the right order
    int searchtree(vector<int> &parents, int coursecode, map<int,int> &binarymap, Node* searcher);
    void getting_parents(Node* parent, vector<int> parents);
    void checking(vector<int> &takencourses, vector<vector<int>> &vec, Node* parent);
    void binary(map<int,int> &binarycourses, Node* looper);
    Node*location(int coursecode, map<int,int> &b_courses, Node* searcher);
    void detector(vector<int> &parents, vector<int> &wantcourses, vector<vector<int>> &avacourses, map<int,int> &b_courses);
    void printTree(int x, int y, int index,int total_elements, Node* looper);
};

//New student structure
class studentbst: public BST{
private:
    Node* sroot;
};
//Prints the tree nodes from the far left to the far right
void BST ::inorder(Node * looper = NULL){
    if(looper == NULL)                       // checks if the function is called without a value
        looper = root;                   // makes the first value of the looper equal to the root, so it has the root of the tree
    else if (looper->coursecode == -1) // if the value of a node is -1, then this node is the last node in its branch
        return;
    inorder(looper->left);
    cout<<looper->coursecode<<"-";
    inorder(looper->right);
}
//finds the courses that go to the left of the node, priortizing the left, so the first course to match is returned
int BST:: searchcourses_left(int pre, vector<vector<int>> courses){
    for(int i = 0; i<courses.size(); i++){
        if(courses[i][1] == pre){   // return the first course that has the same prequisit code as pre
            return courses[i][0];
        }
    }
    return -1; // default value for the function if the loop ends wihout a match
}
//finds the course that goes on the right of the node, skipping the first match so that it's picked by the left node
int BST :: searchcourses_right(int pre, vector<vector<int>> courses){
    int flag = 0;   //flag to check if the loop has passed a matching course
    for(int i = 0; i<courses.size(); i++){
        if(courses[i][1] == pre){           //if the course of the prequisite matches with the value of courses[i][1]
            if (flag == 0){ // skip the first value
                flag = 1;
                continue;
            }
            else{ // return the second course that has the same prequiste as pre
                return courses[i][0];
            }
        }
    }
    return -1;  //if the loop ends without a match this value is returned instead, meaning the function ended without a match
}
//builds the tree from the zero
void BST :: addtotree(vector<vector<int>> courses, Node *parent = NULL, Node *thisnode = NULL){
    if(parent == NULL){ // if the parent is NULL this means that the tree is empty, so root is given a value
        root = new Node;
        thisnode = root;
        root->coursecode = searchcourses_left(0,courses); // the value given to the root is the first course that has no prequisite/0
        if(root->coursecode == -1){                      // if the value is -1, then there is no course that has no prequisite
            cout<<"The courses file does not contain a root course...\n"; // the function exits
            return;
        }
        else{                           // creats nodes for the right and left pointers (initially NULL)
            thisnode->left = new Node;
            thisnode->right = new Node;
            addtotree(courses, thisnode, thisnode->left); //calls the function with thisnode as parent, left as the next
            addtotree(courses, thisnode, thisnode->right); // and right as next
        }
    }
    else{
        if(thisnode == parent->left){ // if the address of this pointer matches the address of parent->left
            thisnode->coursecode = searchcourses_left(parent->coursecode ,courses); //find the course that suit this position
            if (thisnode->coursecode == -1){ // if none was found, return
                // delete thisnode; //?why does this mess up the code???
                thisnode = NULL;
                return;
            }
            else{ // creat a left node for this node
                thisnode->left = new Node;
                addtotree(courses, thisnode, thisnode->left); // send thisnode as a parent, and its left as next
                thisnode ->right = new Node;
                addtotree(courses, thisnode, thisnode->right);
            }
        }
        else if (thisnode == parent->right){ // if this node matches the address of the right node of parent
            thisnode->coursecode = searchcourses_right(parent->coursecode, courses); // find the course suitable for this position
            if (thisnode->coursecode == -1){ // if none was found return
                // delete thisnode;
                thisnode = NULL;
                return;
            }
            else{ // create new nodes for the right and left pointers of thisnode
                thisnode->left = new Node;
                addtotree(courses, thisnode, thisnode->left); //call this function with thisnode as a parent and left as next
                thisnode ->right = new Node;
                addtotree(courses, thisnode, thisnode->right);// call this function with thisnode as parent and right as next
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------------------------

void BST:: binary(map<int,int> &binarycourses, Node* looper = NULL){
    if (looper == NULL){
        looper = root;
    }
    if(looper->coursecode == -1)
        return;
    binary(binarycourses, looper->left);
    binarycourses[(looper->coursecode)] = ++countt;
    binary(binarycourses, looper->right);
}
//btgeb kol el parents
int BST::  searchtree(vector<int> &parents, int coursecode, map<int,int> &b_courses, Node* searcher = NULL){
     if(searcher == NULL)                       // checks if the function is called without a value
        searcher = root;                   // makes the first value of the looper equal to the root, so it has the root of the tree
     else if (searcher->coursecode == -1) // if the value of a node is -1, then this node is the last node in its branch
        return -1;

     if(searcher->coursecode == coursecode){
        parents.push_back(searcher->coursecode);
         return 0;
     }
     else{
        if(b_courses[coursecode] < b_courses[searcher->coursecode]){
            parents.push_back(searcher->coursecode);
            searchtree(parents, coursecode, b_courses, searcher->left);

        }
        else{
            parents.push_back(searcher->coursecode);
            searchtree(parents, coursecode, b_courses, searcher->right);
        }


     }

 }
Node* BST:: location(int coursecode, map<int,int> &b_courses, Node* searcher = NULL){

     if(searcher == NULL)                       // checks if the function is called without a value
        searcher = root;                   // makes the first value of the looper equal to the root, so it has the root of the tree
     else if (searcher->coursecode == -1) // if the value of a node is -1, then this node is the last node in its branch
        return NULL;

     if(searcher->coursecode == coursecode){

         return searcher;
     }
     else{
        if(b_courses[coursecode] < b_courses[searcher->coursecode]){

            location(coursecode, b_courses, searcher->left);

        }
        else{
            location(coursecode, b_courses, searcher->right);
        }
     }

 }

 void BST:: detector(vector<int> &parents, vector<int> &wantcourses, vector<vector<int>> &avacourses, map<int,int> &b_courses){
     Node *temp;
     int countt = 0, indic = 0, tempint;
     //Resizing avacourses
    avacourses.resize(wantcourses.size()); // resize co and make it the same size of co
    for(int i = 0; i<avacourses.size(); i++){ // resize each index in the both vector to contain 2 numbers
        avacourses[i].resize(2);
    }
    //error
        for(auto i: wantcourses){
            indic = 0;

            for(auto j: parents){
                temp = location(j, b_courses); //Return pointer to the location of the parent
                if((temp->left->coursecode == i) || (temp->right->coursecode == i)){
                    avacourses[countt][0] = i;
                    avacourses[countt][1] = 1;
                    break;
                }
                else{
                    avacourses[countt][0] = i;
                    avacourses[countt][1] = 0;
                }
            }

            countt ++;
        }



 }
 void BST:: printTree(int x, int y, int index,int total_elements, Node* looper = NULL)
{

    if (looper == NULL){
        looper = root;
    }
    if(looper->coursecode == -1)
        return;
    // Base Case
    if (index >= total_elements)
        return ;
    if(looper->coursecode == -1)
        return;
    // Convert int value into string
    ostringstream str1;
    str1 << looper->coursecode;

    string str2 = str1.str();
    char* str = &str2[0u];

    // Set color of the boundary of
    // circle as green
    setcolor(RED);

    // Draw the circle of radius 15
    // that represent node of Tree
    circle(x, y, 15);
    floodfill(x, y, RED);

    // Print the values of the node
    // in the circle
    outtextxy(x - 2, y - 3, str);

    // Set the color of the line
    // from parent to child as green
    setcolor(GREEN);

    // Evaluating left and right child
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Recursively draw the left subtree
    // and the right subtree

    printTree(x - y / (index + 1), y + 50, left, total_elements, looper->left);
    printTree(x + y / (index + 1), y + 50, right, total_elements, looper->right);
    // Draw the line (or link) when the
    // node is not the leaf node

    if (left < total_elements) {
        line(x, y, x - y / (index + 1), y + 50);
    }

    if (right < total_elements) {
        line(x, y, x + y / (index + 1), y + 50);
    }

    return;
}

// reads the file named courses.csv and takes a refrence to a 2d vector to assign the courses and prequisits to it
void readfile(vector<vector<int>> &both, map<string, int> &keys, map<int, string> &values){
    ifstream fin("courses (2).csv"); //creats the file pointer
    if (!fin.is_open()){
        cout<<"No such file or directory found. Please check that 'courses.csv'"
                "exists in the same file as this app."<<endl;
        return;
    }
    string line,partline; //strings assign read strings from the file to
    vector<string> courses, prequesites; // vectors to assign the courses and prequisits to each seperated
    while(fin.good()){ // fin.good() checks if the next pointer is a good pointer (not end of file)
        getline(fin,line); // get this line and assign it to "line" variable
        stringstream sline; // stringstream variable is used to slice up the strings
        sline<<line;        // put the string in line to sline variable
        int flag = 0;               // this flag is used to alternate between courses and prequesits
        while(getline(sline,partline,',')){ //slice the line from the file at the comma and put the sliced half in partline
            if (flag == 0){ //if this is the first half of the line push it in courses
                courses.push_back(partline);
                flag = 1; // change flag so it goes to the else next time
            }
            else{ // add this course to the prequisits
                prequesites.push_back(partline);
                flag = 0;
            }
        }
    }
    string coursenum = ""; // empty string used to hold the course code until pushed
    vector<string> co, pre; //vectors to contain the courses as string codes only
    for(auto i: courses){ // for each element in courses,
        for(int j =0; j<3; j++){ //put the first 3 characters in coursnum string
            coursenum += i[j];
        }
        co.push_back(coursenum); //save the course code as an element
        coursenum = ""; // reset coursenum to be used in the next code
    }
    vector<int> intcourses, intprequisites; //int vectors to save the courses as their integer codes
    for (auto i: co){ // push each element in co to intcourses as after converting it to int
        intcourses.push_back(stoi(i));
    }
    for(auto i: prequesites){ // for each element in prequesites
        for (int j = 0; j<3; j++){  // in every first 3 characters in the string if it's a number add it to coursenum
            if(isdigit(i[j])){
                coursenum += i[j];
            }
            else{ // if the character is not a number, like in "None" make coursenum = "0" and skip this element
                coursenum = "0";  // for the no prequist course
                break;
            }
        }
        pre.push_back(coursenum); // push the coursenum in the pre vector
        coursenum = ""; // reset the coursecode for the next iteration of the loop
    }
    for(auto i: pre){
        intprequisites.push_back(stoi(i));// convert each element in pre to int and push it in interequisites
    }
    both.resize(co.size()); // resize co and make it the same size of co
    for(int i = 0; i<both.size(); i++){ // resize each index in the both vector to contain 2 numbers
        both[i].resize(2);
    }
    for(int i =0; i<10; i++){ // assignes the courses and prequisits to the both vector in 2D shape
        for(int j= 0 ; j<2; j++){
            if(j==0){ // acts as a flag
                both[i][j] = intcourses[i]; // adds the courses from intcourses to the 'both' vector in the subvector
            }
            else{
                both[i][j] = intprequisites[i]; // adds the prequsites from intprequisites to the "both" vector in the subvector
            }
        }
    }
    for(int  i = 0; i<courses.size(); i++){
        keys[courses[i]] = intcourses[i];
        values[intcourses[i]] = courses[i];
    }
}
