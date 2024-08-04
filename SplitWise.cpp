#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>

using namespace std;

// Enum for ExpenseSplitType
enum class ExpenseSplitType {
    EQUAL,
    UNEQUAL,
    PERCENTAGE
};

// Forward declarations
class User;
class Balance;
class UserExpenseBalanceSheet;
class Split;
class Expense;
class Group;
class ExpenseSplit;

// Define Balance
class Balance {
public:
    double amountGetBack = 0;
    double amountOwe = 0;
};

// Define UserExpenseBalanceSheet
class UserExpenseBalanceSheet {
public:
    double totalPayment = 0;
    double totalYourExpense = 0;
    double totalYouGetBack = 0;
    double totalYouOwe = 0;
    unordered_map<string, Balance> userVsBalance;
};

// Define Split
class Split {
public:
    User* user;
    double amountOwe;

    Split(User* user, double amountOwe) : user(user), amountOwe(amountOwe) {}
    User* getUser() const { return user; }
    double getAmountOwe() const { return amountOwe; }
};

// Define ExpenseSplit
class ExpenseSplit {
public:
    virtual void validateSplitRequest(const vector<Split*> &splitList, double totalAmount) = 0;
};

// Define EqualExpenseSplit
class EqualExpenseSplit : public ExpenseSplit {
public:
    void validateSplitRequest(const vector<Split*> &splitList, double totalAmount) override {
        double amountShouldBePresent = totalAmount / splitList.size();
        for (const auto &split : splitList) {
            if (split->getAmountOwe() != amountShouldBePresent) {
                throw runtime_error("Invalid amount in equal split");
            }
        }
    }
};

// Define PercentageExpenseSplit
class PercentageExpenseSplit : public ExpenseSplit {
public:
    void validateSplitRequest(const vector<Split*> &splitList, double totalAmount) override {
        // Implement percentage validation logic if needed
    }
};

// Define UnequalExpenseSplit
class UnequalExpenseSplit : public ExpenseSplit {
public:
    void validateSplitRequest(const vector<Split*> &splitList, double totalAmount) override {
        // Implement unequal validation logic if needed
    }
};

// Define SplitFactory
class SplitFactory {
public:
    static ExpenseSplit* getSplitObject(ExpenseSplitType splitType) {
        switch (splitType) {
        case ExpenseSplitType::EQUAL:
            return new EqualExpenseSplit();
        case ExpenseSplitType::UNEQUAL:
            return new UnequalExpenseSplit();
        case ExpenseSplitType::PERCENTAGE:
            return new PercentageExpenseSplit();
        default:
            return nullptr;
        }
    }
};

// Define User
class User {
public:
    User(const string &userId, const string &userName)
        : userId(userId), userName(userName) {}

    string getUserId() const { return userId; }
    UserExpenseBalanceSheet& getUserExpenseBalanceSheet() { return balanceSheet; }

private:
    string userId;
    string userName;
    UserExpenseBalanceSheet balanceSheet;
};

// Define Expense
class Expense {
public:
    string expenseId;
    string description;
    double expenseAmount;
    User* paidByUser;
    ExpenseSplitType splitType;
    vector<Split*> splitDetails;

    Expense(string expenseId, double expenseAmount, string description, User* paidByUser,
            ExpenseSplitType splitType, const vector<Split*> &splitDetails)
        : expenseId(expenseId), expenseAmount(expenseAmount), description(description), paidByUser(paidByUser),
          splitType(splitType), splitDetails(splitDetails) {}

    ~Expense() {
        for (auto* split : splitDetails) {
            delete split;
        }
    }
};

// Define Group
class Group {
public:
    string groupId;
    string groupName;
    vector<User*> groupMembers;
    vector<Expense*> expenseList;

    Group(string id, string name) : groupId(id), groupName(name) {}

    void addMember(User* member) {
        groupMembers.push_back(member);
    }

    Expense* createExpense(string expenseId, string description, double expenseAmount,
                           const vector<Split*> &splitDetails, ExpenseSplitType splitType, User* paidByUser) {
        ExpenseSplit* expenseSplit = SplitFactory::getSplitObject(splitType);
        expenseSplit->validateSplitRequest(splitDetails, expenseAmount);

        Expense* expense = new Expense(expenseId, expenseAmount, description, paidByUser, splitType, splitDetails);
        updateUserExpenseBalanceSheet(paidByUser, splitDetails, expenseAmount);
        expenseList.push_back(expense);
        return expense;
    }

