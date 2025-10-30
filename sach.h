// 2. Hàm menu quản lý sách
void quan_ly_sach();

// 2a.Hàm hiển thị danh sách các loại sách
void danh_sach_sach();

// 2b. Hàm thêm sách
void them_sach();

// 2c. Hàm chỉnh sửa thông tin sách
void chinh_sua_sach(char *isbn_sach);

// 2d. Hàm xóa thông tin sách
void xoa_sach(char *isbn_sach);

// 2e. Hàm tìm kiếm sách theo ISBN
void tim_kiem_theo_isbn(char *isbn_sach);

// 2f. Hàm tìm kiếm sách theo tên sách
void tim_kiem_theo_ten_sach(char *ten_can_tim);

// Hàm bổ sung - tìm index theo ISBN 
int find_index_by_isbn(char *isbn_sach);




// ----------------------------làm xong hết thì xóa -------------
// Hàm đọc dữ liệu sách từ file
void doc_du_lieu_sach_tu_file(const char *filename);

// Hàm đọc dữ liệu độc giả từ file
void doc_du_lieu_doc_gia_tu_file(const char *filename);
