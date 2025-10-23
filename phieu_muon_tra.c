// Quản lý phiếu mượn và trả sách
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "sach.h"     // dùng find_index_by_isbn, gia_sach[], so_quyen[]
#include "doc_gia.h"  // chỉ để đồng bộ include, không phụ thuộc biến trong đó
#include "phieu_muon_tra.h"

// ====== Cấu hình ======
#define MAX_LEN           100
#define MAX_PHIEU         200
#define MAX_SACH_MUON     10
#define PHI_PHAT_MOI_NGAY 5000LL
#define NGAY_MUON_TOI_DA  7

// ====== Dữ liệu phiếu  ======
static int   p_ma_doc_gia[MAX_PHIEU];
static char  p_ngay_muon[MAX_PHIEU][20];
static char  p_ngay_tra_du_kien[MAX_PHIEU][20];
static char  p_ngay_tra_thuc_te[MAX_PHIEU][20]; // "" nếu chưa trả
static int   p_so_luong_sach[MAX_PHIEU]; // số sách muốn mượn
static char  p_ds_isbn[MAX_PHIEU][MAX_SACH_MUON][50];
static int   p_ds_sl_sach[MAX_PHIEU][MAX_SACH_MUON][50];
static long long p_tong_tien_phat[MAX_PHIEU];

int tong_phieu = 0;

// Các biến từ sach.c
extern int tong_so_sach;
extern char the_loai[][200];
extern int so_quyen[];
extern char isbn[][100];
extern double gia_sach[100];

// Các biến từ doc_gia.c
extern int tong_so_doc_gia;
extern char gioi_tinh[][100];
extern char ho_ten[][100];
extern int id_doc_gia[100];

// ====== Tiện ích ngày tháng ======
// Hàm tính ngày mượn
void ngay_muon(int index) {
    time_t t_hien_tai = time(NULL);
    char *time_str_ht = ctime(&t_hien_tai);
    char month_str_ht[4];
    int day_ht, year_ht;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_ht, "%*s %3s %d %*s %d", month_str_ht, &day_ht, &year_ht);
    int month_ht = (strstr(months, month_str_ht) - months) / 3 + 1;

    snprintf(p_ngay_muon[index], MAX_LEN, "%02d/%02d/%04d", day_ht, month_ht, year_ht);
    printf("Ngày mượn sách là: %s\n", p_ngay_muon[index]);
}

//Hàm tính ngày dự kiến trả sách
void ngay_tra_dk(int index) {
    time_t t_hien_tai = time(NULL);
    time_t t_tra_sach = t_hien_tai + (time_t)(NGAY_MUON_TOI_DA * 24 * 60 * 60);
    char *time_str_hh = ctime(&t_tra_sach);
    char month_str_hh[4];
    int day_hh, year_hh;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_hh, "%*s %3s %d %*s %d", month_str_hh, &day_hh, &year_hh);
    int month_hh = (strstr(months, month_str_hh) - months) / 3 + 1;

    snprintf(p_ngay_tra_du_kien[index], MAX_LEN, "%02d/%02d/%04d", day_hh, month_hh, year_hh);
    printf("Ngày dự kiến trả sách là: %s\n", p_ngay_tra_du_kien[index]);
}

// Hàm tính ngày trả sách
void ngay_tra_tt(int index) {
    time_t t_hien_tai = time(NULL);
    char *time_str_ht = ctime(&t_hien_tai);
    char month_str_ht[4];
    int day_ht, year_ht;
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";

    sscanf(time_str_ht, "%*s %3s %d %*s %d", month_str_ht, &day_ht, &year_ht);
    int month_ht = (strstr(months, month_str_ht) - months) / 3 + 1;

    snprintf(p_ngay_tra_thuc_te[index], MAX_LEN, "%02d/%02d/%04d", day_ht, month_ht, year_ht);
    printf("Ngày trả sách là: %s\n", p_ngay_tra_thuc_te[index]);
}

// Kiểm tra năm nhuận
int is_leap(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// Số ngày trong mỗi tháng
int days_in_month(int month, int year) {
    int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && is_leap(year)) return 29;
    return days[month-1];
}

// Chuyển "dd/mm/yyyy" → tổng số ngày từ 1/1/1970 (để so sánh)
long convert_to_days(const char *date_str) {
    int d, m, y;
    sscanf(date_str, "%d/%d/%d", &d, &m, &y);

    long days = 0;

    // cộng số ngày của các năm trước
    for (int year = 1970; year < y; year++) {
        days += is_leap(year) ? 366 : 365;
    }

    // cộng số ngày của các tháng trước trong năm đó
    for (int month = 1; month < m; month++) {
        days += days_in_month(month, y);
    }

    // cộng ngày hiện tại
    days += (d - 1);

    return days;
}

