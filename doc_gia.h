#ifndef DOC_GIA_H
#define DOC_GIA_H

// Hàm thêm độc giả
void them_doc_gia();

// Hàm hiển thị danh sách độc giả
void danh_sach_doc_gia();

 //Hàm chỉnh sửa thông tin một độc giả
void chinh_sua_doc_gia(int id);

// Hàm xóa thông tin một độc giả
void xoa_doc_gia(int id);

// Hàm tìm kiếm độc giả theo CMND
void tim_kiem_theo_cmnd(char *cmnd_can_tim);

// Hàm tìm kiếm độc giả theo họ tên
void tim_kiem_theo_ten_doc_gia(char *ten_can_tim);

// Hàm quản lý đôc giả
void quan_ly_doc_gia();

#endif // DOC_GIA_H
