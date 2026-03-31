#ifndef SRC_HPP
#define SRC_HPP

#include "fraction.hpp"
#include <cmath>
#include <algorithm>

class matrix {
private:

    // m行n列的矩阵，用动态二维数组存储，每个元素是分数类实例
    int m, n;
    fraction **data;

    //****************************
    // TODO: 你可以在此添加任何需要的类成员和函数。
    //****************************

public:
    // 友元声明，允许resistive_network访问私有成员
    friend class resistive_network;

    //****************************
    // TODO: 你可以在此添加任何需要的类成员和函数。
    //****************************

    // 默认构造函数
    matrix() {
        m = n = 0;
        data = nullptr;
    }

    // TODO: 构造函数，构建 m_*n_ 的矩阵，矩阵元素设为0。
    matrix(int m_, int n_) {
        if (m_ <= 0 || n_ <= 0) {
            throw matrix_error();
        }
        m = m_;
        n = n_;
        data = new fraction*[m];
        for (int i = 0; i < m; i++) {
            data[i] = new fraction[n];
            for (int j = 0; j < n; j++) {
                data[i][j] = fraction(0);
            }
        }
    }

    // TODO: 拷贝构造函数，构建与 obj 完全相同的矩阵。
    matrix(const matrix &obj) {
        m = obj.m;
        n = obj.n;
        if (m == 0 || n == 0) {
            data = nullptr;
            return;
        }
        data = new fraction*[m];
        for (int i = 0; i < m; i++) {
            data[i] = new fraction[n];
            for (int j = 0; j < n; j++) {
                data[i][j] = obj.data[i][j];
            }
        }
    }

    // TODO: 移动拷贝构造函数。
    matrix(matrix &&obj) noexcept {
        m = obj.m;
        n = obj.n;
        data = obj.data;
        obj.m = 0;
        obj.n = 0;
        obj.data = nullptr;
    }

    // TODO: 析构函数。
    ~matrix() {
        if (data != nullptr) {
            for (int i = 0; i < m; i++) {
                delete[] data[i];
            }
            delete[] data;
        }
    }

    // TODO: 重载赋值号。
    matrix &operator=(const matrix &obj) {
        if (this == &obj) {
            return *this;
        }
        // 释放现有内存
        if (data != nullptr) {
            for (int i = 0; i < m; i++) {
                delete[] data[i];
            }
            delete[] data;
        }
        // 拷贝新数据
        m = obj.m;
        n = obj.n;
        if (m == 0 || n == 0) {
            data = nullptr;
            return *this;
        }
        data = new fraction*[m];
        for (int i = 0; i < m; i++) {
            data[i] = new fraction[n];
            for (int j = 0; j < n; j++) {
                data[i][j] = obj.data[i][j];
            }
        }
        return *this;
    }

    // TODO: 重载括号，返回矩阵的第i行(1-based)、第j列(0-based)的元素的引用。如果 i、j 不合法，抛出 matrix_error 错误。
    fraction &operator()(int i, int j) {
        if (i < 1 || i > m || j < 0 || j >= n) {
            throw matrix_error();
        }
        return data[i-1][j];
    }

    // TODO: 重载乘号，返回矩阵乘法 lhs * rhs 的结果。如果 lhs 的列数与 rhs 的行数不相等，抛出 matrix_error 错误。
    friend matrix operator*(const matrix &lhs, const matrix &rhs) {
        if (lhs.n != rhs.m) {
            throw matrix_error();
        }
        matrix result(lhs.m, rhs.n);
        for (int i = 0; i < lhs.m; i++) {
            for (int j = 0; j < rhs.n; j++) {
                fraction sum(0);
                for (int k = 0; k < lhs.n; k++) {
                    sum = sum + lhs.data[i][k] * rhs.data[k][j];
                }
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    // TODO: 返回矩阵的转置。若矩阵为空，抛出 matrix_error 错误。
    matrix transposition() {
        if (m == 0 || n == 0 || data == nullptr) {
            throw matrix_error();
        }
        matrix result(n, m);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result.data[j][i] = data[i][j];
            }
        }
        return result;
    }

    // TODO: 返回矩阵的行列式。建议用高斯消元实现。若矩阵不是方阵或为空，抛出 matrix_error 错误。
    fraction determination() {
        if (m != n || m == 0 || data == nullptr) {
            throw matrix_error();
        }

        // 创建临时矩阵进行高斯消元
        matrix temp(*this);
        fraction det(1);

        // 高斯消元
        for (int i = 0; i < n; i++) {
            // 找到第i列中绝对值最大的元素作为主元
            int pivot = i;
            for (int j = i + 1; j < n; j++) {
                // 简单比较：如果当前行为0，交换
                if (temp.data[pivot][i] == fraction(0) && !(temp.data[j][i] == fraction(0))) {
                    pivot = j;
                }
            }

            // 如果主元为0，行列式为0
            if (temp.data[pivot][i] == fraction(0)) {
                return fraction(0);
            }

            // 交换行
            if (pivot != i) {
                std::swap(temp.data[i], temp.data[pivot]);
                det = det * fraction(-1);
            }

            // 消元
            for (int j = i + 1; j < n; j++) {
                fraction factor = temp.data[j][i] / temp.data[i][i];
                for (int k = i; k < n; k++) {
                    temp.data[j][k] = temp.data[j][k] - factor * temp.data[i][k];
                }
            }

            // 累乘对角线元素
            det = det * temp.data[i][i];
        }

        return det;
    }
};

class resistive_network {
private:

    // 节点数量 和 接线数量
    int interface_size, connection_size;

    // 矩阵A 和 矩阵C
    matrix adjacency, conduction;

    //****************************
    // TODO: 你可以在此添加任何需要的类成员和函数。
    //****************************

