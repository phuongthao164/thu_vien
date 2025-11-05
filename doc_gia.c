#include <stdio.h>
#include <string.h>
#include "doc_gia.h"
#include "thong_ke.h"
#include <time.h>

// Khai báo giá trị mặc định
#define MAX_LEN 100
#define MAX_USERS 100

// Đếm tổng số độc giả hiện có trong thư viện
int tong_so_doc_gia = 0;

// Biến lưu trữ thông tin độc giả
int id_doc_gia[MAX_USERS];
char ho_ten[MAX_USERS][MAX_LEN];
char cmnd[MAX_USERS][MAX_LEN];
char ngay_sinh[MAX_USERS][MAX_LEN];
char gioi_tinh[MAX_USERS][MAX_LEN];
char email[MAX_USERS][MAX_LEN];
char dia_chi[MAX_USERS][MAX_LEN];
char ngay_lap_the[MAX_USERS][MAX_LEN];
char ngay_het_han_the[MAX_USERS][MAX_LEN];

// CÁC HÀM BỔ SUNG CHO HÀM CHÍNH
// a. Hàm tính ngày lập thẻ
void ngay_lap(int index) {
    time_t t_hien_tai = time(NULL);
    char *time_str_ht = ctime(&t_hien_tai);
    char month_str_ht[4];
    int day_ht, year_ht;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_ht, "%*s %3s %d %*s %d", month_str_ht, &day_ht, &year_ht);
    int month_ht = (strstr(months, month_str_ht) - months) / 3 + 1;

    snprintf(ngay_lap_the[index], MAX_LEN, "%02d/%02d/%04d", day_ht, month_ht, year_ht);
    printf("  Ngày lập thẻ là: %s\n", ngay_lap_the[index]);
}

// b. Hàm tính ngày hết hạn thẻ
void ngay_het_han(int index) {
    time_t t_hien_tai = time(NULL);
    time_t t_het_han = t_hien_tai + (time_t)(4 * 365 + 1) * 24 * 60 * 60;
    char *time_str_hh = ctime(&t_het_han);
    char month_str_hh[4];
    int day_hh, year_hh;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_hh, "%*s %3s %d %*s %d", month_str_hh, &day_hh, &year_hh);
    int month_hh = (strstr(months, month_str_hh) - months) / 3 + 1;

    snprintf(ngay_het_han_the[index], MAX_LEN, "%02d/%02d/%04d", day_hh, month_hh, year_hh);
    printf("  Ngày hết hạn thẻ là: %s\n", ngay_het_han_the[index]);
}

// c. Hàm xóa bộ nhớ đệm
extern void remove_buffer();

// d. Hàm bổ sung chức năng chọn option menu quản lý
int read_int_safe(const char *prompt, int *out) {
    char buf[64];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    return (sscanf(buf, "%d", out) == 1);
}

// Hàm tính độ dài hiển thị (visual width) của chuỗi UTF-8
int utf8_display_width(const char *str) {
    int width = 0;
    unsigned char c;
    
    while (*str) {
        c = (unsigned char)*str;
        
        if (c < 0x80) {
            // ASCII character (1 byte)
            width++;
            str++;
        } else if (c < 0xE0) {
            // 2-byte UTF-8 character
            width++;
            str += 2;
        } else if (c < 0xF0) {
            // 3-byte UTF-8 character
            width++;
            str += 3;
        } else {
            // 4-byte UTF-8 character
            width++;
            str += 4;
        }
    }
    
    return width;
}

// Hàm in chuỗi với padding phù hợp cho UTF-8
void print_padded(const char *str, int target_width) {
    int display_width = utf8_display_width(str);
    printf("%s", str);
    
    // Thêm khoảng trắng để đạt độ rộng mong muốn
    for (int i = display_width; i < target_width; i++) {
        printf(" ");
    }
}

// Hàm kiểm tra năm nhuận, ngày hợp lệ và email được định nghĩa trong thong_ke.c
extern int la_nam_nhuan(int nam);
extern int kiem_tra_ngay_hop_le(const char *ngay_str);
extern int kiem_tra_email_hop_le(const char *email_str);

