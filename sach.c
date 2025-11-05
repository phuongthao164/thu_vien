#include <stdio.h>
#include <string.h>
#include "sach.h"
#include "thong_ke.h"

// Khai báo giá trị mặc định
#define MAX_LEN 200
#define MAX_SACH 500

// Đếm tổng số sách hiện có trong thư viện
int tong_so_sach = 0;

// Biến lưu trữ thông tin sách
char isbn[MAX_SACH][MAX_LEN];
char ten_sach[MAX_SACH][MAX_LEN];
char tac_gia[MAX_SACH][MAX_LEN];
char nha_xuat_ban[MAX_SACH][MAX_LEN];
int nam_xuat_ban[MAX_SACH];
char the_loai[MAX_SACH][MAX_LEN];
double gia_sach[MAX_SACH];
int so_quyen[MAX_SACH];
int tong_so_quyen_goc[MAX_SACH];

// CÁC HÀM BỔ SUNG CHO HÀM CHÍNH
// a. Hàm tìm kiếm theo ISBN
int find_index_by_isbn(char *isbn_sach) {
    for (int i = 0; i < tong_so_sach; i++) {
        if (strcmp(isbn[i], isbn_sach) == 0) return i;
    }
    return -1;
}

// b. Hàm xóa bộ nhớ đệm
extern void remove_buffer();

// c. Hàm bổ sung chức năng chọn option menu quản lý
extern int read_int_safe(const char *prompt, int *out);

// d. Sử dụng các hàm xử lý UTF-8 từ doc_gia.c
extern int utf8_display_width(const char *str);
extern void print_padded(const char *str, int target_width);


// CÁC HÀM CHÍNH
// 2b. Hàm thêm sách
void them_sach() {
    if (tong_so_sach >= MAX_SACH) {
        printf("  Đã đạt số lượng sách tối đa (%d).\n", MAX_SACH);
        return;
    }

    // Thêm thông tin sách vào danh sách
    printf("\n-- Vui lòng nhập thông tin sách mới: \n");

    // Mã sách
    printf("  Nhập mã sách (vd: MS_001): ");
    scanf(" %[^\n]%*c", isbn[tong_so_sach]);  

    // Tên sách
    printf("  Nhập tên sách: ");
    scanf(" %[^\n]%*c", ten_sach[tong_so_sach]);  

    // Tác giả
    printf("  Nhập tác giả: ");
    scanf(" %[^\n]%*c", tac_gia[tong_so_sach]);  

    // Nhà xuất bản
    printf("  Nhập nhà xuất bản: ");
    scanf(" %[^\n]%*c", nha_xuat_ban[tong_so_sach]);  

    // Năm xuất bản với validation
    int nam_temp;
    while (1) {
        printf("  Nhập năm xuất bản (1900-2100): ");
        scanf(" %d", &nam_temp);
        
        if (nam_temp < 1900 || nam_temp > 2100) {
            printf("  ❌ Năm xuất bản không hợp lệ! Vui lòng nhập năm từ 1900 đến 2100.\n");
            continue;
        }
        
        nam_xuat_ban[tong_so_sach] = nam_temp;
        break;
    }  

    // Thể loại
    printf("  Nhập thể loại: ");
    scanf(" %[^\n]%*c", the_loai[tong_so_sach]);  

    // Giá sách
    printf("  Nhập giá sách (vd: 150000): ");
    scanf(" %lf", &gia_sach[tong_so_sach]);  

    // Số quyển sách
    printf("  Nhập số quyển sách (vd: 15): ");
    scanf(" %d", &so_quyen[tong_so_sach]);  


    tong_so_quyen_goc[tong_so_sach] = so_quyen[tong_so_sach];
    tong_so_sach++;
    printf("Thông tin sách đã được thêm thành công!\n");
    remove_buffer();
}
    
