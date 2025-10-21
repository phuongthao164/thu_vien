// Quản lý sách
#include <stdio.h>
#include <string.h>
#include "sach.h"

#define MAX_LEN 200
#define MAX_SACH 500

// Dữ liệu sách (mảng tĩnh)
char isbn[MAX_SACH][MAX_LEN];
char ten_sach[MAX_SACH][MAX_LEN];
char tac_gia[MAX_SACH][MAX_LEN];
char nha_xuat_ban[MAX_SACH][MAX_LEN];
int nam_xuat_ban[MAX_SACH];
char the_loai[MAX_SACH][MAX_LEN];
double gia_sach[MAX_SACH];
int so_quyen[MAX_SACH];

// Đếm tổng số sách hiện có trong thư viện
int tong_so_sach = 0;

// Hàm thêm sách
void them_sach() {
    if (tong_so_sach >= MAX_SACH) {
        printf("Da dat so luong sach toi da (%d).\n", MAX_SACH);
        return;
    }

    // Thêm thông tin độc giả vào danh sách
    printf("Vui long nhap thong tin sach moi: \n");

    printf("Nhap ma sach (vd: MS_001): ");
    scanf(" %[^\n]%*c", isbn[tong_so_sach]);  // Mã sách
    printf("Nhap ten sach: ");
    scanf(" %[^\n]%*c", ten_sach[tong_so_sach]);  // Tên sách
    printf("Nhap tac gia: ");
    scanf(" %[^\n]%*c", tac_gia[tong_so_sach]);  // Tác giả
    printf("Nhap nha xuat ban: ");
    scanf(" %[^\n]%*c", nha_xuat_ban[tong_so_sach]);  // Nhà xuất bản
    printf("Nhap nam xuat ban: ");
    scanf(" %d", &nam_xuat_ban[tong_so_sach]);  // Năm xuất bản
    printf("Nhap the loai: ");
    scanf(" %[^\n]%*c", the_loai[tong_so_sach]);  // Thể loại
    printf("Nhap gia sach (vd: 150000): ");
    scanf(" %lf", &gia_sach[tong_so_sach]);  // Giá sách
    printf("Nhap so quyen sach (vd: 15): ");
    scanf(" %d", &so_quyen[tong_so_sach]);  // So quyen sach

    tong_so_sach++;
    printf("Thong tin sach da duoc them thanh cong!\n");
}
    
// Hàm hiển thị danh sách sách
void danh_sach_sach() {
    if (tong_so_sach == 0) {
        printf("Khong co sach trong thu vien.\n");
        return;
    }

    printf("So sach hien tai trong thu vien la: %d\n",tong_so_sach);
    printf("Danh sach cac loai sach trong thu vien:\n");
    for (int i = 0; i < tong_so_sach; i++) {
        printf("Loai sach thu %d (ISBN: %s)\n", i+1, isbn[i]);
        printf("Ten sach: %s, Tac gia: %s, NXB: %s, Nam xuat ban: %d, The loai: %s, Gia sach: %.2f, So quyen sach: %d\n",
                ten_sach[i], tac_gia[i], nha_xuat_ban[i], nam_xuat_ban[i], the_loai[i], gia_sach[i], so_quyen[i]);
        }
    }


// Hàm đọc một dòng an toàn (loại newline ở cuối)
static void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
    else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

 //Hàm chỉnh sửa thông tin sách
