// Quản lý phiếu mượn sách

#ifndef PHIEU_MUON_H
#define PHIEU_MUON_H

void ngay_muon(int index);

void ngay_tra_dk(int index);

void ngay_tra_tt(int index);

long kiem_tra_tra_sach(const char *ngay_tra_thuc_te, const char *ngay_tra_du_kien);

void lap_phieu_muon();

void danh_sach_phieu_muon();

void doc_du_lieu_phieu_muon_tu_file(const char *filename);

void quan_ly_phieu_muon();

#endif // PHIEU_MUON_H
