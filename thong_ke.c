#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "sach.h"
#include "doc_gia.h"
#include "phieu_muon.h"
#include "phieu_tra.h"
#include "thong_ke.h"

// Khai báo giá trị mặc định
#define MAX_LEN           100
#define MAX_SACH_MUON     10
#define NGAY_MUON_TOI_DA  7

// Biến lưu trữ thông tin
char ngay_hien_tai[20];     // Tính ngày hôm nay

// Các biến sử dụng lại từ file phieu_muon.c
extern int tong_phieu_muon;
extern int pm_ma_doc_gia[];
extern char pm_ngay_tra_du_kien[][20];
extern char pm_ngay_tra_thuc_te[][20];
extern int pm_so_luong_sach[];
extern char pm_ds_isbn[][10][50];
extern int pm_ds_sl_sach[][10];

// Các biến sử dụng lại từ file sach.c
extern char isbn[][200];
extern char ten_sach[][200];
extern int tong_so_sach;
extern char the_loai[][200];
extern int so_quyen[];

// Các biến sử dụng lại từ file doc_gia.c
extern int tong_so_doc_gia;
extern char gioi_tinh[][100];
extern char ho_ten[][100];

// CÁC HÀM BỔ SUNG CHO HÀM CHÍNH
// a. Hàm tính ngày hiện tại
char* ngay_ht() {
    time_t t_hien_tai = time(NULL);
    char *time_str_ht = ctime(&t_hien_tai);
    char month_str_ht[4];
    int day_ht, year_ht;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_ht, "%*s %3s %d %*s %d", month_str_ht, &day_ht, &year_ht);
    int month_ht = (strstr(months, month_str_ht) - months) / 3 + 1;

    snprintf(ngay_hien_tai, sizeof(ngay_hien_tai), "%02d/%02d/%04d", day_ht, month_ht, year_ht);
    return ngay_hien_tai;
}

// b. Hàm xóa bộ nhớ đệm
void remove_buffer() {
    printf("\n(Nhấn Enter để quay lại menu) ");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
    getchar();
}

// c. Hàm kiểm tra năm nhuận
int la_nam_nhuan(int nam) {
    return (nam % 4 == 0 && nam % 100 != 0) || (nam % 400 == 0);
}

// d. Hàm kiểm tra ngày tháng năm hợp lệ
int kiem_tra_ngay_hop_le(const char *ngay_str) {
    int ngay, thang, nam;
    
    // Kiểm tra format dd/mm/yyyy
    if (sscanf(ngay_str, "%d/%d/%d", &ngay, &thang, &nam) != 3) {
        return 0; // Sai format
    }
    
    // Kiểm tra năm (1900 - 2100)
    if (nam < 1900 || nam > 2100) {
        return 0;
    }
    
    // Kiểm tra tháng (1-12)
    if (thang < 1 || thang > 12) {
        return 0;
    }
    
    // Số ngày tối đa của mỗi tháng
    int ngay_trong_thang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Nếu là năm nhuận, tháng 2 có 29 ngày
    if (la_nam_nhuan(nam)) {
        ngay_trong_thang[2] = 29;
    }
    
    // Kiểm tra ngày hợp lệ
    if (ngay < 1 || ngay > ngay_trong_thang[thang]) {
        return 0;
    }
    
    return 1; // Hợp lệ
}

