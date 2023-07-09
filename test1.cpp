#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const int TABLE_SIZE = 10;
const int MAX_ENTRIES_PER_INDEX = 4;

struct Employee {
    int employeeId;
    std::string firstName;
    std::string lastName;
    int leaveCount;
};

struct HashNode {
    Employee employee;
    HashNode* next;
};

class HashTable {
private:
    std::vector<HashNode*> table;

public:
    HashTable(const std::string& filename) {
        table.resize(TABLE_SIZE, nullptr);
        readFromFile(filename);
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    //new function to read from file
void readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string employeeIdStr, firstName, lastName, leaveCountStr;
            if (std::getline(iss, employeeIdStr, ',') &&
                std::getline(iss, firstName, ',') &&
                std::getline(iss, lastName, ',') &&
                std::getline(iss, leaveCountStr)) {
                int employeeId = std::stoi(employeeIdStr);
                int leaveCount = std::stoi(leaveCountStr);
                insert(employeeId, firstName, lastName, leaveCount);
            }
        }

        file.close();
        std::cout << "Data loaded from file successfully." << std::endl;
    } else {
        std::cout << "file not found." << std::endl;
    }
}



    int hashFunction(int employeeId) {
        return employeeId % TABLE_SIZE;
    }

      bool validate(int employeeId) {
    int index = hashFunction(employeeId);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->employee.employeeId == employeeId) {
            return true;  // Employee ID already exists
        }
        current = current->next;
    }
    return false;  // Employee ID not found
}

    void insert(int employeeId, const std::string& firstName, const std::string& lastName, int leaveCount) {
        int index = hashFunction(employeeId);
        //validation to avoid duplicate employee id entry
         if (validate(employeeId)) {
        std::cout << "Employee with ID " << employeeId << " already exists." << std::endl;
        return;
    }
        // Check if the current index has reached the maximum entries limit
        if (getEntryCount(index) == MAX_ENTRIES_PER_INDEX) {
            std::cout << "Maximum entries reached for index " << index << ". Moving to the next index." << std::endl;
            index = getNextAvailableIndex(index);
        }

        HashNode* newNode = new HashNode;
        newNode->employee.employeeId = employeeId;
        newNode->employee.firstName = firstName;
        newNode->employee.lastName = lastName;
        newNode->employee.leaveCount = leaveCount;
        newNode->next = nullptr;

        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            HashNode* current = table[index];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    int getEntryCount(int index) {
        int count = 0;
        HashNode* current = table[index];
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    int getNextAvailableIndex(int index) {
        int nextIndex = (index + 1) % TABLE_SIZE; // Wrap around to the beginning if end is reached
        while (nextIndex != index && getEntryCount(nextIndex) == MAX_ENTRIES_PER_INDEX) {
            nextIndex = (nextIndex + 1) % TABLE_SIZE;
        }
        return nextIndex;
    }

    void remove(int employeeId) {
        bool found = false;

        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (table[i] == nullptr) {
                continue;
            }

            if (table[i]->employee.employeeId == employeeId) {
                HashNode* temp = table[i];
                table[i] = table[i]->next;
                delete temp;
                std::cout << "Employee with ID " << employeeId << " deleted." << std::endl;
                found = true;
                break;
            }

            HashNode* current = table[i];
            while (current->next != nullptr && current->next->employee.employeeId != employeeId) {
                current = current->next;
            }

            if (current->next != nullptr && current->next->employee.employeeId == employeeId) {
                HashNode* temp = current->next;
                current->next = current->next->next;
                delete temp;
                std::cout << "Employee with ID " << employeeId << " deleted." << std::endl;
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "Employee with ID " << employeeId << " not found." << std::endl;
        }
    }

    void modify(int employeeId, int newLeaveCount) {
        bool found = false;

        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode* current = table[i];
            while (current != nullptr) {
                if (current->employee.employeeId == employeeId) {
                    current->employee.leaveCount = newLeaveCount;
                    std::cout << "Leave count modified for employee with ID " << employeeId << std::endl;
                    found = true;
                    break;
                }
                current = current->next;
            }
            if (found) {
                break;
            }
        }

        if (!found) {
            std::cout << "Employee with ID " << employeeId << " not found." << std::endl;
        }
    }

  void search(int employeeId) {
    bool found = false;

    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* current = table[i];
        while (current != nullptr) {
            if (current->employee.employeeId == employeeId) {
                std::cout << "Employee with ID " << employeeId << " found. Details:" << std::endl;
                std::cout << "First Name: " << current->employee.firstName << std::endl;
                std::cout << "Last Name: " << current->employee.lastName << std::endl;
                std::cout << "Leave Count: " << current->employee.leaveCount << std::endl;
                found = true;
                break;
            }
            current = current->next;
        }
        if (found) {
            break;
        }
    }

    if (!found) {
        std::cout << "Employee with ID " << employeeId << " not found." << std::endl;
    }
}

    void writeToFile(const std::string& filename) {
        std::ofstream file(filename);

        if (file.is_open()) {
            for (int i = 0; i < TABLE_SIZE; ++i) {
                HashNode* current = table[i];
                while (current != nullptr) {
                    file << current->employee.employeeId << ","
                         << current->employee.firstName << ","
                         << current->employee.lastName << ","
                         << current->employee.leaveCount << std::endl;
                    current = current->next;
                }
            }

            file.close();
            std::cout << "Data written to file successfully." << std::endl;
        } else {
            std::cout << "Unable to open file." << std::endl;
        }
    }

    void display() {
        std::cout << "Hash Table Contents:" << std::endl;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (table[i] == nullptr) {
                std::cout << "Index: " << i << " | Empty" << std::endl;
            } else {
                std::cout << "Index: " << i << " | ";
                HashNode* current = table[i];
                while (current != nullptr) {
                    std::cout << "Employee ID: " << current->employee.employeeId
                              << " | First Name: " << current->employee.firstName
                              << " | Last Name: " << current->employee.lastName
                              << " | Leave Count: " << current->employee.leaveCount << " -> ";
                    current = current->next;
                }
                std::cout << "nullptr" << std::endl;
            }
        }
    }

    void displayFileContents(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        std::cout << "File Contents:" << std::endl;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    }

    void displayIndexRecords(int index) {
        std::cout << "Records at Index " << index << ":" << std::endl;
        if (table[index] == nullptr) {
            std::cout << "No records found at Index " << index << std::endl;
        } else {
            HashNode* current = table[index];
            while (current != nullptr) {
                std::cout << "Employee ID: " << current->employee.employeeId
                          << " | First Name: " << current->employee.firstName
                          << " | Last Name: " << current->employee.lastName
                          << " | Leave Count: " << current->employee.leaveCount << std::endl;
                current = current->next;
            }
        }
    }
};

