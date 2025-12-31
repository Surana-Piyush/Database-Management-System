#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include <fstream>
using namespace std;

/*
Table = Schema file + Data file
student.schema   → structure
student.data     → rows

*/

bool isInteger(const string& s){
    if(s.empty()){
        return false;
    }
    int start=0;
    if(s[0]=='-'){
        start=1;
    }

    for(int i=start;i<s.length();i++){
        if(!isdigit(s[i])){
            return false;
        }
    }
    return true;
}

void createSchemaFile(string table_name, string col_name, string col_type){
    ofstream schemaFile(table_name + ".schema", ios::app); 
    //we use app becase everytime the function is called it will clear everything, hence we use append

    schemaFile<<col_name <<":"<<col_type<<"\n";
    schemaFile.close();

}

bool tableExist(string table_name){
    bool file_exists=true;
    ifstream fin(table_name+".schema");
    if(!fin){
        file_exists=false;
    }
    return file_exists;

}

bool tableDataExist(string table_name){
    ifstream fin(table_name+".data");
    if(!fin){
        return false;
    }
    return true;
}

vector<string> schemaDataType(vector<string> token){
    ifstream schemaFile(token[1]+".schema");
    string line;
    vector<string>col_datatype;

    while(getline(schemaFile,line)){
        size_t position = line.find(':');
        if(position==string::npos){
            //":" not found
            continue;
        }
        string col_type=line.substr(position+1);//returns everything from position index to the end
        col_datatype.push_back(col_type);

    }
    
    return col_datatype;
} 

bool inputAndColHeadingLen(vector<string> token){
    bool matching=false;
    
    //Open the schema file and read the col datatype
    vector<string>col_datatype=schemaDataType(token);

    //Get to know the number of input
    int input_len=token.size()-2;//token[0]=insert,token[1]=table name

    //Check the size
    if(input_len==col_datatype.size()){
        matching=true;
    }

    return matching;
}

bool inputAndColHeadingDataType(vector<string> token){
    bool matching=true;

    //Open the schema file and read the col datatype
    vector<string>col_datatype=schemaDataType(token);

    //Storing the input in a vector
    vector<string>input;
    for(int i=2;i<token.size();i++){
        input.push_back(token[i]);
    }

    for(int i=0;i<col_datatype.size();i++){
        if(col_datatype[i]=="int" && !isInteger(input[i])){
            matching=false;
            break;
        }
        else if(col_datatype[i]=="string" && input[i].empty()){
            matching=false;
            break;
        }
        else if(col_datatype[i]=="char" && (input[i].empty()|| input[i].length()>1)){
            matching=false;
            break;
        }
    }

    return matching;
}

void createDataFile(string table_name,vector<string> token){
    vector<string>input;
    for(int i=2;i<token.size();i++){
        input.push_back(token[i]);
    }

    ofstream DataFile(table_name+".data", ios::app);
    for(int i=0;i<input.size()-1;i++){
        DataFile<<input[i]<<',';
    }
    DataFile<<input[input.size()-1];
    DataFile<<"\n";
    DataFile.close();

}

vector<string> getColumnHeadings(string table_name) {
    vector<string> headings;
    ifstream schemaFile(table_name + ".schema");
    string line;

    while (getline(schemaFile, line)) {
        size_t pos = line.find(':');
        if (pos == string::npos) continue;

        string col_name = line.substr(0, pos);
        headings.push_back(col_name);
    }

    schemaFile.close();
    return headings;
}

void displayAllData(string table_name){
    if(!tableExist(table_name)){
        cout<<"Table not found\n";
        return;
    }
    if(!tableDataExist(table_name)){
        cout<<"Table has no data\n";
        return;
    }

    // 1. Print column headings
    vector<string> headings = getColumnHeadings(table_name);
    for (string h : headings) {
        cout << h << "\t";
    }
    cout << "\n";

    // separator line
    for (int i = 0; i < headings.size(); i++) {
        cout << "--------";
    }
    cout << "\n";

    ifstream dataFile(table_name+".data");
    string line, word;
    vector<string> row;
    while (getline(dataFile, line)) {
        row.clear();
        word = "";

        for (char c : line) {
            if (c == ',') {
                row.push_back(word);
                word = "";
            } else {
                word += c;
            }
        }
        row.push_back(word); // last value

        for (string val : row) {
            cout << val << "\t";
        }
        cout << "\n";
    }

    dataFile.close();
}

