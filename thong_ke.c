#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "sach.h"
#include "doc_gia.h"
#include "phieu_muon_tra.h"
#include "thong_ke.h"

// extern từ phieu_muon_tra.c
extern int tong_phieu;
extern int p_ma_doc_gia[];
extern char p_ngay_muon[][20];
extern char p_ngay_tra_du_kien[][20];
extern char p_ngay_tra_thuc_te[][20];
extern int p_so_luong_sach[];
extern char p_ds_isbn[][10][50];
extern long long p_tong_tien_phat[];

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
    printf("\n=== THỐNG KÊ SÁCH ĐANG ĐƯỢC MƯỢN ===\n");

    int tong_dang_muon = 0;
    for (int i = 0; i < tong_so_sach; i++) {
        int so_dang_muon = tong_so_quyen_goc[i] - so_quyen[i];
        if (so_dang_muon > 0) {
            printf("ISBN: %-10s | Tên sách: %-30s | Đang được mượn: %d quyển\n",
                   isbn[i], ten_sach[i], so_dang_muon);
            tong_dang_muon += so_dang_muon;
        }
    }

    if (tong_dang_muon == 0)
        printf("Hiện không có quyển sách nào đang được mượn.\n");
    else
        printf("---------------------------------------------\nTổng cộng: %d quyển đang được mượn.\n", tong_dang_muon);
}

// --- f. Độc giả trả trễ ---
extern long kiem_tra_tra_sach(const char*, const char*);

void thong_ke_doc_gia_tre_han() {
    printf("\n=== DANH SÁCH ĐỘC GIẢ TRẢ TRỄ ===\n");

    int found = 0;

    for (int i = 0; i < tong_phieu; i++) {
        // chỉ xét phiếu chưa trả
        if (p_ngay_tra_thuc_te[i][0] == '\0') {
            long diff = kiem_tra_tra_sach(p_ngay_tra_du_kien[i], p_ngay_muon[i]);
            if (diff > 0) {
                printf("- Mã độc giả %d bị trễ %ld ngày (hạn: %s)\n",
                       p_ma_doc_gia[i], diff, p_ngay_tra_du_kien[i]);
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
    scanf("%d", &choice);

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
