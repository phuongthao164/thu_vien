#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "sach.h"     
#include "doc_gia.h" 
#include "phieu_tra.h"
#include "thong_ke.h"

// Khai báo giá trị mặc định
#define MAX_LEN           100
#define MAX_PHIEU         200
#define MAX_SACH_MUON     10
#define PHI_PHAT_MOI_NGAY 5000LL
#define NGAY_MUON_TOI_DA  7

// Đếm tổng số phiếu trả sách
int tong_phieu_tra = 0;

// Biến lưu trữ thông tin phiếu trả sách
int   pt_ma_doc_gia[MAX_PHIEU];
char  pt_ngay_muon[MAX_PHIEU][20];
char  pt_ngay_tra_du_kien[MAX_PHIEU][20];
char  pt_ngay_tra_thuc_te[MAX_PHIEU][20]; 
int   pt_so_luong_sach[MAX_PHIEU];  // Biến lưu trữ tổng số lượng sách mượn trong 1 phiếu
char  pt_ds_isbn[MAX_PHIEU][MAX_SACH_MUON][50]; // Biến lưu trữ các mã sách isbn của phiếu 
int   pt_ds_sl_sach[MAX_PHIEU][MAX_SACH_MUON]; // Biến lưu trữ số lượng sách mượn tương ứng với từng loại sách
long long pt_tong_tien_phat[MAX_PHIEU];
long long tong_phat_mat = 0;  //  Tổng số tiền phạt khi mất sách

// Các biến sử dụng lại từ file sach.c
extern int so_quyen[];
extern char isbn[][200];
extern double gia_sach[];

// Các biến sử dụng lại từ file phieu_muon.c
extern int tong_phieu_muon;
extern int   pm_ma_doc_gia[];
extern char  pm_ngay_muon[][20];
extern char  pm_ngay_tra_du_kien[][20];
extern char  pm_ngay_tra_thuc_te[][20];
extern int   pm_so_luong_sach[]; 
extern char  pm_ds_isbn[][10][50];
extern int   pm_ds_sl_sach[][10];
extern long long pm_tong_tien_phat[];

// CÁC HÀM BỔ SUNG CHO HÀM CHÍNH
// a. Hàm tính ngày trả sách
void ngay_tra_tt(int index) {
    time_t t_hien_tai = time(NULL);
    char *time_str_ht = ctime(&t_hien_tai);
    char month_str_ht[4];
    int day_ht, year_ht;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_ht, "%*s %3s %d %*s %d", month_str_ht, &day_ht, &year_ht);
    int month_ht = (strstr(months, month_str_ht) - months) / 3 + 1;

    snprintf(pt_ngay_tra_thuc_te[index], MAX_LEN, "%02d/%02d/%04d", day_ht, month_ht, year_ht);
    printf("  Ngày thực tế trả sách là: %s\n", pt_ngay_tra_thuc_te[index]);
}

// b. Kiểm tra năm nhuận
int is_leap(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// c. Số ngày trong mỗi tháng
int days_in_month(int month, int year) {
    int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && is_leap(year)) return 29;
    return days[month-1];
}

// d. Chuyển từ dạng "dd/mm/yyyy" sang tổng số ngày
long convert_to_days(const char *date_str) {
    int d, m, y;
    sscanf(date_str, "%d/%d/%d", &d, &m, &y);
    long days = 0;
    for (int year = 1970; year < y; year++) {
        days += is_leap(year) ? 366 : 365;
    }
    for (int month = 1; month < m; month++) {
        days += days_in_month(month, y);
    }
    days += (d - 1);
    return days;
}

// e. So sánh ngày trả thực tế và ngày dự kiến trả sách
long kiem_tra_tra_sach(const char *ngay_tra_thuc_te, const char *ngay_tra_du_kien) {
    long thuc_te_days = convert_to_days(ngay_tra_thuc_te);
    long du_kien_days = convert_to_days(ngay_tra_du_kien);
    return thuc_te_days - du_kien_days;
}

// f. Hàm bổ sung chức năng chọn option menu quản lý
extern int read_int_safe(const char *prompt, int *out);

// g. Hàm xóa bộ nhớ đệm
extern void remove_buffer();


