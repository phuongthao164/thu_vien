// Quản lý phiếu mượn sách
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "sach.h"     // dùng find_index_by_isbn, gia_sach[], so_quyen[]
#include "doc_gia.h"  // chỉ để đồng bộ include, không phụ thuộc biến trong đó
#include "phieu_muon.h"
#include "thong_ke.h"

extern void remove_buffer();

// ====== Cấu hình ======
#define MAX_LEN           100
#define MAX_PHIEU         200
#define MAX_SACH_MUON     10
#define NGAY_MUON_TOI_DA  7


// ====== Dữ liệu phiếu  ======
int   pm_ma_doc_gia[MAX_PHIEU];
char  pm_ngay_muon[MAX_PHIEU][20];
char  pm_ngay_tra_du_kien[MAX_PHIEU][20];
char  pm_ngay_tra_thuc_te[MAX_PHIEU][20];
int   pm_so_luong_sach[MAX_PHIEU]; // số sách muốn mượn
char  pm_ds_isbn[MAX_PHIEU][MAX_SACH_MUON][50];
int   pm_ds_sl_sach[MAX_PHIEU][MAX_SACH_MUON];
long long pm_tong_tien_phat[MAX_PHIEU];

int tong_phieu_muon = 0;

// Các biến từ sach.c
extern int tong_so_sach;
extern char the_loai[][200];
extern int so_quyen[];
extern char isbn[][200];
extern double gia_sach[];

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

    snprintf(pm_ngay_muon[index], MAX_LEN, "%02d/%02d/%04d", day_ht, month_ht, year_ht);
    printf("  Ngày mượn sách là: %s\n", pm_ngay_muon[index]);
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

    snprintf(pm_ngay_tra_du_kien[index], MAX_LEN, "%02d/%02d/%04d", day_hh, month_hh, year_hh);
    printf("  Ngày dự kiến trả sách là: %s\n", pm_ngay_tra_du_kien[index]);
}

