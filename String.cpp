
#include "String.h"


size_t String::my_strlen(const char* str) const {
    size_t len = 0;
    while (str[len] != '\0') ++len;
    return len;
}

void String::my_strcpy(char* dest, const char* src) {
    size_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        ++i;
    }
    dest[i] = '\0';
}


String::String() {
    data = new char[1];
    data[0] = '\0';
}

String::String(const char* str) {
    if (str) {
        size_t len = my_strlen(str);
        data = new char[len + 1];
        my_strcpy(data, str);
    } else {
        data = new char[1];
        data[0] = '\0';
    }
}

String::String(const String& other) {
    size_t len = my_strlen(other.data);
    data = new char[len + 1];
    my_strcpy(data, other.data);
}

String::~String() {
    delete[] data;
}


String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] data;
        size_t len = my_strlen(other.data);
        data = new char[len + 1];
        my_strcpy(data, other.data);
    }
    return *this;
}


char String::at(int index) const {
    size_t len = size();
    if (index < 0 || (size_t)index >= len)
        return '\0';
    return data[index];
}

bool String::operator==(const String& other) const {
    size_t i = 0;
    while (data[i] != '\0' && other.data[i] != '\0') {
        if (data[i] != other.data[i]) return false;
        ++i;
    }
    return data[i] == other.data[i];
}

size_t String::size() const {
    return my_strlen(data);
}


String String::operator+(const String& other) const {
    size_t len1 = my_strlen(data);
    size_t len2 = my_strlen(other.data);
    char* new_data = new char[len1 + len2 + 1];

    for (size_t i = 0; i < len1; ++i) new_data[i] = data[i];
    for (size_t i = 0; i < len2; ++i) new_data[len1 + i] = other.data[i];
    new_data[len1 + len2] = '\0';

    String result(new_data);
    delete[] new_data;
    return result;
}

String String::operator+(const char* other) const {
    size_t len1 = my_strlen(data);
    size_t len2 = my_strlen(other);
    char* new_data = new char[len1 + len2 + 1];

    for (size_t i = 0; i < len1; ++i) new_data[i] = data[i];
    for (size_t i = 0; i < len2; ++i) new_data[len1 + i] = other[i];
    new_data[len1 + len2] = '\0';

    String result(new_data);
    delete[] new_data;
    return result;
}

String String::operator+(int number) const {
    char num[20];
    int len = 0;
    if (number == 0) {
        num[len++] = '0';
    } else {
        int n = number;
        while (n > 0) {
            num[len++] = '0' + (n % 10);
            n /= 10;
        }
        for (int i = 0; i < len / 2; ++i) {
            char t = num[i];
            num[i] = num[len - i - 1];
            num[len - i - 1] = t;
        }
    }
    num[len] = '\0';
    return *this + num;
}


char String::operator[](size_t index) const {
    if (index >= size()) {
        return '\0';
    }
    return data[index];
}

char& String::operator[](size_t index) {
    if (index >= size()) {
        static char dummy = '\0';
        return dummy;
    }
    return data[index];
}


String String::from_int(int number) {
    char buffer[20];
    int len = 0;

    if (number == 0) {
        buffer[len++] = '0';
    } else {
        bool is_negative = number < 0;
        unsigned int n = is_negative ? -number : number;

        while (n > 0) {
            buffer[len++] = '0' + (n % 10);
            n /= 10;
        }

        if (is_negative) {
            buffer[len++] = '-';
        }

        for (int i = 0; i < len / 2; ++i) {
            char temp = buffer[i];
            buffer[i] = buffer[len - i - 1];
            buffer[len - i - 1] = temp;
        }
    }

    buffer[len] = '\0';
    return String(buffer);
}


ostream& operator<<(ostream& os, const String& s) {
    os << s.data;
    return os;
}

String operator+(const char* lhs, const String& rhs) {
    return String(lhs) + rhs;
}