int main() {
    std::string filename = "emp.txt"; // Specify the filename to read 
HashTable hashTable(filename);
    int choice;
    int employeeId;
    std::string firstName;
    std::string lastName;
    int leaveCount;

    do {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Add employee leave data" << std::endl;
        std::cout << "2. Remove employee leave data" << std::endl;
        std::cout << "3. Modify employee leave count" << std::endl;
        std::cout << "4. Search employee leave data" << std::endl;
        std::cout << "5. Write to file" << std::endl;
        std::cout << "6. Display Hash Table" << std::endl;
        std::cout << "7. Display File Contents" << std::endl;
        std::cout << "8. Display Index Records" << std::endl;
        std::cout << "9. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter employee ID: ";
                std::cin >> employeeId;
                std::cout << "Enter first name: ";
                std::cin >> firstName;
                std::cout << "Enter last name: ";
                std::cin >> lastName;
                std::cout << "Enter leave count: ";
                std::cin >> leaveCount;
                hashTable.insert(employeeId, firstName, lastName, leaveCount);
                break;
            case 2:
                std::cout << "Enter employee ID to remove: ";
                std::cin >> employeeId;
                hashTable.remove(employeeId);
                break;
            case 3:
                std::cout << "Enter employee ID to modify: ";
                std::cin >> employeeId;
                std::cout << "Enter new leave count: ";
                std::cin >> leaveCount;
                hashTable.modify(employeeId, leaveCount);
                break;
            case 4:
                std::cout << "Enter employee ID to search: ";
                std::cin >> employeeId;
                hashTable.search(employeeId);
                break;
            case 5:
                /* std::cout << "Enter filename: ";
                std::cin >> filename; */
                std::cout<<"the data will be written to the emp.txt file \n";
                hashTable.writeToFile(filename);
                break;
            case 6:
                hashTable.display();
                break;
            case 7:
                /* std::cout << "Enter filename: ";
                std::cin >> filename; */
                std:: cout<< " the data stored in the file emp.txt is"<<"\n";
                hashTable.displayFileContents(filename);
                break;
            case 8:
                std::cout << "Enter index: ";
                std::cin >> employeeId;
                hashTable.displayIndexRecords(employeeId);
                break;
            case 9:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }

        std::cout << std::endl;

    } while (choice != 9);

    return 0;
}