// So sánh ngày trả thực tế và ngày dự kiến
long kiem_tra_tra_sach(const char *ngay_tra_thuc_te, const char *ngay_tra_du_kien) {
    long thuc_te_days = convert_to_days(ngay_tra_thuc_te);
    long du_kien_days = convert_to_days(ngay_tra_du_kien);
    return thuc_te_days - du_kien_days;
}

// ====== Chức năng ======
void lap_phieu_muon() {
    if (tong_phieu >= MAX_PHIEU) {
        printf("Đã đạt số lượng phiếu tối đa (%d).\n", MAX_PHIEU);
        return;
    }

    int i = tong_phieu;

    // Mã độc giả
    printf("Nhập mã độc giả: ");
    scanf("%d", &p_ma_doc_gia[i]);
    for(int j=0; j<tong_so_doc_gia; j++){
        if (p_ma_doc_gia[i] != id_doc_gia[j]){
            printf("Mã độc giả không hợp lệ");
            int _c; while ((_c = getchar()) != '\n' && _c != EOF);
        };
    }
    
    // Ngày mượn
    ngay_muon(tong_phieu);

    // ngày trả dự kiến
    ngay_tra_dk(tong_phieu);

    // if (p_so_luong_sach[i] < 1 || p_so_luong_sach[i] > MAX_SACH_MUON) {
    //     printf("So luong khong hop le.\n");
    //     return;
    // }

    // int _c; while ((_c = getchar()) != '\n' && _c != EOF);

    // - danh sách ISBN của các sách được mượn
    printf("Nhập số lượng sách muốn mượn (tối đa %d quyển): ", MAX_SACH_MUON);
    scanf("%d", &p_so_luong_sach[tong_phieu]);

    int so_luong_con_lai = p_so_luong_sach[tong_phieu];
    for (int j = 0; j < so_luong_con_lai; j++) {
        printf("Số sách có thể mượn là: %d",so_luong_con_lai);
        printf("Nhap ISBN sach thu %d: ", j + 1);
        scanf("%s",p_ds_isbn[tong_phieu][j]);

        for (int k = 0;k < tong_so_sach;k++){    
            if(strcmp(p_ds_isbn[tong_phieu][j], isbn[k]) == 0){
                if (so_quyen[k] <= 0) {
                    printf("  -> Đã hết sách trong kho! Không thể mượn tiếp được.\n");  
                    break;
                } else {
                    printf("Còn %d quyển trong kho. Bạn muốn mượn bao nhiêu quyển?",so_quyen[k]);
                    scanf("%d", &p_ds_sl_sach[tong_phieu][j]);
                    so_quyen[k] -= p_ds_sl_sach[tong_phieu][j][50];  // Số sách còn lại trong kho của mã ISBN trên
                    printf("Bạn đã mượn thành công sách có mã %s. Còn %d quyển trong kho!",p_ds_isbn[j],so_quyen[k]);
                    so_luong_con_lai = so_luong_con_lai - p_ds_sl_sach[tong_phieu][j][50];
                    break;
                }
            } else {
                printf ("Mã không hợp lệ");
            }
        }
    }
    tong_phieu++;
    printf("Lập phiếu mượn thành công!\n");
}