void chinh_sua_sach(char *isbn_sach) {
    int found = 0;
    for (int i = 0; i < tong_so_sach; i++) {
        if (strcmp(isbn_sach, isbn[i]) == 0) {  // Tìm theo mã sách
            found = 1;

            // Cập nhật thông tin sách
            printf("Ban dang thuc hien chinh sua thong tin sach co ma %s \n",isbn_sach);
            printf ("Thong tin hien tai la: Ten sach: %s, Tac gia: %s, NXB: %s, Nam xuat ban: %d, The loai: %s, Gia sach: %.2f, So quyen sach: %d\n",
                    ten_sach[i], tac_gia[i], nha_xuat_ban[i], nam_xuat_ban[i], the_loai[i], gia_sach[i], so_quyen[i]);      
            printf("Nhap thong tin ban muon chinh sua:\n");

            // // --- OPTION: cho phép đổi ISBN nhưng kiểm tra trùng ---
            // printf("Nhap so 1 de doi ISBN moi hoac so bat ki de bo qua");
            // int chon;
            // if (scanf("%d", &chon) != 1) {
            //     // dọn input nếu nhập không hợp lệ
            //     int _c;
            //     while ((_c = getchar()) != '\n' && _c != EOF);
            //     printf("Lua chon khong hop le. Bo qua doi ISBN.\n");
            //     chon = 0;
            // } else {
            //     // dọn newline dư
            //     int _c;
            //     while ((_c = getchar()) != '\n' && _c != EOF);
            // }

            // if (chon == 1) {
            //     char new_isbn[256];
            //     printf("Nhap ma sach (vd: MS_001): ");
            //     if (fgets(new_isbn, sizeof(new_isbn), stdin) != NULL) {
            //         size_t ln = strlen(new_isbn);
            //         if (ln && new_isbn[ln-1] == '\n') new_isbn[ln-1] = '\0';
            //     } else {
            //         new_isbn[0] = '\0';
            //     }

            //     if (new_isbn[0] != '\0') {
            //         // kiem tra trung ISBN (khong tinh i)
            //         int dup = 0;
            //         for (int j = 0; j < tong_so_sach; j++) {
            //             if (j == i) continue;
            //             if (strcmp(isbn[j], new_isbn) == 0) { dup = 1; break; }
            //         }
            //         if (dup) {
            //             printf("Loi: ISBN moi da ton tai. Khong doi ISBN.\n");
            //         } else {
            //             // Sao chep an toan
            //             strncpy(isbn[i], new_isbn, sizeof(isbn[i]));
            //             isbn[i][sizeof(isbn[i]) - 1] = '\0';
            //             printf("ISBN da duoc cap nhat.\n");
            //         }
            //     } else {
            //         printf("ISBN rong, giu nguyen.\n");
            //     }
            // } // nếu không chọn đổi thì tiếp tục cập nhật các trường khác

            // Đổi thông tin khác
            printf("Nhap ten sach: ");
            scanf(" %[^\n]%*c", ten_sach[i]);  // Tên sách
            printf("Nhap tac gia: ");
            scanf(" %[^\n]%*c", tac_gia[i]);  // Tác giả
            printf("Nhap nha xuat ban: ");
            scanf(" %[^\n]%*c", nha_xuat_ban[i]);  // Nhà xuất bản
            printf("Nhap nam xuat ban: ");
            scanf(" %d", &nam_xuat_ban[i]);  // Năm xuất bản
            printf("Nhap the loai: ");
            scanf(" %[^\n]%*c", the_loai[i]);  // Thể loại
            printf("Nhap gia sach (vd: 150000): ");
            scanf(" %lf", &gia_sach[i]);  // Giá sách
            printf("Nhap so quyen sach (vd: 15): ");
            scanf(" %d", &so_quyen[i]);  // So quyen sach
            printf("Thong tin sach da duoc cap nhat!\n");
            break;
            }
    }
    if (!found) {
        printf("Khong tim thay loai sach nao co ma %s.\n", isbn_sach);
    };
}

// Tìm index sách theo ISBN (trả index hoặc -1)
int find_index_by_isbn(char *isbn_sach) {
    for (int i = 0; i < tong_so_sach; i++) {
        if (strcmp(isbn[i], isbn_sach) == 0) return i;
    }
    return -1;
}