// ====== Chức năng ======
// Hàm lập phiếu mượn
void lap_phieu_muon() {
    if (tong_phieu_muon >= MAX_PHIEU) {
        printf("Đã đạt số lượng phiếu tối đa (%d).\n", MAX_PHIEU);
        return;
    }

    int i = tong_phieu_muon;

    // Mã độc giả
    printf("  Nhập mã độc giả: ");
    scanf("%d", &pm_ma_doc_gia[i]);
    int f_ma = 0;
    for (int j = 0; j < tong_so_doc_gia; j++) {
        if (pm_ma_doc_gia[i] == id_doc_gia[j]) {
            printf("-> Độc giả có tên %s\n",ho_ten[j]);
            f_ma = 1;
            break;
        }
    }
    if (!f_ma) {
        printf("-> Mã độc giả không hợp lệ!\n");
        int _c; while ((_c = getchar()) != '\n' && _c != EOF);
        return; 
    }
    
    // Ngày mượn
    ngay_muon(tong_phieu_muon);

    // ngày trả dự kiến
    ngay_tra_dk(tong_phieu_muon);

    // Ngày trả thực tế
    pm_ngay_tra_thuc_te[tong_phieu_muon][0] = '\0';

    // if (p_so_luong_sach[i] < 1 || p_so_luong_sach[i] > MAX_SACH_MUON) {
    //     printf("So luong khong hop le.\n");
    //     return;
    // }

    // int _c; while ((_c = getchar()) != '\n' && _c != EOF);

    // - danh sách ISBN của các sách được mượn
    printf("  Nhập số lượng sách muốn mượn (tối đa %d quyển): ", MAX_SACH_MUON);
    scanf("%d", &pm_so_luong_sach[tong_phieu_muon]);

    // Kiểm tra số sách mượn vượt quá tối đa 10 quyển
    if (pm_so_luong_sach[tong_phieu_muon] >= MAX_SACH_MUON) {
        printf("Đã quá số lượng sách tối đa có thể mượn.\n");
    return;
    }

    int so_luong_con_lai = pm_so_luong_sach[tong_phieu_muon]; 
    for (int j = 0; j < pm_so_luong_sach[tong_phieu_muon] && so_luong_con_lai > 0; ) {
        printf("\n  Số lượng sách muốn mượn còn lại là: %d\n",so_luong_con_lai);
        printf("  Nhap ISBN sach thu %d: ", j + 1);
        scanf("%s",pm_ds_isbn[tong_phieu_muon][j]);
        
        int f_sach = 0;
        for (int k = 0;k < tong_so_sach;k++){    
            if(strcmp(pm_ds_isbn[tong_phieu_muon][j], isbn[k]) == 0){
                f_sach = 1;

                if (so_quyen[k] <= 0) {
                    printf("  -> Đã hết sách trong kho! Không thể mượn tiếp được.\n");
                    break;
                } 
                // printf("[DBG] so sanh '%s' vs isbn[%d]='%s', so_quyen=%d\n",
                // pm_ds_isbn[i][j], k, isbn[k], so_quyen[k]);
                printf("  Còn %d quyển trong kho. Bạn muốn mượn bao nhiêu quyển? ",so_quyen[k]);
                scanf("%d", &pm_ds_sl_sach[tong_phieu_muon][j]);

                if (pm_ds_sl_sach[tong_phieu_muon][j] > so_quyen[k] || pm_ds_sl_sach[tong_phieu_muon][j] > so_luong_con_lai || pm_ds_sl_sach[tong_phieu_muon][j] <= 0) {
                    printf("  -> Số lượng mượn không hợp lệ!\n");
                    f_sach = 0; // coi như lỗi
                    break;
                }

                so_quyen[k] -= pm_ds_sl_sach[tong_phieu_muon][j];  // Số sách còn lại trong kho của mã ISBN trên
                so_luong_con_lai -= pm_ds_sl_sach[tong_phieu_muon][j]; // số sách còn lại có thể mượn tiếp

                printf("-> Bạn đã mượn thành công sách có mã %s. Còn %d quyển trong kho!\n",pm_ds_isbn[tong_phieu_muon][j],so_quyen[k]);
                j++;
                break;
            }    
        } 
        if(!f_sach) {  
            printf ("-> Mã sách không hợp lệ hoặc lỗi số lượng. Nhập lại!\n");
            // j--;
        }

        if (so_luong_con_lai == 0) {
            printf(" -> Đã đạt giới hạn số lượng mượn cho phiếu này.\n");
            break;
        }
    }

    // cập nhật lại số dòng sách thật sự được ghi vào phiếu
    pm_so_luong_sach[i] = (pm_so_luong_sach[i] - so_luong_con_lai);   

    tong_phieu_muon++;
    printf("=-> Lập phiếu mượn thành công!\n");
    remove_buffer();
}

// Hàm xem danh sách phiếu mượn
void danh_sach_phieu_muon() {
    printf("=== Danh sách phiếu mượn ===\n");

    if (tong_phieu_muon == 0) {
        printf("Chưa có phiếu mượn nào!\n");
        return;
    }  

    for (int i = 0; i < tong_phieu_muon; i++) {
        printf("\nPhiếu mượn %d:\n", i+1);
        printf("  Mã độc giả: %d\n", pm_ma_doc_gia[i]);
        printf("  Ngày mượn: %s\n", pm_ngay_muon[i]);
        printf("  Ngày trả dự kiến: %s\n", pm_ngay_tra_du_kien[i]);
        printf("  Ngày trả thực tế: %s\n", pm_ngay_tra_thuc_te[i][0] ? pm_ngay_tra_thuc_te[i] : "Chưa trả");
        printf("  Số sách mượn: %d quyển\n", pm_so_luong_sach[i]);
        for (int j = 0; j < MAX_SACH_MUON && strlen(pm_ds_isbn[i][j]) > 0; j++) {
            printf("    - %s: %d quyển\n", pm_ds_isbn[i][j],pm_ds_sl_sach[i][j]);
        }
        if(pm_tong_tien_phat[i] != 0){
            printf("  Tổng tiền phạt: %lld VND\n", pm_tong_tien_phat[i]);
        }
    }
    remove_buffer();
}

