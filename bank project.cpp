#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;
const string FileName = "client data.txt";
const string UserFile = "users.txt";
void printMainMenue();
void printTransactionScreen();
void Login();
void manageUsersScreen();
void printAccessDenidScreen();
struct sClient
{
	string Name;
	string Number;
	double Balance;
	string PhoneNumber;
	string PinCode;
	bool markToDelete = false;
};

struct sUser
{
	string User;
	string Password;
	short Permation;
};

enum enMainMenueOption
{
	enListClient = 1,
	enAddClient = 2,
	enUpdateClient = 3,
	enDeleteClient = 4,
	enFindClient = 5,
	enTransaction = 6,
	enManageUser = 7,
	enLogout = 8
};

enum enTransactionOption
{
	enDeposite = 1,
	enWithDrow = 2,
	enShowBalance = 3,
	enGoBackToMainMenue = 4

};

enum enManageUsers
{
	enListUsers = 1,
	enAddUser = 2,
	enUpdateUser = 3,
	enDeleteUser = 4,
	enFindUser = 5,
	enMainMenue = 6
};

enum enMainMenuePermations
{
	pAll = -1,
	pListClient = 1,
	pAddClient = 2,
	pUpdateClient = 4,
	pDeleteClient = 8,
	pFindClient = 16,
	pTransaction = 32,
	pManageUser = 64,

};

sUser CurrentUser;
bool checkAccessPermations(enMainMenuePermations Permations);

string readAccountNumber()
{
	string s1;
	cout << "Enter Account Number :";
	getline(cin >> ws, s1);
	return s1;
}

vector<string> vSplit(string s1, string delimetr)
{
	vector<string> vString;
	short pos = 0;
	string sWord;

	while ((pos = s1.find(delimetr)) != std::string::npos)
	{
		sWord = s1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		s1.erase(0, pos + delimetr.length());
	}
	sWord = s1.substr(0, pos);
	if (s1 != "")
		vString.push_back(sWord);

	return vString;
}

sClient convertLineToRecord(string line, string sep)
{
	vector<string> Client = vSplit(line, sep);
	sClient data;
	data.Number = Client[0];
	data.PinCode = Client[1];
	data.Name = Client[2];
	data.PhoneNumber = Client[3];
	data.Balance = stod(Client[4]);

	return data;
}

vector<sClient> loadClientsDataFromFile(string FileName)
{
	vector<sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in); //read mod

	if (MyFile.is_open())
	{
		string line;
		sClient client;
		while (getline(MyFile, line))
		{
			client = convertLineToRecord(line, "#//#");
			vClients.push_back(client);
		}
		MyFile.close();
	}
	return vClients;
}

string convertRecordToLine(sClient Client, string sep = "#//#")
{
	string s1 = Client.Number + sep;
	s1 += Client.PinCode + sep;
	s1 += Client.Name + sep;
	s1 += Client.PhoneNumber + sep;
	s1 += to_string(Client.Balance);

	return s1;
}

void saveClientDataToFile(string FileName, vector<sClient> vClient)
{
	string DataLine;
	fstream file;
	file.open(FileName, ios::out);

	if (file.is_open())
	{
		for (sClient C : vClient)
		{
			if (C.markToDelete == false)
			{
				DataLine = convertRecordToLine(C);
				file << DataLine << endl;
			}
		}
		file.close();
	}
}

void printClientCard(sClient client)
{
	cout << "Number      :" << client.Number << endl;
	cout << "Pin Code    :" << client.PinCode << endl;
	cout << "Name        :" << client.Name << endl;
	cout << "Phone Number:" << client.PhoneNumber << endl;
	cout << "Balance     :" << client.Balance << endl;
}

bool markClientForDeleteByAccountNumber(string AccountNumber, vector<sClient> &vClient)
{
	for (sClient &C : vClient)
	{
		if (AccountNumber == C.Number)
		{
			C.markToDelete = true;
			return true;
		}
	}

	return false;
}