// CÁC HÀM CHÍNH
// 4a. Hàm lập phiếu trả sách
void lap_phieu_tra() {
    if (tong_phieu_muon == 0) {
        printf("-> Chưa có phiếu mượn nào để trả!\n");
        remove_buffer();
        return;
    }

    int ma;
    printf("  Nhập mã độc giả cần trả sách: ");
    scanf("%d", &ma);

    int found = 0;
    for (int i = 0; i < tong_phieu_muon; i++) {
        if (pm_ma_doc_gia[i] != ma) continue;
        found = 1;

        // Mã phiếu mượn = Mã phiếu trả
        pt_ma_doc_gia[i] = pm_ma_doc_gia[i];

        printf("-> Độc giả có lịch sử mượn sách: \n");

        // Ngày mượn pm = Ngày mượn pt
        strcpy(pt_ngay_muon[i], pm_ngay_muon[i]);
        printf("  Ngày mượn là: %s\n", pt_ngay_muon[i]);

        // Ngày trả dự kiến pm = Ngày trả dự kiến pt
        strcpy(pt_ngay_tra_du_kien[i], pm_ngay_tra_du_kien[i]);
        printf("  Ngày trả dự kiến là: %s\n", pm_ngay_tra_du_kien[i]);

        // Ngày trả thực tế
        ngay_tra_tt(i);
        strcpy(pm_ngay_tra_thuc_te[i], pt_ngay_tra_thuc_te[i]);

        // Tổng số sách đã mượn
        pt_so_luong_sach[i] = pm_so_luong_sach[i];
        printf("  Tổng số sách đã mượn là: %d quyển.\n",pt_so_luong_sach[i]);

        // Danh sách ISBN của các sách mượn
        printf("  Sách đã mượn là:\n");
        for (int k=0; k < pt_so_luong_sach[i]; k++){
            strcpy(pt_ds_isbn[i][k], pm_ds_isbn[i][k]);
            pt_ds_sl_sach[i][k] = pm_ds_sl_sach[i][k];
            if (strcmp(pt_ds_isbn[i][k], "") != 0) {
                printf("  - Mã %s : %d quyển.\n",pt_ds_isbn[i][k],pt_ds_sl_sach[i][k]); 
            }
        }

        // Phạt tiền khi trả trễ ngày hẹn
        long so_ngay_tre = kiem_tra_tra_sach(pt_ngay_tra_thuc_te[i], pt_ngay_tra_du_kien[i]);
        long long tong_phat_tre = (long long)so_ngay_tre * PHI_PHAT_MOI_NGAY;

        if (so_ngay_tre <= 0) {
            printf("  -> Trả đúng hạn!\n");
            tong_phat_tre = 0;
        } else {
            printf("  -> Trả trễ %ld ngày.\n", so_ngay_tre);
            printf("  -> Số tiền bị phạt khi trả trễ sách là %lld VND. (5000 VND/ ngày)\n",tong_phat_tre);
        }

        // Phạt tiền khi mất sách
        for (int j = 0; j < pt_so_luong_sach[i]; j++) {
            int idx = find_index_by_isbn(pt_ds_isbn[i][j]);
            if (idx < 0) continue;

            if(strcmp(pt_ds_isbn[i][j], "") != 0){
                int mat_sach;
                printf("  Sách ISBN %s có bị mất không? (Nhập 1: Có hoặc Số bất kì: Không): ", pt_ds_isbn[i][j]);
                scanf("%d",&mat_sach);

                if (mat_sach == 1) {
                    int so_quyen_mat = 0;

                    // Lặp cho tới khi nhập hợp lệ
                    while (1) {
                        printf("Số quyển bị mất là: ");
                        scanf("%d", &so_quyen_mat);

                        if (so_quyen_mat <= 0) {
                            printf("  -> Số quyển phải lớn hơn 0.\n");
                        } else if (so_quyen_mat > pt_ds_sl_sach[i][j]) {
                            printf("  -> Số lượng sách mất không thể nhiều hơn số đã mượn (%d quyển).\n",
                                pt_ds_sl_sach[i][j]);
                        } else {
                            break; 
                        }
                    }

                    if (idx >= 0) {
                        long long phat_mat = (long long)(2.0 * gia_sach[idx] * so_quyen_mat);
                        tong_phat_mat += phat_mat;

                        printf("  -> Mất %d quyển! Phạt 200%% giá sách: %lld VND\n",
                            so_quyen_mat, phat_mat);

                        // Nếu chưa mất hết → cộng lại số sách còn lại vào kho
                        int so_con_lai = pt_ds_sl_sach[i][j] - so_quyen_mat;
                        if (so_con_lai > 0) {
                            so_quyen[idx] += so_con_lai;
                            printf("  -> %d quyển còn lại đã được trả. Số quyển hiện có trong kho: %d\n",
                                so_con_lai, so_quyen[idx]);
                        }
                    }
                } else {
                    // Không mất → cộng toàn bộ sách đã mượn vào kho
                    if (idx >= 0) {
                        so_quyen[idx] += pt_ds_sl_sach[i][j];
                        printf("  -> Đã trả toàn bộ sách. Số quyển hiện có trong kho: %d\n", so_quyen[idx]);
                    }
                }
            }
        }
        pt_tong_tien_phat[i] = tong_phat_mat + tong_phat_tre;
        pm_tong_tien_phat[i] = pt_tong_tien_phat[i];

        printf("\n== KẾT QUẢ TRẢ SÁCH ==\n");
        printf("  Mã độc giả: %d\n", pt_ma_doc_gia[i]);
        printf("  Ngày trả thực tế: %s\n", pt_ngay_tra_thuc_te[i]);
        if(so_ngay_tre > 0){
            printf("  Trễ hạn: %ld ngày -> Phạt trễ: %lld VND\n", so_ngay_tre, tong_phat_tre);
        }
        if(tong_phat_mat > 0){
            printf("  Mất sách -> Phạt: %lld VND\n", tong_phat_mat);
        }
        if(pt_tong_tien_phat[i] > 0){
            printf("  Tổng tiền phạt (bao gồm mất sách nếu có): %lld VND\n", pt_tong_tien_phat[i]);
        } else {
            printf("  -> Đã trả sách đúng hạn và không bị mất sách");
        }
        tong_phieu_tra++;
        return;
    }

    if (!found) {
        printf("-> Không tìm thấy phiếu mượn nào của độc giả %d.\n", ma);
    }
    remove_buffer();
}

