/** 
 * Names: Brayden Tremper & Ezra Taylor (GROUP)
 * Emails: tremperb@oregonstate.edu && taylorez@oregonstate.edu
 * Date: 6/1/2020
 */ 

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>
#include <cstring>

using namespace std;


vector<string> vocab(char* file);
vector <vector<string> > convert(vector<string> temp, char* file, string outFile);
void classification(vector<vector<float> > training, vector<vector <string> > testingData, int max_size, char* trainedOn, char* testedOn);
void printToFile(vector<string> vocab, vector<string> label);
vector<string> sort_vector(vector<string> data);
void printToFile(vector<string> vocab, vector<vector <string> > labels, string file);
float getProbability(string id, vector <string> currSentence, vector <vector <string> > testingData, vector <vector <float> > train, int idx, int max_size);
vector <vector<float> > get_prob(vector <vector<string> > trainingData);


int main(int argc, char** argv){
    /*Get Training Data*/
    vector<string> train_vec;
    train_vec = vocab(argv[1]);

    vector<string> final_vocab;
    final_vocab = sort_vector(train_vec);
    //Good
    vector<string> temp;
    temp = final_vocab;
    //at this point the words are all held in final_vocab and sending that and the file to read into convert
    vector <vector<string> > trainingData = convert(temp, argv[1], "preprocessed_train.txt");
    //Now we can train the data
    vector <vector<float> > probVec;
    probVec = get_prob(trainingData);


    int max_size = temp.size(); //so we dont use the class label
    /*Get Testing Data*/
    
    cout << "Testing Against " << argv[2] << endl;

    
    //at this point the words are all held in final_vocab and sending that and the file to read into convert
    vector <vector<string> > testingData = convert(temp, argv[2], "preprocessed_test.txt");

    classification(probVec, testingData, max_size, argv[1], argv[2]);

    return 0;
}

/*
 * This function trains our input date
 * it creates a probability of each which will be
 * used to test
 */
vector <vector<float> > get_prob(vector <vector<string> > trainingData){
    vector <vector<float> > probVec(trainingData[0].size());
    int temp[trainingData[0].size()][4];
    for(int i = 0; i < trainingData[0].size(); i++){
        for(int y = 0; y < 4; y++){
            temp[i][y] = 0;
        }
    }
    for(int i = 0; i < trainingData.size(); i++){
        for(int y = 0; y < trainingData[i].size(); y++){
            if(trainingData[i][y] == "0" && trainingData[i][trainingData[i].size() - 1] == "0"){
                temp[y][0]++;
            }
            else if(trainingData[i][y] == "0" && trainingData[i][trainingData[i].size() - 1] == "1"){
                temp[y][1]++;
            }
            else if(trainingData[i][y] == "1" && trainingData[i][trainingData[i].size() - 1] == "0"){
                temp[y][2]++;
            }
            else if(trainingData[i][y] == "1" && trainingData[i][trainingData[i].size() - 1] == "1"){
                temp[y][3]++;
            }
        }
    }
    int probGood = 0;
    int probBad = 0;
    for(int i=0; i < trainingData.size(); i++) {
        if(trainingData[i][trainingData[i].size()-1] == "1") {
            probGood++;
        }
        else{
            probBad++;
        }
    }
    float pGood = ((float)probGood) / ((float)trainingData.size()-1);
    float pBad = 1.0 - pGood;
    for(int i = 0; i < trainingData[0].size(); i++){
        probVec[i].push_back(((float)temp[i][0]+1) / (probBad+2));
        probVec[i].push_back(((float)temp[i][1]+1) / (probGood+2));
        probVec[i].push_back(((float)temp[i][2]+1) / (probBad+2));
        probVec[i].push_back(((float)temp[i][3]+1) / (probGood+2));
        //This is just in case something odd happens
        for(int y=0; y < 4; y++) {
            if(probVec[i][y] > 1.0){
                probVec[i][y] = 1.0;
            }
            if(probVec[i][y] < 0.0) {
                probVec[i][y] = 0.0;
            }
        }
    }

    return probVec;

}



/**
 * This function convert the data to its 1 or 0 format
 */
