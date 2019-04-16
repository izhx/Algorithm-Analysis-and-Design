#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;

// P145 big integer class and big integer multiplication.
// PS: There are some excellent library , such as GMP and LibTomMath.
struct BigInt {
    string s; // tempt
    const static unsigned int DEFAULT_SIZE = 20;
    const static unsigned int KARATSUBA_THRESHOLD = 10;
    unsigned int len; // len of num, must be less than alloc.
    unsigned int alloc; // len of allocated memory space, maximum 4294967295.
    unsigned short neg;  // neg = 1 means negative, 0 means it's 0 or positive.
    short *data;  // low digit at the first, at least one resize at the array end.


    BigInt() : len(0), alloc(DEFAULT_SIZE), neg(0) {
        data = new short[alloc];
        fill(data, data + alloc, 0);
    }

    /**
     * Construct a BigInt object with a specific length.
     * @param size
     */
    explicit BigInt(const unsigned int size) {
        len = 0, neg = 0, alloc = size > DEFAULT_SIZE ? size : DEFAULT_SIZE;
        data = new short[alloc];
        fill(data, data + alloc, 0);
    }

    /**
     * Construct a BigInt object from a int variable.
     * @param num a int number
     */
    explicit BigInt(const long num) {
        int quotient = 0, n = num;
        if (num < 0) neg = 1, n = -num;
        else neg = 0;
        len = 0, alloc = DEFAULT_SIZE;
        data = new short[alloc];
        fill(data, data + alloc, 0);
        do {
            quotient = n / 10;
            data[len] = (short) (n - quotient * 10);
            len++, n = quotient;
        } while (n > 0 && len < alloc);
    }

    /**
     * Construct a BigNum object from a string which expressed a big num,
     * such as '101010101010101010' or '-111111111111111111111'.
     * @param str a str expressed a big num
     */
    explicit BigInt(const char *str) {
        alloc = strlen(str) + 1, len = 0, neg = 0;
        data = new short[alloc];
        if ('-' == str[0]) neg = 1;
        int i = alloc - 2;
        while (i >= neg) {
            data[len++] = (short) (str[i--] - '0');
        }
        fill(data + len, data + alloc, 0);  // make sure every str start by 0.
        this->s = string(str);  // todo
    }

    /**
     * The Copy constructor.
     * @param bigInt
     */
    BigInt(const BigInt &bigInt) : len(bigInt.len), alloc(bigInt.alloc), neg(bigInt.neg) {
        data = new short[alloc];
        memcpy(data, bigInt.data, alloc * sizeof(short));
        this->s = bigInt.s; // todo
    }

    /**
     * Explicitly initializes a 0 with a specific length.
     * @param size  todo
     */
    void resize(const unsigned int size) {
        len = 0, neg = 0;
        if (alloc >= size) return;
        alloc = size;
        delete[] data;
        data = new short[alloc];
        fill(data, data + alloc, 0);
    }

    short operator[](const int index) const {
        if (index >= 0) return data[index];
        else {
            if (-index > len)throw logic_error("Index out of range!");
            else return data[len + index];
        }
    }

    /**
     * Overloaded input operator, should input a string
     * such as '101010101010101010' or '-111111111111111111111'.
     * @param is
     * @param bigInt
     * @return
     */
    friend std::istream &operator>>(istream &is, BigInt &bigInt) {
        string str;
        is >> str;
        if ('-' == str[0]) bigInt.neg = 1;
        if (str.length() > bigInt.alloc) {
            bigInt.alloc = str.length();
            delete[] bigInt.data;
            bigInt.data = new short[bigInt.alloc];
        }
        bigInt.len = 0;
        int i = bigInt.alloc - 1;
        while (i >= bigInt.neg) {
            bigInt.data[bigInt.len++] = (short) (str[i--] - '0');
        }
        return is;
    }

    friend std::ostream &operator<<(ostream &os, const BigInt &bigInt) {
        if (bigInt.len == 0) os << "null";
        else {
            if (bigInt.neg) os << '-';
            int i = bigInt.len - 1;
            while (i >= 0) os << bigInt.data[i--];
        }
        return os;
    }

    BigInt &operator=(const BigInt &bigInt) {
        len = bigInt.len, alloc = bigInt.alloc, neg = bigInt.neg;
        delete[] data;
        data = new short[alloc];
        memcpy(data, bigInt.data, alloc * sizeof(short));
        this->s = bigInt.s; // todo
        return *this;
    }

