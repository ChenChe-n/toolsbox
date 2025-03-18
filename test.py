# 设置整数最大长度
import sys
sys.set_int_max_str_digits(10000)
n = 1
for i in range(500):
    n*=2
print(n)