// 4b. Hàm xem danh sách phiếu trả sách
void danh_sach_phieu_tra() {
    if (tong_phieu_tra == 0) {
        printf("Chưa có phiếu trả nào!\n");
        remove_buffer();
        return;
    }
    printf("=== Danh sách phiếu trả ===\n");
    for (int i = 0; i < tong_phieu_muon; i++) {
        if(strcmp(pt_ngay_muon[i], "") != 0){
            printf("\nPhiếu trả %d:\n", i+1);
            printf("  Mã độc giả: %d\n", pt_ma_doc_gia[i]);
            printf("  Ngày mượn: %s\n", pt_ngay_muon[i]);
            printf("  Ngày trả dự kiến: %s\n", pt_ngay_tra_du_kien[i]);
            printf("  Ngày trả thực tế: %s\n", pt_ngay_tra_thuc_te[i][0] ? pt_ngay_tra_thuc_te[i] : "Chưa trả");
            printf("  Số sách mượn: %d quyển\n", pt_so_luong_sach[i]);
            for (int j = 0; j < pt_so_luong_sach[i]; j++) {
                if (strcmp(pt_ds_isbn[i][j], "") != 0) {
                    printf("    - %s: %d quyển\n", pt_ds_isbn[i][j],pt_ds_sl_sach[i][j]);
                }
            }
            if(pt_tong_tien_phat[i] != 0){
                printf("  Tổng tiền phạt: %lld VND\n", pt_tong_tien_phat[i]);
            }
            if(tong_phat_mat != 0){
                printf("Sách bị mất phạt %lld đồng\n", tong_phat_mat);
            }
        }
    }
    remove_buffer();
}

// 4. Hàm quản lý phiếu trả sách
void quan_ly_phieu_tra() {
    int chon;
    printf("\n===== MENU PHIẾU TRẢ SÁCH =====\n");
    printf("1. Lập phiếu trả sách\n");
    printf("2. Xem danh sách phiếu\n");
    printf("0. Quay lại\n");

    if (!read_int_safe("Chọn chức năng: ", &chon)) {
        printf("  -> Nhập không hợp lệ!\n");
        return;
    }

    switch (chon) {
        case 1:
            lap_phieu_tra();
            break;
        case 2:
            danh_sach_phieu_tra();
            break;
        case 0:
            return;
        default:
            printf("Chọn không hợp lệ. Thử lại nhé!\n");
    }
}

