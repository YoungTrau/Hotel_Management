#include "../inc/Manager.hpp"
/* Menu lớn */
Manager::Manager(){
    int read_result = file_manager.LogData(ROOM_CSV_PATH,ROOM_CSV);
    if(read_result == FILE_EMPTY) UI::showMessage("Room list information not available");
    else if(read_result == FAIL)  UI::showMessage("Download service list failed");
    else                          UI::showMessage("Download room list successfully");
    read_result = file_manager.LogData(SERVICE_CSV_PATH,SERVICE_CSV);
    if(read_result == FILE_EMPTY) UI::showMessage("Room service information not available");
    else if(read_result == FAIL)  UI::showMessage("Service list download failed");
    else                          UI::showMessage("Service list download successful");
    read_result = file_manager.LogData(EMPLOYEE_CSV_PATH,EMMPLOYEE_CSV);
    if(read_result == FILE_EMPTY) UI::showMessage("Employee information not available");
    else if(read_result == FAIL)  UI::showMessage("Employee list download failed");
    else                          UI::showMessage("Employee list download successful");
  
}
void Manager::ShowManagerMenu()
{
    
    Menu mainmenu{"Manager", {{1,"Employee management", [this](){ manageEmployee(); }},
                              {2, "Room management", [this](){ manageRoom(); }},
                              {3, "Service management", [this](){ manageService(); }},
                              {4, "Information storage", [this](){ SaveDataCSV(); }},
                              {0, "Back", [](){ UI::showMessage("Please select feature"); }}}};
    mainmenu.showMenu();
}

void Manager::manageEmployee() {
    Menu employeeMenu("Employee management", {
        {1, "Add employee", [this]() { addEmployee(); }},
        {2, "Edit employee information", [this]() { editEmployee(); }},
        {3, "Delete employee", [this]() { deleteEmployee(); }},
        {4, "View Employee List", [this]() { listEmployees(); }},
        {5, "Set Employee Shifts",[this](){setWorkShift();}},
        {6, "Working Hours",[this](){TableSchedule();}},
        {0, "Back", []() { UI::showMessage("Back"); }}
    });

    employeeMenu.showMenu();  // Hiển thị menu quản lý nhân viên
}

void Manager::addEmployee() {
   
    while (true) {
        UI::showMessage("=== Add employee ===");

        user_input:
        // Nhập thông tin từ người dùng
        std::string name = UI::getInputString("Enter employee name: ");
        std::string phoneNumber = UI::getInputString("Enter phone number (ID): ");
        std::string position = UI::getInputString("Enter job position (Janitor-TV/Security-BV/Cook-DB/Receptionist-LT): ");

        // Thêm nhân viên mới vào danh sách
        if(employeeManage.addEmployee(accountList, name, phoneNumber, position) == FAIL){
            UI::showMessage("Please re-enter information: ");
            goto user_input;
        }

        // In ra tên của nhân viên mới
        UI::showMessage("Add employee successfully " + name);

        // Tự động đăng ký tài khoản cho nhân viên với mật khẩu mặc định
        UI::showMessage("----------------------------");
        inputvalidchoice:
        UI::showMessage("1.Add another employee");
        UI::showMessage("0.Back");
        // Hiển thị tùy chọn để thêm nhân viên khác
        int choice = UI::getUserChoice("Please select feature: ");        
        if (choice == 0) {
            system("cls");
            break;  // Quay lại nếu người dùng chọn 0
        }
        else
        {
            if (choice != 1){
                system("cls");
                UI::showMessage("Invalid selection, please re-enter");
                goto inputvalidchoice;
            }
        }
    }
    
}

void Manager::editEmployee() {
    while(1){
        UI::showMessage("Edit employee information...");
        // Danh sách nhân viên
        employeeManage.listEmployees();

        //nhập thông tin chỉnh sửa
        UI::showMessage("Please enter the name of the employee whose information you want to edit: ");
        std::string id = UI::getInputString("ID: ");
        std::string newname = UI::getInputString("Enter new name: ");
        std::string newposition= UI::getInputString("Enter new job: ");
    
        //thực hiện cập nhật thông tin nhân viên
        employeeManage.editEmployeeById(id,newname,newposition);

        UI::showMessage("1.Continue");
        UI::showMessage("0.Back");
        int choice = UI::getUserChoice("Enter selection: ");
        if(choice == 0){
            system("cls");
            break;
        }
    }
}

void Manager::deleteEmployee() {
    while(1){
        UI::showMessage("Delete Employee...");
        employeeManage.listEmployees();
        UI::showMessage("------------------");
        std::string id = UI::getInputString("Enter the ID of the employee you want to delete: ");
        employeeManage.deleteEmployeeById(id);

        UI::showMessage("0.Back");
        option:
        int choice = UI::getUserChoice("Press 0 to go back: ");
        if(choice == 0){
            system("cls");
            break;
        }
        else{
            UI::showMessage("Invalid selection please re-enter");
            goto option;
        }
            
    }
}