// e. Hàm kiểm tra CMND có đúng 12 số không
int kiem_tra_cmnd_hop_le(char *cmnd_check) {
    int len = strlen(cmnd_check);
    
    // Kiểm tra độ dài phải đúng 12 ký tự
    if (len != 12) {
        return 0;
    }
    
    // Kiểm tra tất cả ký tự phải là số
    for (int i = 0; i < len; i++) {
        if (cmnd_check[i] < '0' || cmnd_check[i] > '9') {
            return 0;
        }
    }
    
    return 1;
}

// f. Hàm kiểm tra CMND có bị trùng không (bỏ qua index hiện tại khi chỉnh sửa)
int kiem_tra_cmnd_trung(char *cmnd_check, int skip_index) {
    for (int i = 0; i < tong_so_doc_gia; i++) {
        // Bỏ qua index hiện tại (dùng cho chỉnh sửa)
        if (i == skip_index) continue;
        
        // Bỏ qua độc giả đã bị xóa
        if (cmnd[i][0] == '\0') continue;
        
        // Kiểm tra trùng lặp
        if (strcmp(cmnd_check, cmnd[i]) == 0) {
            return 1; // Trùng
        }
    }
    return 0; // Không trùng
}


// CÁC HÀM CHÍNH
// 1b. Hàm thêm độc giả
void them_doc_gia() {
    if (tong_so_doc_gia >= MAX_USERS) {
    printf("  Đã đạt số lượng độc giả tối đa (%d).\n", MAX_USERS);
    return;
    }

    // Mã độc giả
    int ma_doc_gia = tong_so_doc_gia + 1;
    printf("\n  --Mã độc giả của bạn là: %d \n",ma_doc_gia);

    // ID độc giả
    id_doc_gia[tong_so_doc_gia] = ma_doc_gia;

    // Thêm thông tin độc giả vào danh sách
    printf("--Vui lòng nhập thông tin của bạn: \n");

    // Họ tên
    printf("  Nhập họ tên: ");
    scanf(" %[^\n]%*c", ho_ten[tong_so_doc_gia]);  

    // CMND với validation
    char cmnd_temp[MAX_LEN];
    while (1) {
        printf("  Nhập CMND (12 số): ");
        scanf("%99s", cmnd_temp);
        
        // Kiểm tra CMND có đúng 12 số không
        if (!kiem_tra_cmnd_hop_le(cmnd_temp)) {
            printf("  ❌ CMND không hợp lệ! CMND phải có đúng 12 chữ số.\n");
            continue;
        }
        
        // Kiểm tra CMND có bị trùng không
        if (kiem_tra_cmnd_trung(cmnd_temp, -1)) {
            printf("  ❌ CMND đã tồn tại trong hệ thống! Vui lòng nhập CMND khác.\n");
            continue;
        }
        
        // CMND hợp lệ, lưu vào mảng
        strcpy(cmnd[tong_so_doc_gia], cmnd_temp);
        break;
    }  

    // Ngày tháng năm sinh với validation
    char ngay_sinh_temp[MAX_LEN];
    while (1) {
        printf("  Nhập ngày tháng năm sinh (dd/mm/yyyy): ");
        scanf("%99s", ngay_sinh_temp);
        
        if (!kiem_tra_ngay_hop_le(ngay_sinh_temp)) {
            printf("  ❌ Ngày sinh không hợp lệ! Vui lòng nhập đúng định dạng dd/mm/yyyy.\n");
            printf("     (Ví dụ: 01/01/2000, chú ý ngày/tháng phải hợp lệ)\n");
            continue;
        }
        
        strcpy(ngay_sinh[tong_so_doc_gia], ngay_sinh_temp);
        break;
    }  

    // Giới tính
    printf("  Nhập giới tính (Nhập 1.Nam , 2.Nữ , 3. Khác): ");
    int chon_gioi_tinh;
    scanf("%d", &chon_gioi_tinh);  
    while(1){
        if (chon_gioi_tinh == 1){
            strcpy(gioi_tinh[tong_so_doc_gia], "Nam");
            break;
        } else if (chon_gioi_tinh == 2){
            strcpy(gioi_tinh[tong_so_doc_gia], "Nữ");
            break;
        } else if (chon_gioi_tinh == 3){
            strcpy(gioi_tinh[tong_so_doc_gia], "Khác");
            break;
        } else {
            printf("Nhập không đúng, vui lòng nhập lại!\n");
        }
    }

    // Email với validation
    char email_temp[MAX_LEN];
    while (1) {
        printf("  Nhập email: ");
        scanf("%99s", email_temp);
        
        if (!kiem_tra_email_hop_le(email_temp)) {
            printf("  ❌ Email không hợp lệ! Email phải có dạng: user@domain.com\n");
            printf("     (Ví dụ: nguyenvana@gmail.com)\n");
            continue;
        }
        
        strcpy(email[tong_so_doc_gia], email_temp);
        break;
    }
    
    // Địa chỉ
    printf("  Nhập địa chỉ: ");
    scanf(" %[^\n]%*c", dia_chi[tong_so_doc_gia]);  

    // Ngày lập thẻ và 
    ngay_lap(tong_so_doc_gia);

    // Ngày hết hạn
    ngay_het_han(tong_so_doc_gia);

    tong_so_doc_gia++;
    printf("-> Độc giả đã được thêm thành công\n");
    remove_buffer();
}

