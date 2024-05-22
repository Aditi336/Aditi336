#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Department {
public:
    void control_panel();
    void add_item();
    void display_item();
    void check_item();
    void updateItem();
    void delete_item();
};

void Department::control_panel() {
    cout << "\n1->ADD NEW ITEM\n2->PRINTING DATA\n3->SEARCH FOR PRODUCT\n4->UPDATE ITEM\n5->DELETE ITEM\n0->EXIT" << endl;
}

void Department::add_item() {
    fstream file;
    int item_id;
    string item_name;
    float cost_item;
    int number_item;
    float grams_item;

    cout << "\t\t\t\tADD NEW ITEM\n";
    cout << "---------------------------------------------\n";
    cout << "Enter the item ID: ";
    cin >> item_id;
    cin.ignore();  // Clear the newline character from the input buffer

    ifstream existingFile("department.csv");
    string line;
    bool idExists = false;

    while (getline(existingFile, line)) {
        stringstream ss(line);
        int id;
        char comma;

        ss >> id >> comma;
        if (id == item_id) {
            idExists = true;
            break;
        }
    }

    existingFile.close();

    if (idExists) {
        cout << "Item with ID " << item_id << " already exists. Please choose a different ID." << endl;
        return;
    }

    cout << "Enter the item name: ";
    getline(cin, item_name);
    cout << "Enter the cost of the item: ";
    cin >> cost_item;
    cout << "Enter the number of items: ";
    cin >> number_item;
    cout << "Enter the number of grams: ";
    cin >> grams_item;

    file.open("department.csv", ios::out | ios::app);
    file << item_id << "," << cost_item << "," << number_item << "," << grams_item << "," << item_name << endl;
    file.close();
}


void Department::check_item() {
    int item_code;
    cout << "\n\n\t\t\t\tCheck Specific Product\n";
    cout << "----------------------------------------------\n";
    cout << "Enter Item Code: ";
    cin >> item_code;

    ifstream file("department.csv");
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    string line;
    int count = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        int code, cost_item, number_item;
        float grams_item;
        string item_name;
        char comma;

        ss >> code >> comma >> cost_item >> comma >> number_item >> comma >> grams_item >> comma;
        getline(ss, item_name, '\n');

        if (code == item_code) {
            cout << "ID: " << code << ", Name: " << item_name << ", Cost: " << cost_item << ", Number: " << number_item << ", Grams: " << grams_item << endl;
            count++;
            break;
        }
    }

    if (count == 0) {
        cout << "Item Not Found...." << endl;
    }

    file.close();
}

void Department::display_item() {
    ifstream file("department.csv");
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    string line;
    cout << "-------------------------------------------------------------------------------------------------\n";
    cout << "Item ID\t\t" << "Cost of Item\t" << "Number of Items\t" << "Grams\t" << "Item Name\n";

    while (getline(file, line)) {
        stringstream ss(line);
        int item_id, number_item;
        float cost_item, grams_item;
        string item_name;
        char comma;

        ss >> item_id >> comma >> cost_item >> comma >> number_item >> comma >> grams_item >> comma;
        getline(ss, item_name, '\n');

        cout << item_id << "\t\t" << cost_item << "\t\t" << number_item << "\t\t" << grams_item << "\t" << item_name << endl;
    }

    cout << "-------------------------------------------------------------------------------------------------\n";
    file.close();
}

void Department::updateItem() {
    fstream fin, fout;
    fin.open("department.csv", ios::in);
    fout.open("departmentnew.csv", ios::out);
    cout << "-------------------------------------------------------------------------------------------------\n";
    int item_id, id1, number_item;
    string item_name;
    float cost_item, grams_item;
    string line;
    char comma;

    cout << "Enter the item ID to be updated: ";
    cin >> id1;

    while (getline(fin, line)) {
        stringstream ss(line);
        ss >> item_id >> comma >> cost_item >> comma >> number_item >> comma >> grams_item >> comma;

        getline(ss, item_name, '\n');

        if (item_id == id1) {
            cout << "Enter the new item name: ";
            cin.ignore(); // Clear the newline character
            getline(cin, item_name);
            cout << "Enter the new cost of item: ";
            cin >> cost_item;
            cout << "Enter the new number of items: ";
            cin >> number_item;
            cout << "Enter the new grams: ";
            cin >> grams_item;
        }

        fout << item_id << "," << cost_item << "," << number_item << "," << grams_item << "," << item_name << endl;

    }

    fin.close();
    fout.close();

    remove("department.csv");
    rename("departmentnew.csv", "department.csv");

    if (id1 == item_id) {
        cout << "Item with ID " << id1 << " updated successfully." << endl;
    } else {
        cout << "Item with ID " << id1 << " not found." << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------\n";
}

void Department::delete_item() {
    int item_id;
    cout << "\n\n\t\t\t\tDelete Specific Product\n";
    cout << "----------------------------------------------\n";
    cout << "Enter Item Code: ";
    cin >> item_id;

    ifstream fin("department.csv");
    ofstream fout("temp.csv", ios::out);

    if (!fin.is_open() || !fout.is_open()) {
        cout << "Error opening files." << endl;
        return;
    }

    string line;
    int count = 0;

    while (getline(fin, line)) {
        stringstream ss(line);
        int code, cost_item, number_item;
        float grams_item;
        string item_name;
        char comma;

        ss >> code >> comma >> cost_item >> comma >> number_item >> comma >> grams_item >> comma;
        getline(ss, item_name, '\n');

        if (code == item_id) {
            cout << "Record with ID " << code << " deleted." << endl;
            count++;
        } else {
            fout << line << endl;
        }
    }

    if (count == 0) {
        cout << "Item Not Found...." << endl;
    }

    fin.close();
    fout.close();

    remove("department.csv");
    rename("temp.csv", "department.csv");
}

int main() {
    Department d;
    int choice;
    std::cout << "\t\t\t###########################################################################" << std::endl;
    std::cout << "\n\t\t\t############                                                   ############" << std::endl;
    std::cout << "\n\t\t\t############         STORE DEPARTMENT MANAGEMENT               ############" << std::endl;
    std::cout << "\n\t\t\t############                                                   ############" << std::endl;
    std::cout << "\n\t\t\t###########################################################################" << std::endl;
    std::cout << "\n\t\t\t---------------------------------------------------------------------------" << std::endl;
    std::cout << "\n\n\n\n\n";
    std::cout << "\n\t\t\t                     ------------------\n";
    std::cout << "\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "\n\t\t\t        =                  WELCOME                  =" << std::endl;
    std::cout << "\n\t\t\t        =                  TO THE                   =" << std::endl;
    std::cout << "\n\t\t\t        =                 FRESH FARE                =" << std::endl;
    std::cout << "\n\t\t\t        =                   MARKET                  =" << std::endl;
    std::cout << "\n\t\t\t        =                                           =" << std::endl;
    std::cout << "\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
    std::cout << "\n\t\t\t                     ------------------\n";

    do {
        d.control_panel();
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                char x;
                do {
                    d.add_item();
                    cout << "Do you want to enter another item (y/n): ";
                    cin >> x;
                } while (x == 'y');
                break;

            case 2:
                d.display_item();
                break;

            case 3:
                d.check_item();
                break;

            case 4:
                d.updateItem();
                break;

            case 5:
                d.delete_item();
                break;

        }

    } while (choice != 0);

    return 0;
}
