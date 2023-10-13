#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;



enum enChoice { Show = 1, Add, Delete, Update, Find, Transactions };
enum enTransaction { Deposit = 1, Withdraw, TatalBalances };

struct stClient
{
	string Number;
	string pinCode;
	string name;
	string phone;
	double balance;
	bool markToDelete = false;
};
void showTaskTitle(string TaskTitle)
{
	cout << "\n----------------------------------\n";
	cout << "           " << TaskTitle << "          \n";
	cout << "----------------------------------\n";
}
stClient readClient(string AccountNumber)
{
	stClient client;

	client.Number = AccountNumber;

	cout << "\nEnter PinCode? ";
	cin >> client.pinCode;

	cout << "Enter Name? ";
	getline(cin >> ws, client.name);

	cout << "Enter Phone ";
	cin >> client.phone;

	cout << "Enter Account balance ";
	cin >> client.balance;

	return client;
}
string readAccountNumber(string msg)
{
	string AccountNumber = "";

	cout << msg;
	cin >> AccountNumber;

	return AccountNumber;
}
char readAnswer(string msg)
{
	char answer;

	cout << msg;
	cin >> answer;

	return answer;

}
enChoice readOperationChoice(string msg)
{
	enChoice taskNum;
	int choice;
	cout << msg;
	cin >> choice;

	taskNum = (enChoice)choice;

	return taskNum;
}
enTransaction transactionChoice(string msg)
{
	int choice;

	cout << msg;
	cin >> choice;

	return (enTransaction)choice;
}

void MainMenuScreen()
{
	cout << "\n==============================================================\n";
	cout << "                     Main Menu Screen                             ";
	cout << "\n==============================================================\n";
	cout << "                     [1] Show Client List.                      \n";
	cout << "                     [2] Add New Client.                        \n";
	cout << "                     [3] Delete Client.                         \n";
	cout << "                     [4] Update Client.                         \n";
	cout << "                     [5] Find Client.                           \n";
	cout << "                     [6] Transactions.                          \n";
	cout << "                     [7] Exit.                                  \n";
	cout << "==============================================================\n";
}

vector <string> LineDataToVector(string line)
{//A150#//#1234#//#Gomaa ElSherbini#//#00925365232#//#5000.000000

	vector <string> vClient;
	string delim = "#//#";
	short pos = 0;
	string clientItem = "";

	while ((pos = line.find(delim)) != -1)
	{
		clientItem = line.substr(0, pos);
		if (clientItem != "")
		{
			vClient.push_back(clientItem);
		}
		line.erase(0, pos + delim.length());
	}
	if (line != "")
	{
		vClient.push_back(line);
	}
	return vClient;
}

stClient VectorDataToStClient(vector <string> vClient)
{
	stClient client;

	client.Number = vClient[0];
	client.pinCode = vClient[1];
	client.name = vClient[2];
	client.phone = vClient[3];
	client.balance = stod(vClient[4]);

	return client;
}

vector <stClient> loadDataToVector(string fileName)
{
	fstream ClientsFile;
	string line;
	vector <stClient> vStClients;

	ClientsFile.open(fileName, ios::in);

	if (ClientsFile.is_open())
	{
		while (getline(ClientsFile, line))
		{
			if (line != "")
			{
				vStClients.push_back(VectorDataToStClient(LineDataToVector(line)));
			}
		}
		ClientsFile.close();
	}
	return vStClients;
}

string convertStAccountToStringLine(stClient clientRecord)
{
	string recordAataLine;
	string delim = "#//#";

	recordAataLine = clientRecord.Number;
	recordAataLine += delim + clientRecord.pinCode;
	recordAataLine += delim + clientRecord.name;
	recordAataLine += delim + clientRecord.phone;
	recordAataLine += delim + to_string(clientRecord.balance);

	return recordAataLine;
}

