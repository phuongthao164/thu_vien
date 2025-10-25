//  File book.h sẽ chứa các khai báo hàm liên quan đến việc quản lý sách, bao gồm các chức năng như thêm sách, chỉnh sửa sách, xóa sách, tìm kiếm sách, và hiển thị danh sách sách.

#ifndef SACH_H
#define SACH_H

// Hàm hiển thị danh sách sách trong thư viện
void danh_sach_sach();

// Hàm thêm một quyển sách vào thư viện
void them_sach();

// Hàm chỉnh sửa thông tin sách
void chinh_sua_sach(char *isbn_sach);

// khai báo hàm tìm index theo ISBN (chuỗi)
int find_index_by_isbn(char *isbn_sach);

// Hàm xóa thông tin sách
void xoa_sach(char *isbn_sach);

// Hàm tìm kiếm sách theo ISBN
void tim_kiem_theo_isbn(char *isbn_sach);

// Hàm tìm kiếm sách theo tên sách
void tim_kiem_theo_ten_sach(char *ten_can_tim);

// Hàm quản lý sách
void quan_ly_sach();

// Hàm đọc dữ liệu sách từ file
void doc_du_lieu_sach_tu_file(const char *filename);

// Hàm đọc dữ liệu độc giả từ file
void doc_du_lieu_doc_gia_tu_file(const char *filename);

#endif // BOOK_H
