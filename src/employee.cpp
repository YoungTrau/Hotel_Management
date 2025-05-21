#include "../inc/employee.hpp"
Employee::Employee()
{
        int read_result = file_Employee.LogData(GUESS_CSV_PATH,GUESS_CSV);
        if(read_result == FILE_EMPTY) UI::showMessage("No Booking Information");
        else if(read_result== FAIL)  UI::showMessage("Failed to Load Guest Information");
        else                         UI::showMessage("Successfully Load Guest Information");
}
void Employee::ShowEmployeeMenu(const string& user_name){
    if(ReturnPosition(user_name) == "RECEPTIONIST"){
        Menu menu{"------" + ReturnPosition(user_name) + "------", {
                    {1, "View Shifts", [this,user_name](){ ReviewWorkshift(user_name);}},
                    {2, "Edit Information", [this,user_name](){ EditInfoEmployee(user_name);}},
                    {3, "Book Room", [this](){ BookRoom(); }},
                    {4, "Check Out", [this](){ UnbookRoom(); }},
                    {5, "Room List", [this](){ ShowRoom(); }},
                    {6, "Store Guest Information", [this](){ SaveGuessInfo(); }},
                    {0, "Back", [](){ UI::showMessage("Please Select Feature"); }}}};
        menu.showMenu();
    }
    else{
        Menu menu{"------" + ReturnPosition(user_name) + "------", {
                    {1, "View Shifts", [this,user_name](){ ReviewWorkshift(user_name);}},
                    {2, "Edit Information", [this,user_name](){ EditInfoEmployee(user_name); }},
                    {0, "Back", [](){ UI::showMessage("Please Select Feature"); }}}};
        menu.showMenu();
    }
}
void Employee::ReviewWorkshift(const string& user_name)
{       UI::showMessage("--------SHIFTS--------");
        while(1){
            EmployeeManager::ShowWorkshiftTablebyPhone(user_name);
            inputvalidchoice:
            int choice = UI::getUserChoice("Enter 0 to Return: ");
            if(choice == 0){
                system("cls");
                break;
            }
            else{
                UI::showMessage("Invalid Selection");
                goto inputvalidchoice;
            }
        }
}
void Employee::EditInfoEmployee(const string& user_name)
{
    UI::showMessage("--------Edit Information--------");
    while(1){
        UI::showMessage("Please Enter a 6-Character Password");
        UI::showMessage("----------------------------------");
        string old_password = UI::getInputString("Please Enter Old Password: ");
        string new_password =  UI::getInputString("Please Enter a New Password: ");

        //kiểm tra mật khẩu mới có hợp không và mật khẩu cũ có khớp với sdt của nhân viên hiện tại không
        if(EmployeeManager::editEmployeePassword(accountList,user_name,old_password,new_password) == FAIL){
            UI::showMessage("Invalid Password");
        }
        UI::showMessage("\nChange Password Successfully");
        UI::showMessage("------------------------------");
        inputvalidchoice:
        UI::showMessage("1.Continue Editing Information");
        UI::showMessage("0.Back");
        int choice = UI::getUserChoice("Please Select Feature: ");
        if (choice == 0) {
            system("cls");
            break;
        }
        else{
            if(choice != 1){
                UI::showMessage("Invalid Selection");
                goto inputvalidchoice;
            }
        }
    }
}