void lap_phieu_tra() {
    if (tong_phieu == 0) {
        printf("Chưa có phiếu mượn nào!\n");
        return;
    }

    int ma;
    printf("Nhập mã độc giả cần trả sách: ");
    scanf("%d", &ma);
    int _c; while ((_c = getchar()) != '\n' && _c != EOF);

    int found = 0;
    for (int i = 0; i < tong_phieu; i++) {
        if (p_ma_doc_gia[i] != ma) continue;
        found = 1;

        printf("Ngày mượn là: %s\n", p_ngay_muon[i]);
        printf("Ngày trả dự kiến là: %s\n", p_ngay_tra_du_kien[i]);

        // Ngày trả thực tế
        ngay_tra_tt(tong_phieu);

        // Danh sách ISBN
        for (int k=0; k < p_so_luong_sach[i]; k++){
            printf("Sách có mã %s đã mượn %d quyển.\n",p_ds_isbn[i][k],p_ds_sl_sach[i][k]); 
        }

        // Phạt tiền khi trả trễ ngày hẹn
        ngay_tra_tt(i);
        long so_ngay_tre = kiem_tra_tra_sach(p_ngay_tra_thuc_te[i], p_ngay_tra_du_kien[i]);
            if (so_ngay_tre <= 0) {
                printf("Trả đúng hạn!\n");
            } else {
                printf("Trả trễ %ld ngày.\n", so_ngay_tre);
            }
        long long tong_phat_tre = (long long)so_ngay_tre * PHI_PHAT_MOI_NGAY;
        printf("Số tiền bị phạt khi trả trễ sách là %lld VND.\n",tong_phat_tre);

        // Phạt tiền khi mất sách
        long long tong_phat_mat = 0;
        for (int j = 0; j < p_so_luong_sach[i]; j++) {
            char ans[8];
            printf("Sách ISBN %s có bị mất không? (Y. Có / N. Không): ", p_ds_isbn[i][j]);
            if (fgets(ans, sizeof(ans), stdin) == NULL) ans[0] = '\0';
            if (ans[0] == 'y' || ans[0] == 'Y') {
                int idx = find_index_by_isbn(p_ds_isbn[i][j]);
                if (idx >= 0) {
                    long long phat_mat = (long long)(2.0 * gia_sach[idx] + 0.5);
                    tong_phat_mat += phat_mat;
                    printf("  -> Mất sách! Phạt 200%% giá sách: %lld VND\n", phat_mat);
                }
            } else {
                // không mất → cộng lại vào kho
                int idx = find_index_by_isbn(p_ds_isbn[i][j]);
                if (idx >= 0) {
                    so_quyen[idx] += p_ds_sl_sach[i][j][50];
                    printf("  -> Đã trả sách. Số quyển hiện có trong kho: %d\n", so_quyen[idx]);
                }
            }
        }

        p_tong_tien_phat[i] = tong_phat_mat + tong_phat_tre;

        printf("\n== KẾT QUẢ TRẢ SÁCH ==\n");
        printf("Mã độc giả: %d\n", ma);
        printf("Ngày trả thực tế: %s\n", p_ngay_tra_thuc_te[i]);
        printf("Trễ hạn: %d ngày -> Phạt trễ: %lld VND\n", so_ngay_tre, tong_phat_tre);
        printf("Mất sách -> Phạt: %lld VND\n", tong_phat_mat);
        printf("Tổng tiền phạt (bao gồm mất sách nếu có): %lld VND\n", p_tong_tien_phat[i]);
        return;
    }

    if (!found) {
        printf("Không tìm thấy phiếu mượn nào của độc giả %d.\n", ma);
    }
}

void danh_sach_phieu_muon() {
    if (tong_phieu == 0) {
        printf("Chưa có phiếu mượn/ trả nào!\n");
        return;
    }
    printf("=== Danh sách phiếu mượn ===\n");
    for (int i = 0; i < tong_phieu; i++) {
        printf("\nPhiếu %d:\n", i+1);
        printf("  Mã độc giả: %d\n", p_ma_doc_gia[i]);
        printf("  Ngày mượn: %s\n", p_ngay_muon[i]);
        printf("  Ngày trả dự kiến: %s\n", p_ngay_tra_du_kien[i]);
        printf("  Ngày trả thực tế: %s\n", p_ngay_tra_thuc_te[i][0] ? p_ngay_tra_thuc_te[i] : "Chưa trả");
        printf("  Số sách mượn: %d\n", p_so_luong_sach[i]);
        for (int j = 0; j < p_so_luong_sach[i]; j++) {
            printf("    - %s\n", p_ds_isbn[i][j]);
        }
        printf("  Tổng tiền phạt: %lld VND\n", p_tong_tien_phat[i]);
    }
}

void quan_ly_phieu_muon() {
    int chon;
    printf("\n1. Lập phiếu mượn sách\n");
    printf("2. Lập phiếu trả sách\n");
    printf("3. Xem danh sách phiếu\n");
    printf("0. Quay lại\n");
    printf("Chọn: ");
    if (scanf("%d", &chon) != 1) {
        int _c; while ((_c = getchar()) != '\n' && _c != EOF);
        printf("Lựa chọn không hợp lệ.\n");
        return;
    }

    switch (chon) {
        case 1:
            lap_phieu_muon();
            break;
        case 2:
            lap_phieu_tra();
            break;
        case 3:
            danh_sach_phieu_muon();
            break;
        case 0:
            return;
        default:
            printf("Lựa chọn không hợp lệ.\n");
    }
}