// Hàm đọc dữ liệu phiếu mượn từ file
void doc_du_lieu_phieu_muon_tu_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Không thể mở file %s\n", filename);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        // bỏ CR/LF
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue; // dòng trống

        // Tách 8 trường bằng ';' (chấp nhận rỗng)
        char *fields[8] = {0};
        int ok = 1;
        char *p = line;
        for (int k = 0; k < 7; k++) {
            char *semi = strchr(p, ';');
            if (!semi) { ok = 0; break; }
            *semi = '\0';
            fields[k] = p;
            p = semi + 1;
        }
        if (!ok) {
            printf("⚠️  Dòng bị lỗi format, bỏ qua: %s\n", line);
            continue;
        }
        fields[7] = p; // trường cuối

        // Ép kiểu + copy an toàn
        int idx = tong_phieu_muon;
        if (idx >= MAX_PHIEU) { printf("Đã đầy bộ nhớ phiếu.\n"); break; }

        pm_ma_doc_gia[idx] = atoi(fields[0]);

        strncpy(pm_ngay_muon[idx],        fields[1], 19); pm_ngay_muon[idx][19]        = '\0';
        strncpy(pm_ngay_tra_du_kien[idx], fields[2], 19); pm_ngay_tra_du_kien[idx][19] = '\0';
        // có thể rỗng: ""
        if (fields[3] && fields[3][0] != '\0')
            strncpy(pm_ngay_tra_thuc_te[idx], fields[3], 19);
        else
            pm_ngay_tra_thuc_te[idx][0] = '\0';


        pm_so_luong_sach[idx] = atoi(fields[4]);

        // Tách danh sách ISBN theo '|'
        int n_isbn = 0;
        if (fields[5] && fields[5][0] != '\0') {
            char *cur = fields[5];
            while (n_isbn < MAX_SACH_MUON) {
                char *bar = strchr(cur, '|');
                if (bar) *bar = '\0';
                strncpy(pm_ds_isbn[idx][n_isbn], cur, 49);
                pm_ds_isbn[idx][n_isbn][49] = '\0';
                n_isbn++;
                if (!bar) break;
                cur = bar + 1;
            }
        }

        // Tách danh sách số lượng tương ứng
        int n_sl = 0;
        if (fields[6] && fields[6][0] != '\0') {
            char *cur = fields[6];
            while (n_sl < MAX_SACH_MUON) {
                char *bar = strchr(cur, '|');
                if (bar) *bar = '\0';
                pm_ds_sl_sach[idx][n_sl] = atoi(cur);
                n_sl++;
                if (!bar) break;
                cur = bar + 1;
            }
        }

        // Nếu thiếu dòng thì clear phần dư cho sạch
        for (int j = pm_so_luong_sach[idx]; j < MAX_SACH_MUON; j++) {
            pm_ds_isbn[idx][j][0] = '\0';
            pm_ds_sl_sach[idx][j] = 0;
        }

        // Tổng tiền phạt (có thể là 0)
        pm_tong_tien_phat[idx] = atoll(fields[7]);

        tong_phieu_muon++;
    }

    fclose(f);
    printf("✅ Đã nạp %d phiếu mượn từ file %s\n", tong_phieu_muon, filename);
}


// Hàm quản lý phiếu
void quan_ly_phieu_muon() {
    int chon;
    printf("\n===== MENU PHIẾU MƯỢN SÁCH =====\n");
    printf("1. Lập phiếu mượn sách\n");
    printf("2. Xem danh sách phiếu mượn\n");
    printf("0. Quay lại\n");
    printf("Chọn: ");

    if (!read_int_safe("Chọn chức năng: ", &chon)) {
        printf("  -> Nhập không hợp lệ!\n");
        return;
    }

    switch (chon) {
        case 1:
            lap_phieu_muon();
            break;
        case 2:
            danh_sach_phieu_muon();
            break;
        case 0:
            return;
        default:
            printf("Lựa chọn không hợp lệ.\n");
    }
}

extern int read_int_safe(const char *prompt, int *out);