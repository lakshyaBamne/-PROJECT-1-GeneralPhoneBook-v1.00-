//header guide that prevents inclusion of the same header file multiple times in the same translation unit
#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<cctype>
#include<vector>

//enum to represent the status of a check or request
enum requestStatus{ _OK_=200 ,
                    _ERROR_ = 400 ,
                    _ERROR__BAD_EMAIL_ = 401 , 
                    _ERROR__BAD_CONTACT_ = 402 ,
                    _ERROR__NOT_FOUND_ = 404 };

//function takes a string and replaces all the spaces in it with hyphens '-' and lowercases everything.
//function tested and works fine.
std::string nameConvert(std::string name){
    for(int i=0 ; i<name.length() ; i++){
        if(int(name[i]) == 32){
            name[i] = '-';
        }

        name[i] = char( tolower( int( name[i] ) ) );
    }

    return name;
}

//function takes a string and replaces all the hyphens '-' with spaces
//function tested and works fine.
std::string nameReConvert(std::string nameToBeChanged){
    nameToBeChanged[0] = char( int(nameToBeChanged[0]) - 32 ); //capitalizes first character
    
    for(int i=0 ; i<nameToBeChanged.length() ; i++){
        if(nameToBeChanged[i] == '-'){
            nameToBeChanged[i] = ' ';
            nameToBeChanged[i+1] = char( toupper(nameToBeChanged[i+1]) );
        }
    }

    return nameToBeChanged;
}

//function to check if entered email is of correct form
//function tested and works fine
int emailFormCheck(std::string emailToBeChecked){
    char atSymbol = '@';
    int numAtInStr = 0;
    int checkStatus;

    for(int i=0 ; i<emailToBeChecked.length() ; i++){
        if(emailToBeChecked[i] == atSymbol){
            numAtInStr++;

            if( i>0 && i<(emailToBeChecked.length()-1) ){
                checkStatus = 1;
            }
            else{
                checkStatus = 0;
            }
        }
    }
    
    if(numAtInStr > 1){
        checkStatus = 0;
    }

    if(checkStatus == 1){
        return 200; //email ok
    }
    else{
        return 401; //bad email
    }
}

//function to check if entered contact is of correct form
//for a contact to be correct:-
//length should be 10
//all characters of the string should be digits
//function tested and works fine
int contactFormCheck(std::string contactToBeChecked){
    int checkLen = 0, checkDig = 0;

    if(contactToBeChecked.length() == 10){
        checkLen = 1;
    }

    for(int i=0 ; i<10 ; i++){
        if( int(contactToBeChecked[i])>47 && int(contactToBeChecked[i])<58 ){ //in correct ASCII range
            checkDig++;
        }
    }

    if(checkLen==1 && checkDig==10){
        return 200; //correct contact
    }
    else{
        return 402; //bad contact
    }
}

//function than calculates the number of lines in a text file
//since one line has one entry it calculates number of entries in the phonebook
//function tested and works fine.
int totalEntriesCheck(){
    int numEntries=0;
    std::string oneLine;

    std::ifstream readFromFile;
    readFromFile.open("contactlog.txt");

    while(readFromFile.eof() == 0){
        getline(readFromFile , oneLine);
        numEntries++;
    }
    
    readFromFile.close();

    return numEntries;
}

//function to create an entry in the text file and then return the uniqueID to the user for later reference
//function tested and works fine.
int createEntry(std::string name , std::string contactNo , std::string email , int* statusOfRequest){
    int uniqueID = totalEntriesCheck();
    int checkStatusEmail;
    int checkStatusContact;

    name = nameConvert(name); //convert the name to suitable form to be appended in the phonebook.
    
    checkStatusEmail = emailFormCheck(email);           //check if entered email is valid
    checkStatusContact = contactFormCheck(contactNo);   //check if entered contact number is valid

    if(checkStatusEmail==200 && checkStatusContact==200){
        //valid entries thus function progresses to create the entry.
    }
    else if(checkStatusEmail == 401){
        *statusOfRequest = 401;
        return 0; //when entry is not created, 0 is returned
    }
    else if(checkStatusContact == 402){ 
        *statusOfRequest = 402;
        return 0; //when entry is not created, 0 is returned
    }

    //actual writing happens
    std::ofstream writeToFile;
    //extra argument is added so that when writing to a file, the pre existing text is not deleted.
    writeToFile.open("contactlog.txt" , std::ios::out|std::ios::app);

    writeToFile << uniqueID << " " << name << " " << contactNo << " " << email << "\n"; 

    writeToFile.close();
    
    *statusOfRequest = 200;
    return uniqueID;
}

