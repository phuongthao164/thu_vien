// Giao diện chính
#include <stdio.h>
#include "doc_gia.h"
#include "sach.h"
#include "phieu_muon.h"
#include "phieu_tra.h"
#include "thong_ke.h"

// Hàm hiển thị menu
void display_menu(){
    printf("1. Quản lý độc giả\n");
    printf("2. Quản lý sách\n");
    printf("3. Lập phiếu mượn\n");
    printf("4. Lập phiếu trả\n");
    printf("5. Thống kê\n");
    printf("0. Thoát\n");
}

// Hàm chính
int main(){
    int choice;
    doc_du_lieu_sach_tu_file("data_sach.txt");
    doc_du_lieu_doc_gia_tu_file("data_doc_gia.txt");
    doc_du_lieu_phieu_muon_tu_file("data_phieu_muon.txt");
    do {
        display_menu();
        // printf("Chọn chức năng: ");
        // scanf("%d", &choice);
        while (1) {
            printf("Chọn chức năng: ");
            if (scanf("%d", &choice) != 1) {
                printf("  -> Nhập không hợp lệ! Vui lòng nhập số.\n");
                // Xóa phần rác còn trong stdin
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
                continue; // quay lại hỏi tiếp
            }

            // Dọn ký tự '\n' còn dư sau khi nhập đúng
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            break; // nhập hợp lệ thì thoát vòng
        }

        switch(choice) {
            case 1:
                quan_ly_doc_gia();  // Hàm quản lý độc giả
                break;
            case 2:
                quan_ly_sach();    // Hàm quản lý sách
                break;
            case 3:
                quan_ly_phieu_muon();     // Lập phiếu mượn sách
                break;
            case 4:
                quan_ly_phieu_tra();     // Lập phiếu trả sách
                break;
            case 5:
                thong_ke_co_ban();  // Thống kê
                break;
            case 0:
                printf("Thoát chương trình.\n");
                break;
            default:
                printf("Chọn không hợp lệ. Thử lại nhé!\n");
        }
    } while (choice != 0);
    return 0;
}