    BigInt operator+(const BigInt &b) const {
        // sum of a+b, a means this.
        BigInt sum(max(len, b.len) + 2);  // no more than 1 digit will increase.
        short cmp = this->absValueCmpWith(b);
        if (this->neg ^ b.neg) {  // different sign
            if (cmp == 1) {  // |a|>|b|
                absBigSubSmall(*this, b, sum);  // |sum|=|a-b|
                sum.neg = this->neg;  // sign is same to a
            } else if (cmp == -1) {  // |a|<|b|
                absBigSubSmall(b, *this, sum);  // |sum|=|b-a|
                sum.neg = b.neg;  // sign is same to b
            } else {  // |a|=|b|
                sum.len = 1;  // return 0.
            }
        } else {  // same sign |sum|=|a+b| or |b+a|
            if (cmp == -1) absBigAddSmall(b, *this, sum);  // |a|<|b|
            else absBigAddSmall(*this, b, sum); // |a|>=|b|
            sum.neg = b.neg;
        }
        sum.save_str();  // todo
        return sum;
    }

    BigInt operator-(const BigInt &b) const {
        // difference of a-b, a means this.
        BigInt dif(max(len, b.len) + 2);  // no more than 1 digit will increase.
        short cmp = this->absValueCmpWith(b);
        if (this->neg ^ b.neg) {  // different sign |dif| = |a+b| or |b+a|
            if (cmp == -1) absBigAddSmall(b, *this, dif);  // |a|<|b|
            else absBigAddSmall(*this, b, dif); // |a|>=|b|
            dif.neg = this->neg;  // sign is same to a
        } else {   // same sign
            if (cmp == 1) {  // |a|>|b|
                absBigSubSmall(*this, b, dif);  // |dif|=|a-b|
                dif.neg = this->neg;  // sign is same to a
            } else if (cmp == -1) {  // |a|<|b|
                absBigSubSmall(b, *this, dif);  // |dif|=|b-a|
                dif.neg = (this->neg == 1) ? 0 : 1;  // sign is opposite to a
            } else {  // |a|=|b|
                dif.len = 1;  // return 0.
            }
        }
        dif.save_str();  // todo
        return dif;
    }

    BigInt operator*(const BigInt &b) const {
        BigInt product(len + b.len + 1);  // no more than al+bl digits, add a 0
        if ((len == 1 && data[0] == 0) || (b.len == 1 && b.data[0] == 0))
            product.len = 1;  // if this == 0 or b == 0, return 0 obj
        else {
            if (len < KARATSUBA_THRESHOLD && b.len < KARATSUBA_THRESHOLD) {
                absAMulB(*this, b, product);
            } else {
                absKaratsubaMul(*this, b, product);
            }
            product.neg = neg ^ b.neg;
        }
        product.save_str();  // todo
        return product;
    }

    /**
     * Compare absolute values of 2 big int.
     * @param b another big int.
     * @return |a|>|b|: 1, |a|=|b|: 0, |a|<|b|: -1
     */
    short absValueCmpWith(const BigInt &b) const {
        if (len > b.len) return 1;
        else if (len < b.len) return -1;
        else {
            int i = len - 1;
            while (i >= 0) {
                if (data[i] > b.data[i]) return 1;
                else if (data[i] < b.data[i]) return -1;
                else i--;
            }
            return 0;
        }
    }

    void absBigAddSmall(const BigInt &a, const BigInt &b, BigInt &sum) const {
        short carry = 0;
        unsigned int i = 0;
        while (i <= b.len) {
            carry += a.data[i] + b.data[i];
            sum.data[i] = carry % 10;
            carry /= 10;
            i++;
        }
        if (carry == 0) {
            int d = a.len - b.len - 1;
            if (d > 0) {
                memcpy(sum.data + i, a.data + i, d * sizeof(short));
                sum.len = i + d;
            } else {
                sum.len = i;
            }
        } else {
            while (i <= a.len) {
                carry += a.data[i];
                sum.data[i] = carry % 10;
                carry /= 10;
                i++;
            }
            sum.len = i;
        }
        while (sum.data[sum.len - 1] == 0)sum.len--;
    }