//function to delete an entry in the text file
//to delete entry, user must input both uniqueID and name correctly
//function returns 200 if all ok
//function returns 400 if entry invalid
//function tested and works fine.
int deleteEntry(int uniqueID , std::string name , std::string& contact , std::string& email){
    std::ifstream readFromFile;
    readFromFile.open("contactlog.txt"); //main file open in read mode

    std::ofstream writeToFile;  
    //contents will be written to this file and then this file will replace the original file.
    writeToFile.open("new.txt");

    std::string uniqueIDinDB;
    std::string uniqueIDstr = std::to_string(uniqueID); //converting unique integer to string for search
    std::string tempName = nameConvert(name);           //we need to convert name to the form in contact file
    std::string nameInDB; //name stored in the contact file.

    std::string oneline;
    int emailLen;
    int check = 0;
    int indexOfSpace , bufferIndex;

    while(readFromFile.eof() == 0){
        getline(readFromFile , oneline);
        indexOfSpace = oneline.find(' ');
        bufferIndex = indexOfSpace;

        uniqueIDinDB = oneline.substr(0 , indexOfSpace);

        if(uniqueIDinDB == uniqueIDstr){
            indexOfSpace = oneline.find(' ' , indexOfSpace+1); //space character at the end of name and start of contact.
            nameInDB = oneline.substr(bufferIndex+1 , indexOfSpace - (bufferIndex+1));
            if(tempName == nameInDB){  //entry is present and can be deleted
                //uniqueID is already given by the user and can be returned.
                //name is already given by the user and can be returned.
                //contact and email need to be fetched from the file before deleting the entry.
                contact = oneline.substr(indexOfSpace+1 , 10); //updating contact for showing
            
                emailLen = (oneline.length()-1) - (indexOfSpace+11); //finding length of email
                email = oneline.substr(indexOfSpace+12 , emailLen);  //updating email for showing

                //essentially the following two lines delete the entry.
                oneline.replace(0 , oneline.length() , "[THIS ENTRY HAS BEEN DELETED]\n");
                writeToFile << oneline;

                check = 1;
            }
            else{
                readFromFile.close();
                writeToFile.close();
                //all other lines are copied without any change
                return 400; //ERROR - invalid credentials
            }
        }
        else{
            writeToFile << oneline << "\n";
        }
    }

    readFromFile.close();
    writeToFile.close();

    remove("contactlog.txt");               //original file is discarded
    rename("new.txt" , "contactlog.txt");   //new file has updated entries

    if(check == 0){ //no entry found with the given unique ID
        return 404; //ERROR-entry not found
    }
    else{
        return 200; //OK-entry deleted
    }
}

//function to access an entry using its Unique ID
//function returns an integer representing the status of the request.
//function tested and works fine.
int accessEntryByUniquID(int uniqueID , std::string& name , std::string& contact , std::string& email){
    std::ifstream readFromFile;
    readFromFile.open("contactlog.txt"); //contact file opened in read mode

    std::string uniqueIDstr = std::to_string(uniqueID);
    std::string uniqueIDinDB;
    std::string oneline;
    std::string tempName;

    int check = 0;
    int emailLen;
    int bufferIndex;

    while(readFromFile.eof() == 0){
        getline(readFromFile , oneline);
        int indexOfSpace = oneline.find(' '); //first space is always at index 1 in the contactlog file
        bufferIndex = indexOfSpace;

        uniqueIDinDB = oneline.substr(0 , indexOfSpace); //uniqueID of an entry in the data base

        if(uniqueIDstr == uniqueIDinDB){ //required entry
            indexOfSpace = oneline.find(' ' , bufferIndex+1); //index of space at the end of name and start of email
            tempName = oneline.substr(bufferIndex+1 , indexOfSpace-(bufferIndex+1)); //name as present in the data base

            name = nameReConvert(tempName);

            contact = oneline.substr(indexOfSpace+1 , 10); //updating contact for showing
            
            emailLen = (oneline.length()-1) - (indexOfSpace+11); //finding length of email
            email = oneline.substr(indexOfSpace+12 , emailLen);  //updating email for showing

            check = 1;
            return 200; //OK-entry retrieved
        }
    }

    readFromFile.close();

    if(check == 0){ //required entry not found
        return 404; //ERROR - not found
    }
}

//function that logs the details of an entry in the correct format when fiven the details as arguments
//function tested and works fine.
void logEntryDetails(const std::string& uniqueID , const std::string& name ,const std::string& contactNo ,const std::string& email){
    std::cout << "Unique ID  - " << uniqueID << std::endl;
    std::cout << "Name       - " << name << std::endl;
    std::cout << "Contact    - " << contactNo << std::endl;
    std::cout << "email      - " << email << std::endl;
    std::cout << "\n\n";
}