    void updateUserExpenseBalanceSheet(User* expensePaidBy, const vector<Split*> &splits, double totalExpenseAmount) {
        UserExpenseBalanceSheet &paidByUserExpenseSheet = expensePaidBy->getUserExpenseBalanceSheet();
        paidByUserExpenseSheet.totalPayment += totalExpenseAmount;

        for (const auto &split : splits) {
            User* userOwe = split->getUser();
            UserExpenseBalanceSheet &oweUserExpenseSheet = userOwe->getUserExpenseBalanceSheet();
            double oweAmount = split->getAmountOwe();

            if (expensePaidBy->getUserId() == userOwe->getUserId()) {
                paidByUserExpenseSheet.totalYourExpense += oweAmount;
            } else {
                paidByUserExpenseSheet.totalYouGetBack += oweAmount;
                Balance &userOweBalance = paidByUserExpenseSheet.userVsBalance[userOwe->getUserId()];
                userOweBalance.amountGetBack += oweAmount;

                oweUserExpenseSheet.totalYouOwe += oweAmount;
                oweUserExpenseSheet.totalYourExpense += oweAmount;

                Balance &userPaidBalance = oweUserExpenseSheet.userVsBalance[expensePaidBy->getUserId()];
                userPaidBalance.amountOwe += oweAmount;
            }
        }
    }

    void showBalanceSheetOfUser(User* user) {
        cout << "---------------------------------------\n";
        cout << "Balance sheet of user: " << user->getUserId() << "\n";

        UserExpenseBalanceSheet &userExpenseBalanceSheet = user->getUserExpenseBalanceSheet();

        cout << "TotalYourExpense: " << userExpenseBalanceSheet.totalYourExpense << "\n";
        cout << "TotalGetBack: " << userExpenseBalanceSheet.totalYouGetBack << "\n";
        cout << "TotalYourOwe: " << userExpenseBalanceSheet.totalYouOwe << "\n";
        cout << "TotalPaymentMade: " << userExpenseBalanceSheet.totalPayment << "\n";

        for (const auto &entry : userExpenseBalanceSheet.userVsBalance) {
            cout << "userID: " << entry.first << " YouGetBack: " << entry.second.amountGetBack << " YouOwe: " << entry.second.amountOwe << "\n";
        }
        cout << "---------------------------------------\n";
    }

    ~Group() {
        for (auto* expense : expenseList) {
            delete expense;
        }
        for (auto* member : groupMembers) {
            // Do not delete members as they may be shared elsewhere
        }
    }
};

// Define UserController
class UserController {
    vector<User*> userList;

public:
    void addUser(User* user) {
        userList.push_back(user);
    }

    User* getUser(const string &userID) {
        for (const auto &user : userList) {
            if (user->getUserId() == userID) {
                return user;
            }
        }
        return nullptr;
    }

    vector<User*> getAllUsers() const {
        return userList;
    }

    ~UserController() {
        for (auto* user : userList) {
            delete user;
        }
    }
};

// Define Splitwise
class Splitwise {
    UserController userController;
    vector<Group*> groupList;

public:
    void setupUserAndGroup() {
        addUsersToSplitwiseApp();
        User* user1 = userController.getUser("U1001");
        User* user2 = userController.getUser("U1002");
        User* user3 = userController.getUser("U1003");

        Group* group = new Group("G1001", "Group1");
        group->addMember(user1);
        group->addMember(user2);
        group->addMember(user3);

        vector<Split*> splits = {new Split(user2, 30.0), new Split(user3, 30.0)};
        group->createExpense("E1001", "Lunch", 60.0, splits, ExpenseSplitType::EQUAL, user1);

        group->showBalanceSheetOfUser(user1);
        group->showBalanceSheetOfUser(user2);
        group->showBalanceSheetOfUser(user3);

        for (auto* g : groupList) {
            delete g;
        }
    }

    void addUsersToSplitwiseApp() {
        User* user1 = new User("U1001", "User1");
        User* user2 = new User("U1002", "User2");
        User* user3 = new User("U1003", "User3");

        userController.addUser(user1);
        userController.addUser(user2);
        userController.addUser(user3);
    }
};

int main() {
    Splitwise splitwise;
    splitwise.setupUserAndGroup();
    return 0;
}
