#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

bool isValid(char num, vector<char> &box, vector<char> &column, vector<char> &row){
    //check if number can be placed by checking rows, columns, and boxes
    for(int i = 0; i < 9; i++){
        if(box[i] == num)
            return false;
        if(column[i] == num)
            return false;
        if(row[i] == num)
            return false;
    }
    return true;
}

void print_board(vector<char> &board){
    for(int i = 1; i < board.size()+1; i++){
        cout << board[i-1] << " ";
        if(i % 9 == 0) cout << endl;
    }
}

bool find_solution(vector<char>* boxes, vector<char>* rows, vector<char>* cols, vector<char> &board, int k = 0){
    //used for ptr arithmetic to get specific vectors for validity check
    int r = k / 9;
    int c = k % 9;
    int b = (k / 27)*3 + (c / 3);

    //basecase
    if(k == 80) {
        if(board[k] == '-'){
            for(int i = 49; i < 58; i++){
                if(isValid((char)i, *(boxes+b), *(cols+c), *(rows+r))) {
                    board[k] = (char)i;
                    //updating boxes, rows, cols not necessary because printing is
                    //done through the board vector.
                    return true;
                }
            }
        }
        //if last num is known, return true because all other numbers have to be valid
        //to make it here.
        else return true;
    }
    else {
        if(board[k] != '-') //if the index is a number, skip it because the number is already known
            return find_solution(boxes,rows,cols,board,k+1);

        for(int i = 49; i < 58; i++){
            if(isValid((char)i, *(boxes+b), *(cols+c), *(rows+r))) {
                board[k] = (char)i;
                //need to update row, cols, boxes. 
                (*(boxes+b))[3*(r%3)+c%3] = (char)i;
                (*(rows+r))[c] = (char)i;
                (*(cols+c))[r] = (char)i;
                if(find_solution(boxes, rows, cols, board, k+1)) return true;
            }
        }
        //reset board, boxes, row, and cols vectors to original state
        board[k] = '-';
        (*(boxes+b))[3*(r%3)+c%3] = '-';
        (*(rows+r))[c] = '-';
        (*(cols+c))[r] = '-';
    }
    return false;
}

int main(int argc, char** argv){
    if(argc != 2){
        cerr << "usage: ./solver file.txt" << endl;
        return 1;
    }
    
    ifstream fin;
    fin.open(argv[1]);
    if(!fin.is_open()){
        cerr << "unable to open " << argv[1] << endl;
        return 1;
    }    
    //read in the file
    vector<char> board; 
    string line;
    while(fin >> line){
        if(line.size() != 9){
            cerr << "invalid format" << endl;
            return 1;
        }
        for(int i = 0; i < line.size(); i++){
            //checks if char is num or empty char.
            if(line[i] != '-' && !isdigit(line[i])){
                cerr << "invalid format" << endl;
                return 1;
            }
            else board.push_back(line[i]);
        }
    }

    //push chars into vectors
    //allocate memory for 9 vectors that the chars will be added
    vector<char>* rows = new vector<char>[9];
    vector<char>* boxes = new vector<char>[9];
    vector<char>* cols = new vector<char>[9];

    for(int i = 0; i < board.size(); i++){
        //boxes need to incremented every 3 except at multiples of 9 and 27
        //cols need to increment every iteration except at multiples of 9
        //rows need to increment every 9
        if(i % 3 == 0 && i != 0)
            boxes++;
        if(i % 9 == 0 && i != 0){
            cols-=9;
            rows++;
            boxes-=3;
        }
        //three boxes will have 9 elements, so increment to next 3 boxes
        if(i % 27 == 0 && i != 0)
            boxes+=3;
        rows->push_back(board[i]);
        boxes->push_back(board[i]);
        cols->push_back(board[i]);
        cols++;
    }
    //reset pointers to the first of each vector.
    rows-=8;
    cols-=9;
    boxes-=8;
   
    //print original board
    cout << "Original:" << endl;
    print_board(board);
    
    if(!find_solution(boxes, rows, cols, board)){
        cout << endl << "cannot be solved" << endl;
        return 0;
    }

    //print board
    cout << endl << "Solution:" << endl;
    print_board(board);

    //deallocate memory
    delete[] rows;
    delete[] cols;
    delete[] boxes;

    return 0;
}