void printStClientData(stClient client)
{
	cout << "\nThe following are the eclient details:\n\n";
	cout << "------------------------------------------------------\n";
	cout << " Account Number       :  " << client.Number << endl;
	cout << " Pin Code             :  " << client.pinCode << endl;
	cout << " Name                 :  " << client.name << endl;
	cout << " Phone                :  " << client.phone << endl;
	cout << " Balance              :  " << client.balance << endl;
	cout << "------------------------------------------------------\n";
}

bool findAccountWithAccountNumber(vector <stClient>& vStClients, string AccountNumber, stClient& clientToFind)
{
	for (stClient& client : vStClients)
	{
		if (client.Number == AccountNumber)
		{
			clientToFind = client;
			return 1;
		}
	}
	return 0;
}

void findAccount(vector <stClient>& vStClients, string fileName)
{
	system("cls");
	showTaskTitle("Find Client Screen");

	vStClients = loadDataToVector(fileName);
	string AccountNumber = readAccountNumber("\n Please Enter Account Number? ");
	stClient clientToFind;

	if (findAccountWithAccountNumber(vStClients, AccountNumber, clientToFind))
	{
		printStClientData(clientToFind);
	}
	else
	{
		cout << "\n\n Client with account Number (" << AccountNumber << ") Not Found!\n";
	}
	system("pause");
}

void updateClient(vector <stClient>& vStClients, string AccountNumber)
{
	stClient updatedClient = readClient(AccountNumber);

	for (stClient& client : vStClients)
	{
		if (client.Number == AccountNumber)
		{
			client = updatedClient;
			break;
		}
	}
}

void saveAccountsToFile(string fileName, vector <stClient> vStClients)
{
	fstream myFile;

	myFile.open(fileName, ios::out);

	if (myFile.is_open())
	{
		for (stClient& client : vStClients)
		{
			if (client.markToDelete != 1)
			{
				myFile << convertStAccountToStringLine(client) << "\n";
			}
		}
		myFile.close();
	}
}

void updateAccount(vector <stClient>& vStClients, string fileName)
{
	system("cls");
	showTaskTitle("Update Client Screen:");

	char answer;
	stClient client;
	string AccountNumber = readAccountNumber("\n Please Enter Account Number? ");

	if (findAccountWithAccountNumber(vStClients, AccountNumber, client))
	{
		printStClientData(client);

		answer = readAnswer("\nAre You sure to update This Account? y/n ");

		if (answer == 'y' || answer == 'Y')
		{
			updateClient(vStClients, AccountNumber);
			saveAccountsToFile(fileName, vStClients);
			cout << "\n\nClient uodated Successfully!\n";
		}
	}
	else
	{
		cout << "\n\n Client with account Number (" << AccountNumber << ") Not Found!\n";
	}
	system("pause");
}

void showClientList(vector <stClient>& vStClients)
{
	system("cls");

	cout << endl;
	cout << "                                         Client List (" << vStClients.size() << ") Client(s)\n";
	cout << " ___________________________________________________________________________________________________________________\n";
	cout << endl;
	cout << " |" << " Account Number " << "|" << " Pin Code " << "|" << " Client Name " << setw(35) << "|" << " Phone " << setw(12) << "|" << " Balance " << setw(10) << "|" << endl;
	cout << " ___________________________________________________________________________________________________________________\n";
	cout << endl;


	for (stClient& line : vStClients)
	{
		cout << "| " << setw(15) << left << line.Number
			<< "| " << setw(9) << left << line.pinCode
			<< "| " << setw(46) << left << line.name
			<< "| " << setw(17) << left << line.phone
			<< "| " << setw(17) << left << line.balance
			<< "|";

		cout << endl;
	}
	cout << " ___________________________________________________________________________________________________________________\n";
	
	cout << "\nPress any key to go back to main menu screen... ";
	system("pause");
}

