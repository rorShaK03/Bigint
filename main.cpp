#include <iostream>
#include <vector>
#include <string>

class bigint
{
public:
    bigint() : _digits(1, 0), _negative(false){};
    bigint(const bigint& other) = default;
    bigint& operator =(const bigint& other) = default;
    ~bigint() = default;
    friend std::ostream& operator << (std::ostream& out, const bigint& num);
    friend std::istream& operator >>(std::istream& in, bigint& num);




    bigint(int number): _digits(), _negative(number < 0)
    {
        number = std::abs(number);

        do {
            _digits.push_back(number % 10);
            number /= 10;
        } while(number > 0);
    }

    bigint(const std::string &str)
    {
        for(int i = str.size() - 1; i >= 1; i--)
            _digits.push_back(str[i] - '0');
        if(str[0] == '-')
            _negative = true;
        else {
            _negative = false;
            _digits.push_back(str[0] - '0');
        }
        clean_leading_zeros();
    }

    bigint(const std::vector<int> digits, bool neg): _digits(digits), _negative(neg){clean_leading_zeros();}

    void clean_leading_zeros()
    {
        size_t i = _digits.size() - 1;
        while(i > 0 && _digits[i] == 0)
            i--;
        _digits.resize(i + 1);
    }

    explicit operator bool () const {return (_digits.size() > 1) || (_digits[0] != 0);}

    bool get_sign() const {return _negative;}

    std::string to_string () const
    {
        std::string res = "";
        for(int dig : _digits)
            res = static_cast<char>(dig + '0') + res;
        if(_negative)
            res = '-' + res;
        return res;
    }


    bigint& operator +=(const bigint& other)
    {
        int c = 0;
        size_t i = 0;
        while(_digits.size() < other._digits.size())
            _digits.push_back(0);
        if(_negative == other._negative) {
            for(; i < _digits.size(); i++) {
                int full_sum;
                if(i < other._digits.size())
                    full_sum = (_digits[i] + other._digits[i] + c);
                else
                    full_sum = (_digits[i] + c);
                _digits[i] = full_sum % 10;
                c = full_sum / 10;
            }
            if(c != 0)
                _digits.push_back(c);
        }
        else {
            // Subtracting other from this
            for(; i < _digits.size(); i++) {
                int full_sub;
                if(i < other._digits.size())
                    full_sub = (_digits[i] - other._digits[i] - c);
                else
                    full_sub = (_digits[i] - c);
                _digits[i] = (full_sub + 10) % 10;
                c = (full_sub < 0);
            }
            // If no decimal place left for one-taking, change the sign and subtract from 10^n
            if(c != 0) {
                _negative = !_negative;
                c = 0;
                for(int &dig : _digits) {
                    int full_sub = -dig - c;
                    dig = (full_sub + 10) % 10;
                    c = (full_sub < 0);
                }
            }
        }
        clean_leading_zeros();
        return *this;
    }
    bigint& operator -=(const bigint& other)
    {
        bigint temp(other);
        temp._negative = !temp._negative;
        return operator +=(temp);
    }
    bigint& operator *=(const bigint& other)
    {
        bigint res = 0;
        int dec_exp = 0;
        for(int i = 0; i < other._digits.size(); i++) {
            int c = 0;
            std::vector<int> cur_digits;
            for(int j = 0; j < dec_exp; j++)
                cur_digits.push_back(0);
            dec_exp++;
            for(int j = 0; j < _digits.size(); j++) {

                cur_digits.push_back((_digits[j] * other._digits[i] + c) % 10);
                c = (_digits[j] * other._digits[i] + c) / 10;
            }
            if(c != 0)
                cur_digits.push_back(c);
            res += bigint(cur_digits, false);
        }
        this->_digits = res._digits;
        this->_negative = _negative ^ other._negative;
        return *this;
    }
    bigint& operator ++() {return operator +=(1);}
    bigint& operator --() {return operator -=(1);}
    const bigint operator ++(int)
    {
        bigint temp(*this);
        operator +=(1);
        return temp;
    }
    const bigint operator --(int)
    {
        bigint temp(*this);
        operator -=(1);
        return temp;
    }
    bigint operator -() const
    {
        bigint temp(*this);
        temp._negative = !temp._negative;
        return temp;
    }
    bigint operator +() const {return bigint(*this);}


private:
    std::vector<int> _digits;
    bool _negative;

};

bigint operator +(const bigint& lhs, const bigint& rhs){return bigint(lhs) += rhs;}
bigint operator -(const bigint& lhs, const bigint& rhs){return bigint(lhs) -= rhs;}
bigint operator *(const bigint& lhs, const bigint& rhs){return bigint(lhs) *= rhs;}
bool operator ==(const bigint& lhs, const bigint& rhs) {return !static_cast<bool>(lhs - rhs);}
bool operator !=(const bigint& lhs, const bigint& rhs) {return !(lhs == rhs);}
bool operator <(const bigint& lhs, const bigint& rhs) {return ((lhs - rhs).get_sign()) && (lhs != rhs);}
bool operator <=(const bigint& lhs, const bigint& rhs) {return (lhs < rhs) || (lhs == rhs);}
bool operator >(const bigint& lhs, const bigint& rhs) {return !(lhs <= rhs);}
bool operator >=(const bigint& lhs, const bigint& rhs) {return !(lhs < rhs);}


std::istream& operator >> (std::istream& in, bigint& num)
{
    std::string str;
    in >> str;
    num = bigint(str);
    return in;
}
std::ostream& operator << (std::ostream& out, const bigint& num)
{
    out << num.to_string();
    return out;
}

int main() {
    while(true) {
        int x, y;
        std::cin >> x >> y;
        bigint a = x;
        bigint b = y;
        a *= b;
        std::cout << a << '\n';
    }
    return 0;
}