//function to access an entry using its Name
//function tested and works fine.
void printEntriesByName(std::string name){

    std::string tempName = nameConvert(name); //converting name to a form stored in the data base
    
    std::ifstream readFromFile;
    readFromFile.open("contactlog.txt"); //main contact log file opened in read mode

    std::string oneline;

    std::string nameInDB;

    std::string uniqueIDstr;
    std::string contactNo;
    std::string email;

    int indexOfSpace , bufferIndex;
    int entryCount = 0; //number of entries found with the given name

    while(readFromFile.eof() == 0){
        getline(readFromFile , oneline);

        indexOfSpace = oneline.find(' ');
        bufferIndex = indexOfSpace;

        indexOfSpace = oneline.find(' ' , bufferIndex+1);

        nameInDB = oneline.substr(bufferIndex+1 , indexOfSpace-(bufferIndex+1));

        if(nameInDB == tempName){
            entryCount++;

            uniqueIDstr = oneline.substr(0 , bufferIndex); //unique ID of the required entry
            contactNo = oneline.substr(indexOfSpace+1 , 10);
            email = oneline.substr(indexOfSpace+12);

            std::cout << "[ENTRY FOUND]" << std::endl;
            logEntryDetails(uniqueIDstr , name , contactNo , email); //logging the found entry
        }
    }

    if(entryCount == 0){ //no entry found with the given name
        std::cout << "[ERROR] No entry found by the given name !! Try again" << std::endl; 
    }
}

//function that finds a given entry by its Unique ID and Name
//returns 200 if entry is found
//returns 404 if entry is not found
int findEntry(int uniqueID , std::string name){
    std::ifstream readFromFile;
    readFromFile.open("contactlog.txt"); //main file open in read mode

    std::string uniqueIDinDB;
    std::string uniqueIDstr = std::to_string(uniqueID); //converting unique integer to string for search
    std::string tempName = nameConvert(name);           //we need to convert name to the form in contact file
    std::string nameInDB; //name stored in the contact file.

    std::string oneline;

    int check = 0;
    int indexOfSpace , bufferIndex;

    while(readFromFile.eof() == 0){
        getline(readFromFile , oneline);
        indexOfSpace = oneline.find(' ');
        bufferIndex = indexOfSpace;

        uniqueIDinDB = oneline.substr(0 , indexOfSpace);

        if(uniqueIDinDB == uniqueIDstr){
            indexOfSpace = oneline.find(' ' , bufferIndex+1); //space character at the end of name and start of contact.
            nameInDB = oneline.substr(bufferIndex+1 , indexOfSpace - (bufferIndex+1));
            if(tempName == nameInDB){  //entry is present and can be deleted
                readFromFile.close();
                check = 1;
                return 200; //entry found
            }
            else{
                readFromFile.close();
                return 400; //ERROR - invalid credentials
            }
        }
    }

    readFromFile.close();

    if(check == 0){ //no entry found with the given unique ID
        return 404; //ERROR-entry not found
    }
}

//function to edit an entry in the database
//given uniqueID and name as arguments
//function asks the user to input new details and then updates them
//if entry is present, asks the user to input new details
//after updation returns 200
//if entry is not present, returns 404
int takeAndEditEntry(int uniqueID , std::string& name , std::string& contact , std::string& email){
    std::cout << "[ENTRY FOUND] please enter new details for the entry:-" << std::endl;
    
    std::string newName;

    std::cout << "              Name - ";
    getline(std::cin , newName);

    std::string tempName = nameConvert(newName); //converted into a form present in the data base

    int contactCheckRes , emailCheckRes;

    std::cout << "              Contact No - ";
    std::cin >> contact;
    contactCheckRes = contactFormCheck(contact); //the new contact entered should be of correct format

    if(contactCheckRes != 200){
        return 402; //bad contact
    }
    
    std::cout << "              email - ";
    std::cin >> email;
    emailCheckRes = emailFormCheck(email); //the new email entered should be of correct format

    if(emailCheckRes != 200){
        return 401; //bad email.
    }

    //now we have to replace the already existing values with the new entries
    std::string uniqueIDstr = std::to_string(uniqueID);
    std::string uniqueIdInDB;
    
    std::string newEntry = "";

    std::ifstream readFromFile;
    readFromFile.open("contactlog.txt"); //opening the original file in read mode

    std::ofstream writeToFile;
    writeToFile.open("new.txt");  //opening a new file and copying the updates into it.

    int indexToSpace , bufferIndex;
    int emailLen;
    int check = 0;

    std::string oneline;

    while(readFromFile.eof() == 0){
        getline(readFromFile , oneline);

        indexToSpace = oneline.find(' '); //finds and stores the first instance of a space in the entry
        bufferIndex = indexToSpace;

        uniqueIdInDB = oneline.substr(0 , indexToSpace);

        if(uniqueIdInDB == uniqueIDstr){
            check = 1;

            std::cout << uniqueIDstr << std::endl;
            //since we know the correct entry exists, we can proceed with updating the details
            newEntry = newEntry + uniqueIDstr + " " + tempName + " " + contact + " " + email + "\n";
            //all new details have been added in the line newEntry

            oneline.replace(0 , oneline.length() , newEntry);
            writeToFile << oneline;

        }
        else{
            writeToFile << oneline << "\n";
        }
    }

    readFromFile.close();
    writeToFile.close();

    remove("contactlog.txt");               //original file is discarded
    rename("new.txt" , "contactlog.txt");   //new file has updated entries

    if(check == 0){ //entry not found, this is extra check to be safe
        return 404; //entry not found to be edited
    }
    else{
        return 200; //entry updated successfully
    }
}