// 1a. Hàm hiển thị danh sách độc giả
void danh_sach_doc_gia() {
    if (tong_so_doc_gia == 0) {
        printf("  Không có độc giả nào trong thư viện.\n");
        return;
    }

    printf("\n  So doc gia hien tai cua thu vien la: %d\n",tong_so_doc_gia);
    printf("\n+-----+-----+--------------------------------+----------------------+------------+-----------+-------------------------------+--------------------------------+------------+------------+\n");
    printf("| %-3s | %-3s | %-30s | %-20s | %-10s | %-9s | %-29s | %-30s | %-10s | %-10s |\n", 
           "STT", "ID", "Ho ten", "CMND", "Ngay sinh", "Gioi tinh", "Email", "Dia chi", "Ngay lap", "Het han");
    printf("+-----+-----+--------------------------------+----------------------+------------+-----------+-------------------------------+--------------------------------+------------+------------+\n");
    
    int stt = 1;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        if (ho_ten[i][0] == '\0') {
            // Độc giả đã bị xóa
            printf("| %-3d | %-3d | %-30s | %-20s | %-10s | %-9s | %-29s | %-30s | %-10s | %-10s |\n",
                   stt++, id_doc_gia[i], "[DA XOA]", "-", "-", "-", "-", "-", "-", "-");
        } else {
            // In từng cột với padding UTF-8
            printf("| %-3d | %-3d | ", stt++, id_doc_gia[i]);
            print_padded(ho_ten[i], 30);
            printf(" | ");
            print_padded(cmnd[i], 20);
            printf(" | ");
            print_padded(ngay_sinh[i], 10);
            printf(" | ");
            print_padded(gioi_tinh[i], 9);
            printf(" | ");
            print_padded(email[i], 29);
            printf(" | ");
            print_padded(dia_chi[i], 30);
            printf(" | ");
            print_padded(ngay_lap_the[i], 10);
            printf(" | ");
            print_padded(ngay_het_han_the[i], 10);
            printf(" |\n");
        }
    }
    printf("+-----+-----+--------------------------------+----------------------+------------+-----------+-------------------------------+--------------------------------+------------+------------+\n");
    
    remove_buffer();
}