bool findClientByAccountNumber(string AccountNumber, vector<sClient> vClient, sClient &client)
{
	for (sClient C : vClient)
	{
		if (AccountNumber == C.Number)
		{
			client = C;
			return true;
		}
	}
	return false;
}

//// find client /////

void findClient()
{
	if (checkAccessPermations(enMainMenuePermations::pFindClient) != true)
	{
		printAccessDenidScreen();
	}

	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	sClient client;
	cout << "\n=================================================\n";
	cout << "\t\tFind Client \n";
	cout << "=================================================\n";
	string s1 = readAccountNumber();
	if (findClientByAccountNumber(s1, vClient, client))
	{
		printClientCard(client);
	}
	else
	{
		cout << "\n\nClient Account " << s1 << " not found ...!";
	}
}

//////// add client ////////

bool findClientByAccountNumber(string AccountNumber, vector<sClient> vClient)
{
	for (sClient C : vClient)
	{
		if (AccountNumber == C.Number)
		{
			return true;
		}
	}
	return false;
}

void saveClientDataToFile(string data, string FileName)
{
	fstream myFile;
	myFile.open(FileName, ios::out | ios::app);
	if (myFile.is_open())
	{
		myFile << data << endl;
	}
	myFile.close();
}

sClient AddNewClinet()
{
	sClient AddClient;
	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	cout << "enter the account number?" << endl;
	getline(cin >> ws, AddClient.Number);
	while (findClientByAccountNumber(AddClient.Number, vClient))
	{
		cout << "Account " << AddClient.Number << " has been exist\n";
		cout << "enter the account number?" << endl;
		getline(cin >> ws, AddClient.Number);
	}
	cout << "enter the pin code?" << endl;
	getline(cin, AddClient.PinCode);

	cout << "enter the name?" << endl;
	getline(cin, AddClient.Name);

	cout << "enter the phone number?" << endl;
	getline(cin, AddClient.PhoneNumber);

	cout << "enter the balance?" << endl;
	cin >> AddClient.Balance;

	return AddClient;
}

void AddClient()
{
	if (checkAccessPermations(enMainMenuePermations::pAddClient) != true)
	{
		printAccessDenidScreen();
	}

	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	cout << "=================================================\n";
	cout << "\t\tAdd Client \n";
	cout << "=================================================\n";
	char answer;
	do
	{
		saveClientDataToFile(convertRecordToLine(AddNewClinet()), FileName);
		cout << "\nClient Added Successfully ..! Do you want to add more client ? y/n ";
		cin >> answer;
	} while (answer == 'y' || answer == 'Y');
}

/////// update client /////////

sClient changeClientRecord(string AccountNumber)
{
	sClient client;
	client.Number = AccountNumber;

	cout << "\nEnter Pin Code ?";
	getline(cin >> ws, client.PinCode);

	cout << "\nEnter Name ?";
	getline(cin, client.Name);

	cout << "\nEnter Phone Number ?";
	getline(cin, client.PhoneNumber);

	cout << "\nEnter Balance ?";
	cin >> client.Balance;

	return client;
}

bool updateClientByAccountNumber(vector<sClient> &vClient, string AccountNumber)
{
	char answer = 'n';
	sClient client;
	if (findClientByAccountNumber(AccountNumber, vClient, client))
	{
		printClientCard(client);
		cout << "\n\n Are you sure you want to update this account ... y/n ?";
		cin >> answer;
		if (answer == 'y' || answer == 'Y')
		{
			for (sClient &C : vClient)
			{
				if (C.Number == AccountNumber)
				{
					C = changeClientRecord(AccountNumber);
					break;
				}
			}

			saveClientDataToFile(FileName, vClient);
			cout << "\nAccount Updated successfully ...!\n";
			//refresh client data
			vClient = loadClientsDataFromFile(FileName);
			return true;
		}
	}
	else
	{
		cout << "\n the account " << AccountNumber << " not found ...!\n";
		return false;
	}
	return false;
}

void updateClient()
{
	if (checkAccessPermations(enMainMenuePermations::pUpdateClient) != true)
	{
		printAccessDenidScreen();
	}

	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	cout << "=================================================\n";
	cout << "\t\tUpdate Client \n";
	cout << "=================================================\n";
	updateClientByAccountNumber(vClient, readAccountNumber());
}

