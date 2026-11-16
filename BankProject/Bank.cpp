#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <filesystem>
using namespace std;
const string ClientsFileName = "clint.txt";
const string UserFileName = "users.txt";
struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

struct sUser {
	string UserName;
	string PassWord;
	int Permissions;
	bool markfordelet = false;
};

string ReadClientAccountNumber();

void printfinalcard();

sUser Currentuser;

void TransationMenueScreen();

enum  emanageMenue { listuser = 1, addnewuser = 2, deletuser = 3, updateuser = 4, fnduser = 5, Mainmenue = 6 };

enum  etranMenue { deposit = 1, withdraw = 2, totalbalance = 3, mainmenue = 4 };
enum  enPermission { plistuser = 1, paddnewuser = 2, pdeletuser = 4, pupdateuser = 8,
	pfnduser = 16, Transaction = 32, Manageusers = 64, all = -1 };

enum emenue { showclientlist = 1, addnewclient = 2, deletclient = 3, updateclient = 4,
	findclient = 5, transaction = 6, manageusers = 7, logout = 8 };

bool CheckAccessPermission(enPermission permission);

sUser ReadLogin();

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable
	// use find() function to get the position of the delimiters
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length()); /* erase() until
		positon and move to next word. */
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}
	return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double
	return Client;
}

sUser ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	sUser user;
	vector<string> vUserData;
	vUserData = SplitString(Line, Seperator);
	user.UserName = vUserData[0];
	user.PassWord = vUserData[1];
	user.Permissions =stoi( vUserData[2]);
	return user;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}

string ConvertUsingRecordToLine(sUser user, string Seperator = "#//#")
{
	string stuserRecord = "";
	stuserRecord += user.UserName + Seperator;
	stuserRecord += user.PassWord + Seperator;
	stuserRecord += to_string(user.Permissions) + Seperator;
	return stuserRecord;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

vector<sUser> LoadUsersDataFromFile(string FileName)
{
	vector <sUser> vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sUser Client;
		while (getline(MyFile, Line))
		{
				Client = ConvertLineToRecord(Line);
				vUser.push_back(Client);
		}
		MyFile.close();
	}
	return vUser;
}

bool UserExistsByUsername(string Username, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in); // فتح الملف مباشرة

	if (MyFile.is_open())
	{
		string Line;
		sUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertLineToRecord(Line); // تحويل السطر
			if (User.UserName == Username)
			{
				MyFile.close(); // إغلاق الملف فوراً
				return true;    // العثور عليه
			}
		}
		MyFile.close();
	}
	return false;
}

void ShowAccessDeniedMessage() {
	cout << "\n-------------------------------------------\n";
	cout << "Access Denied, \nYou donot Have Permission to this,\nPlease Conact Your Admin";
	cout << "\n-------------------------------------------\n";

}

void PrintClientCard(sClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
}