// 1c. Hàm chỉnh sửa thông tin một độc giả
void chinh_sua_doc_gia(int id) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        if (id == id_doc_gia[i]) {  
            found = 1;
            // Đối với độc giả đã bị xóa -> Cho nhập lại thông tin độc giả mới
            if (ho_ten[i][0] == '\0') {
                printf("-> Thông tin của độc giả %d đã bị xóa. Bạn có muốn cập nhật thông tin mới cho độc giả này không?\n", id);
                printf("Nhập 1. Tiếp tục hoặc Nhập kí tự bất kì để Thoát. \n");
                int chon;
                scanf("%d", &chon);
                if (chon == 1) {    
                    printf("-- Nhập thông tin mới:\n");
                    printf("  Nhập họ tên: ");
                    scanf(" %[^\n]%*c", ho_ten[i]);
                    
                    // CMND với validation
                    char cmnd_temp[MAX_LEN];
                    while (1) {
                        printf("  Nhập CMND (12 số): ");
                        scanf("%99s", cmnd_temp);
                        
                        if (!kiem_tra_cmnd_hop_le(cmnd_temp)) {
                            printf("  ❌ CMND không hợp lệ! CMND phải có đúng 12 chữ số.\n");
                            continue;
                        }
                        
                        if (kiem_tra_cmnd_trung(cmnd_temp, i)) {
                            printf("  ❌ CMND đã tồn tại trong hệ thống! Vui lòng nhập CMND khác.\n");
                            continue;
                        }
                        
                        strcpy(cmnd[i], cmnd_temp);
                        break;
                    }
                    
                    // Ngày sinh với validation
                    char ngay_sinh_temp[MAX_LEN];
                    while (1) {
                        printf("  Nhập ngày tháng năm sinh (dd/mm/yyyy): ");
                        scanf("%99s", ngay_sinh_temp);
                        
                        if (!kiem_tra_ngay_hop_le(ngay_sinh_temp)) {
                            printf("  ❌ Ngày sinh không hợp lệ! Vui lòng nhập đúng định dạng dd/mm/yyyy.\n");
                            continue;
                        }
                        
                        strcpy(ngay_sinh[i], ngay_sinh_temp);
                        break;
                    }
                    
                    printf("  Nhập giới tính (Nhập 1.Nam , 2.Nữ , 3. Khác): ");
                    int chon_gioi_tinh;
                    scanf("%d", &chon_gioi_tinh);
                    while(1){
                        if (chon_gioi_tinh == 1){
                            strcpy(gioi_tinh[i], "Nam");
                            break;
                        } else if (chon_gioi_tinh == 2){
                            strcpy(gioi_tinh[i], "Nữ");
                            break;
                        } else if (chon_gioi_tinh == 3){
                            strcpy(gioi_tinh[i], "Khác");
                            break;
                        } else {
                            printf("Nhập không đúng, vui lòng nhập lại!\n");
                        }
                    }
                    
                    // Email với validation
                    char email_temp[MAX_LEN];
                    while (1) {
                        printf("  Nhập email: ");
                        scanf("%99s", email_temp);
                        
                        if (!kiem_tra_email_hop_le(email_temp)) {
                            printf("  ❌ Email không hợp lệ! Email phải có dạng: user@domain.com\n");
                            continue;
                        }
                        
                        strcpy(email[i], email_temp);
                        break;
                    }
                    
                    printf("  Nhập địa chỉ: ");
                    scanf(" %[^\n]%*c", dia_chi[i]);
                    ngay_lap(i);
                    ngay_het_han(i);
                    printf("-> Thông tin độc giả đã được cập nhật mới!\n");
                    break;
                } else {
                    remove_buffer();
                    printf("Kết thúc chỉnh sửa! \n");
                    break;
                }
            } else {
            // Cập nhật thông tin độc giả đang hoạt động
            printf("* Bạn đang thực hiện chỉnh sửa thông tin của độc giả %d \n",id);
            printf ("  Thông tin hiện tại là: Họ tên: %s, CMND: %s, Ngày sinh: %s, Giới tính: %s, Email: %s, Địa chỉ: %s, Ngày lập thẻ: %s, Ngày hết hạn thẻ: %s\n",
                        ho_ten[i], cmnd[i], ngay_sinh[i], gioi_tinh[i], email[i], dia_chi[i], ngay_lap_the[i], ngay_het_han_the[i]);      
            printf("-- Nhập thông tin bạn muốn chỉnh sửa:\n");
            // Họ tên
            printf("  Nhập họ tên chỉnh sửa: ");
            scanf(" %[^\n]%*c", ho_ten[i]);  
            
            // CMND với validation
            char cmnd_temp[MAX_LEN];
            while (1) {
                printf("  Nhập CMND chỉnh sửa (12 số): ");
                scanf("%99s", cmnd_temp);
                
                if (!kiem_tra_cmnd_hop_le(cmnd_temp)) {
                    printf("  ❌ CMND không hợp lệ! CMND phải có đúng 12 chữ số.\n");
                    continue;
                }
                
                if (kiem_tra_cmnd_trung(cmnd_temp, i)) {
                    printf("  ❌ CMND đã tồn tại trong hệ thống! Vui lòng nhập CMND khác.\n");
                    continue;
                }
                
                strcpy(cmnd[i], cmnd_temp);
                break;
            }
            
            // Ngày sinh với validation
            char ngay_sinh_temp[MAX_LEN];
            while (1) {
                printf("  Nhập ngày sinh chỉnh sửa (dd/mm/yyyy): ");
                scanf("%99s", ngay_sinh_temp);
                
                if (!kiem_tra_ngay_hop_le(ngay_sinh_temp)) {
                    printf("  ❌ Ngày sinh không hợp lệ! Vui lòng nhập đúng định dạng dd/mm/yyyy.\n");
                    continue;
                }
                
                strcpy(ngay_sinh[i], ngay_sinh_temp);
                break;
            }
            
            // Giới tính
            printf("  Nhập giới tính chỉnh sửa (Nhập 1.Nam , 2.Nữ , 3. Khác): ");
            int chon_gioi_tinh;
            scanf("%d", &chon_gioi_tinh);  // Giới tính
            while(1){
                if (chon_gioi_tinh == 1){
                    strcpy(gioi_tinh[i], "Nam");
                    break;
                } else if (chon_gioi_tinh == 2){
                    strcpy(gioi_tinh[i], "Nữ");
                    break;
                } else if (chon_gioi_tinh == 3){
                    strcpy(gioi_tinh[i], "Khác");
                    break;
                } else {
                    printf("Nhập không đúng, vui lòng nhập lại!\n");
                }
            }
            
            // Email với validation
            char email_temp[MAX_LEN];
            while (1) {
                printf("  Nhập email chỉnh sửa: ");
                scanf("%99s", email_temp);
                
                if (!kiem_tra_email_hop_le(email_temp)) {
                    printf("  ❌ Email không hợp lệ! Email phải có dạng: user@domain.com\n");
                    continue;
                }
                
                strcpy(email[i], email_temp);
                break;
            }
            
            // Địa chỉ
            printf("  Nhập địa chỉ chỉnh sửa: ");
            scanf(" %[^\n]%*c", dia_chi[i]);  
            // Ngày lập thẻ và hết hạn không được chỉnh sửa
            printf("  Ngày lập thẻ và Ngày hết hạn không thể thay đổi!\n");

            printf("-> Thông tin độc giả đã được cập nhật!\n");
            break;
            }
        }
    }
    if (!found) {
        printf("-> Không tìm thấy độc giả nào có mã %d.\n", id);
    }
    remove_buffer();
}