//////// delete client account ///////////

bool deleteClientByAccountNumber(vector<sClient> &vClient, string AccountNumber)
{
	char answer = 'n';
	sClient client;
	if (findClientByAccountNumber(AccountNumber, vClient, client))
	{
		printClientCard(client);
		cout << "\n\n Are you sure you want to delete this account ... y/n ?";
		cin >> answer;
		if (answer == 'y' || answer == 'Y')
		{
			markClientForDeleteByAccountNumber(AccountNumber, vClient);
			saveClientDataToFile(FileName, vClient);
			cout << "\nAccount Deleted successfully ...!\n";
			//refresh client data
			vClient = loadClientsDataFromFile(FileName);
			return true;
		}
	}
	else
	{
		cout << "\n the account " << AccountNumber << " not found ...!\n";
		return false;
	}
	return false;
}

void deleteClient()
{
	if (checkAccessPermations(enMainMenuePermations::pDeleteClient) != true)
	{
		printAccessDenidScreen();
	}

	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	cout << "=================================================\n";
	cout << "\t\tDelete Account \n";
	cout << "=================================================\n";
	deleteClientByAccountNumber(vClient, readAccountNumber());
}

///////// delete client account /////

//////////print clinet list /////////

void printClientRecord(sClient client)
{
	cout << "|" << setw(15) << left << client.Number;
	cout << "|" << setw(10) << left << client.PinCode;
	cout << "|" << setw(40) << left << client.Name;
	cout << "|" << setw(12) << left << client.PhoneNumber;
	cout << "|" << setw(12) << client.Balance;
}

void printAllClientData(vector<sClient> vClient)
{
	cout << "\n\n\t\t\t\t\t Client List " << vClient.size() << " Client(s)";
	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";
	cout << "|" << left << setw(15) << "Account Number";
	cout << "|" << left << setw(10) << "Pin Code";
	cout << "|" << left << setw(40) << "Client Name";
	cout << "|" << left << setw(12) << "Phone Number";
	cout << "|" << left << setw(12) << "Balance";
	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";

	for (sClient client : vClient)
	{
		printClientRecord(client);
		cout << endl;
	}

	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";
}

void ShowClient()
{
	if (checkAccessPermations(enMainMenuePermations::pListClient) != true)
	{
		printAccessDenidScreen();
	}

	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	printAllClientData(vClient);
}
////////
void goBackToMainMenue()
{
	cout << "\n\n press any key to go back to main menue ...!\n";
	system("pause >0");
	printMainMenue();
}

void goBackToTransactionScreen()
{
	cout << "\n\n press any key to go back to main menue ...!\n";
	system("pause >0");
	printTransactionScreen();
}

short readMainMenue(string message)
{
	cout << message;
	short answer;
	cin >> answer;
	return answer;
}

void Deposit(string accountNumber, double deposit, vector<sClient> &vClient)
{
	for (sClient &C : vClient)
	{
		if (C.Number == accountNumber)
		{
			C.Balance += deposit;
			saveClientDataToFile(FileName, vClient);
			return;
		}
	}
}

void performWithDrow()
{
	string s1 = readAccountNumber();
	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	while (findClientByAccountNumber(s1, vClient) != true)
	{
		cout << "The Account " << s1 << " Not Fount ...\n";
		s1 = readAccountNumber();
	}

	double drow = 0;

	for (sClient C : vClient)
	{
		if (C.Number == s1)
		{
			drow = C.Balance;
		}
	}

	cout << "Enter The Amount Of WithDrow :";
	double WithDrow;
	cin >> WithDrow;
	while (WithDrow == 0 || WithDrow > drow)
	{
		cout << "\nYou Can Not Make WithDrow With More Than  " << drow << "  .\n";
		cout << "Enter The Amount Of WithDrow :";
		cin >> WithDrow;
	}
	WithDrow *= -1;
	Deposit(s1, WithDrow, vClient);
	cout << "WithDrow Performed Seccessfully ...!\n";
}