    void absBigSubSmall(const BigInt &a, const BigInt &b, BigInt &dif) const {
        short borrow = 0;
        unsigned int i = 0;
        while (i <= b.len) {
            borrow += a.data[i] - b.data[i];
            if (borrow < 0) {
                dif.data[i] = borrow + 10;
                borrow = -1;
            } else {
                dif.data[i] = borrow;
                borrow = 0;
            }  // -4 % 10 = -4, nmd why???
            i++;
        }
        if (borrow == 0) {
            int d = a.len - b.len - 1;
            if (d > 0) {
                memcpy(dif.data + i, a.data + i, d * sizeof(short));
                dif.len = i + d;
            } else {
                dif.len = i;
            }
        } else {
            while (i <= a.len) {
                borrow += a.data[i];
                if (borrow < 0) {
                    dif.data[i] = borrow + 10;
                    borrow = -1;
                } else {
                    dif.data[i] = borrow;
                    borrow = 0;
                }
                i++;
            }
        }
        while (dif.data[dif.len - 1] == 0)dif.len--;
    }


    void absAMulB(const BigInt &a, const BigInt &b, BigInt &product) const {
        short carry = 0;
        unsigned int i = 0, j = 0, k = 0;
        while (i < b.len) {
            k = i;
            while (j < a.len)
                product.data[k++] += a.data[j++] * b.data[i];
            ++i, j = 0;
        }
        if (product.len != 0) throw logic_error("abs mul error");
        i = a.len + b.len;  // no more than al+bl digits
        while (product.len < i) {
            product.data[product.len] += carry;
            carry = product.data[product.len] / 10;
            product.data[product.len] -= carry * 10;
            product.len++;
        }
        while (product.data[product.len - 1] == 0)product.len--;
    }

    void absKaratsubaMul(const BigInt &a, const BigInt &b, BigInt &product) const {
        unsigned int half = (max(a.len, b.len) + 1) / 2;
        BigInt a0 = a.getLower(half), a1 = a.getUpper(half);
        BigInt b0 = b.getLower(half), b1 = b.getUpper(half);
        BigInt c0 = a0 * b0;
        BigInt c2 = a1 * b1;
        BigInt c1 = (a1 + a0) * (b1 + b0) - (c2 + c0);
        product = c2.mul10n(2 * half) + c1.mul10n(half) + c0;
    }

    /**
     * get a obj consists of 1st ~ index th digits of this.
     * index range: 0 ~ index-1.
     * @param index
     * @return
     */
    BigInt getLower(const unsigned int index) const {
        BigInt l(index + 1);
        l.len = index, l.neg = this->neg;
        memcpy(l.data, this->data, index * sizeof(short));
        while (l.data[l.len - 1] == 0)l.len--;
        l.save_str();  // todo
        return l;
    }

    /**
     * get a obj consists of index th ~ last one digits of this.
     * index range: index ~ len-1.
     * @param index
     * @return
     */
    BigInt getUpper(const unsigned int index) const {
        unsigned int length = this->len - index;
        BigInt u(length + 1);
        u.len = length, u.neg = this->neg;
        memcpy(u.data, this->data + index, length * sizeof(short));
        u.save_str(); // todo
        return u;
    }

    BigInt mul10n(const unsigned int n) const {
        unsigned int length = this->len + n;
        BigInt p(length + 1);
        memcpy(p.data + n, this->data, this->len * sizeof(short));
        p.len = length, p.neg = this->neg;
        p.save_str(); // todo
        return p;
    }

//    BigInt operator/(const int &) const;     //重载除法运算符，大数对一个整数进行相除运算
//    BigInt operator^(const int &) const;     //大数的n次方运算
//    int operator%(const int &) const;        //大数对一个int类型的变量进行取模运算
//    bool operator>(const BigInt &T) const;   //大数和另一个大数的大小比较
//    bool operator>(const int &t) const;      //大数和一个int类型的变量的大小比较

    void save_str() {
        this->s.erase();
        if (this->len == 0) this->s = "null";
        else {
            if (this->neg) this->s = '-';
            int i = this->len - 1;
            while (i >= 0) this->s.append(to_string(this->data[i--]));
        }
    }
};


// P147 Strassen matrix multiplication and matrix template struct
template<typename T>
struct Matrix {
    T **data;
    int row, col;

    Matrix() : data(nullptr), row(0), col(0) {}

    Matrix(int init_row, int init_col) {
        row = init_row, col = init_col;
        data = new T *[row];
        for (int i = 0; i < row; ++i)
            data[i] = new T[col];
    }

    Matrix(int init_row, int init_col, T default_value) {
        row = init_row, col = init_col;
        data = new T *[row];
        for (int i = 0; i < row; ++i) {
            data[i] = new T[col];
            fill(data[i], data[i] + col, default_value);
        }
    }

    Matrix(const Matrix &m) {
        row = m.row, col = m.col;
        data = new T *[row];
        for (int i = 0; i < row; ++i) {
            data[i] = new T[col];
            memcpy(data[i], m.data[i], col * sizeof(T));
        }
    }

