#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "sach.h"
#include "doc_gia.h"
#include "phieu_muon.h"
#include "phieu_tra.h"
#include "thong_ke.h"

// ====== Cấu hình ======
#define MAX_LEN           100
#define MAX_SACH_MUON     10
#define NGAY_MUON_TOI_DA  7

char ngay_hien_tai[20];     // ngày hôm nay

// extern từ phieu_muon.c
extern int tong_phieu_muon;
extern int pm_ma_doc_gia[];
extern char pm_ngay_muon[][20];
extern char pm_ngay_tra_du_kien[][20];
extern char pm_ngay_tra_thuc_te[][20];
extern int pm_so_luong_sach[];
extern char pm_ds_isbn[][10][50];
extern long long pm_tong_tien_phat[];
extern int pm_ds_sl_sach[100][50];

// extern từ phieu_tra.c
extern int tong_phieu_tra;
extern int pt_ma_doc_gia[];
extern char pt_ngay_muon[][20];
extern char pt_ngay_tra_du_kien[][20];
extern char pt_ngay_tra_thuc_te[][20];
extern int pt_so_luong_sach[];
extern char pt_ds_isbn[][10][50];
extern long long pt_tong_tien_phat[];
extern int pt_ds_sl_sach[][50];

// extern từ sach.c
extern char isbn[][200];
extern char ten_sach[][200];
extern int tong_so_sach;
extern char the_loai[][200];
extern int so_quyen[];
extern int tong_so_quyen_goc[];

// extern từ doc_gia.c
extern int tong_so_doc_gia;
extern char gioi_tinh[][100];
extern char ho_ten[][100];

// ====== Tiện ích ngày tháng ======
// Hàm tính ngày hiện tại
char* ngay_ht() {
    time_t t_hien_tai = time(NULL);
    char *time_str_ht = ctime(&t_hien_tai);
    char month_str_ht[4];
    int day_ht, year_ht;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_ht, "%*s %3s %d %*s %d", month_str_ht, &day_ht, &year_ht);
    int month_ht = (strstr(months, month_str_ht) - months) / 3 + 1;

    snprintf(ngay_hien_tai, MAX_LEN, "%02d/%02d/%04d", day_ht, month_ht, year_ht);
    return ngay_hien_tai;
}


// --- a. Tổng số sách ---
void thong_ke_tong_sach() {
    printf("\n=== THỐNG KÊ TỔNG SỐ SÁCH ===\n");
    printf("Tổng số loại sách: %d\n", tong_so_sach);

    int tong_tat_ca = 0;
    for (int i = 0; i < tong_so_sach; i++)
        tong_tat_ca += so_quyen[i];

    printf("Tổng số quyển sách hiện còn trong kho: %d\n", tong_tat_ca);
}

// --- b. Thống kê theo thể loại ---
void thong_ke_the_loai() {
    printf("\n=== THỐNG KÊ SÁCH THEO THỂ LOẠI ===\n");

    if (tong_so_sach == 0) {
        printf("Không có sách nào trong thư viện.\n");
        return;
    }

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
}

// --- c. Tổng số độc giả ---
void thong_ke_tong_doc_gia() {
    printf("\n=== THỐNG KÊ TỔNG SỐ LƯỢNG ĐỘC GIẢ ===\n");
    printf("\nTổng số lượng độc giả: %d\n", tong_so_doc_gia);
}

// --- d. Độc giả theo giới tính ---
void thong_ke_doc_gia_theo_gioi_tinh() {
    int nam = 0, nu = 0, khac = 0;

    for (int i = 0; i < tong_so_doc_gia; i++) {
        if (ho_ten[i][0] == '\0') continue; // bỏ độc giả bị xóa

        if (strcasecmp(gioi_tinh[i], "Nam") == 0)
            nam++;
        else if (strcasecmp(gioi_tinh[i], "Nu") == 0 || strcasecmp(gioi_tinh[i], "Nữ") == 0)
            nu++;
        else
            khac++;
    }

    printf("\n=== THỐNG KÊ ĐỘC GIẢ THEO GIỚI TÍNH ===\n");
    printf("Nam : %d\n", nam);
    printf("Nữ  : %d\n", nu);
    printf("Khác: %d\n", khac);
}