void WithDrowScreen()
{
	cout << "=================================================\n";
	cout << "\t\tWithDrow Screen \n";
	cout << "=================================================\n";
	performWithDrow();
}

void performDeposite()
{
	string s1 = readAccountNumber();
	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	while (findClientByAccountNumber(s1, vClient) != true)
	{
		cout << "The Account " << s1 << " Not Fount ...\n";
		s1 = readAccountNumber();
	}
	cout << "Enter The Amount Of Deposit :";
	double deposit;
	cin >> deposit;
	while (deposit <= 0)
	{
		cout << "\nYou Can Not Make Deposit With <= 0 Value .\n";
		cout << "Enter The Amount Of Deposit :";
		cin >> deposit;
	}
	Deposit(s1, deposit, vClient);
	cout << "Deposite Performed Seccessfully ...!\n";
}

void depositScreen()
{
	cout << "=================================================\n";
	cout << "\t\tDeposite Screen \n";
	cout << "=================================================\n";
	performDeposite();
}

void printClientRecordBalance(sClient client)
{
	cout << "|" << setw(15) << left << client.Number;
	cout << "|" << setw(40) << left << client.Name;
	cout << "|" << setw(12) << client.Balance;
}

void printBalanceData(vector<sClient> vClient)
{
	cout << "\n\n\t\t\t\t\t Client List " << vClient.size() << " Client(s)";
	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";
	cout << "|" << left << setw(15) << "Account Number";
	cout << "|" << left << setw(40) << "Client Name";
	cout << "|" << left << setw(12) << "Balance";
	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";

	for (sClient client : vClient)
	{
		printClientRecordBalance(client);
		cout << endl;
	}

	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";
}

void performBalance()
{
	vector<sClient> vClient = loadClientsDataFromFile(FileName);
	printBalanceData(vClient);
}

void showBalanceScreen()
{
	cout << "=================================================\n";
	cout << "\t\tBalance Screen \n";
	cout << "=================================================\n";
	performBalance();
}

void performTransactionScreen(enTransactionOption Option)
{
	switch (Option)
	{
	case enTransactionOption::enDeposite:
		system("cls");
		depositScreen();
		goBackToTransactionScreen();
		break;
	case enTransactionOption::enWithDrow:
		system("cls");
		WithDrowScreen();
		goBackToTransactionScreen();
		break;
	case enTransactionOption::enShowBalance:
		system("cls");
		showBalanceScreen();
		goBackToTransactionScreen();
		break;
	case enTransactionOption::enGoBackToMainMenue:
		printMainMenue();
		break;
	default:
		break;
	}
}

void printTransactionScreen()
{
	if (checkAccessPermations(enMainMenuePermations::pTransaction) != true)
	{
		printAccessDenidScreen();
	}

	system("cls");
	cout << "=================================================\n";
	cout << "\t\tTransaction Menu Screen \n";
	cout << "=================================================\n";
	cout << "[1] Deposite \n";
	cout << "[2] Withdrow \n";
	cout << "[3] Show Balance \n";
	cout << "[4] Go Back To Main Menue \n";
	cout << "\n=================================================\n";
	performTransactionScreen((enTransactionOption)readMainMenue("What Do You Want To Do ? [1 To 4] "));
}

///////// print client list //////

sUser convertUserLineToRecord(string line)
{
	vector<string> vString = vSplit(line, "#//#");
	sUser user;
	user.User = vString[0];
	user.Password = vString[1];
	user.Permation = stoi(vString[2]);
	return user;
}

string convertUserRecordToLine(sUser user, string sep = "#//#")
{
	string data;
	data = user.User + sep;
	data += user.Password + sep;
	data += to_string(user.Permation);
	return data;
}

vector<sUser> loadUserDataFromFile(string userFile)
{
	vector<sUser> vUser;
	sUser user;
	fstream file;
	file.open(userFile, ios::in);
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			user = convertUserLineToRecord(line);
			vUser.push_back(user);
		}
		file.close();
	}
	return vUser;
}

