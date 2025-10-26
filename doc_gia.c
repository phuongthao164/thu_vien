// Quản lý độc giả
#include <stdio.h>
#include <string.h>
#include "doc_gia.h"
#include "thong_ke.h"
#include <time.h>

#define MAX_LEN 100
#define MAX_USERS 100

extern void remove_buffer();

// Đếm tổng số độc giả hiện có trong thư viện
int tong_so_doc_gia = 0;

// Mảng lưu trữ thông tin độc giả
int id_doc_gia[MAX_USERS];
char ho_ten[MAX_USERS][MAX_LEN];
char cmnd[MAX_USERS][MAX_LEN];
char ngay_sinh[MAX_USERS][MAX_LEN];
char gioi_tinh[MAX_USERS][MAX_LEN];
char email[MAX_USERS][MAX_LEN];
char dia_chi[MAX_USERS][MAX_LEN];
char ngay_lap_the[MAX_USERS][MAX_LEN];
char ngay_het_han_the[MAX_USERS][MAX_LEN];

// Hàm tính ngày lập thẻ và ngày hết hạn thẻ
void ngay_lap(int index) {
    time_t t_hien_tai = time(NULL);
    char *time_str_ht = ctime(&t_hien_tai);
    char month_str_ht[4];
    int day_ht, year_ht;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_ht, "%*s %3s %d %*s %d", month_str_ht, &day_ht, &year_ht);
    int month_ht = (strstr(months, month_str_ht) - months) / 3 + 1;

    snprintf(ngay_lap_the[index], MAX_LEN, "%02d/%02d/%04d", day_ht, month_ht, year_ht);
    printf("Ngày lập thẻ là: %s\n", ngay_lap_the[index]);
}

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
    printf("Ngày hết hạn thẻ là: %s\n", ngay_het_han_the[index]);
}

// Hàm thêm độc giả
void them_doc_gia() {
    if (tong_so_doc_gia >= MAX_USERS) {
    printf("Đã đạt số lượng độc giả tối đa (%d).\n", MAX_USERS);
    return;
    }

    // Mã độc giả sẽ được tự động đếm số tăng dần
    int ma_doc_gia = tong_so_doc_gia + 1;
    printf("  Mã độc giả của bạn là: %d \n",ma_doc_gia);

    // Lưu ID độc giả vào mảng id_doc_gia
    id_doc_gia[tong_so_doc_gia] = ma_doc_gia;

    // Thêm thông tin độc giả vào danh sách
    printf("--Vui lòng nhập thông tin của bạn: \n");

    // Họ tên
    printf("  Nhập họ tên: ");
    scanf(" %[^\n]%*c", ho_ten[tong_so_doc_gia]);  

    // CMND
    printf("  Nhập CMND: ");
    scanf("%99s", cmnd[tong_so_doc_gia]);  

    // Ngày tháng năm sinh
    printf("  Nhập ngày tháng năm sinh (dd/mm/yyyy): ");
    scanf("%99s", ngay_sinh[tong_so_doc_gia]);  

    // Giới tính
    printf("  Nhập giới tính (Nam/Nữ): ");
    scanf("%99s", gioi_tinh[tong_so_doc_gia]);

    // // Giới tính -------------------------------NEW -------------------------------------
    // printf("Nhap gioi tinh (Nhap 1.Nam , 2.Nu hoac Ki tu khac de bo qua): ");
    // int chon_gioi_tinh;
    // scanf("%d", &chon_gioi_tinh);  
    // if (chon_gioi_tinh == 1){
    //     *gioi_tinh[tong_so_doc_gia] = "Nam";
    // } else if (chon_gioi_tinh == 2){
    //     *gioi_tinh[tong_so_doc_gia] = "Nu";
    // } else {
    //     *gioi_tinh[tong_so_doc_gia] = "Khong xac dinh";
    // }

    // Email
    printf("  Nhập email: ");
    scanf("%99s", email[tong_so_doc_gia]); 
    
    // Địa chỉ
    printf("  Nhập địa chỉ: ");
    scanf(" %[^\n]%*c", dia_chi[tong_so_doc_gia]);  

    // Ngày lập thẻ và ngày hết hạn
    ngay_lap(tong_so_doc_gia);
    ngay_het_han(tong_so_doc_gia);

    tong_so_doc_gia++;
    printf("-> Độc giả đã được thêm thành công\n");
    remove_buffer();
}

// Hàm hiển thị danh sách độc giả
void danh_sach_doc_gia() {
    if (tong_so_doc_gia == 0) {
        printf("Không có độc giả nào trong thư viện.\n");
        return;
    }

    printf("  Số độc giả hiện tại của thư viện là: %d\n",tong_so_doc_gia);
    printf("--Danh sách độc giả:\n");
    for (int i = 0; i < tong_so_doc_gia; i++) {
    printf("* Độc giả thứ %d (ID: %d)\n", i+1, id_doc_gia[i]);
    if (ho_ten[i][0] == '\0') {
        printf("  >> Thông tin độc giả đã bị xóa \n");
    } else {
        printf("  Họ tên: %s\n", ho_ten[i]);
        printf("  CMND: %s\n", cmnd[i]);
        printf("  Ngày sinh: %s\n", ngay_sinh[i]);
        printf("  Giới tính: %s\n", gioi_tinh[i]);
        printf("  Email: %s\n", email[i]);
        printf("  Địa chỉ: %s\n", dia_chi[i]);
        printf("  Ngày lập thẻ: %s\n", ngay_lap_the[i]);
        printf("  Ngày hết hạn thẻ: %s\n", ngay_het_han_the[i]);
    }
    }
    remove_buffer();
}

 //Hàm chỉnh sửa thông tin một độc giả