void addNewClient(string fileName, vector <stClient>& vStClients)
{
	char answer = 'y';
	stClient client;
	string AccountNumber;

	while (answer == 'y' || answer == 'Y')
	{
		system("cls");

		showTaskTitle("Add New Client Screen");
		cout << "Adding New Client:\n";

		AccountNumber = readAccountNumber("\n Please Enter Account Number? ");

		while (findAccountWithAccountNumber(vStClients, AccountNumber, client))
		{
			cout << "\nClient with [" << AccountNumber << "] aready exists, ";
			AccountNumber = readAccountNumber("Enter another account number? ");
		}

		client = readClient(AccountNumber);
		vStClients.push_back(client);
		saveAccountsToFile(fileName, vStClients);

		cout << "\n\nClient added successfully,";
		answer = readAnswer("\ndo you want to add more clients? Y/N ");
	}
}

void markAccountToDelete(vector <stClient>& vStClients, string AccountNumber)
{
	for (stClient& client : vStClients)
	{
		if (client.Number == AccountNumber)
		{
			client.markToDelete = 1;
			break;
		}
	}
}

void deleteAccount(vector <stClient>& vStClients, string fileName)
{
	system("cls");

	showTaskTitle("Delete Client Screen");

	char answer;
	stClient client;
	string AccountNumber = readAccountNumber("\n Please Enter Account Number? ");

	if (findAccountWithAccountNumber(vStClients, AccountNumber, client))
	{
		printStClientData(client);

		answer = readAnswer("\nAre you sure do you want to delete This Client? Y/N ");

		if (answer == 'y' || answer == 'Y')
		{
			markAccountToDelete(vStClients, AccountNumber);
			saveAccountsToFile(fileName, vStClients);
			cout << "\n\nClient deleted Successfully!\n";
		}
	}
	else
	{
		cout << "\n\n Client with account Number (" << AccountNumber << ") Not Found!\n";
	}
	system("pause");
}

void TransactionsMenuScreen()
{
	cout << "\n==============================================================\n";
	cout << "                     Main Menu Screen                             ";
	cout << "\n==============================================================\n";
	cout << "                     [1] Deposit.                               \n";
	cout << "                     [2] Withdraw.                              \n";
	cout << "                     [3] Total Balances.                        \n";
	cout << "                     [4] Main menu.                             \n";
	cout << "==============================================================\n";
}

void depositToAccountBalance(vector <stClient>& vStClients, string AccountNumber, int depositAmmount)
{
	for(stClient &client : vStClients)
	{
		if (client.Number == AccountNumber)
		{
			client.balance += depositAmmount;
		}
	}
}

void depositTransaction(vector <stClient> &vStClients, string fileName)
{
	system("cls");
	showTaskTitle("Deposit Screen");

	int depositAmmount = 0;
	stClient client;
	string AccountNumber = readAccountNumber("\nPlease enter Account number? ");
	char answer;

	if (findAccountWithAccountNumber(vStClients, AccountNumber, client))
	{
		printStClientData(client);

		cout << "\nPlease enter deposit ammount? ";
		cin >> depositAmmount;

		answer = readAnswer("\nAre you sure you want to perform this transaction? y/n ? ");

		if (answer == 'y' || answer == 'Y')
		{
			depositToAccountBalance(vStClients, AccountNumber, depositAmmount);
			saveAccountsToFile(fileName, vStClients);
			cout << "\nDone successfully, New Balance is: " << client.balance + depositAmmount << endl;

		}
	}
	else
	{
		cout << "\n\n Client with account Number (" << AccountNumber << ") Not Found!\n\n";
	}
	system("pause");
}

int calculateSumOfNumbers(vector <stClient>& vStClients)
{
	int sumOfBalances = 0;

	for (stClient &client : vStClients)
	{
		sumOfBalances += client.balance;
	}
	return sumOfBalances;
}

