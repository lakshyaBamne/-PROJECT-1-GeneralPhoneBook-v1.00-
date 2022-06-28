# -PROJECT-1-GeneralPhoneBook-v1.00-

# THIS IS A PERSONAL PROJECT FOR MY OWN PRACTICE OF FILE HANDLING AND FUNCTIONS IN C++, HOWEVER CONTRIBUTIONS FROM ANYONE TRYING TO IMPROVE ON THE PROJECT ARE WELCOME.

# BASIC CONCEPT BEHIND THE PROGRAM -
  * a phonebook is used to store the contact information of people, this project accomplishes this task in the most basic way using functions and file handling.
  * the program can accomplish basic - Creates , Reads , Updates , Deletes (CRUD) operations using only a text file.
  * a blank text file with the name "contactlog.txt" needs to be present in the root directory on the main translation unit "main.cpp" along with the main header file(which contains all the functions used) "PhoneBookContacts.h"
  * the program in it's release version v1.00 gives 5 options to the user as follows:-
    1) CREATE an entry which will have the following fields - UNIQUE ID , NAME , CONTACT NUMBER , EMAIL
    2) DELETE an entry by providing a valid ( UNIQUE ID , NAME ) combination which is only given to the user when an entry is created.
    3) ACCESS(READ) an entry by the UNIQUE ID
    4) ACCESS(READ) an entry by the NAME
    5) EDIT(UPDATE) an entry by providing a valid ( UNIQUE ID , NAME ) combination which is only given to the user when an entry is created.
    
# EXACT PROCESS BY WHICH THIS TAKES PLACE CAN BE FOUND IN THE WELL DOCUMENTED FUNCTIONS PRESENT IN THE HEADER FILE - "PhoneBookContacts.h"
# IN ESSENCE - A SINGULAR ENTRY IS STORED IN THE TEXT FILE IN A SINGLE LINE IN A VERY SPECIFIC PATTERN AND ALL OPERATIONS ARE DONE BY EXPLOITING THIS PATTERN

# THIS IS NOT THE BEST WAY TO CREATE A PHONEBOOK, BUT I CHOSE THIS METHOD JUST FOR PRACTICE PURPOSES
# LOOKING FORWARD TO REVIEWS FROM FELLOW PROGRAMMERS.