// Hàm xóa sách theo ISBN
void xoa_sach(char *isbn_sach) {
    int idx = find_index_by_isbn(isbn_sach); 
    if (idx == -1) {
        printf("Khong tim thay sach co ISBN: %s\n", isbn_sach);
        return;
    }

    // Dời các phần tử phía sau lên trước
    for (int j = idx; j < tong_so_sach - 1; j++) {
        strcpy(isbn[j], isbn[j+1]);
        strcpy(ten_sach[j], ten_sach[j+1]);
        strcpy(tac_gia[j], tac_gia[j+1]);
        strcpy(nha_xuat_ban[j], nha_xuat_ban[j+1]);
        nam_xuat_ban[j] = nam_xuat_ban[j+1];
        strcpy(the_loai[j], the_loai[j+1]);
        gia_sach[j] = gia_sach[j+1];
        so_quyen[j] = so_quyen[j+1];
    }

    // Xóa phần tử cuối (tùy chọn, để tránh dữ liệu rác)
    // isbn_arr[tong_so_sach-1][0] = '\0';
    // ten_sach[tong_so_sach-1][0] = '\0';
    // tac_gia[tong_so_sach-1][0] = '\0';
    // nha_xuat_ban[tong_so_sach-1][0] = '\0';
    // nam_xuat_ban[tong_so_sach-1][0] = '\0';
    // the_loai[tong_so_sach-1][0] = '\0';
    // gia_sach[tong_so_sach-1] = 0.0;
    // so_quyen[tong_so_sach-1] = 0;

    tong_so_sach--;
    printf("Da xoa sach co ISBN: %s\n", isbn_sach);
}

// Hàm tìm kiếm sách theo ISBN
void tim_kiem_theo_isbn(char *isbn_sach) {
    int found = 0;
    for (int i = 0; i < tong_so_sach; i++) {
        if (strstr(isbn[i], isbn_sach) != NULL) { // isbn_sach xuất hiện trong isbn[i]
            printf("Tim thay sach co ISBN = %s voi thong tin: Ten sach: %s, Tac gia: %s, NXB: %s, Nam xuat ban: %d, The loai: %s, Gia sach: %.2f, So quyen sach: %d\n",
                isbn[i], ten_sach[i], tac_gia[i], nha_xuat_ban[i], nam_xuat_ban[i], the_loai[i], gia_sach[i], so_quyen[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("Khong tim thay sach nao co ISBN: %99s \n", isbn_sach);
    }
}

// Hàm tìm kiếm sách theo tên sách
void tim_kiem_theo_ten_sach(char *ten_can_tim) {
    int found = 0;
    for (int i = 0; i < tong_so_sach; i++) {
        if (strstr(ten_sach[i], ten_can_tim) != NULL) { // ten_can_tim xuất hiện trong ten_sach[i]
            printf("Tim thay sach co ISBN = %s voi thong tin: Ten sach: %s, Tac gia: %s, NXB: %s, Nam xuat ban: %d, The loai: %s, Gia sach: %.2f, So quyen sach: %d\n",
                isbn[i], ten_sach[i], tac_gia[i], nha_xuat_ban[i], nam_xuat_ban[i], the_loai[i], gia_sach[i], so_quyen[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("Khong tim thay sach nao co ISBN: %99s \n", ten_can_tim);
    }
}

// Hàm quản lý sách
void quan_ly_sach() {
    int choice;
    char isbn_sach[100];
    char ten_can_tim[100];
    printf("1. Xem danh sach sach\n");
    printf("2. Them sach\n");
    printf("3. Chinh sua thong tin sach\n");
    printf("4. Xoa sach\n");
    printf("5. Tim kiem sach theo ISBN\n");
    printf("6. Tim kiem sach theo ten sach\n");
    printf("0. Quay lai\n");
    printf("Chon chuc nang: ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            danh_sach_sach();
            break;
        case 2:
            them_sach(); // Hàm thêm sách
            break;
        case 3:
            printf("Nhap Ma sach ISBN muon chinh sua: ");
            scanf(" %[^\n]%*c", isbn_sach);
            chinh_sua_sach(isbn_sach); // Hàm chỉnh sửa thông tin sách
            break;
        case 4:
            printf("Nhap Ma sach ISBN muon xoa: ");
            scanf(" %[^\n]%*c", isbn_sach);
            xoa_sach(isbn_sach);   // Hàm xóa thông tin sách
            break;  
        case 5:
            printf("Nhap Ma sach ISBN can tim: ");
            scanf(" %[^\n]%*c", isbn_sach);
            tim_kiem_theo_isbn(isbn_sach);   // Hàm tìm kiếm sách theo ISBN
            break;
        case 6:
            printf("Nhap ten sach can tim: ");
            scanf(" %[^\n]%*c", ten_can_tim);
            tim_kiem_theo_ten_sach(ten_can_tim);   // Hàm tìm kiếm sách theo tên 
        case 0:
            return;
        default:
            printf("Lua chon khong hop le!\n");
    }
}
