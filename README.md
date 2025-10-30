# Quản Lý Thư Viện

Đây là một hệ thống quản lý thư viện được viết bằng ngôn ngữ lập trình C. Hệ thống giúp quản lý các chức năng liên quan đến độc giả, sách, phiếu mượn, phiếu trả và thống kê.

## Cấu Trúc Dự Án

- `main.c`: File chính, nơi hiển thị menu và gọi các chức năng quản lý thư viện.
- `doc_gia.c`: Quản lý thông tin độc giả trong thư viện.
- `sach.c`: Quản lý thông tin sách trong thư viện.
- `phieu_muon.c`: Quản lý phiếu mượn sách.
- `phieu_tra.c`: Quản lý phiếu trả sách.
- `thong_ke.c`: Thực hiện các chức năng thống kê.
- `doc_gia.h`: Header file cho các hàm quản lý độc giả.
- `phieu_muon.h`: Header file cho các hàm quản lý phiếu mượn.
- `phieu_tra.h`: Header file cho các hàm quản lý phiếu trả.
- `sach.h`: Header file cho các hàm quản lý sách.
- `thong_ke.h`: Header file cho các hàm thống kê.

## Các Chức Năng Chính

### Quản Lý Độc Giả
1. **Thêm Độc Giả**: Thêm thông tin độc giả mới vào hệ thống.
2. **Xem Danh Sách Độc Giả**: Hiển thị danh sách các độc giả trong thư viện.
3. **Chỉnh Sửa Thông Tin Độc Giả**: Cập nhật thông tin của một độc giả.
4. **Xóa Độc Giả**: Xóa thông tin của một độc giả khỏi hệ thống.
5. **Tìm Kiếm Độc Giả**: Tìm kiếm độc giả theo CMND hoặc tên.

### Quản Lý Sách
1. **Thêm Sách**: Thêm sách mới vào thư viện.
2. **Xem Danh Sách Sách**: Hiển thị danh sách các sách trong thư viện.
3. **Chỉnh Sửa Thông Tin Sách**: Cập nhật thông tin sách.
4. **Xóa Sách**: Xóa sách khỏi thư viện.
5. **Tìm Kiếm Sách**: Tìm kiếm sách theo ISBN hoặc tên sách.

### Quản Lý Phiếu Mượn
1. **Lập Phiếu Mượn**: Lập phiếu mượn sách cho độc giả.
2. **Xem Danh Sách Phiếu Mượn**: Hiển thị danh sách các phiếu mượn hiện tại.

### Quản Lý Phiếu Trả
1. **Lập Phiếu Trả**: Lập phiếu trả sách khi độc giả trả lại sách.
2. **Xem Danh Sách Phiếu Trả**: Hiển thị danh sách các phiếu trả sách.

### Thống Kê
1. **Số Lượng Sách**: Thống kê tổng số sách trong thư viện.
2. **Thống Kê Theo Thể Loại**: Thống kê số sách theo thể loại.
3. **Số Lượng Độc Giả**: Thống kê tổng số lượng độc giả.
4. **Thống Kê Độc Giả Theo Giới Tính**: Thống kê số lượng độc giả theo giới tính.
5. **Sách Đang Được Mượn**: Thống kê các sách đang được mượn.
6. **Độc Giả Trễ Hạn**: Thống kê các độc giả trả sách trễ.

## Các Bước Cài Đặt và Chạy
**Bước 1** Biên Dịch Các File

- Sử dụng câu lệnh sau để biên dịch các file mã nguồn thành một file thực thi thu_vien.exe:

 `gcc main_phu.c doc_gia.c sach.c phieu_muon.c phieu_tra.c thong_ke.c -o thu_vien.exe -finput-charset=UTF-8 -fexec-charset=UTF-8`

**Bước 2** Thiết Lập Môi Trường Tiếng Việt

- Trước khi chạy chương trình, bạn cần thiết lập mã hóa tiếng Việt trên hệ điều hành Windows bằng câu lệnh sau:

 `chcp 65001`

**Bước 3** Chạy Chương Trình

- Sau khi biên dịch xong và thiết lập mã hóa, bạn có thể chạy chương trình bằng câu lệnh:

 `thu_vien.exe`

- Chương trình sẽ hiển thị menu cho phép bạn lựa chọn các chức năng để quản lý thư viện.