void printUsersRecord(sUser user)
{
	cout << "|" << setw(15) << left << user.User;
	cout << "|" << setw(40) << left << user.Password;
	cout << "|" << setw(12) << user.Permation;
}

void printUsersList(vector<sUser> vUser)
{
	cout << "\n\n\t\t\t\t\t  List " << vUser.size() << " User(s)";
	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";
	cout << "|" << left << setw(15) << "Acount User";
	cout << "|" << left << setw(40) << "Password";
	cout << "|" << left << setw(12) << "Permation";
	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";

	for (sUser user : vUser)
	{
		printUsersRecord(user);
		cout << endl;
	}

	cout << "\n____________________________________________________________";
	cout << "___________________________________________________\n";
}

void performListUsers()
{
	vector<sUser> vUser = loadUserDataFromFile(UserFile);
	printUsersList(vUser);
}

void listUsersScreen()
{
	cout << "\t\t=================================================\n";
	cout << "\t\t\t\tList Users Screen \n";
	cout << "\t\t=================================================\n";
	performListUsers();
}

void goBackToManageUsers()
{
	cout << "\nPress Any Key To Go Back To Manage Users Menue ...!\n";
	system("pause > 0");
	manageUsersScreen();
}

bool findUserByUser(string userName, sUser &userInfo)
{
	vector<sUser> vUser;
	vUser = loadUserDataFromFile(UserFile);
	for (sUser user : vUser)
	{
		if (user.User == userName)
		{
			userInfo = user;
			return true;
		}
	}
	return false;
}

void printUsersCard(sUser user)
{
	cout << "\n\nUser Name : " << user.User << endl;
	cout << "Password  : " << user.Password << endl;
	cout << "Permation : " << user.Permation << endl;
}

void findUserScreen()
{
	cout << "=================================================\n";
	cout << "\t\tFind Users Screen \n";
	cout << "=================================================\n\n";
	string userName;
	cout << "Enter The User Name You Want To Find : ";
	getline(cin >> ws, userName);
	sUser userInfo;
	if (findUserByUser(userName, userInfo))
	{
		printUsersCard(userInfo);
	}
	else
	{
		cout << "The User (" << userName << ") Not Found ...!\n";
	}
}

short readPermations()
{
	short permation = 0;
	char answer = 'n';
	cout << "\nDo You Want To Give Full Access y/n ? ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		return -1;

	cout << "\nDo You Want To Give Access To Show Client List y/n ? ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		permation += 1;

	cout << "\nDo You Want To Give Access To Add Client List y/n ? ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		permation += 2;

	cout << "\nDo You Want To Give Access To Update Client List y/n ? ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		permation += 4;

	cout << "\nDo You Want To Give Access To Delete Client List y/n ? ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		permation += 8;

	cout << "\nDo You Want To Give Access To Find Client List y/n ? ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		permation += 16;

	cout << "\nDo You Want To Give Access To Transaction List y/n ? ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		permation += 32;

	cout << "\nDo You Want To Give Access To Manage Users y/n ? ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
		permation += 64;

	return permation;
}

sUser AddNewUser()
{
	sUser AddUser;
	vector<sUser> vUser = loadUserDataFromFile(UserFile);
	cout << "enter the user account number?" << endl;
	getline(cin >> ws, AddUser.User);
	while (findUserByUser(AddUser.User, AddUser))
	{
		cout << "User " << AddUser.User << " has been exist\n";
		cout << "enter the user account number?" << endl;
		getline(cin >> ws, AddUser.User);
	}
	cout << "enter the password?" << endl;
	getline(cin, AddUser.Password);

	cout << "enter the permition?" << endl;
	AddUser.Permation = readPermations();
	return AddUser;
}

void saveUserDataToFile(string data, string UserFile)
{
	fstream file;
	file.open(UserFile, ios::out | ios::app);
	if (file.is_open())
	{
		file << data << endl;
		file.close();
	}
}

void updateUserDataToFile(vector<sUser> vUser, string UserFile)
{
	fstream file;
	file.open(UserFile, ios::out);
	if (file.is_open())
	{
		for (sUser u : vUser)
			file << convertUserRecordToLine(u) << endl;
		file.close();
	}
}

