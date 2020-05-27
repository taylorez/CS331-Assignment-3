#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>



using namespace std;


vector<string> vocab(char* file);
vector<string> set_words(vector<string> temp);
void convert(vector<string> temp, char* file);

/////////////////////////
//Start Classification
float check_accuracy(vector<int> myTruths, vector<int> expectedTruths);
void classification(vector<string> data, vector<int> labels);


int main(int argc, char** argv){
    vector<string> train_vec;
    train_vec = vocab(argv[1]);
    vector<string> final_vocab;
    final_vocab = set_words(train_vec);
    vector<string> temp;
    temp = final_vocab;
    int num = final_vocab.size();
    sort(final_vocab.begin(), final_vocab.end());

    //at this point the words are all held in final_vocab and sending that and the file to read into convert

    //convert(temp, argv[1]);
    for(int i = 0; i < num; i++){
        cout << final_vocab.back() << "," << endl;;
        final_vocab.pop_back();
    }
    cout << endl;
    return 0;
}


//this function still needs to convert the file to 0 and 1, i will finish this part. my plan was to call another function in this function sending the new file of 0 and 1 to it
//from there we will be at the classification part
void convert(vector<string> temp, char* file){
    int num = temp.size();
    string array[num];
    for(int i = 0; i < num; i++){
        array[i] = temp.back();
        temp.pop_back();
    }
    ifstream infile;
    infile.open(file);
    //working here
}


vector<string> set_words(vector<string> temp){
    int num = temp.size();
    string array[num];
    for(int i = 0; i < num; i++){
        array[i] = temp.back();
        temp.pop_back();
    }
    string final_array[num];
    final_array[0] = array[0];
    int count = 0;
    int max = 1;
    string temp_s;
    int good = 1;
    for(int i = 1; i < num; i++){
        temp_s = array[i];
        good = 1;
        count = 0;
        while(count < max){
            if(temp_s == final_array[count]){
                good = 0;
                count = max;
            }
            else{
                count++;
            }
        }
        if(good == 1){
            final_array[max] = temp_s;
            max++;
        }
    }
    vector<string> final_vec;
    for(int i = 0; i < max; i++){
        final_vec.push_back(final_array[i]);
    }
    return final_vec;
}


vector<string> vocab(char* file){
    char character;
    vector<string> lines;
    ifstream infile;
    infile.open(file);
    string word = "";
    int check = 0;
    if(infile.is_open()){
        while(infile.get(character)){
                if(character == ' ' && check == 0){
                    check = 1;
                    lines.push_back(word);
                    word = "";
                }
                else{
                    if(character != '\n' && character != ' ' && character != '1' && character != '0' && character != '.' && character != ',' && character != '?' && character != '!' && character != '\'' && character != '-'){
                        character = tolower(character);
                        word = word + character;
                        check = 0;
                    }
                }
        }
    }
    lines.push_back("classlabel");
    infile.close();
    return lines;
}

/**
 * This function checks the accuracy of the given vector labels
 * i.e. 0 or 1 and compares it against the expect values.
 * If im understanding right this will be kinda the final 
 * function needed which will be what is printed 
 */
float check_accuracy(vector<int> myTruths, vector<int> expectedTruths) {
    int correct_prediction = 0; //our number of matches with actual value

    for(int i=0; i < expectedTruths.size(); i++) {
        if(myTruths[i] == expectedTruths[i]) { //then it is good so increment total matches
            correct_prediction++;
        }
    }

    float accuracy = ((float) correct_prediction) / ((float) expectedTruths.size()); //correct divided by total 

    return accuracy;
}

void classification(vector<string> data, vector<int> labels) {

    /**
     * data[i] corresponds with labels[i]?
     * Maybe data as a 2d vector for each sentence?
     * Then for each sentence:
     * float pTrue = P(Class=1 | data[0]=labels[0], data[1]=labels[1] and so on)
     * float pTrue = P(Class=0 | data[0]=labels[0], data[1]=labels[1] and so on)
     * if(pTrue > pFalse) {
     *      more likely to be true
     * So the class label would be 1 otherwise it is predicted as 0
     * do this for all of data and when done check the accuracy
     */




    return;
}










