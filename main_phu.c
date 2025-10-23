// Giao diện chính
#include <stdio.h>
#include "doc_gia.h"
#include "sach.h"
#include "phieu_muon_tra.h"
#include "thong_ke.h"

// Hàm hiển thị menu
void display_menu(){
    printf("1. Quản lý độc giả\n");
    printf("2. Quản lý sách\n");
    printf("3. Lập phiếu mượn trả\n");
    printf("4. Lập phiếu trả sách\n");
    printf("5. Thống kê\n");
    printf("0. Thoát\n");
}

// Hàm chính
int main(){
    int choice;

    do {
        display_menu();
        printf("Chọn chức năng: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                quan_ly_doc_gia();  // Hàm quản lý độc giả
                break;
            case 2:
    //             quan_ly_sach();    // Hàm quản lý sách
    //             break;
    //         case 3:
    //             quan_ly_phieu_muon();;     // Lập phiếu mượn sách
    //             break;
    // //         case 4:
    // //             return_book();     // Lập phiếu trả sách
    // //             break;
    //         case 5:
    //             thong_ke_co_ban();  // Thống kê
                break;
            case 0:
                printf("Thoát chương trình.\n");
                break;
            default:
                printf("Chọn không hợp lệ. Thử lại nhé!\n");
        }
    } while (choice != 0);
    //display_users();
    return 0;
}