// 1d. Hàm xóa thông tin một độc giả
void xoa_doc_gia(int id) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        if (id == id_doc_gia[i]) { 
            found = 1;
            // Cập nhật các thông tin của độc giả đã xóa thành rỗng để không ảnh hưởng đến id của các độc giả khác
            ho_ten[i][0] = '\0';
            cmnd[i][0] = '\0';
            ngay_sinh[i][0] = '\0';
            gioi_tinh[i][0] = '\0';
            email[i][0] = '\0';
            dia_chi[i][0] = '\0';
            ngay_lap_the[i][0] = '\0';
            ngay_het_han_the[i][0] = '\0';
            printf("-> Đã xóa thông tin của độc giả có mã %d.\n", id);
            break;
        }
    }
    if (!found) {
        printf("-> Không tìm thấy độc giả nào có mã %d.\n", id);
    }
    remove_buffer();
}

// 1e. Hàm tìm kiếm độc giả theo CMND
void tim_kiem_theo_cmnd(char *cmnd_can_tim) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        // Bỏ qua độc giả đã bị xóa (CMND rỗng)
        if (cmnd[i][0] == '\0') continue;
        // Hiển thị thông tin của độc giả đã tìm thấy
        if (strcmp(cmnd_can_tim, cmnd[i]) == 0) {
            printf("  Tìm thấy độc giả có id = %d với thông tin: \n",id_doc_gia[i]);
            printf("  Họ tên: %s\n", ho_ten[i]);
            printf("  CMND: %s\n", cmnd[i]);
            printf("  Ngày sinh: %s\n", ngay_sinh[i]);
            printf("  Giới tính: %s\n", gioi_tinh[i]);
            printf("  Email: %s\n", email[i]);
            printf("  Địa chỉ: %s\n", dia_chi[i]);
            printf("  Ngày lập thẻ: %s\n", ngay_lap_the[i]);
            printf("  Ngày hết hạn thẻ: %s\n", ngay_het_han_the[i]);                
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("-> Không tìm thấy độc giả nào có CMND: %s.\n", cmnd_can_tim);
    }
    remove_buffer();
}