void showBalancesList(vector <stClient> &vStClients, string fileName)
{
	int totalBalances = 0;

	system("cls");

	cout << endl;
	cout << "                                         Balances List (" << vStClients.size() << ") Client(s)\n";
	cout << " ___________________________________________________________________________________________________________________\n";
	cout << endl;
	cout << " |" << " Account Number " << " |" << " Client Name " << setw(36) << " |" << " Balance " << setw(10) << endl;
	cout << " ___________________________________________________________________________________________________________________\n";
	cout << endl;

	for (stClient& line : vStClients)
	{
		cout << " | " << setw(15) << left << line.Number
			<< " | " << setw(46) << left << line.name
			<< " | " << setw(17) << left << line.balance;
		cout << endl;
	}
	cout << " ___________________________________________________________________________________________________________________\n";

	cout << "                                             Total Balances = " << calculateSumOfNumbers(vStClients) << endl;

	cout << "\nPress any key to go back to transactons menu screen... ";
	system("pause");
}

void withdrawToAccountBalance(vector <stClient>& vStClients, string AccountNumber, int WithdrawAmmount)
{
	for (stClient& client : vStClients)
	{
		if (client.Number == AccountNumber)
		{
			client.balance -= WithdrawAmmount;
		}
	}
}

void WithdrawTransaction(vector <stClient>& vStClients, string fileName)
{
	system("cls");
	showTaskTitle("Withdraw Screen");

	int WithdrawAmmount = 0;
	stClient client;
	string AccountNumber = readAccountNumber("\nPlease enter Account number? ");
	char answer;

	if (findAccountWithAccountNumber(vStClients, AccountNumber, client))
	{
		printStClientData(client);

		cout << "\nPlease enter withdraw ammount? ";
		cin >> WithdrawAmmount;

		while (WithdrawAmmount > client.balance)
		{
			cout << "\n\nAmount Exceeds the balance, you can withdraw up to : " << client.balance << endl;
			
			cout << "\nPlease enter another withdraw ammount? ";
			cin >> WithdrawAmmount;
		}

		answer = readAnswer("\nAre you sure you want to perform this transaction? y/n ? ");

		if (answer == 'y' || answer == 'Y')
		{
			withdrawToAccountBalance(vStClients, AccountNumber, WithdrawAmmount);
			saveAccountsToFile(fileName, vStClients);

			cout << "\nDone successfully, New Balance is: " << client.balance - WithdrawAmmount << endl;
		}
	}
	else
	{
		cout << "\n\n Client with account Number (" << AccountNumber << ") Not Found!\n\n";
	}
	system("pause");
}

void transactionProccess(enTransaction transaction, vector <stClient> vStClients, string fileName)
{
	vStClients = loadDataToVector(fileName);

	switch (transaction)
	{
	case Deposit:
		depositTransaction(vStClients, fileName);
		break;
	case Withdraw:
		WithdrawTransaction(vStClients, fileName);
		break;
	case TatalBalances:
		showBalancesList(vStClients, fileName);
		break;
	}
}

void startTransactions(vector <stClient>& vStClients, string fileName)
{
	enTransaction transaction;

	do
	{
		system("cls");
		TransactionsMenuScreen();
		transaction = transactionChoice("\nchoose what do you want to do from [1 to 4]? ");
		transactionProccess(transaction, vStClients, fileName);

	} while (transaction != 4);

}

void doOperation(enChoice choice, vector <stClient> vStClients, string fileName)
{
	vStClients = loadDataToVector(fileName);
	switch (choice)
	{
	case Show:
		showClientList(vStClients);
		break;
	case Add:
		addNewClient(fileName, vStClients);
		break;
	case Delete:
		deleteAccount(vStClients, fileName);
		break;
	case Update:
		updateAccount(vStClients, fileName);
		break;
	case Find:
		findAccount(vStClients, fileName);
		break;
	case Transactions:
		startTransactions(vStClients, fileName);
		break;
	}
}

void start()
{
	string fileName = "clients.txt";
	vector <stClient> vStClients = loadDataToVector(fileName);

	enChoice choice;
	
	do
	{
		MainMenuScreen();
		choice = readOperationChoice("\nchoose what do you want to do from [1 to 7]? ");

		doOperation(choice, vStClients, fileName);

		system("cls");
			
	} while (choice != 7);
}



int main()
{

	start();



	system("cls");
	showTaskTitle("End Program :-)");



	system("pause>0");

	return 0;
}