void chinh_sua_doc_gia(int id) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        if (id == id_doc_gia[i]) {  // Tìm theo mã độc giả
            found = 1;
            // Bản ghi đã bị xóa
            if (ho_ten[i][0] == '\0') {
                // printf("Thong tin cua doc gia %d da bi xoa.",id);
                // break;
                printf("-> Thông tin của độc giả %d đã bị xóa. Bạn có muốn cập nhật thông tin mới cho độc giả này không?\n", id);
                printf("Nhập 1. Tiếp tục hoặc Nhập kí tự bất kì để Thoát. \n");
                int chon;
                scanf("%d", &chon);
                if (chon == 1) {    
                    printf("-- Nhập thông tin mới:\n");
                    printf("  Nhập họ tên: ");
                    scanf(" %[^\n]%*c", ho_ten[i]);
                    printf("  Nhập CMND: ");
                    scanf("%99s", cmnd[i]);
                    printf("  Nhập ngày tháng năm sinh (dd/mm/yyyy): ");
                    scanf("%99s", ngay_sinh[i]);
                    printf("  Nhập giới tính (Nam/Nữ): ");
                    scanf("%99s", gioi_tinh[i]);
                    printf("  Nhập email: ");
                    scanf("%99s", email[i]);
                    printf("  Nhập địa chỉ: ");
                    scanf(" %[^\n]%*c", dia_chi[i]);
                    ngay_lap(i);
                    ngay_het_han(i);
                    printf("-> Thông tin độc giả đã được cập nhật mới!\n");
                    break;
                } else {
                    // Dọn bộ nhớ đệm
                    int _c;
                    while ((_c = getchar()) != '\n' && _c != EOF);
                    // Thoát chỉnh sửa
                    printf("Kết thúc chỉnh sửa! \n");
                    break;
                }
            } else {
            // Cập nhật thông tin độc giả đang hoạt động
            printf("* Bạn đang thực hiện chỉnh sửa thông tin của độc giả %d \n",id);
            printf ("  Thông tin hiện tại là: Họ tên: %s, CMND: %s, Ngày sinh: %s, Giới tính: %s, Email: %s, Địa chỉ: %s, Ngày lập thẻ: %s, Ngày hết hạn thẻ: %s\n",
                        ho_ten[i], cmnd[i], ngay_sinh[i], gioi_tinh[i], email[i], dia_chi[i], ngay_lap_the[i], ngay_het_han_the[i]);      
            printf("-- Nhập thông tin bạn muốn chỉnh sửa:\n");
            printf("  Nhập họ tên chỉnh sửa: ");
            scanf(" %[^\n]%*c", ho_ten[i]);
            printf("  Nhập CMND chỉnh sửa: ");
            scanf("%99s", cmnd[i]);
            printf("  Nhập ngày sinh chỉnh sửa (dd/mm/yyyy): ");
            scanf("%99s", ngay_sinh[i]);
            printf("  Nhập giới tính chỉnh sửa (Nam/Nữ): ");
            scanf("%99s", gioi_tinh[i]);
            printf("  Nhập email chỉnh sửa: ");
            scanf("%99s", email[i]);
            printf("  Nhập địa chỉ chỉnh sửa: ");
            scanf(" %[^\n]%*c", dia_chi[i]);
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

// Hàm xóa thông tin một độc giả
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

// Hàm tìm kiếm độc giả theo CMND
void tim_kiem_theo_cmnd(char *cmnd_can_tim) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        // Bỏ qua độc giả đã bị xóa (CMND rỗng)
        if (cmnd[i][0] == '\0') continue;

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

// Hàm tìm kiếm độc giả theo họ tên
void tim_kiem_theo_ten_doc_gia(char *ten_can_tim) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        // Bỏ qua độc giả đã bị xóa (họ tên rỗng)
        if (ho_ten[i][0] == '\0') continue;

        if (strstr(ho_ten[i], ten_can_tim) != NULL) { // ten_can_tim xuất hiện trong ho_ten[i]
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

// Hàm quản lý đôc giả
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
    // printf("Chọn chức năng: ");
    // scanf("%d", &choice);

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
            tim_kiem_theo_cmnd(cmnd);   // // Hàm tìm kiếm độc giả theo CMND
            break;
        case 6:
            printf("Nhập tên độc giả cần tìm: ");
            scanf(" %[^\n]%*c", ho_ten);
            tim_kiem_theo_ten_doc_gia(ho_ten);   // // Hàm tìm kiếm độc giả theo họ tên
            break;
        case 0:
            return;
        default:
            printf("Chọn không hợp lệ. Thử lại nhé!\n");
    }
}


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

// helper: đọc int an toàn
int read_int_safe(const char *prompt, int *out) {
    char buf[64];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    return (sscanf(buf, "%d", out) == 1);
}