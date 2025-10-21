//  Thống kê
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sach.h"
#include "doc_gia.h"

// Để truy cập dữ liệu phiếu mượn (dùng extern, vì đã có trong phieu_muon_tra.c)
extern int tong_phieu;
extern int pm_ma_doc_gia[];
extern char pm_ngay_muon[][20];
extern char pm_ngay_tra_du_kien[][20];
extern char pm_ngay_tra_thuc_te[][20];
extern int pm_so_luong_sach[];
extern char pm_ds_isbn[][10][50];
extern long long pm_tong_tien_phat[];

// Các biến từ sach.c và doc_gia.c
extern int tong_so_sach;
extern char the_loai[][200];
extern int so_quyen[];
extern int tong_so_doc_gia;
extern char gioi_tinh[][100];
extern char ho_ten[][100];

// Hàm tính số ngày trễ giữa 2 ngày (dd/mm/yyyy)
static int days_between(const char *a, const char *b) {
    int d1,m1,y1,d2,m2,y2;
    if (sscanf(a, "%d/%d/%d", &d1, &m1, &y1) != 3) return 0;
    if (sscanf(b, "%d/%d/%d", &d2, &m2, &y2) != 3) return 0;
    struct tm t1 = {0}, t2 = {0};
    t1.tm_mday=d1; t1.tm_mon=m1-1; t1.tm_year=y1-1900;
    t2.tm_mday=d2; t2.tm_mon=m2-1; t2.tm_year=y2-1900;
    time_t T1 = mktime(&t1);
    time_t T2 = mktime(&t2);
    int diff = (int)((T2 - T1)/(24*3600));
    return (diff > 0) ? diff : 0;
}

// Lấy ngày hiện tại dạng dd/mm/yyyy
static void get_today(char *buf) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, 20, "%d/%m/%Y", tm_info);
}

// a. Thống kê tổng số sách
void thong_ke_tong_sach() {
    printf("Tong so loai sach trong kho la: %d\n", tong_so_sach);
    int tong_tat_ca_sach = 0;
    for (int i=0;i<tong_so_sach;i++) {
        tong_tat_ca_sach += so_quyen[i];
    }
    printf("Tong so sach của tat ca cac loai sach trong kho la: %d\n", tong_tat_ca_sach);
}

// b. Thống kê số lượng sách theo thể loại
void thong_ke_the_loai() {
    printf("\n Thong ke so luong sach theo the loai:\n");
    if (tong_so_sach == 0) {
        printf("Khong co sach nao trong thu vien.\n");
        return;
    }

    char chuoi_loai[500][200];
    int dem_loai_sach[500] = {0};
    int dem_so_sach[500] = {0};
    int loai_count = 0;

    for (int i=0; i < tong_so_sach; i++){
        for (int j=0; j < loai_count ; j++) {
            if (strcmp(chuoi_loai[j], the_loai[i]) == 0) {
                dem_loai_sach[loai_count] += 1;
                dem_so_sach[loai_count] += (so_quyen[i]*dem_loai_sach[loai_count]);
            } else {
                strcpy(the_loai[loai_count][200], the_loai[i]);
                loai_count ++ ;
                dem_loai_sach[loai_count] = 1;
                dem_so_sach[loai_count] += so_quyen[i];
            }
        }
    }
    
    for (int i = 0; i < loai_count; i++) {
        printf("%d. The loai %s co: \n",i+1,chuoi_loai[i]);
        printf("- %d loai sach",dem_loai_sach[i]);
        printf("- %d quyen sach",dem_so_sach[i]);
    }
    

    // for (int i = 0; i < tong_so_sach; i++) {
    //     int found = 0;
    //     for (int j = 0; j < loai_count; j++) {
    //         if (strcmp(loai[j], the_loai[i]) == 0) {
    //             dem[j]++;
    //             found = 1;
    //             break;
    //         }
    //     }
    //     if (!found) {
    //         strcpy(loai[loai_count], the_loai[i]);
    //         dem[loai_count] = 1;
    //         loai_count++;
    //     }
    // }

    // for (int i = 0; i < loai_count; i++) {
    //     printf("   - %s: %d dau sach\n", loai[i], dem[i]);
    // }
}