void addUsersScreen()
{
	cout << "=================================================\n";
	cout << "\t\tAdd Users Screen \n";
	cout << "=================================================\n\n";
	char answer;
	do
	{
		saveUserDataToFile(convertUserRecordToLine(AddNewUser()), UserFile);
		cout << "\nUser Added Successfully ..! Do you want to add more Users ? y/n ";
		cin >> answer;
	} while (answer == 'y' || answer == 'Y');
}

sUser changeUserRecord(string userAccount)
{
	sUser user;
	user.User = userAccount;
	cout << "\nEnter The Password ?";
	getline(cin >> ws, user.Password);

	cout << "\nEnter The Permation ?";
	user.Permation = readPermations();

	return user;
}

bool updateUser(vector<sUser> &vUser, string userAccount)
{
	char answer = 'n';
	sUser user;
	if (findUserByUser(userAccount, user))
	{
		printUsersCard(user);
		cout << "\n\n Are you sure you want to update this user ... y/n ?";
		cin >> answer;
		if (answer == 'y' || answer == 'Y')
		{
			for (sUser &C : vUser)
			{
				if (C.User == userAccount)
				{
					C = changeUserRecord(userAccount);
					break;
				}
			}

			updateUserDataToFile(vUser, UserFile);
			cout << "\nUser Updated successfully ...!";
			//refresh User data
			vUser = loadUserDataFromFile(UserFile);
			return true;
		}
	}
	else
	{
		cout << "\nthe user " << userAccount << " not found ...!\n";
		return false;
	}
	return false;
}

void updateUsersScreen()
{
	vector<sUser> vUser = loadUserDataFromFile(UserFile);
	cout << "=================================================\n";
	cout << "\t\tUpdate Users Screen \n";
	cout << "=================================================\n\n";
	cout << "Enter The User	Account : ";
	string userAccount;
	cin >> userAccount;
	while (updateUser(vUser, userAccount) != true)
	{
		cout << "Enter The User	Account : ";
		cin >> userAccount;
	}
}

void deleteUser(string userAccount)
{
	if (userAccount == "Admin")
	{
		cout << "You Can Not  Delete Admin Account." << endl;
		return;
	}

	char answer = 'n';
	sUser user;
	if (findUserByUser(userAccount, user))
	{
		printUsersCard(user);
		cout << "\n\n Are you sure you want to delete this user ... y/n ?";
		cin >> answer;
		if (answer == 'y' || answer == 'Y')
		{
			vector<sUser> vUser, vUser2;
			vUser = loadUserDataFromFile(UserFile);
			for (sUser u : vUser)
				if (u.User != userAccount)
					vUser2.push_back(u);
			updateUserDataToFile(vUser2, UserFile);
			cout << "\nUser Account Deleted successfully ...!\n";
		}
	}
	else
	{
		cout << "\n the account " << userAccount << " not found ...!\n";
	}
}

void deleteUsersScreen()
{
	cout << "=================================================\n";
	cout << "\t\tDelete Users Screen \n";
	cout << "=================================================\n\n";
	string userAccount;
	cout << "Enter The User Name : ";
	cin >> userAccount;
	deleteUser(userAccount);
}

bool checkAccessPermations(enMainMenuePermations Permations)
{
	if (Permations == enMainMenuePermations::pAll)
		return true;

	if ((Permations & CurrentUser.Permation) == Permations)
		return true;
	else
		return false;
}

void printAccessDenidScreen()
{
	system("cls");
	cout << "\n------------------------------------------------------------------------------------\n";
	cout << "Access Denid ...!\nYou Don Not Have Permations To Do This ...!\nPlease Contact Your Admin To Solve The Problem ...!.\n";
	cout << "\n------------------------------------------------------------------------------------\n";
	cout << "Press Any Key To Back To Main Menue ...!\n";
	system("pause > 0");
	printMainMenue();
}

