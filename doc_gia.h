// 1. Hàm menu quản lý độc giả
void quan_ly_doc_gia();

// 1a. Hàm hiển thị danh sách độc giả
void danh_sach_doc_gia();

// 1b. Hàm thêm độc giả
void them_doc_gia();

// 1c. Hàm chỉnh sửa thông tin một độc giả
void chinh_sua_doc_gia(int id);

// 1d. Hàm xóa thông tin một độc giả
void xoa_doc_gia(int id);

// 1e. Hàm tìm kiếm độc giả theo CMND
void tim_kiem_theo_cmnd(char *cmnd_can_tim);

// 1f. Hàm tìm kiếm độc giả theo họ tên
void tim_kiem_theo_ten_doc_gia(char *ten_can_tim);

// Hàm bổ sung
int read_int_safe(const char *prompt, int *out);

// Hàm kiểm tra CMND hợp lệ (12 số)
int kiem_tra_cmnd_hop_le(char *cmnd_check);

// Hàm kiểm tra CMND trùng lặp
int kiem_tra_cmnd_trung(char *cmnd_check, int skip_index);