// 1f. Hàm tìm kiếm độc giả theo họ tên
void tim_kiem_theo_ten_doc_gia(char *ten_can_tim) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        // Bỏ qua độc giả đã bị xóa (họ tên rỗng)
        if (ho_ten[i][0] == '\0') continue;

        if (strstr(ho_ten[i], ten_can_tim) != NULL) { 
            printf("  Tìm thấy độc giả có id = %d với thông tin: \n",id_doc_gia[i]);
            printf("  Họ tên: %s\n", ho_ten[i]);
            printf("  CMND: %s\n", cmnd[i]);
            printf("  Ngày sinh: %s\n", ngay_sinh[i]);
            printf("  Giới tính: %s\n", gioi_tinh[i]);
            printf("  Email: %s\n", email[i]);
            printf("  Địa chỉ: %s\n", dia_chi[i]);
            printf("  Ngày lập thẻ: %s\n", ngay_lap_the[i]);
            printf("  Ngày hết hạn thẻ: %s\n", ngay_het_han_the[i]);    
            found = 1;
        }
    }
    if (!found) {
        printf("-> Không tìm thấy độc giả nào có tên: %s.\n", ten_can_tim);
    }
    remove_buffer();
}

// 1. Hàm menu quản lý độc giả
void quan_ly_doc_gia() {
    int chon;
    int id;
    char cmnd[100];
    char ho_ten[100];
    printf("\n===== MENU ĐỘC GIẢ =====\n");
    printf("\n1. Thêm độc giả\n");
    printf("2. Xem danh sách độc giả\n");
    printf("3. Chỉnh sửa thông tin độc giả\n");
    printf("4. Xóa thông tin độc giả\n");
    printf("5. Tìm kiếm thông tin độc giả theo CMND\n");
    printf("6. Tìm kiếm thông tin độc giả theo tên\n");
    printf("0. Quay lại\n");

    if (!read_int_safe("Chọn chức năng: ", &chon)) {
        printf("  -> Nhập không hợp lệ!\n");
        return;
    }
    
    switch(chon) {
        case 1:
            them_doc_gia();  // Hàm thêm độc giả
            break;
        case 2:
            danh_sach_doc_gia();  // Hàm in danh sách độc giả
            break;
        case 3:
            printf("Nhập ID độc giả muốn chỉnh sửa: ");
            scanf("%d", &id);
            chinh_sua_doc_gia(id);   // Hàm chỉnh sửa thông tin độc giả
            break;
        case 4:
            printf("Nhập ID độc giả muốn xóa: ");
            scanf("%d", &id);
            xoa_doc_gia(id);   // Hàm xóa thông tin độc giả
            break;
        case 5:
            printf("Nhập CMND của độc giả cần tìm: ");
            scanf("%99s", cmnd);
            tim_kiem_theo_cmnd(cmnd);   // Hàm tìm kiếm độc giả theo CMND
            break;
        case 6:
            printf("Nhập tên độc giả cần tìm: ");
            scanf(" %[^\n]%*c", ho_ten);
            tim_kiem_theo_ten_doc_gia(ho_ten);   // Hàm tìm kiếm độc giả theo họ tên
            break;
        case 0:
            return;
        default:
            printf("Chọn không hợp lệ. Thử lại nhé!\n");
    }
}



// ----------------------------làm xong hết thì xóa -------------------------------------
// Hàm đọc dữ liệu độc giả từ file
void doc_du_lieu_doc_gia_tu_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Không thể mở file %s\n", filename);
        return;
    }

    while (!feof(f) && tong_so_doc_gia < MAX_USERS) {
        fscanf(
            f,
            "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^\n]\n",
            &id_doc_gia[tong_so_doc_gia],
            ho_ten[tong_so_doc_gia],
            cmnd[tong_so_doc_gia],
            ngay_sinh[tong_so_doc_gia],
            gioi_tinh[tong_so_doc_gia],
            email[tong_so_doc_gia],
            dia_chi[tong_so_doc_gia],
            ngay_lap_the[tong_so_doc_gia],
            ngay_het_han_the[tong_so_doc_gia]
        );
        tong_so_doc_gia++;
    }

    fclose(f);
    printf("✅ Đã nạp %d độc giả từ file %s\n", tong_so_doc_gia, filename);
}

