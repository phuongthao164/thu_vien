// Quản lý phiếu mượn và trả sách (không dùng struct)
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sach.h"     // dùng find_index_by_isbn, gia_sach[], so_quyen[]
#include "doc_gia.h"  // chỉ để đồng bộ include, không phụ thuộc biến trong đó

// ====== Cấu hình ======
#define MAX_PHIEU         200
#define MAX_SACH_MUON     10
#define PHI_PHAT_MOI_NGAY 5000LL
#define NGAY_MUON_TOI_DA  7

// ====== Dữ liệu phiếu  ======
static int   p_ma_doc_gia[MAX_PHIEU];
static char  p_ngay_muon[MAX_PHIEU][20];
static char  p_ngay_tra_du_kien[MAX_PHIEU][20];
static char  p_ngay_tra_thuc_te[MAX_PHIEU][20]; // "" nếu chưa trả
static int   p_so_luong_sach[MAX_PHIEU];
static char  p_ds_isbn[MAX_PHIEU][50];
static long long p_tong_tien_phat[MAX_PHIEU];

int tong_phieu = 0;

// Các biến từ sach.c
extern int tong_so_sach;
extern char the_loai[][200];
extern int so_quyen[];
extern char isbn[][100];

// Các biến từ doc_gia.c
extern int tong_so_doc_gia;
extern char gioi_tinh[][100];
extern char ho_ten[][100];
extern int id_doc_gia[100];

// ====== Tiện ích ngày tháng ======
void get_today(char *buf /* >=20 */) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, 20, "%d/%m/%Y", tm_info);
}

void add_days_ddmmyyyy(const char *ddmmyyyy, int days, char *out /* >=20 */) {
    int d, m, y;
    if (sscanf(ddmmyyyy, "%d/%d/%d", &d, &m, &y) != 3) {
        out[0] = '\0';
        return;
    }
    struct tm tmv = {0};
    tmv.tm_mday = d;
    tmv.tm_mon  = m - 1;
    tmv.tm_year = y - 1900;
    time_t t = mktime(&tmv);
    if (t == (time_t)-1) { out[0]='\0'; return; }
    t += (time_t)days * 24 * 3600;
    struct tm *due = localtime(&t);
    strftime(out, 20, "%d/%m/%Y", due);
}

int days_between_ddmmyyyy(const char *a, const char *b) {
    int d1,m1,y1,d2,m2,y2;
    if (sscanf(a, "%d/%d/%d", &d1, &m1, &y1) != 3) return 0;
    if (sscanf(b, "%d/%d/%d", &d2, &m2, &y2) != 3) return 0;
    struct tm tma = {0}, tmb = {0};
    tma.tm_mday = d1; tma.tm_mon = m1-1; tma.tm_year = y1-1900;
    tmb.tm_mday = d2; tmb.tm_mon = m2-1; tmb.tm_year = y2-1900;
    time_t ta = mktime(&tma);
    time_t tb = mktime(&tmb);
    if (ta == (time_t)-1 || tb == (time_t)-1) return 0;
    long long diff = (long long)(tb - ta) / (24*3600);
    return (diff > 0) ? (int)diff : 0;
}

// ====== Chức năng ======
void lap_phieu_muon() {
    if (tong_phieu >= MAX_PHIEU) {
        printf("Da dat so luong phieu toi da (%d).\n", MAX_PHIEU);
        return;
    }

    int i = tong_phieu;
    // Mã độc giả
    printf("Nhap ma doc gia: ");
    scanf("%d", &p_ma_doc_gia[i]);
    for(int j=0; j<tong_so_doc_gia; j++){
        if(strcmp(p_ma_doc_gia[i], id_doc_gia[j]) != 0){
            printf("Ma doc gia khong hop le");
            int _c; while ((_c = getchar()) != '\n' && _c != EOF);
        };

    }
    
    // Ngày mượn
    get_today(p_ngay_muon[i]);
    add_days_ddmmyyyy(p_ngay_muon[i], NGAY_MUON_TOI_DA, p_ngay_tra_du_kien[i]);
    p_ngay_tra_thuc_te[i][0] = '\0';
    p_tong_tien_phat[i] = 0;

    printf("Ngay muon: %s\n", p_ngay_muon[i]);
    // ngày trả dự kiến
    printf("Ngay tra du kien: %s\n", p_ngay_tra_du_kien[i]);

    printf("Nhap so luong sach muon (toi da %d): ", MAX_SACH_MUON);
    scanf("%d", &p_so_luong_sach[i]);
    if (p_so_luong_sach[i] < 1 || p_so_luong_sach[i] > MAX_SACH_MUON) {
        printf("So luong khong hop le.\n");
        return;
    }

    int _c; while ((_c = getchar()) != '\n' && _c != EOF);

    // - danh sách ISBN của các sách được mượn
    for (int j = 0; j < p_so_luong_sach[i]; j++) {
        printf("Nhap ISBN sach thu %d: ", j + 1);
        scanf("%s",p_ds_isbn[j]);
        // printf("Con %d quyen trong kho. Ban muon muon bao nhieu quyen?",)
        // if (fgets(p_ds_isbn[i][j], sizeof(p_ds_isbn[i][j]), stdin) == NULL) {
        //     p_ds_isbn[i][j][0] = '\0';
        // } else {
        //     size_t ln = strlen(p_ds_isbn[i][j]);
        //     if (ln && p_ds_isbn[i][j][ln-1] == '\n') p_ds_isbn[i][j][ln-1] = '\0';
        // }

    //     int idx = find_index_by_isbn(p_ds_isbn[i][j]);
    //     if (idx < 0) {
    //         printf("  -> Khong tim thay sach %s. Bo qua!\n", pm_ds_isbn[i][j]);
    //         continue;
    //     }
    //     if (so_quyen[idx] <= 0) {
    //         printf("  -> Het sach trong kho! Khong the muon.\n");
    //     } else {
    //         so_quyen[idx]--;
    //         printf("  -> Da muon thanh cong, so quyen con lai: %d\n", so_quyen[idx]);
    //     }
        for (int k = 0;k < tong_so_sach;k++){    
            if(strcmp(p_ds_isbn[j], isbn[k]) == 0){
                int so_quyen_muon = 0;
                printf("Con %d quyen trong kho. Ban muon muon bao nhieu quyen?",so_quyen[k]);
                scanf("%s", so_quyen_muon);
                so_quyen[k] = so_quyen[k] - so_quyen_muon;
                printf("Ban da muon thanh cong sach co ma %s. Con %d quyen trong kho!",p_ds_isbn[j],so_quyen[k]);
                break;
            } else {
                printf ("Ma khong hop le");
            }
        }

    tong_phieu++;
    printf("Lap phieu muon thanh cong!\n");
    }
}