void Employee::BookRoom()
{
    UI::showMessage("--------Book Room--------");
    while (1)
    {
        if (guessEmp.ShowListRoomAvailable() == FAIL)
        {
            UI::showMessage("No rooms available yet");
        }
        UI::showMessage("X.In use");
        UI::showMessage("O.Available");
        InputValidRoom:
        int room = UI::getUserChoice("Please select room:");
        if (RoomManager::IsRoomAvailableExist(room) == NOT_EXIST)
        {
            UI::showMessage("Room is not available, please re-enter");
            goto InputValidRoom;
        }
        string name = UI::getInputString("Please enter guest name:");
        string phone = UI::getInputString("Please enter guest phone number:");
        string checkin = UI::getInputString("Please enter check-in time:");
        if (guessEmp.BookRoom(room, name, phone, checkin) == SUCCESS)
        {
            UI::showMessage("Room booked for guest successfully");
        }
        else
        {
            UI::showMessage("Room booked, please select another room");
            goto InputValidRoom;
        }

        UI::showMessage("1.Continue booking room");
        UI::showMessage("0.Back");
        inputvalidchoice:
        int choice = UI::getUserChoice("Please enter your selection:");
        UI::showMessage("--------------------------------");
        if (choice == 0) {
            system("cls");
            break;
        }
        else{
            if(choice != 1){
                UI::showMessage("Invalid choice");
                goto inputvalidchoice;
            }
        }
    }
}
void Employee::UnbookRoom()
{
    UI::showMessage("--------Check out--------");
    while (1)
    {
        if(guessEmp.ShowListGuess() == FAIL){
            UI::showMessage("No room booked yet");
            return;
        }
        InputValidPhone:
        string phone = UI::getInputString("Please enter phone number:");
        Guess *guess = guessEmp.GuessInfo(phone);
        if (guess == NULL)
        {
            UI::showMessage("Cannot find customer with phone number" + phone);
            goto InputValidPhone;
        }
        else
        {
            string checkout = UI::getInputString("Please enter checkout day:");
            guessEmp.UnbookRoom(*guess,checkout);
            guessEmp.ServiceFeedbackInfo(*guess);
            UI::showMessage("Guest check out successful");
            UI::showMessage("------------------------------------");
            UI::showMessage("1.Continue");
            UI::showMessage("0.Back");
            UI::showMessage("--------------------------------");
            inputvalidchoice:
            int choice = UI::getUserChoice("Please enter your selection:");
            if (choice == 0){
                system("cls");
                break;
            }
            else{
                if(choice != 1){
                    UI::showMessage("Invalid choice");
                    goto inputvalidchoice;
                }
            }
        }
    }
}
void Employee::ShowRoom()
{
    UI::showMessage("--------ROOM LIST--------");
    while (1)
    {
        if (guessEmp.ShowListRoomAvailable() == FAIL)
        {
            UI::showMessage("No rooms available yet");
        }
        UI::showMessage("X.In use");
        UI::showMessage("O.Available");
        UI::showMessage("-----------------------------");
        UI::showMessage("0.Back");
        inputvalidchoice:
        int choice = UI::getUserChoice("Please enter your selection:");
        if (choice == 0){
            system("cls");
            break;
        }
        else{
            if(choice != 1){
                UI::showMessage("Invalid choice");
                goto inputvalidchoice;
            }
        }
    }
}

void Employee::SaveGuessInfo(){
    UI::showMessage("---------INFORMATION RETENTION----------");
    while(1){
        inputvalidchoice:
        UI::showMessage("1.Save room reservation list");
        UI::showMessage("0.Back");
        int choice = UI::getUserChoice("Please enter your selection:");
        if(choice == 1) {
            bool save_result = file_Employee.SaveData(GUESS_CSV_PATH,GUESS_CSV);
            if(save_result == SUCCESS)    {UI::showMessage("Saved successfully");}
            else                         {UI::showMessage("No information");}
            break;
        }
        else if (choice == 0){
            system("cls");
            break;
        }
        else
        {
            if (choice != 1){
                UI::showMessage("Invalid selection, please re-enter");
                goto inputvalidchoice;
            }
        }
    }
}

string Employee::ReturnPosition(const string& user_name){
    //duyệt qua danh sách nhân viên
    for(auto& employee : list_employee){
        //so sánh account và sdt của nhân viên hiện tại
        if(employee.getPhoneNumber() == user_name){
            string position = employee.getPosition();
            if(position == "BV")      {position ="SECURITY";}
            else if(position == "LT") {position = "RECEPTIONIST";}
            else if(position == "DB") {position = "COOK";}
            else if(position == "TV") {position = "JANITOR";}
            else                      {position = "EMPLOYEE";}
            return position;
        }
    }
    return string("Invalid");
}