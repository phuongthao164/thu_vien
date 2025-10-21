// Quản lý độc giả
#include <stdio.h>
#include <string.h>
#include "doc_gia.h"
#include <time.h>

#define MAX_LEN 100
#define MAX_USERS 100

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
// char created[MAX_USERS][MAX_LEN];
// char expire[MAX_USERS][MAX_LEN];

// Lấy ngày hiện tại và ngày hết hạn (4 năm sau) mẫu dd/mm/yyyy
// void make_dates(char *created, int csize, char *expire, int esize) {
//     time_t t = time(NULL);
//     struct tm tm_now = *localtime(&t);
//     struct tm tm_exp = tm_now;
//     tm_exp.tm_year += 4; // +4 năm

//     // mktime chuẩn hóa (để phòng trường hợp 29/2...)
//     mktime(&tm_exp);

//     strftime(created, csize, "%d/%m/%Y", &tm_now);
//     strftime(expire, esize, "%d/%m/%Y", &tm_exp);
// }


// Hàm thêm độc giả
void them_doc_gia() {
    if (tong_so_doc_gia >= MAX_USERS) {
    printf("Da dat so luong doc gia toi da (%d).\n", MAX_USERS);
    return;
    }

    // Mã độc giả sẽ được tự động đếm số tăng dần
    int ma_doc_gia = tong_so_doc_gia + 1;
    printf("Ma doc gia cua ban la: %d \n",ma_doc_gia);

    // Lưu ID độc giả vào mảng id_doc_gia
    id_doc_gia[tong_so_doc_gia] = ma_doc_gia;

    // Thêm thông tin độc giả vào danh sách
    printf("Vui long nhap thong tin ca nhan cua ban: \n");

    // Họ tên
    printf("Nhap ho ten: ");
    scanf(" %[^\n]%*c", ho_ten[tong_so_doc_gia]);  

    // CMND
    printf("Nhap CMND: ");
    scanf("%99s", cmnd[tong_so_doc_gia]);  

    // Ngày tháng năm sinh
    printf("Nhap ngay thang nam sinh (dd/mm/yyyy): ");
    scanf("%99s", ngay_sinh[tong_so_doc_gia]);  

    // Giới tính
    printf("Nhap gioi tinh (Nam/Nu): ");
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
    printf("Nhap email: ");
    scanf("%99s", email[tong_so_doc_gia]); 
    
    // Địa chỉ
    printf("Nhap dia chi: ");
    scanf(" %[^\n]%*c", dia_chi[tong_so_doc_gia]);  

    tong_so_doc_gia++;
    printf("Doc gia da duoc them thanh cong!\n");

    //Tính ngày hết hạn
    // char created[16], expire[16];
    // make_dates(created, sizeof(created), expire, sizeof(expire));
    // created[tong_so_doc_gia] = created;
    // expire[tong_so_doc_gia] = expire;
    // printf("Ngay lap the cua ban la: %s\n",created[tong_so_doc_gia]);
    // printf("Ngay het han the cua ban la: %s\n",expire[tong_so_doc_gia]);

    printf("Ngay lap the cua ban la: 18/10/2025\n"); //dùng tạm
    printf("Ngay het han the cua ban la: 18/10/2029\n"); //dùng tạm 
}

// Hàm hiển thị danh sách độc giả
void danh_sach_doc_gia() {
    if (tong_so_doc_gia == 0) {
        printf("Khong co doc gia trong thu vien.\n");
        return;
    }

    printf("So doc gia hien tai cua thu vien la: %d\n",tong_so_doc_gia);
    printf("Danh sach doc gia:\n");
    for (int i = 0; i < tong_so_doc_gia; i++) {
    printf("Doc gia thu %d (ID: %d)\n", i+1, id_doc_gia[i]);
    if (ho_ten[i][0] == '\0') {
        printf("  >> Thong tin doc gia da bi xoa \n");
    } else {
        printf("  Ho ten: %s, CMND: %s, Ngay sinh: %s, Gioi tinh: %s, Email: %s, Dia chi: %s\n",
            ho_ten[i], cmnd[i], ngay_sinh[i], gioi_tinh[i], email[i], dia_chi[i]);
    }
    }
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
                printf("Thong tin cua doc gia %d da bi xoa. Ban co muon cap nhat thong tin moi cho doc gia nay khong?\n", id);
                printf("Nhap 1. Tiep tuc hoac Nhap ki tu bat ki de Thoat. \n");
                int chon;
                scanf("%d", &chon);
                if (chon == 1) {    
                    printf("Nhap thong tin moi:\n");
                    printf("Nhap ho ten: ");
                    scanf(" %[^\n]%*c", ho_ten[i]);
                    printf("Nhap CMND: ");
                    scanf("%99s", cmnd[i]);
                    printf("Nhap ngay sinh (dd/mm/yyyy): ");
                    scanf("%99s", ngay_sinh[i]);
                    printf("Nhap gioi tinh (Nam/Nu): ");
                    scanf("%99s", gioi_tinh[i]);
                    printf("Nhap email: ");
                    scanf("%99s", email[i]);
                    printf("Nhap dia chi: ");
                    scanf("  %[^\n]%*c", dia_chi[i]);
                    printf("Thong tin doc gia da duoc cap nhat moi!\n");
                    break;
                } else {
                    // Dọn bộ nhớ đệm
                    int _c;
                    while ((_c = getchar()) != '\n' && _c != EOF);
                    // Thoát chỉnh sửa
                    printf("Ket thuc chinh sua! \n");
                    break;
                }
            } else {
            // Cập nhật thông tin độc giả đang hoạt động
            printf(" Ban dang thuc hien chinh sua thong tin cua doc gia %d \n",id);
            printf ("Thong tin hien tai la: Ho ten: %s, CMND: %s, Ngay sinh: %s, Gioi tinh: %s, Email: %s, Dia chi: %s\n",
                        ho_ten[i], cmnd[i], ngay_sinh[i], gioi_tinh[i], email[i], dia_chi[i]);      
            printf("Nhap thong tin ban muon chinh sua:\n");
            printf("Nhap ho ten chinh sua: ");
            scanf(" %[^\n]%*c", ho_ten[i]);
            printf("Nhap CMND chinh sua: ");
            scanf("%99s", cmnd[i]);
            printf("Nhap ngay sinh chinh sua (dd/mm/yyyy): ");
            scanf("%99s", ngay_sinh[i]);
            printf("Nhap gioi tinh chinh sua (Nam/Nu): ");
            scanf("%99s", gioi_tinh[i]);
            printf("Nhap email chinh sua: ");
            scanf("%99s", email[i]);
            printf("Nhap dia chi chinh sua: ");
            scanf("  %[^\n]%*c", dia_chi[i]);
            printf("Thong tin doc gia da duoc cap nhat!\n");
            break;
            }
        }
    }
    if (!found) {
        printf("Khong tim thay doc gia nao co ma %d.\n", id);
    }
}