void lap_phieu_tra() {
    if (tong_phieu == 0) {
        printf("Chua co phieu muon nao!\n");
        return;
    }

    int ma;
    printf("Nhap ma doc gia can tra sach: ");
    scanf("%d", &ma);
    int _c; while ((_c = getchar()) != '\n' && _c != EOF);

    int found = 0;
    for (int i = 0; i < tong_phieu; i++) {
        if (pm_ma_doc_gia[i] != ma) continue;
        found = 1;

        get_today(pm_ngay_tra_thuc_te[i]);
        int tre = days_between_ddmmyyyy(pm_ngay_tra_du_kien[i], pm_ngay_tra_thuc_te[i]);
        long long tong_phat = (long long)tre * PHI_PHAT_MOI_NGAY;

        for (int j = 0; j < pm_so_luong_sach[i]; j++) {
            char ans[8];
            printf("Sach ISBN %s co bi mat khong? (y/n): ", pm_ds_isbn[i][j]);
            if (fgets(ans, sizeof(ans), stdin) == NULL) ans[0] = '\0';
            if (ans[0] == 'y' || ans[0] == 'Y') {
                int idx = find_index_by_isbn(pm_ds_isbn[i][j]);
                if (idx >= 0) {
                    long long phat_mat = (long long)(2.0 * gia_sach[idx] + 0.5);
                    tong_phat += phat_mat;
                    printf("  -> Mat sach! Phat 200%% gia: %lld VND\n", phat_mat);
                }
            } else {
                // không mất → cộng lại vào kho
                int idx = find_index_by_isbn(pm_ds_isbn[i][j]);
                if (idx >= 0) {
                    so_quyen[idx]++;
                    printf("  -> Da tra sach. So quyen con lai: %d\n", so_quyen[idx]);
                }
            }
        }

        pm_tong_tien_phat[i] = tong_phat;

        printf("\n== KET QUA TRA SACH ==\n");
        printf("Ma doc gia: %d\n", ma);
        printf("Ngay tra thuc te: %s\n", pm_ngay_tra_thuc_te[i]);
        printf("Tre han: %d ngay -> Phat tre: %lld VND\n", tre, (long long)tre * PHI_PHAT_MOI_NGAY);
        printf("Tong tien phat (bao gom mat sach neu co): %lld VND\n", pm_tong_tien_phat[i]);
        return;
    }

    if (!found) {
        printf("Khong tim thay phieu muon nao cua doc gia %d.\n", ma);
    }
}

void danh_sach_phieu_muon() {
    if (tong_phieu == 0) {
        printf("Chua co phieu muon nao!\n");
        return;
    }
    printf("=== Danh sach phieu muon ===\n");
    for (int i = 0; i < tong_phieu; i++) {
        printf("\nPhieu %d:\n", i+1);
        printf("  Ma doc gia: %d\n", pm_ma_doc_gia[i]);
        printf("  Ngay muon: %s\n", pm_ngay_muon[i]);
        printf("  Ngay tra du kien: %s\n", pm_ngay_tra_du_kien[i]);
        printf("  Ngay tra thuc te: %s\n", pm_ngay_tra_thuc_te[i][0] ? pm_ngay_tra_thuc_te[i] : "Chua tra");
        printf("  So sach muon: %d\n", pm_so_luong_sach[i]);
        for (int j = 0; j < pm_so_luong_sach[i]; j++) {
            printf("    - %s\n", pm_ds_isbn[i][j]);
        }
        printf("  Tong tien phat: %lld VND\n", pm_tong_tien_phat[i]);
    }
}

void quan_ly_phieu_muon() {
    int chon;
    printf("\n1. Lap phieu muon sach\n");
    printf("2. Lap phieu tra sach\n");
    printf("3. Xem danh sach phieu muon\n");
    printf("0. Quay lai\n");
    printf("Chon: ");
    if (scanf("%d", &chon) != 1) {
        int _c; while ((_c = getchar()) != '\n' && _c != EOF);
        printf("Lua chon khong hop le.\n");
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
            printf("Lua chon khong hop le.\n");
    }
}
