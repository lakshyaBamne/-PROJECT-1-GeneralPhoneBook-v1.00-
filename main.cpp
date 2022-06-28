#include "PhoneBookContacts.h"

int main(){
    int totalEntries , uniqueID , statusOfRequest , statusOfRequest2;
    std::string name , contactNo , email;

    std::string uniqueIDstr;

    int selectedOption = 0;

    requestStatus status = _OK_;

    std::cout << "Welcome to General PhoneBooks!!" << std::endl;
    std::cout << "How can we help you today??\n" << std::endl;

    while(selectedOption != 6){
        std::cout << "1) Create an entry in the phonebook" << std::endl;
        std::cout << "2) Delete an entry from the phonebook" << std::endl;
        std::cout << "3) Access an entry using the uniquID" << std::endl;
        std::cout << "4) Access an entry using name" << std::endl;
        std::cout << "5) Edit an exiting entry" << std::endl;
        std::cout << "6) Exit program" << std::endl;

        std::cout << "\nPlease enter your choice - ";
        std::cin >> selectedOption;

        switch(selectedOption){

            case 1: //create an entry
                std::cout << "\n[CREATE ENTRY] Enter the following details in order to create an entry" << std::endl;
            
                std::cout << "                Name - ";
                std::cin.ignore();
                getline(std::cin , name);

                std::cout << "                Contact Number - ";
                std::cin >> contactNo;

                std::cout << "                Email - ";
                std::cin >> email;

                std::cout << "\nDetails acquired... Processing and creating an entry\n" << std::endl;
            
                uniqueID = createEntry(name , contactNo , email , &statusOfRequest);

                switch (statusOfRequest){
                    case _OK_: //operation successfull
                        std::cout << "[SUCCESS] Entry created successfully !!" << std::endl;
                        std::cout << "          Unique ID for the created entry is : " << uniqueID << std::endl;
                        std::cout << "          Please remember your uniqueID if you wish to make changes in the entry later" << std::endl;
                        break;
                    case _ERROR__BAD_EMAIL_: //ERROR - BAD EMAIL
                        std::cout << "[ERROR] Entry not created due to invalid details!!" << std::endl;
                        std::cout << "        entered Email is invalid!! try again" << std::endl;
                        break;
                    case _ERROR__BAD_CONTACT_: //ERROR - BAD CONTACT
                        std::cout << "[ERROR] Entry not created due to invalid details!!" << std::endl;
                        std::cout << "        entered contact number is invalid!! try again" << std::endl;
                        break;

                    default:
                        std::cout << "[ERROR] Could not process the request!! sorry for the inconvinience" << std::endl;
                        break;
                }

            break;
        
            case 2: //delete an entry
                std::cout << "[DELETE ENTRY] To delete an existing entry, enter the following details" << std::endl;

                std::cout << "               Unique ID - ";
                std::cin >> uniqueID;

                std::cout << "               Name - ";
                std::cin.ignore();
                getline(std::cin , name);

                name = nameConvert(name); //converting name to correct form in the text file for search.

                std::cout << "\nDetails acquired... Processing and deleting the entry\n" << std::endl;

                //function call to delte the entry.
                statusOfRequest =  deleteEntry(uniqueID , name , contactNo , email); 

                switch (statusOfRequest){
                    case _OK_:
                        std::cout << "[SUCCESS] The following entry has been successfully deleted" << std::endl;
                        std::cout << "          " << uniqueID << " " << name << " " << contactNo << " " << email << std::endl;
                        break;
                    case _ERROR__NOT_FOUND_:
                        std::cout << "[ERROR] The entry does not exist!! Try again with a valid entry" << std::endl;
                        break;
                    case _ERROR_:
                        std::cout << "[ERROR] invalid credentials !! given combination of  Unique ID and Name does not exist in our data base" << std::endl;
                        break;
            
                    default:
                        std::cout << "[ERROR] Could not process the request!! sorry for the inconvinience" << std::endl;
                        break;
                }

            break;
        
            case 3: //access an entry using uniquID
                std::cout << "\n\n";
                std::cout << "[ACCESS ENTRY] To access your entry, provide the Unique ID of the Contact:-" << std::endl;
                std::cout << "               Unique ID - ";
                std::cin >> uniqueID;

                std::cout << "\nDetails Acquired... Processing your request\n" << std::endl;

                //function call to retrieve data from the database
                statusOfRequest = accessEntryByUniquID(uniqueID , name , contactNo , email);

                switch (statusOfRequest){
                    case _OK_: //OK - entry retrieved
                        std::cout << "[ENTRY FOUND] The contact with the uniqueID " << uniqueID << " is as follows - " << std::endl;
                        std::cout << "              Name       - " << name << std::endl;
                        std::cout << "              Contact    - " << contactNo << std::endl;
                        std::cout << "              email      - " << email << std::endl;
                        std::cout << "\n\n";
                    break;

                    case _ERROR__NOT_FOUND_: // not found
                        std::cout << "[ERROR] No entry found with the entered Unique ID\n\n" << std::endl;
                    break;
                
                    default:
                        std::cout << "[ERROR] Could not process the request!! sorry for the inconvinience" << std::endl;
                    break;
                }                

            break;
        
            case 4: //access an entry using name
                std::cout << "\n\n";
                std::cout << "[ACCESS ENTRY] To access your entry, provide the Name of the Contact:-" << std::endl;
                std::cout << "               Name - ";
                std::cin.ignore();
                std::getline(std::cin , name);

                std::cout << "\nDetails Acquired... Processing your request\n" << std::endl;

                //function call to retrieve data from the database
                printEntriesByName(name);

                //multiple contacts with the same name may exist, so we need to show all to the user.
                //this is done automatically by the function, it goes through the data base
                //and prints all the relevant entries on its own.
                std::cout << "\n" << std::endl;

            break;
        
            case 5: //edit an existing entry
                std::cout << "[EDIT ENTRY] To edit an existing entry, enter the following details :- " << std::endl;
                
                std::cout << "             Unique ID - ";
                std::cin >> uniqueID;

                std::cout << "             Name - ";
                std::cin.ignore();
                getline(std::cin , name);
                std::cout << "\n";

                std::cout << "Details acquired, fetching your entry !!" << std::endl;

                statusOfRequest = findEntry(uniqueID , name);

                switch (statusOfRequest){
                    case _OK_: //entry found in data base
                        statusOfRequest2 = takeAndEditEntry(uniqueID , name , contactNo , email); //function call to edit the details

                        switch (statusOfRequest2){
                            case _OK_:
                                std::cout << "[SUCCESS] Entry updated" << std::endl;
                                std::cout << "          Entry after the update is as follows:-" << std::endl;

                                uniqueIDstr = std::to_string(uniqueID);

                                logEntryDetails(uniqueIDstr , name , contactNo , email);
                            break;

                            case _ERROR__NOT_FOUND_:
                                std::cout << "[ERROR] No entry found with the given Unique ID and Name combination!! Try again" << std::endl;
                            break;

                            case _ERROR__BAD_EMAIL_:
                                std::cout << "[ERROR] New email entered is invalid!! try again" << std::endl;
                            break;

                            case _ERROR__BAD_CONTACT_: 
                                std::cout << "[ERROR] New contact number entered is invalid!! try again" << std::endl;
                            break;
                        
                            default:
                                std::cout << "[ERROR] Could not process the request!! sorry for the inconvinience" << std::endl;
                            break;
                        }
                    break;

                    case _ERROR__NOT_FOUND_: //no such entry found in the data base
                        std::cout << "[ERROR] No entry found with the given Unique ID and Name combination!! Try again" << std::endl;
                    break;

                    case _ERROR_: //bad credentials
                        std::cout << "[ERROR] Bad credentials!! try again" << std::endl;
                    break;
                
                    default: //unknown error - bug
                        std::cout << "[ERROR] Could not process the request!! sorry for the inconvinience" << std::endl;
                    break;
                }

            break;
            
            case 6: //exit from the program
                std::cout << "Thankyou for using General PhoneBook :)" << std::endl;
            break;
            
            default:
                std::cout << "[ERROR] No valid option selected!! pls try again" << std::endl;
        }   
    }

    

    return 0;
}