void Manager::listEmployees() {
    while(1){
        UI::showMessage("=== Employee List ===");

        employeeManage.listEmployees();

        UI::showMessage("------------------");
        option:
        int choice = UI::getUserChoice("Back (0): ");
        if(choice == 0){
            system("cls");
            break;
        }
        else{
            UI::showMessage("Invalid selection please re-enter");
            goto option;
        }
    }
}

void Manager::setWorkShift(){
    UI::showMessage("------------------SET SHIFT FOR EMPLOYEES--------------------");
    while(1){
        employeeManage.listEmployees();
        UI::showMessage("-------------------------");
        setschedule:
        string id = UI::getInputString("Enter the id of the employee you want to set the shift for: ");
        UI::showMessage("Please enter the corresponding weekly shift in the following format \n(Day - Session, Day - Session, Day - Session): 2-S, 2-C, 3-C, 4-T, 5-C, 6-S ");
        string workShift = UI::getInputString("Day - Session:");
        employeeManage.setWorkShift(id,workShift);
        UI::showMessage("-------------------------");
        UI::showMessage("1.Continue");
        UI::showMessage("0.Back");
        int choice = UI::getUserChoice("Please select feature: ");
        if(choice == 0){
            system("cls");
            break;
        }
        goto setschedule;
    }
}

void Manager::TableSchedule(){
    while(1){
    UI::showMessage("-------Employee working hours-------");
       employeeManage.ShowWorkshiftTable();
       int choice = UI::getUserChoice("Select 0 to return: ");
       if(choice == 0){
          system("cls");
          break; 
       }
    }

}

                                    /* QUẢN LÝ PHÒNG */
