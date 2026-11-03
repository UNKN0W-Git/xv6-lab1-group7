# Quy ước kỹ thuật – Lab 1: Sysinfo

## 1. Mục đích

Tài liệu thống nhất cấu trúc dữ liệu, giao diện hàm và quy tắc triển khai system call `sysinfo` cho nhóm 7.

Cả 3 thành viên cần đọc tài liệu trước khi lập trình.

## 2. Cấu trúc dữ liệu

Sử dụng chung file `kernel/sysinfo.h`, gồm:

- `struct sysinfo_file`: Thông tin một đối tượng file.
- `struct sysinfo_fd`: Thông tin file descriptor của tiến trình.
- `struct sysinfo_process`: Thông tin tiến trình.
- `struct sysinfo`: Kết quả thống kê toàn hệ thống.

Không tự ý đổi tên trường hoặc kiểu dữ liệu khi chưa thống nhất với nhóm.

## 3. Giao diện hàm

| Hàm                                          | Người phụ trách   | File               |
| -------------------------------------------- | ----------------- | ------------------ |
| `collect_file_info(struct sysinfo *info)`    | Mai Khắc Hoàng Vũ | `kernel/file.c`    |
| `collect_process_info(struct sysinfo *info)` | Đặng Quang Sang   | `kernel/proc.c`    |
| `sys_sysinfo(void)`                          | Hồ Sỹ Thi         | `kernel/sysproc.c` |

Hai hàm `collect_*` trả về `int`: `0` nếu thành công, `-1` nếu thất bại.

## 4. Quy ước dữ liệu

- `file_index`: Chỉ số của đối tượng trong `ftable.file[]`, không phải file descriptor.
- `open_file_count` toàn hệ thống: Số đối tượng `struct file` có `ref > 0`.
- `pids[]`: Mỗi PID chỉ xuất hiện một lần trong danh sách tiến trình tham chiếu đến cùng một đối tượng file.
- `open_file_count` của tiến trình: Số ô `ofile[]` khác `0`.
- `scheduling_count`: Tăng khi `scheduler()` chọn tiến trình chuyển sang `RUNNING`.
- Mảng kết quả được điền từ vị trí `0` đến `count - 1`; các phần tử chưa sử dụng được khởi tạo bằng `0`.
- `parent_pid = 0` nếu tiến trình không có cha.
- Không vượt quá các giới hạn `NFILE`, `NPROC`, `NOFILE`.

## 5. Quy ước bộ nhớ và đồng bộ

- Không cấp phát toàn bộ `struct sysinfo` trên kernel stack vì kích thước lớn.
- Các hàm thu thập dữ liệu sử dụng con trỏ `struct sysinfo *info` do hàm điều phối cung cấp.
- Truy cập bảng file và bảng tiến trình phải sử dụng khóa phù hợp.
- Tránh giữ đồng thời nhiều khóa khi chưa xác định thứ tự khóa an toàn.
- Không giữ các khóa bảng file hoặc tiến trình khi gọi `copyout()`.
- Nhóm cần kiểm thử các trường hợp dữ liệu thay đổi đồng thời và giải thích giới hạn nhất quán của kết quả.

## 6. Quy tắc phối hợp Git

- Sử dụng chung mã nguồn nền xv6-labs-2023.
- `main` là nhánh tích hợp.
- Mỗi thành viên lập trình trên nhánh `feature/` riêng.
- Không tự ý chỉnh sửa `kernel/sysinfo.h` hoặc giao diện hàm chung.
- Tạo Pull Request trước khi merge.
- Sang phụ trách tổng hợp cuối cùng, với sự hỗ trợ kiểm thử của cả nhóm.

## 7. Kiểm thử

Nhóm cần kiểm thử:

- Mở và đóng file.
- `dup()` và thay đổi reference count.
- `fork()` và chia sẻ file giữa tiến trình cha/con.
- Nhiều tiến trình đang hoạt động.
- Số lần được lập lịch.
- Gọi `sysinfo()` nhiều lần.
- Con trỏ user không hợp lệ.
- Các trường hợp đạt giới hạn dữ liệu.

## 8. Quy định thay đổi

Mọi thay đổi đối với cấu trúc `sysinfo.h`, tên hàm, kiểu dữ liệu hoặc ý nghĩa bộ đếm phải được cả nhóm thống nhất trước khi merge.