void PrintUserCard(sUser user)
{
	cout << "\nThe following are the users details:\n";
	cout << "\nUser Name: " << user.UserName;
	cout << "\nPassword : " << user.PassWord;
	cout << "\nPermission: " << user.Permissions;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{

	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindClientByUserName(string Username, vector<sUser>vuser, sUser& user)
{

	for (sUser N : vuser)
	{
		if (N.UserName == Username)
		{
			user = N;
			return true;
		}
	}
	return false;
}

bool FindClientByUserNameAndPassword(string Username, string password, sUser& user)
{
	vector<sUser>vuser =
		LoadUsersDataFromFile(UserFileName);
	for (sUser U: vuser)
	{
		if (U.UserName == Username&&U.PassWord==password)
		{
			user = U;
			return true;
		}
	}
	return false;
}

bool LoadUserInfo(string username, string password) {
	if (FindClientByUserNameAndPassword(username, password, Currentuser))
		return true;
	else
		return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

vector <sUser> SaveUsersDataToFile(string FileName, vector<sUser> vUser)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;
	if (MyFile.is_open())
	{
		for (sUser C : vUser)
		{
			if (C.markfordelet == false)
			{
				//we only write records that are not marked for delete.
				DataLine = ConvertUsingRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUser;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients,
		Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient Updated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber
			<< ") is Not Found!";
		return false;
	}
}

void UpdateClientScreen(vector<sClient>& vClients) {
	if (!CheckAccessPermission(enPermission::pupdateuser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n--------------------------------\n";
	cout << "\t\t\t Update Client Screen\t\t\t\t";
	cout << "\n---------------------------------\n";
	string acountnum = ReadClientAccountNumber();
	UpdateClientByAccountNumber(acountnum, vClients);
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

void printclintrecord(sClient data) {
	cout << "|" << setw(15) << left << data.AccountNumber;
	cout << "|" << setw(15) << left << data.PinCode;
	cout << "|" << setw(15) << left << data.Name;
	cout << "|" << setw(15) << left << data.Phone;
	cout << "|" << setw(15) << left << data.AccountBalance;
}

void PrintUserRecord(sUser user)
{
	cout << "|" << setw(15) << left << user.UserName;
	cout << "|" << setw(15) << left << user.PassWord;
	cout << "|" << setw(15) << left << user.Permissions;

}

void printallclintsdata(vector<sClient>vdata) {
	sClient client;
	cout << "\n----------------------------------------------------------------------------\n";
	cout << "\t\t\t\t\t\tList (" << vdata.size() << ") Clints";
	cout << "\n----------------------------------------------------------------------------\n";
	cout << "| " << setw(15) << "AcountNumber";
	cout << "|" << left << setw(15) << "PinCode";
	cout << "|" << left << setw(15) << "Name";
	cout << "|" << left << setw(15) << "Phone";
	cout << "|" << left << setw(15) << "AcountBalance";
	cout << "\n-----------------------------------------------------------------------------\n";
	for (sClient& i : vdata) {
		client = i;
		printclintrecord(client);
		cout << endl;
	}
	cout << "\n-----------------------------------------------------------------------------\n";
}

void PrintAllUserData(vector<sUser>vuser) {
	sUser user;
	cout << "\tUser List (" << vuser.size() << ") Users\n";
	cout << "\n-----------------------------------------------------------------------------\n";
	cout << "|" << setw(15) << "User Name";
	cout << "|" << setw(15) << "Password";
	cout << "|" << setw(15) << "Permissions";
	cout << "\n-----------------------------------------------------------------------------\n";
	for (sUser& i : vuser) {
		user = i;
		PrintUserRecord(user);
		cout << endl;
	}
	cout << "\n-------------------------------------------------------------------------------\n";
}

void ClientListScreen(vector<sClient>vClient) {
	if (!CheckAccessPermission(enPermission::plistuser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n----------------------------------------------\n";
	cout << "\t\t\t Client list Screen\t\t\t\t";
	cout << "\n----------------------------------------------\n";
	printallclintsdata(vClient);
}

bool remarktodeletdata(string acountnum, vector< sClient>& vdata) {
	for (sClient& i : vdata)
	{
		if (i.AccountNumber == acountnum)
		{
			i.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool remarktodeletdata(string username, vector< sUser>& vdata) {
	for (sUser& i : vdata)
	{
		if (i.UserName == username)
		{
			i.markfordelet = true;
			return true;
		}
	}
	return false;
}

void deletdatafromfail(string acountnum, vector<sClient>& vdata) {
	char yesno = ' ';
	sClient data;
	if (FindClientByAccountNumber(acountnum, vdata, data))
	{
		PrintClientCard(data);
		cout << "Are you sure you want delet this clint y/n\n";
		cin >> yesno;
		if (yesno == 'y' || yesno == 'Y')
		{
			remarktodeletdata(acountnum, vdata);
			SaveCleintsDataToFile(ClientsFileName, vdata);
			vdata = LoadCleintsDataFromFile(ClientsFileName);
			cout << "\nclint delet successfuly\n";

		}
	}
	else {
		cout << "clint with acountnumber " << acountnum << " is not found";
	}
}

void DeleteClientScreen(vector<sClient>& vClient) {
	if (!CheckAccessPermission(enPermission::pdeletuser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------------------------\n";
	cout << "\t\t\t Delete Client Screen\t\t\t\t";
	cout << "\n-----------------------------------------------------\n";
	string acountnum = ReadClientAccountNumber();
	deletdatafromfail(acountnum, vClient);
}

void FindCleint(vector<sClient>vClient, string acountnum) {
	sClient Client;
	for (sClient& c : vClient)
	{
		if (c.AccountNumber == acountnum)
		{
			Client = c;
			PrintClientCard(Client);

		}
	}
	cout << "clint with acountnumber " << acountnum << " is not found";
}

bool FindUser(vector<sUser>vUser, string username) {
	sUser user;
	for (sUser& u : vUser)
	{
		if (u.UserName == username)
		{
			user = u;
			PrintUserCard(user);
			return true;
		}
	}
	cout << "clint with acountnumber " << username<< " is not found";
}

void FindClientScreen(vector<sClient>vClient) {
	if (!CheckAccessPermission(enPermission::pfnduser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n--------------------------------\n";
	cout << "\t\t\t Find Client Screen\t\t\t\t";
	cout << "\n---------------------------------\n";
	string acountnum = ReadClientAccountNumber();
	FindCleint(vClient, acountnum);
}

void addlinetofail(string failname, string stdata) {
	fstream myfail;
	myfail.open(failname, ios::out | ios::app);
	if (myfail.is_open()) {
		myfail << stdata << endl;
		myfail.close();
	}
}

void AddClient(vector<sClient>vClient, string acountnum) {
	char answer = 'n';
	sClient client;
	do {
		for (sClient& c : vClient)
		{
			while (FindClientByAccountNumber(acountnum, vClient,client ))
			{
				cout << "This AccountNumber[" << acountnum << "]Found at fail add another one please \n";
				cin >> acountnum;
			}
		}
		client = ChangeClientRecord(acountnum);
		addlinetofail(ClientsFileName, ConvertRecordToLine(client));
		cout << "Client Added Successfully, do you want to add another client? y/n\n";
		cin >> answer;
	} while (answer == 'y' || answer == 'Y');
}

void AddClientScreen(vector<sClient>vClient) {
	if (!CheckAccessPermission(enPermission::paddnewuser))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n--------------------------------------------------------------------\n";
	cout << "\t\t\t Add Client Screen\t\t\t\t";
	cout << "\n--------------------------------------------------------------------\n";
	string acountnum = ReadClientAccountNumber();
	AddClient(vClient, acountnum);
}

short ReadMainMenueOption() {
	short chois = 0;
	cout << "chois what you want to do? [1 to 8]?";
	cin >> chois;
	return chois;
}

void GotoMainMenue() {
	cout << "\nPress any key to go back to main menue...";
	system("pause>0");
	printfinalcard();
}

void LogoutScreen() {
	cout << "\n---------------------------------------------\n";
	cout << "\t\t\t Program End :)\t\t\t\t";
	cout << "\n----------------------------------------------\n";
}

void Deposit(vector<sClient>& vClient) {
	char g = 'n';
	short depos = 0;
	short balance = 0;
	sClient Client;
	string acountnum = ReadClientAccountNumber();
	while (!FindClientByAccountNumber(acountnum, vClient, Client))
	{
		cout << " This AccoubtNumber not found  ";
		acountnum = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	cout << "\nPlease enter Deposit amount?";
	cin >> depos;
	cout << "\nAre you sure perform this transaction? y/n ?";
	cin >> g;
	if (g == 'y' || g == 'Y')
	{
		for (sClient& c : vClient)
		{
			balance = c.AccountBalance + depos;
		}
		SaveCleintsDataToFile(ClientsFileName, vClient);
		cout << "Acountbalance become :" << balance << endl;
		cout << "\nDeposit successful\n";
	}

}

void DepositScreen(vector<sClient>vClient) {
	cout << "\n============================================\n";
	cout << "\tDeposit Screen";
	cout << "\n============================================\n";
	Deposit(vClient);
}

void Withdraw(vector<sClient>& vClient) {
	char g = 'n';
	short depos = 0;
	sClient Client;
	short balance = 0;
	string acountnum = ReadClientAccountNumber();
	while (!FindClientByAccountNumber(acountnum, vClient, Client))
	{
		cout << " This AccountNumber not Found  ";
		acountnum = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	cout << "\nPleas enter withdraw amount?";
	cin >> depos;
	for (sClient& c : vClient)
	{
		if (c.AccountBalance < depos)
		{
			cout << "\nAmount Exceeds the balance , you cant withdraw up to : " << c.AccountBalance;
			cout << "\nPlease enter another amount? ";
			cin >> depos;
		}
	}
	cout << "\nAre you sure perform this transaction? y/n ?";
	cin >> g;
	if (g == 'y' || g == 'Y')
	{


		for (sClient& c : vClient)
		{
			balance = c.AccountBalance - depos;
		}
		SaveCleintsDataToFile(ClientsFileName, vClient);
		cout << "AccountBalance become: " << balance << endl;

		cout << "\nWithdraw successful\n";
	}
}

void WithdrawScreen(vector<sClient>vClient) {
	cout << "\n--------------------------------\n";
	cout << "\t\t\t Withdraw Screen\t\t\t\t";
	cout << "\n---------------------------------\n";
	Withdraw(vClient);
}

void TotalBalanceScreen(vector<sClient>vClient) {
	double totalbalance = 0;
	cout << "\n-------------------------------------------------------------\n";
	cout << "\t\t\t\t\t\t  Balance List  (" << vClient.size() << ") Clints";
	cout << "\n---------------------------------------------------------------\n";
	cout << "| " << setw(15) << "AcountNumber";
	cout << "|" << left << setw(15) << "Name";
	cout << "|" << left << setw(15) << "AcountBalance";
	cout << "\n-------------------------------------------------------------------\n";
	for (sClient& i : vClient)
	{
		cout << "|" << setw(15) << left << i.AccountNumber;
		cout << "|" << setw(15) << left << i.Name;
		cout << "|" << setw(15) << left << i.AccountBalance;
		cout << endl;
		totalbalance += i.AccountBalance;
	}
	cout << "\n--------------------------------------------------------------------\n";

	cout << "Total Balance = " << totalbalance << endl;
}

short TransactionChois() {
	short chois = 0;
	cout << "Chose What do you want to do ?[1 to 4]?\n";
	cin >> chois;
	return chois;
}

void GotoTrancationMenue();

void ReadTransactionMenue(etranMenue chois, vector<sClient>vClient) {
	switch (chois)
	{
	case etranMenue::deposit:
	{
		system("cls");
		DepositScreen(vClient);
		GotoTrancationMenue();
		break;
	}

	case etranMenue::withdraw:
	{
		system("cls");
		WithdrawScreen(vClient);
		GotoTrancationMenue();
	}

	case etranMenue::totalbalance:
	{
		system("cls");
		TotalBalanceScreen(vClient);
		GotoTrancationMenue();
		break;
	}

	case etranMenue::mainmenue:
	{

		GotoMainMenue();
	}
	}
}

void TransactionMenueScreen(vector<sClient>vClient) {
	if (!CheckAccessPermission(enPermission::Transaction))
	{
		ShowAccessDeniedMessage();
		GotoMainMenue();
	}
	system("cls");
	cout << "\n=====================================================\n";
	cout << "\tTransaction Menue Screen\n";
	cout << "\n=======================================================\n";
	cout << "[1]Deposit.\n";
	cout << "[2]Withdraw.\n";
	cout << "[3]Total Balance.\n";
	cout << "[4]Main Menue.\n";
	cout << "=============================================================\n";
	ReadTransactionMenue((etranMenue)TransactionChois(), vClient);
}

void GotoTrancationMenue() {
	cout << "\nPress any key to go back to main menue...";
	system("pause>0");
	TransactionMenueScreen(LoadCleintsDataFromFile(ClientsFileName));
}

short ManageUserChois() {
	short chois;
	cout << "\nWhat do You Want to do?[1 to 6]?";
	cin >> chois;
	return chois;
}

void ManageUserMenueScreen();

void GotoManageMenue() {
	cout << "\nPress any key to go back to main menue...";
	system("pause>0");
	ManageUserMenueScreen();
}

int FullAscces() {
	char c1 = 'n';
	int permission = 0;
	cout << "\nDo you Want Gave full access? y/n?";
	cin >> c1;
	if (c1 == 'y' || c1 == 'Y')
	{
		return -1;
		cout << "\nAdded successfuly\n";
	}
	else
	{
		cout << "Do you want to give acces to : \n";

		cout << "\nShow Client list?y/n?";
		cin >> c1;
		if (c1 == 'y' || c1 == 'Y')
		{
			permission += (int)enPermission::plistuser;
		}
		cout << "\nAdd New User list?y/n?";
		cin >> c1;
		if (c1 == 'y' || c1 == 'Y')
		{
			permission += (int)enPermission::paddnewuser;
		}
		cout << "\nDelet User?y/n?";
		cin >> c1;
		if (c1 == 'y' || c1 == 'Y')
		{
			permission += (int)enPermission::pdeletuser;
		}
		cout << "\nUpdate User?y/n?";
		cin >> c1;
		if (c1 == 'y' || c1 == 'Y')
		{
			permission += (int)enPermission::pupdateuser;
		}
		cout << "\nFind User ?y/n?";
		cin >> c1;
		if (c1 == 'y' || c1 == 'Y')
		{
			permission += (int)enPermission::pfnduser;
		}
		cout << "\nManage User ?y/n?";
		cin >> c1;
		if (c1 == 'y' || c1 == 'Y')
		{
			permission += (int)enPermission::Manageusers;
		}
		cout << "\nTransactions?y/n?";
		cin >> c1;
		if (c1 == 'y' || c1 == 'Y')
		{
			permission += (int)enPermission::Transaction;
		}
	}
	return permission;
}

bool CheckAccessPermission(enPermission permission) {
	if (Currentuser.Permissions == enPermission::all)
		return true;
		if ((permission & Currentuser.Permissions) == permission)
			return true;
		else
			return false;
}

void AddNewUser() {
	sUser user;
	user=ReadLogin();
	addlinetofail(UserFileName, ConvertUsingRecordToLine(user));
}

sUser ReadLogin() {
	sUser user;
	cout << "\nEnter Username?";
	getline(cin >> ws, user.UserName);
	while (UserExistsByUsername(user.UserName,UserFileName))
	{
	cout << "\nUser Name With[" << user.UserName << "] already exists , Enter another user name";
	getline(cin >> ws, user.UserName);
	}	
	cout << "\nEnter Password?";
	getline(cin, user.PassWord);
	user.Permissions = FullAscces();

	return user;
}

void AddUserScreen() {
	sUser user;
	char c = 'n';
	cout << "\n--------------------------------------------------------------------\n";
	cout << "\t\t\t Add User Screen\t\t\t\t";
	cout << "\n--------------------------------------------------------------------\n";
	do
	{
		AddNewUser();
		cout << "\nUser Add successfully,do you want add more users ? y/n ?";
		cin >> c;
	} while (toupper(c)=='Y');

}

void deletdatafromfail(string username, vector<sUser>& vdata) {
	char yesno = ' ';
	sUser data;
	if (FindClientByUserName(username, vdata, data))
	{
		PrintUserCard(data);
		cout << "\nAre you sure you want delet this clint y/n\n";
		cin >> yesno;
		if (yesno == 'y' || yesno == 'Y')
		{
			remarktodeletdata(username, vdata);
			SaveUsersDataToFile(UserFileName, vdata);
			vdata = LoadUsersDataFromFile(UserFileName);
			cout << "\nclint delet successfuly:)\n";

		}
	}
	else {
		cout << "\nclint with acountnumber " << username << " is not found\n";
	}
};

string ReadUserName();

void DeletUserScreen(vector <sUser>& vuser) {
	string user=ReadUserName();
	cout << "\n--------------------------------------------------------------------\n";
	cout << "\t\t\t Delet User Screen\t\t\t\t";
	cout << "\n--------------------------------------------------------------------\n";

	if (user== "Admin") {
		cout << "\nYou canot delet this user.\n";
	}
	else
	{
		deletdatafromfail(user, vuser);
	}
}

bool UpdateClientByUserName(string Username, vector<sUser>& vUser)
{
	sUser user;
	char Answer = 'n';
	if (FindClientByUserName(Username, vUser,user))
	{
		PrintUserCard(user);
		cout << "\n\nAre you sure you want update this client? y/n? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (sUser& U: vUser)
			{
				if (U.UserName== Username)
				{
					U = ReadLogin();
					break;
				}
			}
			SaveUsersDataToFile(UserFileName, vUser);
			cout << "\n\nUser Updated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << Username<< ") is Not Found!";
		return false;
	}
}

string ReadUserName() {
	string username;
	cout << "\nPlease enter Username? ";
	cin >> username;
	return username;
}

void UpdateUserScreen() {
	string username;
	cout << "\n--------------------------------------------------------------------\n";
	cout << "\t\t\t Update User Screen\t\t\t\t";
	cout << "\n--------------------------------------------------------------------\n";
vector <sUser>vuser= LoadUsersDataFromFile(UserFileName);
	username = ReadUserName();
	UpdateClientByUserName(username, vuser);
}

void FindUserScreen(vector <sUser> vuser) {
	string user;
	cout << "\n--------------------------------------------------------------------\n";
	cout << "\t\t\t Find User Screen\t\t\t\t";
	cout << "\n--------------------------------------------------------------------\n";
	user = ReadUserName();
	FindUser(vuser,user );
}

void ReadManageMenue(emanageMenue chois, vector<sUser>vuser) {
	switch (chois)
	{
	case emanageMenue::listuser:
	{
		system("cls");
		PrintAllUserData(vuser);
		GotoManageMenue();
		break;
	}

	case emanageMenue::addnewuser:
	{
		system("cls");
		AddUserScreen();
		GotoManageMenue();
		break;
	}

	case emanageMenue::deletuser:
	{
		system("cls");
		DeletUserScreen(vuser);
		GotoManageMenue;
		break;
	}

	case emanageMenue::updateuser:
	{
		system("cls");
		UpdateUserScreen();
		GotoManageMenue();
		break;
	}

	case emanageMenue::fnduser:
	{
		system("cls");
		FindUserScreen(vuser);
		GotoManageMenue();
		break;
	}

	case emanageMenue::Mainmenue:
	{
		system("cls");
		GotoMainMenue();
	}
	}
}

void ManageUserMenueScreen() {
	if (!CheckAccessPermission(enPermission::Manageusers))
	{
		ShowAccessDeniedMessage();
		GotoMainMenue();
	}
	vector <sUser> vuser = LoadUsersDataFromFile(UserFileName);
	system("cls");
	cout << "===================================================\n";
	cout << "\t\tManage User Menue Screen\n";
	cout << "===================================================\n";
	cout << "\t[1] List User.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "===================================================\n";
	ReadManageMenue((emanageMenue)ManageUserChois(), vuser);
}

void readFinalCard(vector<sClient>vClient, emenue chois) {
	switch (chois)
	{
	case emenue::showclientlist:
	{
		system("cls");
		ClientListScreen(vClient);
		GotoMainMenue();
		break;
	}

	case emenue::addnewclient:
	{
		system("cls");
		AddClientScreen(vClient);
		GotoMainMenue();
		break;
	}

	case emenue::deletclient:
	{
		system("cls");
		DeleteClientScreen(vClient);
		GotoMainMenue();
		break;
	}

	case emenue::updateclient:
	{
		system("cls");
		UpdateClientScreen(vClient);
		GotoMainMenue();
		break;
	}

	case emenue::findclient:
	{
		system("cls");
		FindClientScreen(vClient);
		GotoMainMenue();
		break;
	}

	case emenue::transaction:
	{
		system("cls");
		TransactionMenueScreen(vClient);
		break;
	}

	case emenue::manageusers:
	{
		system("cls");
		ManageUserMenueScreen();
		GotoManageMenue();
	}
	case emenue::logout:
	{
		system("cls");
		LogoutScreen();
	}
	}
}

void printfinalcard() {

	vector <sClient> vClient =
		LoadCleintsDataFromFile(ClientsFileName);
	system("cls");
	cout << "===================================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6]  Transaction.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===================================================\n";
	readFinalCard(vClient, (emenue)ReadMainMenueOption());
}

void PrintLoginScreen() {
	string username, password;
	bool loginfail = false;
	vector<sUser>vuser =
		LoadUsersDataFromFile(UserFileName);
	do {
		system("cls");
		cout << "\n=========================================\n";
		cout << "\tLogin Screen";
		cout << "\n=========================================\n";
		if (loginfail)
		{
			cout << "Invlaid Username/Password!\n";
		}
		cout << "Enter User name\n";
		cin >> username;
		cout << "Enter password\n";
		cin >> password;
		loginfail =!LoadUserInfo(username ,password) ;

	} while (loginfail);
	printfinalcard();
}

int main() {
	
	PrintLoginScreen();
	system("pause>0");
	return 0;
}