// e. Hàm kiểm tra email hợp lệ
int kiem_tra_email_hop_le(const char *email_str) {
    int len = strlen(email_str);
    
    // Email phải có ít nhất 5 ký tự: a@b.c
    if (len < 5) {
        return 0;
    }
    
    // Tìm vị trí của @
    const char *at_pos = strchr(email_str, '@');
    if (at_pos == NULL) {
        return 0; // Không có @
    }
    
    // @ không được ở đầu
    if (at_pos == email_str) {
        return 0;
    }
    
    // Kiểm tra chỉ có 1 dấu @
    if (strchr(at_pos + 1, '@') != NULL) {
        return 0; // Có nhiều hơn 1 dấu @
    }
    
    // Phần sau @ phải có dấu .
    const char *dot_pos = strchr(at_pos + 1, '.');
    if (dot_pos == NULL) {
        return 0; // Không có dấu . sau @
    }
    
    // Dấu . không được ngay sau @
    if (dot_pos == at_pos + 1) {
        return 0;
    }
    
    // Phải có ít nhất 1 ký tự sau dấu . cuối cùng (extension)
    if (strlen(dot_pos) < 2) {
        return 0;
    }
    
    // Kiểm tra ký tự hợp lệ (chữ, số, @, ., _, -)
    for (int i = 0; i < len; i++) {
        char c = email_str[i];
        if (!((c >= 'a' && c <= 'z') || 
              (c >= 'A' && c <= 'Z') || 
              (c >= '0' && c <= '9') || 
              c == '@' || c == '.' || c == '_' || c == '-')) {
            return 0;
        }
    }
    
    return 1; // Hợp lệ
} 

// c. Hàm so sánh ngày hiện tại và ngày dự kiến trả sách
extern long kiem_tra_tra_sach(const char*, const char*);

// d. Hàm bổ sung chức năng chọn option menu quản lý
extern int read_int_safe(const char *prompt, int *out);


// CÁC HÀM CHÍNH
// 5a. Thống kê số lượng sách trong thư viện
void thong_ke_tong_sach() {
    printf("\n=== THỐNG KÊ TỔNG SỐ SÁCH ===\n");
    printf("Tổng số loại sách: %d\n", tong_so_sach);

    int tong_tat_ca = 0;
    for (int i = 0; i < tong_so_sach; i++)
        tong_tat_ca += so_quyen[i];

    printf("Tổng số quyển sách hiện còn trong kho: %d\n", tong_tat_ca);
    remove_buffer();
}