// 2a.Hàm hiển thị danh sách các loại sách
void danh_sach_sach() {
    if (tong_so_sach == 0) {
        printf("  Không có sách trong thư viện.\n");
        return;
    }

    printf("\n  So sach hien tai trong thu vien la: %d\n", tong_so_sach);
    printf("\n+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    printf("| %-3s | %-13s | %-28s | %-28s | %-23s | %-6s | %-19s | %-10s | %-9s |\n", 
           "STT", "ISBN", "Ten sach", "Tac gia", "NXB", "Nam XB", "The loai", "Gia", "So quyen");
    printf("+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    
    for (int i = 0; i < tong_so_sach; i++) {
        // Format giá sách
        char gia_str[15];
        snprintf(gia_str, 15, "%.0f VND", gia_sach[i]);
        
        // Format năm xuất bản
        char nam_str[7];
        snprintf(nam_str, 7, "%d", nam_xuat_ban[i]);
        
        // Format số quyển
        char so_quyen_str[10];
        snprintf(so_quyen_str, 10, "%d", so_quyen[i]);
        
        // In từng cột với padding UTF-8
        printf("| %-3d | ", i+1);
        print_padded(isbn[i], 13);
        printf(" | ");
        print_padded(ten_sach[i], 28);
        printf(" | ");
        print_padded(tac_gia[i], 28);
        printf(" | ");
        print_padded(nha_xuat_ban[i], 23);
        printf(" | ");
        print_padded(nam_str, 6);
        printf(" | ");
        print_padded(the_loai[i], 19);
        printf(" | ");
        print_padded(gia_str, 10);
        printf(" | ");
        print_padded(so_quyen_str, 9);
        printf(" |\n");
    }
    printf("+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    
    remove_buffer();
}

// 2c. Hàm chỉnh sửa thông tin sách
void chinh_sua_sach(char *isbn_sach) {
    int idx = find_index_by_isbn(isbn_sach);
    if (idx == -1) {
        printf("Không tìm thấy loại sách nào có mã %s.\n", isbn_sach);
    };
    for (int i = 0; i < tong_so_sach; i++) {
        if (strstr(isbn[i], isbn_sach) != NULL) {  
            // Cập nhật thông tin sách
            printf("* Bạn đang thực hiện chỉnh sửa thông tin sách có mã %s \n",isbn_sach);
            printf ("  Thông tin hiện tại là: Tên sách: %s, Tác giả: %s, NXB: %s, Năm xuất bản: %d, Thể loại: %s, Giá sách: %.2f, Số quyển sách: %d\n",
                    ten_sach[i], tac_gia[i], nha_xuat_ban[i], nam_xuat_ban[i], the_loai[i], gia_sach[i], so_quyen[i]);      
            printf("-- Nhập thông tin bạn muốn chỉnh sửa:\n");

            printf("  Nhập tên sách: ");
            scanf(" %[^\n]%*c", ten_sach[i]);  // Tên sách
            printf("  Nhập tác giả: ");
            scanf(" %[^\n]%*c", tac_gia[i]);  // Tác giả
            printf("  Nhập nhà xuất bản: ");
            scanf(" %[^\n]%*c", nha_xuat_ban[i]);  // Nhà xuất bản
            
            // Năm xuất bản với validation
            int nam_temp;
            while (1) {
                printf("  Nhập năm xuất bản (1900-2100): ");
                scanf(" %d", &nam_temp);
                
                if (nam_temp < 1900 || nam_temp > 2100) {
                    printf("  ❌ Năm xuất bản không hợp lệ! Vui lòng nhập năm từ 1900 đến 2100.\n");
                    continue;
                }
                
                nam_xuat_ban[i] = nam_temp;
                break;
            }
            
            printf("  Nhập thể loại: ");
            scanf(" %[^\n]%*c", the_loai[i]);  // Thể loại
            printf("  Nhập giá sách (vd: 150000): ");
            scanf(" %lf", &gia_sach[i]);  // Giá sách
            printf("  Nhập số quyển sách (vd: 15): ");
            scanf(" %d", &so_quyen[i]);  // So quyen sach
            tong_so_quyen_goc[i] = so_quyen[i];
            printf("Thông tin sách đã được cập nhật!\n");
            break;
            }
    }
    remove_buffer();
}

// 2d. Hàm xóa thông tin sách
void xoa_sach(char *isbn_sach) {
    int idx = find_index_by_isbn(isbn_sach); 
    if (idx == -1) {
        printf("-> Không tìm thấy sách có ISBN: %s\n", isbn_sach);
        return;
    }

    // Dời các phần tử phía sau lên trước sau khi xóa
    for (int j = idx; j < tong_so_sach - 1; j++) {
        strcpy(isbn[j], isbn[j+1]);
        strcpy(ten_sach[j], ten_sach[j+1]);
        strcpy(tac_gia[j], tac_gia[j+1]);
        strcpy(nha_xuat_ban[j], nha_xuat_ban[j+1]);
        nam_xuat_ban[j] = nam_xuat_ban[j+1];
        strcpy(the_loai[j], the_loai[j+1]);
        gia_sach[j] = gia_sach[j+1];
        so_quyen[j] = so_quyen[j+1];
        tong_so_quyen_goc[j] = tong_so_quyen_goc[j+1];
    }

    // Xóa phần tử cuối
    isbn[tong_so_sach-1][0] = '\0';
    ten_sach[tong_so_sach-1][0] = '\0';
    tac_gia[tong_so_sach-1][0] = '\0';
    nha_xuat_ban[tong_so_sach-1][0] = '\0';
    nam_xuat_ban[tong_so_sach-1] = '\0';
    the_loai[tong_so_sach-1][0] = '\0';
    gia_sach[tong_so_sach-1] = 0.0;
    so_quyen[tong_so_sach-1] = 0;

    tong_so_sach--;
    printf("Đã xóa sách có ISBN: %s\n", isbn_sach);
    remove_buffer();
}

// 2e. Hàm tìm kiếm sách theo ISBN
void tim_kiem_theo_isbn(char *isbn_sach) {
    int idx = find_index_by_isbn(isbn_sach); 
    if (idx == -1) {
        printf("-> Không tìm thấy sách có ISBN: %s\n", isbn_sach);
        remove_buffer();
        return;
    }

    printf("\n  Tim thay sach co ISBN: %s\n", isbn_sach);
    printf("\n+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    printf("| %-3s | %-13s | %-28s | %-28s | %-23s | %-6s | %-19s | %-10s | %-9s |\n", 
           "STT", "ISBN", "Ten sach", "Tac gia", "NXB", "Nam XB", "The loai", "Gia", "So quyen");
    printf("+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    
    int stt = 1;
    for (int i = 0; i < tong_so_sach; i++) {
        if (strstr(isbn[i], isbn_sach) != NULL) {
            // Format giá sách
            char gia_str[15];
            snprintf(gia_str, 15, "%.0f VND", gia_sach[i]);
            
            // Format năm xuất bản
            char nam_str[7];
            snprintf(nam_str, 7, "%d", nam_xuat_ban[i]);
            
            // Format số quyển
            char so_quyen_str[10];
            snprintf(so_quyen_str, 10, "%d", so_quyen[i]);
            
            // In từng cột với padding UTF-8
            printf("| %-3d | ", stt++);
            print_padded(isbn[i], 13);
            printf(" | ");
            print_padded(ten_sach[i], 28);
            printf(" | ");
            print_padded(tac_gia[i], 28);
            printf(" | ");
            print_padded(nha_xuat_ban[i], 23);
            printf(" | ");
            print_padded(nam_str, 6);
            printf(" | ");
            print_padded(the_loai[i], 19);
            printf(" | ");
            print_padded(gia_str, 10);
            printf(" | ");
            print_padded(so_quyen_str, 9);
            printf(" |\n");
        }
    }
    printf("+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    
    remove_buffer();
}

// 2f. Hàm tìm kiếm sách theo tên sách
void tim_kiem_theo_ten_sach(char *ten_can_tim) {
    int found = 0;
    
    // Kiểm tra xem có sách nào khớp không
    for (int i = 0; i < tong_so_sach; i++) {
        if (strstr(ten_sach[i], ten_can_tim) != NULL) { 
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("-> Không tìm thấy sách nào có tên: %s \n", ten_can_tim);
        remove_buffer();
        return;
    }
    
    printf("\n  Tim kiem sach co ten: %s\n", ten_can_tim);
    printf("\n+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    printf("| %-3s | %-13s | %-28s | %-28s | %-23s | %-6s | %-19s | %-10s | %-9s |\n", 
           "STT", "ISBN", "Ten sach", "Tac gia", "NXB", "Nam XB", "The loai", "Gia", "So quyen");
    printf("+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    
    int stt = 1;
    for (int i = 0; i < tong_so_sach; i++) {
        if (strstr(ten_sach[i], ten_can_tim) != NULL) {
            // Format giá sách
            char gia_str[15];
            snprintf(gia_str, 15, "%.0f VND", gia_sach[i]);
            
            // Format năm xuất bản
            char nam_str[7];
            snprintf(nam_str, 7, "%d", nam_xuat_ban[i]);
            
            // Format số quyển
            char so_quyen_str[10];
            snprintf(so_quyen_str, 10, "%d", so_quyen[i]);
            
            // In từng cột với padding UTF-8
            printf("| %-3d | ", stt++);
            print_padded(isbn[i], 13);
            printf(" | ");
            print_padded(ten_sach[i], 28);
            printf(" | ");
            print_padded(tac_gia[i], 28);
            printf(" | ");
            print_padded(nha_xuat_ban[i], 23);
            printf(" | ");
            print_padded(nam_str, 6);
            printf(" | ");
            print_padded(the_loai[i], 19);
            printf(" | ");
            print_padded(gia_str, 10);
            printf(" | ");
            print_padded(so_quyen_str, 9);
            printf(" |\n");
        }
    }
    printf("+-----+---------------+------------------------------+------------------------------+-------------------------+--------+---------------------+------------+-----------+\n");
    
    remove_buffer();
}

// 2. Hàm menu quản lý sách
void quan_ly_sach() {
    int chon;
    char isbn_sach[100];
    char ten_can_tim[100];
    printf("\n===== MENU SÁCH =====\n");
    printf("\n1. Xem danh sách sách\n");
    printf("2. Thêm sách\n");
    printf("3. Chỉnh sửa thông tin sách\n");
    printf("4. Xóa sách\n");
    printf("5. Tìm kiếm sách theo ISBN\n");
    printf("6. Tìm kiếm sách theo tên sách\n");
    printf("0. Quay lại\n");
    
    if (!read_int_safe("Chọn chức năng: ", &chon)) {
        printf("  -> Nhập không hợp lệ!\n");
        return;
    }
    
    switch(chon) {
        case 1:
            danh_sach_sach();
            break;
        case 2:
            them_sach(); // Hàm thêm sách
            break;
        case 3:
            printf("Nhập Mã sách ISBN muốn chỉnh sửa: ");
            scanf(" %[^\n]%*c", isbn_sach);
            chinh_sua_sach(isbn_sach); // Hàm chỉnh sửa thông tin sách
            break;
        case 4:
            printf("Nhập Mã sách ISBN muốn xóa: ");
            scanf(" %[^\n]%*c", isbn_sach);
            xoa_sach(isbn_sach);   // Hàm xóa thông tin sách
            break;  
        case 5:
            printf("Nhập Mã sách ISBN cần tìm: ");
            scanf(" %[^\n]%*c", isbn_sach);
            tim_kiem_theo_isbn(isbn_sach);   // Hàm tìm kiếm sách theo ISBN
            break;
        case 6:
            printf("Nhập tên sách cần tìm: ");
            scanf(" %[^\n]%*c", ten_can_tim);
            tim_kiem_theo_ten_sach(ten_can_tim);   // Hàm tìm kiếm sách theo tên 
            break;
        case 0:
            return;
        default:
            printf("Chọn không hợp lệ. Thử lại nhé!\n");
    }
}


// ----------------------------làm xong hết thì xóa -------------
// Hàm đọc dữ liệu sách từ file
void doc_du_lieu_sach_tu_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Không thể mở file %s\n", filename);
        return;
    }

    while (!feof(f) && tong_so_sach < MAX_SACH) {
        fscanf(
            f,
            "%[^;];%[^;];%[^;];%[^;];%d;%[^;];%lf;%d\n",
            isbn[tong_so_sach],
            ten_sach[tong_so_sach],
            tac_gia[tong_so_sach],
            nha_xuat_ban[tong_so_sach],
            &nam_xuat_ban[tong_so_sach],
            the_loai[tong_so_sach],
            &gia_sach[tong_so_sach],
            &so_quyen[tong_so_sach]
        );
        tong_so_quyen_goc[tong_so_sach] = so_quyen[tong_so_sach];
        tong_so_sach++;
    }

    fclose(f);
    printf("✅ Đã nạp %d sách từ file %s\n", tong_so_sach, filename);
}