vector <vector<string> > convert(vector<string> temp, char* file, string outFile){
    ifstream refile;
    refile.open(file);

    string line;
    vector <string> sentences;
    vector <vector <string> > brokenSentences;

    if(refile.is_open()){
        while(getline(refile, line)) {
            sentences.push_back(line);
        }
    }
    refile.close();


    vector <string> value;
    for(int i=0; i < sentences.size(); i++) {
        string temp = sentences[i];
        for(int j=0, len = temp.size(); j < len; j++) {
            if(ispunct(temp[j]) || temp[j] == '\t' || temp[j] == '\n') { //check for punctuation and some other features
                temp.erase(j--, 1);
                len = temp.size();
            }
        }
        for(int j=0, len = temp.size(); j < len; j++) {
            if(temp[j] == '0' || temp[j] == '1') {
                if(j == temp.size()-2) {
                    value.push_back(string(1, temp[j]));
                }

            }
        }

        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        sentences[i] = temp;
        string word;
        vector <string> wordSentence;
        istringstream iss(temp);
        for(string s; iss >> s;)
            wordSentence.push_back(s);

        brokenSentences.push_back(wordSentence);
        wordSentence.clear();


    }


    //temp is our vocab
    vector <vector <string> > labels; //the labels for each sentence 0,1,0 blah but for each sentence


    for(int i=0; i < brokenSentences.size(); i++) {
        //For each indivdual sentence
        vector <string> indLabels(temp.size(), "0"); //labels for each sentence

        for(int j=0; j < brokenSentences[i].size(); j++) {
            for(int k=0; k < temp.size(); k++) {
                if(brokenSentences[i][j] == temp[k]) {
                    indLabels[k] = "1";
                }
            }
        }
        indLabels.push_back(value[i]);
        labels.push_back(indLabels);
    }


    printToFile(temp, labels, outFile);

    return labels;

}

/**
 * Prints our preprocessed data to file
 * 
 */ 
void printToFile(vector<string> vocab, vector<vector <string> > labels, string file) {
    ofstream wf;
    wf.open(file.c_str());

    for(int i=0; i < vocab.size(); i++) {
        wf << vocab[i];
        if(i != vocab.size()-1) {
            wf << ",";
        }
    }
    wf << '\n';

    for(int i=0; i < labels.size(); i++) {
        for(int j=0; j < labels[i].size(); j++) {
            wf << labels[i][j];
            if(j != labels[i].size()-1) {
                wf << ",";
            }
        }
        wf << '\n';
    }

    wf.close();
}

/**
 * This function simply sorts our vectoor in alphabetical order
 */
vector<string> sort_vector(vector<string> data) {
    fflush(stdout);


    sort(data.begin(), data.end());


    vector <string> newData;
    for(int i=0; i < data.size(); i++) {
        if(i != 0) {
            if(data[i] != data[i-1]) {
                newData.push_back(data[i]);
            }
        }
        else {
            newData.push_back(data[i]);
        }
    }

    newData.push_back("classlabel");

    return newData;
}

/**
 * This function gets and seperates our vocab from the given file
 */
vector<string> vocab(char* file){
    char character;
    vector<string> lines;
    ifstream infile;
    infile.open(file);
    string word = "";
    int check = 0;
    string line;
    ////
    vector <string> sentences;
    if(infile.is_open()){
        while(getline(infile, line)) {
            sentences.push_back(line);
        }
    }
    infile.close();

    for(int i=0; i < sentences.size(); i++) {
        string temp = sentences[i];
        for(int j=0, len = temp.size(); j < len; j++) {
            if(ispunct(temp[j]) || temp[j] == '0' || temp[j] == '1' || temp[j] == '\t' || temp[j] == '\n') {
                temp.erase(j--, 1);
                len = temp.size();
            }
        }
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        string word;
        istringstream iss(temp);
        for(string s; iss >> s;)
            lines.push_back(s);


    }

    return lines;
}


/**
 * This function classifies our data by getting the probability of the occurences
 * and predicts the label of the review
 * 
 */
