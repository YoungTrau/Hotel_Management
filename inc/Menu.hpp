#ifndef MENU_HPP
#define MENU_HPP

#include "MenuItem.hpp"
#include "UI.hpp"
#include <initializer_list>
#include <cstdlib>

class Menu{
private:
    std::string title_;           // Tiêu đề của menu
    std::vector<MenuItem> items_; // Danh sách các mục menu
public:
    // Constructor khởi tạo menu với tiêu đề và danh sách mục
    Menu(const std::string &title, std::initializer_list<MenuItem> items)
        : title_(title), items_(items) {}

    // Hiển thị menu và xử lý lựa chọn
    void showMenu()
    {
        int choice;
        do
        {
            // Hiển thị tiêu đề của menu
            UI::showMessage("\n===== " + title_ + " =====");

            // Hiển thị từng mục menu
            for (const auto &item : items_)
            {
                UI::showMessage(std::to_string(item.getChoice()) + ". " + item.getDescription());
            }

            // Lấy lựa chọn từ người dùng
            choice = UI::getUserChoice("Please Select Feature: ");

            // Thực hiện hành động cho lựa chọn
            bool validChoice = false;
            for (const auto &item : items_)
            {
                if (item.getChoice() == choice)
                {
                    system("cls");
                    item.executeAction(); // Thực hiện hành động
                    validChoice = true;
                    break;
                }
            }

            // Thông báo nếu lựa chọn không hợp lệ
            if (!validChoice)
            {
                UI::showMessage("Invalid Selection, Please Select Again.");
            }

        } while (choice != 0); // Quay lại nếu lựa chọn không phải là thoát
    }
};
#endif // MENU_HPP