    ~Matrix() {
        for (int i = 0; i < row; ++i)
            delete[] data[i];
        delete[] data;
    }

    friend std::ostream &operator<<(std::ostream &os, const Matrix<T> &m) {
        for (int i = 0; i < m.row; ++i) {
            for (int j = 0; j < m.col; ++j)
                os << '\t' << m[i][j];
            os << endl;
        }
        return os;
    }

    T *&operator[](int i) const { return data[i]; }

    Matrix &operator=(const Matrix &m) {
        for (int i = 0; i < row; ++i)
            delete[] data[i];
        delete[] data;
        row = m.row, col = m.col;
        data = new T *[row];
        for (int i = 0; i < row; ++i) {
            data[i] = new T[col];
            memcpy(data[i], m.data[i], col * sizeof(T));
        }
        return *this;
    }

    Matrix operator+(const Matrix &m) const {
        if (row != m.row || col != m.col)
            throw logic_error("Matrix shape doesn't match!");
        Matrix r(row, col);
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j)
                r[i][j] = this->data[i][j] + m.data[i][j];
        return r;
    }

    Matrix operator-(const Matrix &m) const {
        if (row != m.row || col != m.col)
            throw logic_error("Matrix shape doesn't match!");
        Matrix r(row, col);
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j)
                r[i][j] = data[i][j] - m.data[i][j];
        return r;
    }

    Matrix operator*(const Matrix &m) const {
        if (row != m.col)
            throw logic_error("Matrix A.row != Matrix B.col!");
        if (row != col || m.row != m.col || ((unsigned) row & (unsigned) (row - 1)) != 0)
            return this->mul(m);
        else {
            return Matrix(row, col).strassen(*this, m);
        }
    }

    Matrix strassen(const Matrix &A, const Matrix &B) const {
        int n = A.row, hn = n / 2;  // n, half n
        if (n < 3)
            return A.mul(B);
        Matrix a = A.sub_mat(hn, hn, 0, 0), b = A.sub_mat(hn, hn, 0, hn),
                c = A.sub_mat(hn, hn, hn, 0), d = A.sub_mat(hn, hn, hn, hn),
                e = B.sub_mat(hn, hn, 0, 0), f = B.sub_mat(hn, hn, 0, hn),
                g = B.sub_mat(hn, hn, hn, 0), h = B.sub_mat(hn, hn, hn, hn);
        Matrix p1 = a * (f - h);
        Matrix p2 = (a + b) * h;
        Matrix p3 = (c + d) * e;
        Matrix p4 = d * (g - e);
        Matrix p5 = (a + d) * (e + h);
        Matrix p6 = (b - d) * (g + h);
        Matrix p7 = (a - c) * (e + f);
        Matrix AB11 = p4 + p5 + p6 - p2;
        Matrix AB12 = p1 + p2;
        Matrix AB21 = p3 + p4;
        Matrix AB22 = p1 + p5 - p3 - p7;
        for (int i = 0; i < hn; ++i) {
            for (int j = 0; j < hn; ++j) {
                this->data[i][j] = AB11[i][j];
                this->data[i][j + hn] = AB12[i][j];
                this->data[i + hn][j] = AB21[i][j];
                this->data[i + hn][j + hn] = AB22[i][j];
            }
        }
        return *this;
    }

    /**
     * plain matrix multiplication
     * @param A matrix
     * @param m matrix
     * @return AB
     */
    Matrix mul(const Matrix &m) const {
        if (row != m.col)
            throw logic_error("Matrix row != m.col!");
        Matrix c(row, m.col);
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < m.col; ++j) {
                c[i][j] = 0;  // TODO: how to define generic resize element
                for (int k = 0; k < col; ++k)
                    c[i][j] += this->data[i][k] * m.data[k][j];
            }
        return c;
    }

    Matrix sub_mat(const int &nr, const int &nc, const int &r, const int &c) const {
        Matrix sub(nr, nc);
        for (int i = 0; i < nr; ++i)
            for (int j = 0; j < nc; ++j)
                sub[i][j] = this->data[i + r][j + c];
        return sub;
    }

};


int main() {
    Matrix<int> m1(8, 8, 1), m2(8, 8, 2);
    Matrix<int> mm = m1 * m2;
    cout << mm << endl;
    BigInt b1("-1234567890123456789"), b2("123456789123456789"),
            bb = b1 * b2;
    cout << bb << endl;
    return 0;
}
/*
 *  1234567890123456789 * 123456789123456789 = 
 *  152415787669562576267337309750190521
 */