void displayValuesWithCond(string table_name,string col_name,string val){
    if(!tableExist(table_name)){
        cout<<"Table not found\n";
        return;
    }
    if(!tableDataExist(table_name)){
        cout<<"Table has no data\n";
        return;
    }
    bool found=false;
    int index=0;
    ifstream schemaFile(table_name+".schema");
    string line;
    while(getline(schemaFile,line)){
        size_t pos=line.find(':');
        if(pos==string::npos){
            continue;
        }
        string col_heading=line.substr(0,pos);

        
        if(col_heading==col_name){
            found=true;
            break;
        }
        index++;
        
    }

    schemaFile.close();
    if(!found){
        cout<<"No matching input found\n";
        return;
    }

    // 1. Print column headings
    vector<string> headings = getColumnHeadings(table_name);
    for (string h : headings) {
        cout << h << "\t";
    }
    cout << "\n";

    // separator line
    for (int i = 0; i < headings.size(); i++) {
        cout << "--------";
    }
    cout << "\n";

    ifstream dataFile(table_name+".data");
    string word="",line2;
    vector<string>row;

    while(getline(dataFile,line2)){
        for(char c : line2){
            if(c==','){
                row.push_back(word);
                word="";
            }
            else{
                word+=c;
            } 
        }
        row.push_back(word);//This will push back the last word because there is no comma after the last word
        
        if(row[index]==val){
            for(int i=0;i<row.size();i++){
                cout<<row[i]<<"\t";
            }
            cout<<endl;
        }
        row.clear();
        word="";
    }
    dataFile.close();
}

void deleteRowWithCond(string table_name,string col_name,string val){
    if(!tableExist(table_name)){
        cout<<"Table not found\n";
        return;
    }
    if(!tableDataExist(table_name)){
        cout<<"Table has no data\n";
        return;
    }
    bool found=false;
    int index=0;
    ifstream schemaFile(table_name+".schema");
    string line;
    while(getline(schemaFile,line)){
        size_t pos=line.find(':');
        if(pos==string::npos){
            continue;
        }
        string col_heading=line.substr(0,pos);
        if(col_heading==col_name){
            found=true;
            break;
        }
        index++;
        
    }

    schemaFile.close();
    if(!found){
        cout<<"No matching input found\n";
        return;
    }

    ifstream dataFile(table_name+".data");
    ofstream fout("temp.data");
    string word="",line2;
    vector<string>row;
    while(getline(dataFile,line2)){
        for(char c : line2){
            if(c==','){
                row.push_back(word);
                word="";
            }
            else{
                word+=c;
            } 
        }
        row.push_back(word);
        if(row[index]==val){
            continue;
        }
        else{
            for(int i=0;i<row.size()-1;i++){
                fout<<row[i]<<",";
            }
            fout<<row[row.size()-1]<<endl;
        }
        row.clear();
        word="";
    }
    dataFile.close();
    fout.close();
    remove((table_name+".data").c_str());
    rename("temp.data",(table_name+".data").c_str());
    cout<<"Row deleted\n";
}