    // 辅助函数：计算M矩阵去除第i行第j列后的矩阵的行列式
    fraction getMinor(const matrix &M, int row, int col, int size) {
        matrix minor(size - 1, size - 1);
        int mi = 0;
        for (int i = 1; i <= size; i++) {
            if (i == row) continue;
            int mj = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                minor(mi + 1, mj) = M.data[i - 1][j];
                mj++;
            }
            mi++;
        }
        return minor.determination();
    }

    // 辅助函数：计算M矩阵去除第i、j行和第i、j列后的矩阵的行列式
    fraction getDoubleMinor(const matrix &M, int row1, int row2, int col1, int col2, int size) {
        matrix minor(size - 2, size - 2);
        int mi = 0;
        for (int i = 1; i <= size; i++) {
            if (i == row1 || i == row2) continue;
            int mj = 0;
            for (int j = 0; j < size; j++) {
                if (j == col1 || j == col2) continue;
                minor(mi + 1, mj) = M.data[i - 1][j];
                mj++;
            }
            mi++;
        }
        return minor.determination();
    }

public:

    //****************************
    // TODO: 你可以在此添加任何需要的类成员和函数。
    //****************************

    // TODO: 设置电阻网络，构建矩阵A和C。节点数量为interface_size_，接线数量为connection_size_。
    //       对于 1<=i<=connection_size_，从节点from[i-1]到节点to[i-1]有接线，对应电阻为resistance[i-1]。
    //       保证接线使得电阻网络联通，from[i-1] < to[i-1]，resitance[i-1] > 0，均合法。
    resistive_network(int interface_size_, int connection_size_, int from[], int to[], fraction resistance[]) {
        interface_size = interface_size_;
        connection_size = connection_size_;

        // 构建矩阵A (m×n)
        adjacency = matrix(connection_size, interface_size);
        for (int i = 1; i <= connection_size; i++) {
            for (int j = 0; j < interface_size; j++) {
                if (j + 1 == from[i - 1]) {
                    adjacency(i, j) = fraction(1);
                } else if (j + 1 == to[i - 1]) {
                    adjacency(i, j) = fraction(-1);
                } else {
                    adjacency(i, j) = fraction(0);
                }
            }
        }

        // 构建矩阵C (m×m)
        conduction = matrix(connection_size, connection_size);
        for (int i = 1; i <= connection_size; i++) {
            for (int j = 0; j < connection_size; j++) {
                if (i - 1 == j) {
                    conduction(i, j) = fraction(1) / resistance[i - 1];
                } else {
                    conduction(i, j) = fraction(0);
                }
            }
        }
    }

    ~resistive_network() = default;

    // TODO: 返回节点 interface_id1 和 interface_id2 (1-based)之间的等效电阻。
    //       保证 interface_id1 <= interface_id2 均合法。
    fraction get_equivalent_resistance(int interface_id1, int interface_id2) {
        // 计算 M = A^T * C * A
        matrix AT = adjacency.transposition();
        matrix temp = conduction * adjacency;
        matrix M = AT * temp;

        // R_ij = det(M_ij) / det(M_i)
        // M_i: 去除第i行第i列
        // M_ij: 去除第i行第j列、第j行第i列

        fraction det_Mi = getMinor(M, interface_id1, interface_id1 - 1, interface_size);
        fraction det_Mij = getDoubleMinor(M, interface_id1, interface_id2, interface_id1 - 1, interface_id2 - 1, interface_size);

        return det_Mij / det_Mi;
    }

    // TODO: 在给定节点电流I的前提下，返回节点id(1-based)的电压。认为节点interface_size(1-based)的电压为0。
    //       对于 1<=i<=interface_size，节点i(1-based)对应电流为 current[i-1]。
    //       保证 current 使得电阻网络有解，id < interface_size 合法。
    fraction get_voltage(int id, fraction current[]) {
        // 计算 M = A^T * C * A
        matrix AT = adjacency.transposition();
        matrix temp = conduction * adjacency;
        matrix M = AT * temp;

        // u_i = det(M_i^I) / det(M_n)
        // M_n: 去除第n行第n列
        // M_i^I: 将A^T*C*A的第i列替换成I，然后去除第n行第n列

        fraction det_Mn = getMinor(M, interface_size, interface_size - 1, interface_size);

        // 创建M的副本，替换第i列为current
        matrix M_copy(M);
        for (int j = 1; j <= interface_size; j++) {
            M_copy(j, id - 1) = current[j - 1];
        }

        fraction det_MiI = getMinor(M_copy, interface_size, interface_size - 1, interface_size);

        return det_MiI / det_Mn;
    }


    // TODO: 在给定节点电压U的前提下，返回电阻网络的功率。
    //       对于 1<=i<=interface_size，节点i(1-based)对应电压为 voltage[i-1]。
    //       保证 voltage 合法。
    fraction get_power(fraction voltage[]) {
        // P = Σ(u_w[i]^2 / r[i])
        // u_w = A * U^T

        // 创建电压向量矩阵 (n×1)
        matrix U(interface_size, 1);
        for (int i = 1; i <= interface_size; i++) {
            U(i, 0) = voltage[i - 1];
        }

        // 计算 u_w = A * U
        matrix uw = adjacency * U;

        // 计算功率
        fraction power(0);
        for (int i = 1; i <= connection_size; i++) {
            // 从C矩阵中获取1/r[i]
            fraction conductance = conduction(i, i - 1);
            // r[i] = 1 / conductance
            fraction resistance = fraction(1) / conductance;

            fraction uwi = uw(i, 0);
            power = power + (uwi * uwi) / resistance;
        }

        return power;
    }
};


#endif //SRC_HPP