void classification(vector<vector <float> > train, vector<vector <string> > testingData, int max_size, char* trainedOn, char* testedOn) {

    /**
     * data[i] corresponds with labels[i]?
     * Maybe data as a 2d vector for each sentence?
     * Then for each sentence:
     * float pTrue = P(Class=1 | data[0]=labels[0], data[1]=labels[1] and so on)
     * float pTrue = P(Class=0 | data[0]=labels[0], data[1]=labels[1] and so on)
     * if(pTrue > pFalse) {
     *      more likely to be true
     * So the class label would be 1 otherwise it is predicted as 0
     * do this for all of data and when done check the accuracy.
     */
    /*Find P=good and P=False*/
    int probGood = 0;



    /*************************/
    vector <string> predictedLabels;
    float positive = 0.0f;
    float negative = 0.0f;

    cout << "Processing..." << endl;
    for(int i=0; i < testingData.size(); i++) { //get classlabel for each sentence
        //find P(Class=1 | sentence[i] = traindata for each sentence) repeat to get probability of eeach sentence
        if(getProbability("1", testingData[i], testingData, train, i, max_size) >= getProbability("0", testingData[i], testingData, train, i, max_size)) {
            predictedLabels.push_back("1");
        }
        else { //more likely to be false
            predictedLabels.push_back("0");
        }
    }

    //check accuracy
    int correct=0;
    for(int i=0; i < predictedLabels.size(); i++) {
        if(predictedLabels[i] == testingData[i][testingData[i].size()-1]) {
            correct++;
        }
    }

    cout << "Correct: " << correct << endl;
    cout << "Total: " << predictedLabels.size() << endl;
    float total = (((float)correct) / ((float) predictedLabels.size())) * 100.0;

    cout << "Accuracy: " << total << "%" << endl;

    //Print to file
    string outfile = "results.txt";
    ofstream wf;
    wf.open(outfile.c_str(), std::ios_base::app);
    /////
    wf << "Results" << endl;
    wf << endl;
    wf << "Training On: " << trainedOn << endl;
    wf << "Testing On: " << testedOn << endl;
    wf << endl;
    wf << "Correct: " << correct << endl;
    wf << "Total: " << predictedLabels.size() << endl;
    wf << "Accuracy: " << total << "%" << endl;
    wf << endl;

    ////
    wf.close();
    return;
}

/**
 * This function returns the probabilty for each sentences label given both as pos or negative
 */
float getProbability(string id, vector <string> sentence, vector <vector <string> > trainingData, vector <vector <float> > probVec, int num, int max_size) {

    //P(Class=id | data[0]=labels[0], data[1]=labels[1] and so on)

    float prob = 1.0;
    float temp[trainingData[0].size()];

    int probGood = 0;
    for(int i=0; i < trainingData.size(); i++) {
        if(trainingData[i][trainingData[i].size()-1] == "1") {
            probGood++;
        }
    }
    float pGood = ((float)probGood) / ((float)trainingData.size()-1);
    float pBad = 1.0 - pGood;
    float wordMatch = 0.0, classMatch = 0.0, idMatch = 0.0;
    for(int i = 0; i < trainingData.size(); i++) {
        for(int k=0; k < trainingData[i].size(); k++) {
            if(trainingData[num][k] == trainingData[i][k]) {
                
                wordMatch++;
                if(trainingData[num][trainingData[num].size()-1] == trainingData[i][trainingData[i].size()-1]) {
                    classMatch++;
                }
            }
        }
        if(id == trainingData[i][trainingData[i].size()-1]) {
            idMatch++;
        }
        prob += log10((wordMatch+1) / (classMatch+2));
    }
    prob += log10(((idMatch) / (trainingData.size()+1)));
        for(int y = 0; y < max_size; y++){
            if(trainingData[num][y] == "0" && id == "0") { 
                //in our sentence what is the probability  given our sentence is not there and it is a bad review?
                temp[y] = probVec[y][0];
            }
            else if(trainingData[num][y] == "0" && id == "1") {
                temp[y] = probVec[y][1];
            }
            else if(trainingData[num][y] == "1" && id == "0") {
                temp[y] = probVec[y][2];
            }
            else if(trainingData[num][y] == "1" && id == "1") {
                temp[y] = probVec[y][3];
            }
        }
    
   
    for(int i = 0; i < max_size; i++){
        prob = prob * temp[i];
    }
    if(id == "0"){
        prob = prob * pBad; //going against bad review
    }
    else if(id == "1"){
        prob = prob * pGood; //going against good review
    }
    
    return prob;
}