// 5b. Thống kê số lượng sách theo thể loại
void thong_ke_the_loai() {
    printf("\n=== THỐNG KÊ SÁCH THEO THỂ LOẠI ===\n");

    if (tong_so_sach == 0) {
        printf("  -> Không có sách nào trong thư viện.\n");
        return;
    }

    // Thống kê theo thể loại
    char ds_loai[500][200];
    int so_loai = 0;
    int dem_sach[500] = {0};

    for (int i = 0; i < tong_so_sach; i++) {
        int found = 0;
        for (int j = 0; j < so_loai; j++) {
            if (strcmp(ds_loai[j], the_loai[i]) == 0) {
                dem_sach[j] += so_quyen[i];
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(ds_loai[so_loai], the_loai[i]);
            dem_sach[so_loai] = so_quyen[i];
            so_loai++;
        }
    }

    for (int i = 0; i < so_loai; i++) {
        printf("%d. Thể loại %-20s: %d quyển\n", i + 1, ds_loai[i], dem_sach[i]);
    }
    remove_buffer();
}

// 5c. Thống kê số lượng độc giả
void thong_ke_tong_doc_gia() {
    printf("\n=== THỐNG KÊ TỔNG SỐ LƯỢNG ĐỘC GIẢ ===\n");
    printf("\nTổng số lượng độc giả: %d\n", tong_so_doc_gia);
    remove_buffer();
}

// 5d. Thống kê số lượng độc giả theo giới tính
void thong_ke_doc_gia_theo_gioi_tinh() {
    int nam = 0, nu = 0, khac = 0;

    for (int i = 0; i < tong_so_doc_gia; i++) {
        if (ho_ten[i][0] == '\0') continue; 

        if (strcasecmp(gioi_tinh[i], "Nam") == 0)
            nam++;
        else if (strcasecmp(gioi_tinh[i], "Nữ") == 0)
            nu++;
        else
            khac++;
    }

    printf("\n=== THỐNG KÊ ĐỘC GIẢ THEO GIỚI TÍNH ===\n");
    printf("Nam : %d\n", nam);
    printf("Nữ  : %d\n", nu);
    printf("Khác: %d\n", khac);
    remove_buffer();
}

// 5e. Thống kê số sách đang được mượn
void thong_ke_sach_dang_muon() {
    printf("\n===== THỐNG KÊ SÁCH ĐANG ĐƯỢC MƯỢN =====\n");

    char isbn_dang_muon[1000][50];
    int so_luong_dang_muon[1000];
    int tong_sach_dang_muon = 0;

    for (int i = 0; i < tong_phieu_muon; i++) {
        // Bỏ qua phiếu đã có ngày trả (đã trả xong)
        if (strlen(pm_ngay_tra_thuc_te[i]) > 0) continue;

        for (int j = 0; j < MAX_SACH_MUON && pm_ds_sl_sach[i][j] > 0; j++) {
            if (strlen(pm_ds_isbn[i][j]) == 0) continue;

            // Tìm ISBN này trong danh sách thống kê
            int found = 0;
            for (int k = 0; k < tong_sach_dang_muon; k++) {
                if (strcmp(pm_ds_isbn[i][j], isbn_dang_muon[k]) == 0) {
                    so_luong_dang_muon[k] += pm_ds_sl_sach[i][j];
                    found = 1;
                    break;
                }
            }

            // Nếu chưa có thì thêm mới trong danh sách thống kê
            if (!found) {
                strcpy(isbn_dang_muon[tong_sach_dang_muon], pm_ds_isbn[i][j]);
                so_luong_dang_muon[tong_sach_dang_muon] = pm_ds_sl_sach[i][j];
                tong_sach_dang_muon++;
            }
        }
    }

    if (tong_sach_dang_muon == 0) {
        printf("Hiện không có sách nào đang được mượn.\n");
        return;
    }

    // In danh sách thống kê
    for (int k = 0; k < tong_sach_dang_muon; k++) {
        char ten[100] = "Không xác định";
        for (int s = 0; s < tong_so_sach; s++) {
            if (strcmp(isbn_dang_muon[k], isbn[s]) == 0) {
                strcpy(ten, ten_sach[s]);
                break;
            }
        }

        printf("  - Mã %s: Sách %s đang được mượn với số lượng %d quyển.\n", isbn_dang_muon[k], ten, so_luong_dang_muon[k]);
        
    };
    remove_buffer();
}

// 5f. Thống kê danh sách độc giả bị trễ hạn
void thong_ke_doc_gia_tre_han() {
    printf("\n=== DANH SÁCH ĐỘC GIẢ TRẢ TRỄ ===\n");
    int found = 0;
    for (int i = 0; i < tong_phieu_muon; i++) {
        // chỉ kiểm tra phiếu chưa trả sách
        if (pm_ngay_tra_thuc_te[i][0] == '\0') {
            ngay_ht();
            long diff = kiem_tra_tra_sach(ngay_hien_tai, pm_ngay_tra_du_kien[i]);
            if (diff > 0) {
                printf("- Mã độc giả %d bị trễ %ld ngày (hạn: %s)\n",
                       pm_ma_doc_gia[i], diff, pm_ngay_tra_du_kien[i]);
                found = 1;
            }
        }
    }

    if (!found){
        printf("Không có độc giả nào trả trễ.\n");
    }
    remove_buffer();
}

// 5. Hàm quản lý menu thống kê
void thong_ke_co_ban() {
    int chon;
    printf("\n===== MENU THỐNG KÊ =====\n");
    printf("1. Tổng số sách trong thư viện\n");
    printf("2. Số lượng sách theo thể loại\n");
    printf("3. Tổng số độc giả\n");
    printf("4. Độc giả theo giới tính\n");
    printf("5. Sách đang được mượn\n");
    printf("6. Độc giả bị trễ hạn\n");
    printf("0. Quay lại\n");

    if (!read_int_safe("Chọn chức năng: ", &chon)) {
        printf("  -> Nhập không hợp lệ!\n");
        return;
    }

    switch (chon) {
        case 1: thong_ke_tong_sach(); break;
        case 2: thong_ke_the_loai(); break;
        case 3: thong_ke_tong_doc_gia(); break;
        case 4: thong_ke_doc_gia_theo_gioi_tinh(); break;
        case 5: thong_ke_sach_dang_muon(); break;
        case 6: thong_ke_doc_gia_tre_han(); break;
        case 0: return;
        default: printf("Chọn không hợp lệ. Thử lại nhé!\n");
    }
}