// Hàm xóa thông tin một độc giả
void xoa_doc_gia(int id) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        if (id == id_doc_gia[i]) { // Tìm theo mã độc giả
            found = 1;
            // Cập nhật các thông tin của độc giả đã xóa thành rỗng để không ảnh hưởng đến id của các độc giả khác
            ho_ten[i][0] = '\0';
            cmnd[i][0] = '\0';
            ngay_sinh[i][0] = '\0';
            gioi_tinh[i][0] = '\0';
            email[i][0] = '\0';
            dia_chi[i][0] = '\0';
            printf("Da xoa thong tin cua doc gia co ma %d (cac truong duoc lam rong).\n", id);
            //printf("Thong tin doc gia da bi xoa!\n");
            break;
        }
    }
    if (!found) {
        printf("Khong tim thay doc gia nao co ma %d.\n", id);
    }
}

// Hàm tìm kiếm độc giả theo CMND
void tim_kiem_theo_cmnd(char *cmnd_can_tim) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        // Bỏ qua độc giả đã bị xóa (CMND rỗng)
        if (cmnd[i][0] == '\0') continue;

        if (strcmp(cmnd_can_tim, cmnd[i]) == 0) {
            printf("Tim thay doc gia co id = %d voi thong tin: Ho ten: %s, CMND: %s, Ngay sinh: %s, Gioi tinh: %s, Email: %s, Dia chi: %s\n",
                id_doc_gia[i], ho_ten[i], cmnd[i], ngay_sinh[i], gioi_tinh[i], email[i], dia_chi[i]);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Khong tim thay doc gia nao co CMND: %s.\n", cmnd_can_tim);
    }
}

// Hàm tìm kiếm độc giả theo họ tên
void tim_kiem_theo_ten_doc_gia(char *ten_can_tim) {
    int found = 0;
    for (int i = 0; i < tong_so_doc_gia; i++) {
        // Bỏ qua độc giả đã bị xóa (họ tên rỗng)
        if (ho_ten[i][0] == '\0') continue;

        if (strstr(ho_ten[i], ten_can_tim) != NULL) { // ten_can_tim xuất hiện trong ho_ten[i]
            printf("Tim thay doc gia co id = %d voi thong tin: Ho ten: %s, CMND: %s, Ngay sinh: %s, Gioi tinh: %s, Email: %s, Dia chi: %s\n",
                id_doc_gia[i], ho_ten[i], cmnd[i], ngay_sinh[i], gioi_tinh[i], email[i], dia_chi[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("Khong tim thay doc gia nao co ten: %s.\n", ten_can_tim);
    }
}

// Hàm quản lý đôc giả
void quan_ly_doc_gia() {
    int choice;
    int id;
    char cmnd[100];
    char ho_ten[100];
    printf("1. Them doc gia\n");
    printf("2. Xem danh sach doc gia\n");
    printf("3. Chinh sua thong tin doc gia\n");
    printf("4. Xoa thong tin doc gia\n");
    printf("5. Tim kiem thong tin doc gia theo CMND\n");
    printf("6. Tim kiem thong tin doc gia theo ten\n");
    printf("0. Quay lai\n");
    printf("Chon chuc nang: ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            them_doc_gia();  // Hàm thêm độc giả
            break;
        case 2:
            danh_sach_doc_gia();  // Hàm in danh sách độc giả
            break;
        case 3:
            printf("Nhap ID doc gia muon chinh sua: ");
            scanf("%d", &id);
            chinh_sua_doc_gia(id);   // Hàm chỉnh sửa thông tin độc giả
            break;
        case 4:
            printf("Nhap ID doc gia muon xoa: ");
            scanf("%d", &id);
            xoa_doc_gia(id);   // Hàm xóa thông tin độc giả
            break;
        case 5:
            printf("Nhap CMND cua doc gia can tim: ");
            scanf("%99s", cmnd);
            tim_kiem_theo_cmnd(cmnd);   // // Hàm tìm kiếm độc giả theo CMND
            break;
        case 6:
            printf("Nhap ten doc gia can tim: ");
            scanf(" %[^\n]%*c", ho_ten);
            tim_kiem_theo_ten_doc_gia(ho_ten);   // // Hàm tìm kiếm độc giả theo họ tên
            break;
        case 0:
            return;
        default:
            printf("Chon khong hop le. Thu lai nhe!\n");
    }
}