#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <conio.h>

using namespace std;

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

//Classes
class Person {
public:
    int id;
    string name;
    int age;
    char gender;
};

class Patient : public Person { };

class Doctor : public Person {
public:
    string specialty;
};

class Appointment {
public:
    int id;
    int patientId;
    int doctorId;
    string date; 
    float fee;
};

//Global Variables
vector<Patient> patients;
vector<Doctor> doctors;
vector<Appointment> appointments;

void clearScreen() {
    system(CLEAR);
}


bool login() {
    string username, password;
    cin.ignore();
    cout << "Username: "; cin >> username;
    cout << "Password: ";
    
    char ch;
    password = "";
    while ((ch = _getch()) != 13) {
        if (ch == 8) { 
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; 
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;

    if(username == "CMS" && password == "Hwic@123") {
        cout << "Login successful! Press Enter..."; cin.ignore(); cin.get();
        return true;
    }
    cout << "Invalid credentials! Press Enter..."; cin.ignore(); cin.get();
    return false;
}

//Login Menu 
bool loginMenu() {
    int choice;
    while(true){
        clearScreen();
        cout << "----------------------------------------\n";
        cout << "| Welcome to Clinic Management System  |\n";
        cout << "----------------------------------------\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if(choice == 1){
            if(login()) return true;
        } else if(choice == 2){
            return false;
        } else {
            cout << "Invalid choice! Press Enter..."; cin.ignore(); cin.get();
        }
    }
}

//File Handling
void savePatients() {
    ofstream file("patients.txt");
    for(auto &p : patients)
        file << p.id << "," << p.name << "," << p.age << "," << p.gender << "\n";
}

void loadPatients() {
    ifstream file("patients.txt");
    if(!file) return;
    patients.clear();
    string line;
    while(getline(file,line)){
        Patient p;
        size_t pos = line.find(',');
        p.id = stoi(line.substr(0,pos)); line.erase(0,pos+1);
        pos = line.find(',');
        p.name = line.substr(0,pos); line.erase(0,pos+1);
        pos = line.find(',');
        p.age = stoi(line.substr(0,pos)); line.erase(0,pos+1);
        p.gender = line[0];
        patients.push_back(p);
    }
}

void saveDoctors() {
    ofstream file("doctors.txt");
    for(auto &d:doctors)
        file << d.id << "," << d.name << "," << d.age << "," << d.gender << "," << d.specialty << "\n";
}

void loadDoctors() {
    ifstream file("doctors.txt");
    if(!file) return;
    doctors.clear();
    string line;
    while(getline(file,line)){
        Doctor d;
        size_t pos = line.find(',');
        d.id = stoi(line.substr(0,pos)); line.erase(0,pos+1);
        pos = line.find(',');
        d.name = line.substr(0,pos); line.erase(0,pos+1);
        pos = line.find(',');
        d.age = stoi(line.substr(0,pos)); line.erase(0,pos+1);
        pos = line.find(',');
        d.gender = line[0]; line.erase(0,pos+1);
        d.specialty = line;
        doctors.push_back(d);
    }
}

void saveAppointments() {
    ofstream file("appointments.txt");
    for(auto &a:appointments)
        file << a.id << "," << a.patientId << "," << a.doctorId << "," << a.date << "," << a.fee << "\n";
}

void loadAppointments() {
    ifstream file("appointments.txt");
    if(!file) return;
    appointments.clear();
    string line;
    while(getline(file,line)){
        Appointment a;
        size_t pos = line.find(',');
        a.id = stoi(line.substr(0,pos)); line.erase(0,pos+1);
        pos = line.find(',');
        a.patientId = stoi(line.substr(0,pos)); line.erase(0,pos+1);
        pos = line.find(',');
        a.doctorId = stoi(line.substr(0,pos)); line.erase(0,pos+1);
        pos = line.find(',');
        a.date = line.substr(0,pos); line.erase(0,pos+1);
        a.fee = stof(line);
        appointments.push_back(a);
    }
}

//Input Validation
int getValidInt(string prompt,int min,int max){
    int val;
    while(true){
        cout << prompt;
        cin >> val;
        if(cin.fail() || val < min || val > max){
            cout << "Invalid input. Try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        } else break;
    }
    return val;
}

char getValidGender(){
    char g;
    do{
        cout << "Gender (M/F/O): ";
        cin >> g;
        g = toupper(g);
    }while(g!='M' && g!='F' && g!='O');
    return g;
}

string getValidDate(){
    int year = getValidInt("Enter Year (YYYY): ",2000,2100);
    int month = getValidInt("Enter Month (1-12): ",1,12);
    int day = getValidInt("Enter Day (1-30): ",1,30);
    char buffer[11];
    sprintf(buffer,"%04d/%02d/%02d",year,month,day);
    return string(buffer);
}

//Patient Management
void patientManagement(){
    int choice;
    while(true){
        clearScreen();
        cout<<"--- Patient Management ---\n";
        cout<<"1. Add Patient\n2. View All\n3. Delete\n4. Search\n5. Edit\n6. Back\nChoice: ";
        cin>>choice; cin.ignore();
        if(choice==1){
            Patient p;
            p.id = patients.empty()?1:patients.back().id+1;
            cout<<"Name: "; getline(cin,p.name);
            p.age = getValidInt("Age: ",0,150);
            p.gender = getValidGender();
            patients.push_back(p); savePatients();
            cout<<"Patient added. Press Enter..."; cin.get();
        } else if(choice==2){
            if(patients.empty()) cout<<"No patients.\n";
            else for(auto &p:patients) cout<<p.id<<" | "<<p.name<<" | "<<p.age<<" | "<<p.gender<<"\n";
            cout<<"Press Enter..."; cin.get();
        } else if(choice==3){
            int id=getValidInt("Patient ID to delete: ",1,10000); bool found=false;
            for(size_t i=0;i<patients.size();i++){
                if(patients[i].id==id){
                    for(size_t j=0;j<appointments.size();)
                        if(appointments[j].patientId==id) appointments.erase(appointments.begin()+j);
                        else j++;
                    patients.erase(patients.begin()+i);
                    savePatients(); saveAppointments(); found=true; break;
                }
            }
            if(!found) cout<<"Patient not found.\n"; cin.ignore(); cin.get();
        } else if(choice==4){
            cin.ignore(); string name; cout<<"Enter patient name to search: "; getline(cin,name);
            bool found=false;
            for(auto &p:patients) if(p.name==name){ cout<<p.id<<" | "<<p.name<<" | "<<p.age<<" | "<<p.gender<<"\n"; found=true;}
            if(!found) cout<<"Patient not found.\n"; cout<<"Press Enter..."; cin.get();
        } else if(choice==5){
            int id=getValidInt("Patient ID to edit: ",1,10000); bool found=false;
            for(auto &p:patients) if(p.id==id){
                found=true; cin.ignore();
                string n; cout<<"New Name (Enter to keep): "; getline(cin,n); if(!n.empty()) p.name=n;
                int a=getValidInt("New Age (0 to keep): ",0,150); if(a>0) p.age=a;
                char c; cout<<"Change Gender? (Y/N): "; cin>>c; if(toupper(c)=='Y') p.gender=getValidGender();
                savePatients(); break;
            }
            if(!found) cout<<"Patient not found.\n"; cin.ignore(); cin.get();
        } else if(choice==6) break;
    }
}

// Doctor Management
void doctorManagement(){
    int choice;
    while(true){
        clearScreen();
        cout << "--- Doctor Management ---\n";
        cout << "1. Add Doctor\n";
        cout << "2. View All\n";
        cout << "3. Delete\n";
        cout << "4. Search\n";
        cout << "5. Edit\n";
        cout << "6. Back\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // ---------------- ADD DOCTOR ----------------
        if(choice == 1){
            Doctor d;
            d.id = doctors.empty() ? 1 : doctors.back().id + 1;

            cout << "Name: ";
            getline(cin, d.name);

            d.age = getValidInt("Age: ", 0, 150);
            d.gender = getValidGender();

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ? important

            cout << "Specialty: ";
            getline(cin, d.specialty);

            doctors.push_back(d);
            saveDoctors();

            cout << "Doctor added successfully. Press Enter...";
            cin.get();
        }

        // ---------------- VIEW ALL ----------------
        else if(choice == 2){
            if(doctors.empty()){
                cout << "No doctors found.\n";
            } else {
                for(auto &d : doctors){
                    cout << d.id << " | "
                         << d.name << " | "
                         << d.age << " | "
                         << d.gender << " | "
                         << d.specialty << "\n";
                }
            }
            cout << "Press Enter...";
            cin.get();
        }

        // ---------------- DELETE ----------------
        else if(choice == 3){
            int id = getValidInt("Doctor ID to delete: ", 1, 10000);
            bool found = false;

            for(size_t i = 0; i < doctors.size(); i++){
                if(doctors[i].id == id){
                    // remove related appointments
                    for(size_t j = 0; j < appointments.size(); ){
                        if(appointments[j].doctorId == id)
                            appointments.erase(appointments.begin() + j);
                        else
                            j++;
                    }

                    doctors.erase(doctors.begin() + i);
                    saveDoctors();
                    saveAppointments();
                    found = true;
                    break;
                }
            }

            if(!found)
                cout << "Doctor not found.\n";

            cout << "Press Enter...";
            cin.get();
        }

        // ---------------- SEARCH ----------------
        else if(choice == 4){
            string name;
            cout << "Enter doctor name to search: ";
            getline(cin, name);

            bool found = false;
            for(auto &d : doctors){
                if(d.name == name){
                    cout << d.id << " | "
                         << d.name << " | "
                         << d.age << " | "
                         << d.gender << " | "
                         << d.specialty << "\n";
                    found = true;
                }
            }

            if(!found)
                cout << "Doctor not found.\n";

            cout << "Press Enter...";
            cin.get();
        }

        // ---------------- EDIT ----------------
        else if(choice == 5){
            int id = getValidInt("Doctor ID to edit: ", 1, 10000);
            bool found = false;

            for(auto &d : doctors){
                if(d.id == id){
                    found = true;

                    string n;
                    cout << "New Name (Enter to keep): ";
                    getline(cin, n);
                    if(!n.empty()) d.name = n;

                    int a = getValidInt("New Age (0 to keep): ", 0, 150);
                    if(a > 0) d.age = a;

                    char c;
                    cout << "Change Gender? (Y/N): ";
                    cin >> c;
                    if(toupper(c) == 'Y')
                        d.gender = getValidGender();

                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ? important

                    string sp;
                    cout << "New Specialty (Enter to keep): ";
                    getline(cin, sp);
                    if(!sp.empty()) d.specialty = sp;

                    saveDoctors();
                    break;
                }
            }

            if(!found)
                cout << "Doctor not found.\n";

            cout << "Press Enter...";
            cin.get();
        }

        // ---------------- BACK ----------------
        else if(choice == 6){
            break;
        }

        else{
            cout << "Invalid choice! Press Enter...";
            cin.get();
        }
    }
}


//Appointment Scheduling
void appointmentScheduling(){
    int choice;
    while(true){
        clearScreen();
        cout<<"--- Appointment Scheduling ---\n";
        cout<<"1. Schedule New\n2. View All\n3. Delete\n4. Back\nChoice: "; cin>>choice; cin.ignore();
        if(choice==1){
            if(patients.empty()||doctors.empty()){ cout<<"Add patients and doctors first.\n"; cin.get(); continue;}
            Appointment a; a.id=appointments.empty()?1:appointments.back().id+1;
            cout<<"Patients:\n"; for(auto &p:patients) cout<<p.id<<" - "<<p.name<<"\n"; a.patientId=getValidInt("Patient ID: ",1,10000);
            cout<<"Doctors:\n"; for(auto &d:doctors) cout<<d.id<<" - "<<d.name<<"\n"; a.doctorId=getValidInt("Doctor ID: ",1,10000);
            a.date = getValidDate();
            cout<<"Fee: "; cin>>a.fee;
            appointments.push_back(a); saveAppointments(); cin.ignore();
            cout<<"Appointment scheduled. Press Enter..."; cin.get();
        } else if(choice==2){
            if(appointments.empty()) cout<<"No appointments.\n";
            else for(auto &a:appointments){
                string pname="Unknown", dname="Unknown";
                for(auto &p:patients) if(p.id==a.patientId) pname=p.name;
                for(auto &d:doctors) if(d.id==a.doctorId) dname=d.name;
                cout<<"ID: "<<a.id<<" | Patient: "<<pname<<" | Doctor: "<<dname<<" | Date: "<<a.date<<" | Fee: "<<a.fee<<"\n";
            }
            cout<<"Press Enter..."; cin.get();
        } else if(choice==3){
            int id=getValidInt("Appointment ID to delete: ",1,10000); bool found=false;
            for(size_t i=0;i<appointments.size();i++){ if(appointments[i].id==id){ appointments.erase(appointments.begin()+i); saveAppointments(); found=true; break;} }
            if(!found) cout<<"Appointment not found.\n"; cin.ignore(); cin.get();
        } else if(choice==4) break;
    }
}

//Billing System
void billingSystem(){
    int choice;
    while(true){
        clearScreen();
        cout << "--- Billing System ---\n";
        cout << "1. Generate Bill\n";
        cout << "2. View Bills\n";
        cout << "3. Back\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // ---------------- GENERATE BILL ----------------
        if(choice == 1){
            if(appointments.empty()){
                cout << "No appointments available.\n";
                cout << "Press Enter...";
                cin.get();
                continue;
            }

            cout << "Appointments:\n";
            for(auto &a : appointments){
                string pname = "Unknown";
                for(auto &p : patients)
                    if(p.id == a.patientId)
                        pname = p.name;

                cout << a.id << " - " << pname << " (" << a.date << ")\n";
            }

            int id = getValidInt("Enter Appointment ID: ", 1, 10000);
            bool found = false;

            for(auto &a : appointments){
                if(a.id == id){
                    found = true;

                    string pname = "Unknown", dname = "Unknown", specialty = "";

                    for(auto &p : patients)
                        if(p.id == a.patientId)
                            pname = p.name;

                    for(auto &d : doctors)
                        if(d.id == a.doctorId){
                            dname = d.name;
                            specialty = d.specialty;
                        }

                    string filename = "Bill_" + to_string(a.id) + ".txt";
                    ofstream file(filename);

                    file << "======= ABC Clinic =======\n";
                    file << "Date: " << a.date << "\n";     // ? only one date
                    file << "Bill ID: " << a.id << "\n";
                    file << "Patient Name: " << pname << "\n";
                    file << "Doctor Name: " << dname << "\n";
                    file << "Specialty: " << specialty << "\n";
                    file << "Fee: " << a.fee << "\n";
                    file << "=========================\n";

                    file.close();

                    cout << "Bill generated successfully!\n";
                    cout << "File Name: " << filename << "\n";
                    cout << "Press Enter...";
                    cin.get();
                    break;
                }
            }

            if(!found){
                cout << "Appointment not found.\n";
                cout << "Press Enter...";
                cin.get();
            }
        }

        // ---------------- VIEW BILLS ----------------
        else if(choice == 2){
            bool any = false;

            for(auto &a : appointments){
                string filename = "Bill_" + to_string(a.id) + ".txt";
                ifstream file(filename);

                if(file){
                    any = true;
                    cout << "\n--- Bill ID: " << a.id << " ---\n";
                    string line;
                    while(getline(file, line))
                        cout << line << "\n";
                    file.close();
                }
            }

            if(!any)
                cout << "No bills found.\n";

            cout << "Press Enter...";
            cin.get();
        }

        // ---------------- BACK ----------------
        else if(choice == 3){
            break;
        }

        else{
            cout << "Invalid choice!\n";
            cout << "Press Enter...";
            cin.get();
        }
    }
}


//Main
int main(){
    loadPatients(); loadDoctors(); loadAppointments();

    if(!loginMenu()) return 0;

    int choice;
    do{
        clearScreen();
        cout<<"\n--- Main Menu ---\n1. Patient Management\n2. Doctor Management\n3. Appointment Scheduling\n4. Billing System\n5. Exit\nChoice: ";
        cin>>choice; cin.ignore();
        switch(choice){
            case 1: patientManagement(); break;
            case 2: doctorManagement(); break;
            case 3: appointmentScheduling(); break;
            case 4: billingSystem(); break;
            case 5: clearScreen(); cout<<"Thank you!\n"; break;
            default: cout<<"Invalid choice! Press Enter..."; cin.get();
        }
    }while(choice!=5);

    return 0;
}