// --- e. Sách đang được mượn ---
void thong_ke_sach_dang_muon() {
    printf("\n===== THỐNG KÊ SÁCH ĐANG ĐƯỢC MƯỢN =====\n");

    char isbn_dang_muon[1000][50];
    int so_luong_dang_muon[1000];
    int tong_sach_dang_muon = 0;

    for (int i = 0; i < tong_phieu_muon; i++) {
        // Bỏ qua phiếu đã có ngày trả (đã trả xong)
        if (strlen(pm_ngay_tra_thuc_te[i]) > 0) continue;

        // Duyệt từng sách trong phiếu
        for (int j = 0; j < MAX_SACH_MUON && pm_ds_sl_sach[i][j] > 0; j++) {

            if (strlen(pm_ds_isbn[i][j]) == 0) continue; // bỏ qua dòng trống

            // Tìm ISBN này trong danh sách thống kê
            int found = 0;
            for (int k = 0; k < tong_sach_dang_muon; k++) {
                if (strcmp(pm_ds_isbn[i][j], isbn_dang_muon[k]) == 0) {
                    so_luong_dang_muon[k] += pm_ds_sl_sach[i][j];
                    found = 1;
                    break;
                }
            }

            // Nếu chưa có thì thêm mới
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

    printf("\n%-10s | %-40s | %-10s\n", "ISBN", "Tên sách", "SL đang mượn");
    printf("---------------------------------------------------------------\n");

    // In danh sách thống kê
    for (int k = 0; k < tong_sach_dang_muon; k++) {
        // Tìm tên sách tương ứng từ danh mục gốc
        char ten[100] = "Không xác định";
        for (int s = 0; s < tong_so_sach; s++) {
            if (strcmp(isbn_dang_muon[k], isbn[s]) == 0) {
                strcpy(ten, ten_sach[s]);
                break;
            }
        }

        printf("%-10s | %-40s | %-10d\n", isbn_dang_muon[k], ten, so_luong_dang_muon[k]);
    }
}

// --- f. Độc giả trả trễ ---
extern long kiem_tra_tra_sach(const char*, const char*);

void thong_ke_doc_gia_tre_han() {
    printf("\n=== DANH SÁCH ĐỘC GIẢ TRẢ TRỄ ===\n");

    int found = 0;

    for (int i = 0; i < tong_phieu_muon; i++) {
        // chỉ xét phiếu chưa trả
        if (pm_ngay_tra_thuc_te[i][0] == '\0') {
        // if(strcmp(pm_ngay_tra_thuc_te[i][0], "Chưa trả") == 0){
            ngay_ht();
            long diff = kiem_tra_tra_sach(ngay_hien_tai, pm_ngay_tra_du_kien[i]);
            if (diff > 0) {
                printf("- Mã độc giả %d bị trễ %ld ngày (hạn: %s)\n",
                       pm_ma_doc_gia[i], diff, pm_ngay_tra_du_kien[i]);
                found = 1;
            }
        }
    }

    if (!found)
        printf("Không có độc giả nào trả trễ.\n");
}

// --- Menu thống kê ---
void thong_ke_co_ban() {
    int choice;
    printf("\n===== MENU THỐNG KÊ =====\n");
    printf("1. Tổng số sách trong thư viện\n");
    printf("2. Số lượng sách theo thể loại\n");
    printf("3. Tổng số độc giả\n");
    printf("4. Độc giả theo giới tính\n");
    printf("5. Sách đang được mượn\n");
    printf("6. Độc giả bị trễ hạn\n");
    printf("0. Quay lại\n");
    printf("Chọn chức năng: ");
    // scanf("%d", &choice);

    // Xóa bộ nhớ buffer gây lỗi loop
    if (scanf("%d", &choice) != 1) {
        int _c; while ((_c = getchar()) != '\n' && _c != EOF);
        printf("Lựa chọn không hợp lệ!\n");
        return;
    }
    int _c; while ((_c = getchar()) != '\n' && _c != EOF);


    switch (choice) {
        case 1: thong_ke_tong_sach(); break;
        case 2: thong_ke_the_loai(); break;
        case 3: thong_ke_tong_doc_gia(); break;
        case 4: thong_ke_doc_gia_theo_gioi_tinh(); break;
        case 5: thong_ke_sach_dang_muon(); break;
        case 6: thong_ke_doc_gia_tre_han(); break;
        case 0: return;
        default: printf("Lựa chọn không hợp lệ!\n");
    }
}