int main(){
    cout<<"Commands supported in version-1\n";
    cout<<"create table table-name\n";
    cout<<"insert table-name values seperated by space\n";
    cout<<"select table-name\n";
    cout<<"select table-name where condition\n";
    cout<<"delete from table-name where condition\n";
    cout<<"drop table table-name\n";
    cout<<"exit\n";
    while (true){
        cout<<"dbms> ";
        // Taking input
        string ip;
        getline(cin,ip);

        // Splitting the input by space
        vector<string>token;
        string word;
        stringstream ss(ip);
        while(ss >> word){
            token.push_back(word);
        }
        
        if(token.empty()){
            continue;
        }
        
        //add logic to keep taking input
        if (token[0] == "create") {
            if (token.size() != 3 || token[1] != "table") {
                cout << "Invalid CREATE syntax\n";
                continue;
            }

            string table_name=token[2];

            if (tableExist(table_name)) {
                cout << "Table already exists\n";
                continue;
            }
            /*Sample input
            create table student
            id int
            name string
            */

            /*Schema file
            id:int
            name:string
            marks:int
            grade:char
            */

            while(true){
                string line;
                getline(cin,line);
                if(line=="end"){
                    break;
                }

                string col_name,col_type;
                stringstream ss(line);
                ss>>col_name>>col_type;

                if(col_name.empty() || col_type.empty() || (col_type!="int" && col_type!="string" && col_type!="char")){
                    cout<<"Invalid, Try again\n";
                }
                else{
                    createSchemaFile(table_name,col_name,col_type);
                }
            }
            
            cout<<"Table "<<table_name <<" created successfully\n";
        }
        else if(token[0]=="insert"){
            string table_name=token[1];
            /*Sample input
            insert student 1 Piyush
            */

            /*
            Step-1->Does the table exist
            Step-2->Do the number of input match the number of col
            Step-3->Do the given values match the datatype of the schema
            Step-4->Append data to .data file 
            */

            
            if(tableExist(table_name)){
                if(inputAndColHeadingLen(token)){
                    if(inputAndColHeadingDataType(token)){
                        //Write to new .data file
                        createDataFile(table_name,token);
                        cout<<"1 row inserted\n";
                    }
                    else{
                        cout<<"Data type does not match\n";
                    }
                }
                else{
                    cout<<"Invalid number of arguments\n";
                }
            }
            else{
                cout<<"Table not found\n";
            }
            
            
        }
        else if (token[0] == "select") {
            /*Sample input-1
            Select table name
            */
           string table_name=token[1];
            if(token.size()==2){
                displayAllData(table_name);
            }

            /*Sample input-2
            Select table name where cond.
            */
           //Error
            else if(token.size()==4 && token[2]=="where"){
                string cond=token[3];
                size_t pos=cond.find('=');
                if(pos==string::npos){
                    cout<<"Invalid syntax\n";
                    continue;
                }
                string col_name=cond.substr(0,pos);
                string value=cond.substr(pos+1);
                displayValuesWithCond(table_name,col_name,value);
            }

        }
        else if (token[0] == "drop") {
            string table_name=token[2];

            /*Sample input
            drop table table name*/
            if(token.size()==3 && token[1]=="table" && token[2]==table_name){
                if(tableDataExist(table_name) && tableExist(table_name)){
                    int status1 = remove((table_name + ".schema").c_str()); //remove needs a const char*
                    int status2 = remove((table_name + ".data").c_str());
                    if(status1==0 && status2==0){
                        cout<<"Table dropped\n";
                    }
                    else{
                        cout<<"Error occured\n";
                    }
                }
                else if(tableExist(table_name) && !tableDataExist(table_name)){
                    int status1 = remove((table_name + ".schema").c_str()); //remove needs a const char*
                    if(status1==0){
                        cout<<"Table dropped\n";
                    }
                    else{
                        cout<<"Error occured\n";
                    }
                }
                else{
                    cout<<"Table does not exist\n";
                }
            }
            else{
                cout<<"Invalid drop syntax\n";
            }
        }
        else if(token[0]=="delete"){
            /*Sample input
            DELETE FROM table_name WHERE marks=90;*/
            if(token.size()==5 && token[1]=="from" && token[3]=="where"){
                string table_name=token[2];
                string cond=token[4];
                size_t pos = cond.find('=');
                string col_name=cond.substr(0,pos);
                string val=cond.substr(pos+1);
                deleteRowWithCond(table_name,col_name,val);
            }
            else{
                cout<<"Invalid Input\n";
            }
        }
        else if (token[0] == "exit") {
            exit(0);
        }
        else {
            cout << "Unknown command\n";
        }    
    }
    return 0;
}