void performManageUsers(enManageUsers manage)
{
	switch (manage)
	{
	case enManageUsers::enListUsers:
		system("cls");
		listUsersScreen();
		goBackToManageUsers();
		break;
	case enManageUsers::enAddUser:
		system("cls");
		addUsersScreen();
		goBackToManageUsers();
		break;
	case enManageUsers::enUpdateUser:
		system("cls");
		updateUsersScreen();
		goBackToManageUsers();
		break;
	case enManageUsers::enDeleteUser:
		system("cls");
		deleteUsersScreen();
		goBackToManageUsers();
		break;
	case enManageUsers::enFindUser:
		system("cls");
		findUserScreen();
		goBackToManageUsers();
		break;
	case enManageUsers::enMainMenue:
		system("cls");
		printMainMenue();
		break;
	}
}

void manageUsersScreen()
{
	if (checkAccessPermations(enMainMenuePermations::pManageUser) != true)
	{
		printAccessDenidScreen();
	}

	system("cls");
	cout << "=================================================\n";
	cout << "\t\tManage Users Screen \n";
	cout << "=================================================\n";
	cout << "[1] List Users.\n";
	cout << "[2] Add New User.\n";
	cout << "[3] Update User.\n";
	cout << "[4] Delete User.\n";
	cout << "[5] Find User.\n";
	cout << "[6] Main Menue.\n";
	cout << "=================================================\n";
	performManageUsers((enManageUsers)readMainMenue("What Do You Want To Do [1 - 6] "));
}

void performMainMenue(enMainMenueOption Option)
{
	switch (Option)
	{
	case enMainMenueOption::enListClient:
		system("cls");
		ShowClient();
		goBackToMainMenue();
		break;
	case enMainMenueOption::enAddClient:
		system("cls");
		AddClient();
		goBackToMainMenue();
		break;
	case enMainMenueOption::enUpdateClient:
		system("cls");
		updateClient();
		goBackToMainMenue();
		break;
	case enMainMenueOption::enDeleteClient:
		system("cls");
		deleteClient();
		goBackToMainMenue();
		break;
	case enMainMenueOption::enFindClient:
		system("cls");
		findClient();
		goBackToMainMenue();
		break;
	case enMainMenueOption::enTransaction:
		system("cls");
		printTransactionScreen();
		break;
	case enMainMenueOption::enManageUser:
		system("cls");
		manageUsersScreen();
		break;
	case enMainMenueOption::enLogout:
		system("cls");
		Login();
		break;
	default:
		Login();
		break;
	}
}

void printMainMenue()
{
	system("cls");
	cout << "=================================================\n";
	cout << "\t\tMain Menu Screen \n";
	cout << "=================================================\n";
	cout << "[1] Show Clients.\n";
	cout << "[2] Add New Client.\n";
	cout << "[3] Update Client.\n";
	cout << "[4] Delete Client.\n";
	cout << "[5] Find Client.\n";
	cout << "[6] Transaction.\n";
	cout << "[7] Manage Users.\n";
	cout << "[8] Logout.\n";
	cout << "\n=================================================\n";
	performMainMenue((enMainMenueOption)readMainMenue("What Do You Want To Do [1 To 8] "));
}

void loginScreen()
{
	system("cls");
	cout << "--------------------------------------------------\n";
	cout << "\t\tLogin Screen\n";
	cout << "--------------------------------------------------\n";
}

bool findUserByUserAndPassword(string userName, string password)
{
	vector<sUser> vUser;
	vUser = loadUserDataFromFile(UserFile);
	for (sUser user : vUser)
	{
		if (user.User == userName && user.Password == password)
		{
			CurrentUser = user;
			return true;
		}
	}
	return false;
}

void Login()
{
	loginScreen();
	string user, password;
	cout << "Enter User Name :";
	getline(cin >> ws, user);
	cout << "Enter Password:";
	cin >> password;
	while (findUserByUserAndPassword(user, password) != true)
	{
		loginScreen();
		cout << "Invalid User/Password\n\n";
		cout << "Enter User Name :";
		getline(cin >> ws, user);
		cout << "Enter Password:";
		cin >> password;
	}
	printMainMenue();
}

int main()
{
	Login();
	return 0;
}