void Manager::manageRoom()
{
    Menu Roommenu{"Room management", {{1, "Add room", [this](){ AddRoom(); }},
                                    {2, "Delete room", [this](){ DeleteRoom(); }},
                                    {3, "View room status", [this](){ ShowListRoom(); }},
                                    {0, "Back", [](){ UI::showMessage("Please select feature"); }}}};
    Roommenu.showMenu();
}
void Manager::AddRoom()
{
    UI::showMessage("---------ADD ROOM---------");
    while (1)
    {
        inputvalidroom:
        int new_room = UI::getUserChoice("Enter room add:");
        if(roomManage.IsRoomDefaultExist(new_room) == NOT_EXIST){
            UI::showMessage("Room not available, please enter another room");
            goto inputvalidroom;
        }
        if(roomManage.AddRoom(new_room) == FAIL)
        {
            UI::showMessage("Room added, please enter another room");
            goto inputvalidroom;
        }
        
        UI::showMessage("Add success");
        inputvalidchoice:
        UI::showMessage("1.Continue");
        UI::showMessage("0.Back");
        int choice = UI::getUserChoice("Please enter your selection:");
        UI::showMessage("--------------------------------");
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
void Manager::ShowListRoom()
{
    UI::showMessage("---------VIEW ROOM STATUS---------");
    while (1)
    {
        roomManage.ShowListRoomDefault();
        UI::showMessage("\nU:\tUnavailable");
        UI::showMessage("A:\tAvailable");
        inputvalidchoice:
        UI::showMessage("\n1.View room information");
        UI::showMessage("0.Back");
        int choice = UI::getUserChoice("\nPlease select:");
        if (choice == 0){
            system("cls");
            break;
        }
        else if (choice == 1)
        {
            UI::showMessage("---------View room information---------");
            inputvalidroom:
            int room = UI::getUserChoice("Please enter room number:");
            if (RoomManager::IsRoomAvailableExist(room) == EXIST)
            {
                if (roomManage.RoomInfo(room) == FAIL)
                {
                    UI::showMessage("No customer information");  
                }
                inputvalidchoice1:
                UI::showMessage("1.Continue");
                UI::showMessage("0.Back");
                int choice = UI::getUserChoice("\nPlease enter your choice:");
                if (choice == 0){
                    system("cls");
                    break;
                }
                else if(choice == 1){
                    system("cls");
                    goto inputvalidroom;
                }
                else{
                    system("cls");
                    UI::showMessage("Invalid choice");
                    goto inputvalidchoice1;
                }
            }
            else
            {
                system("cls");
                UI::showMessage("Room not available");
                goto inputvalidchoice1;
            }
        }
        else
        {
            system("cls");
            UI::showMessage("Invalid selection, please re-enter");
            goto inputvalidchoice;
        }
    }
}
void Manager::DeleteRoom()
{
    UI::showMessage("---------DELETE ROOM---------");
    while (1)
    {
        roomManage.ShowListRoomDefault();
        int room_delete = UI::getUserChoice("Enter the room to be cleaned:");
        roomManage.deleteRoom(room_delete);
        UI::showMessage("\n1.Continue");
        UI::showMessage("0.Back");
        inputvalidchoice:
        int choice = UI::getUserChoice("Please enter your selection:");
        if (choice == 0){
            system("cls");
            break;
        }
        else
        {
            if (choice != 1){
                system("cls");
                UI::showMessage("Invalid selection, please re-enter");
                goto inputvalidchoice;
            }
        }
    }
}
                                           /* QUẢN LÝ DỊCH VỤ */
void Manager::manageService()
{
    //doc danh sach dich vu tu file csv

    Menu Roommenu{"Dich vu", {{1, "Add service", [this](){ AddService(); }},
                              {2, "Delete service", [this](){ DeleteService(); }},
                              {3, "Service list", [this](){ ShowListService(); }},
                              {0, "Back", [](){ UI::showMessage("Please select feature"); }}}};
    Roommenu.showMenu();
}
void Manager::AddService()
{
    UI::showMessage("---------ADD SERVICES---------");
    while (1)
    {
        UI::showMessage("Services available");
        UI::showMessage("----------------------------");
        UI::showMessage("1.Gym\n2.Birthday party\n3.Car rental\n4.Pet care\n5.Food delivery\n6.Spa and massage");
        UI::showMessage("----------------------------");
        InputValidService:
        int service_number = UI::getUserChoice("Enter the order number of the service you want to add.:");
        
        //kiem tra dich vu co hop le khong
        auto IsServiceInputValid = [service_number]()
        {
            return (service_number < TOTAL_SERVICE) ? true : false;
        };

        if (!IsServiceInputValid())
        {
            UI::showMessage("Please enter valid options");
            goto InputValidService;
        }

        int price = UI::getUserChoiceFloat("Service fee VND:");

        if(serviceManage.AddService(service_number, price) == FAIL)
        {
            UI::showMessage("Service is available, please choose another service");
        }
        else UI::showMessage("\nAdd service successfully");
        UI::showMessage("1.Continue");
        UI::showMessage("0.Back");
        inputvalidchoice:
        int choice = UI::getUserChoice("Please enter your choice:");
        UI::showMessage("--------------------------------");
        if (choice == 0){
            system("cls");
            break;
        }
        else
        {
            if (choice != 1){
                system("cls");
                UI::showMessage("Invalid selection, please re-enter");
                goto inputvalidchoice;
            }
        }
        system("cls");
    }
}
void Manager::DeleteService()
{
    UI::showMessage("---------DELETE SERVICES---------");
    while (1)
    {

        int service_number = UI::getUserChoice("Please enter the number of the service you want to delete.:");
        if (serviceManage.deleteService(service_number) == FAIL)
        {
            UI::showMessage("No service to delete");
        }
        else UI::showMessage("\nDelete service successfully");
        UI::showMessage("1.Continue");
        UI::showMessage("0.Back");
        inputvalidchoice:
        int choice = UI::getUserChoice("Please enter your selection:");
        if (choice == 0){
            system("cls");
            break;
        }
        else
        {
            if (choice != 1){
                system("cls");
                UI::showMessage("Invalid selection, please re-enter");
                goto inputvalidchoice;
            }
        }
    }
}
void Manager::ShowListService()
{
    UI::showMessage("---------LIST OF SERVICES---------");
    while (1)
    {
        if (serviceManage.ShowListService() == FAIL)
        {
            UI::showMessage("No service available");
        }
        UI::showMessage("0.Back");
        inputvalidchoice:
        int choice = UI::getUserChoice("Please enter your selection:");
        if (choice == 0){
            system("cls");
            break;
        }
        else
        {
            if (choice != 1){
                system("cls");
                UI::showMessage("Invalid selection, please re-enter");
                goto inputvalidchoice;
            }
        }
    }
}

void Manager::SaveDataCSV(){
    UI::showMessage("---------INFORMATION STORAGE----------");
    while(1){
        intputvalidchoice:
        UI::showMessage("1.Store room list");
        UI::showMessage("2.Store service list");
        UI::showMessage("3.Store employee information list");
        int choice = UI::getUserChoice("Please enter your selection:");
        bool save_result;
        if(choice == 1) {
            save_result = file_manager.SaveData(ROOM_CSV_PATH,ROOM_CSV);
            if(save_result == SUCCESS)  UI::showMessage("Saved successfully");
            else UI::showMessage("No information");
        }
        else if(choice == 2) {
            save_result = file_manager.SaveData(SERVICE_CSV_PATH,SERVICE_CSV);
            if(save_result == SUCCESS) UI::showMessage("Saved successfully");
            else UI::showMessage("No information");
        }
        else if(choice == 3){
            save_result = file_manager.SaveData(EMPLOYEE_CSV_PATH,EMMPLOYEE_CSV);
            if(save_result == SUCCESS) UI::showMessage("Saved successfully");
            else UI::showMessage("No information");
        }
        else{
            system("cls");
            UI::showMessage("Invalid selection, please re-enter");
            goto intputvalidchoice;
        }
        UI::showMessage("1.Continue");
        UI::showMessage("0.Back");
        choice = UI::getUserChoice("Please enter selection: ");
        if(choice == 0){
            system("cls");
            break;
        }
        goto intputvalidchoice;
    }

}