// c. Thống kê số lượng độc giả
void thong_ke_tong_doc_gia() {
    printf("Tong so luong doc gia: %d\n", tong_so_doc_gia);
}

// d. Thống kê số lượng độc giả theo giới tính
void thong_ke_doc_gia_theo_gioi_tinh() {
    printf("Thong ke doc gia theo gioi tinh:\n");
    int count_nam = 0;
    int count_nu = 0;
    int count_khac = 0;
    for (int i=0 ; i<tong_so_doc_gia ; i++){
        // Bỏ qua độc giả đã bị xóa
        if (ho_ten[i][0] == '\0') continue;

        if(strcasecmp(gioi_tinh[i], "Nam") == 0){
            count_nam ++;
        } else if(strcasecmp(gioi_tinh[i], "Nu") == 0){
            count_nu ++;
        } else {
            count_khac ++;
        }
    }

    printf("Tong so doc gia co gioi tinh Nam la: %d", count_nam);
    printf("Tong so doc gia co gioi tinh Nu la: %d", count_nu);
    printf("Tong so doc gia co gioi tinh Khac la: %d", count_khac);

//     int nam = 0, nu = 0, khac = 0;
//     for (int i = 0; i < tong_so_doc_gia; i++) {
//         if (ho_ten[i][0] == '\0') continue; // bỏ bản ghi xóa
//         if (strcasecmp(gioi_tinh[i], "Nam") == 0) nam++;
//         else if (strcasecmp(gioi_tinh[i], "Nu") == 0) nu++;
//         else khac++;
//     }
//     printf("   - Nam: %d\n", nam);
//     printf("   - Nu: %d\n", nu);
//     printf("   - Khac/Khong xac dinh: %d\n", khac);
}

// e. Thống kê số sách đang được mượn
void thong_ke_sach_dang_muon() {
    printf("\ne. So sach dang duoc muon:\n");
    if (tong_phieu == 0) {
        printf("   Chua co phieu muon nao.\n");
        return;
    }

    int tong_muon = 0;
    for (int i = 0; i < tong_phieu; i++) {
        if (pm_ngay_tra_thuc_te[i][0] == '\0') { // chưa trả
            tong_muon += pm_so_luong_sach[i];
        }
    }
    printf("   Tong so sach dang duoc muon: %d\n", tong_muon);
}

// f. Thống kê danh sách độc giả bị trễ hạn
static void thong_ke_doc_gia_tre_han() {
    printf("\nf. Danh sach doc gia bi tre han:\n");
    if (tong_phieu == 0) {
        printf("   Khong co phieu muon nao.\n");
        return;
    }

    char today[20];
    get_today(today);
    int found = 0;

    for (int i = 0; i < tong_phieu; i++) {
        if (pm_ngay_tra_thuc_te[i][0] != '\0') continue; // đã trả
        int ngay_tre = days_between(pm_ngay_tra_du_kien[i], today);
        if (ngay_tre > 0) {
            printf("   - Doc gia ID %d bi tre %d ngay (han: %s)\n",
                   pm_ma_doc_gia[i], ngay_tre, pm_ngay_tra_du_kien[i]);
            found = 1;
        }
    }
    if (!found)
        printf("   Tat ca doc gia deu tra dung han hoac chua toi han.\n");
}

// Hàm thống kê
void thong_ke_co_ban() {
    int choice;
    printf("1. Thong ke so luong sach trong thu vien\n");
    printf("2. Thong ke so luong sach theo the loai\n");
    printf("3. Thong ke so luong doc gia\n");
    printf("4. Thong ke so luong doc gia theo gioi tinh\n");
    printf("5. Thong ke so sach dang muon\n");
    printf("6. Thong ke danh sach doc gia bi tre han\n");
    printf("0. Quay lai\n");
    printf("Chon chuc nang: ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            thong_ke_tong_sach();
            break;
        case 2:
            thong_ke_the_loai();
            break;
        case 3:
            thong_ke_tong_doc_gia();
            break;
        case 4:
            thong_ke_doc_gia_theo_gioi_tinh();
            break;
        case 5:
            thong_ke_sach_dang_muon();
            break;
        case 6:
            thong_ke_doc_gia_tre_han();
            break;
        case 0:
            return;
        default:
            printf("Lua chon khong hop le!\n");
    }
}
