
#pragma once
#include <iostream>

using namespace std;

class String {

private:
    char* data;

    size_t my_strlen(const char* str) const;
    void my_strcpy(char* dest, const char* src);

public:

    explicit String();
    String(const char* str);
    String(const String& other);
    ~String();

    String& operator=(const String& other);

    char at(int index) const;
    bool operator==(const String& other) const;
    size_t size() const;

    String operator+(const String& other) const;
    String operator+(const char* other) const;
    String operator+(int number) const;

    char operator[](size_t index) const;
    char& operator[](size_t index);

    static String from_int(int number);

    friend ostream& operator<<(ostream& os, const String& s);
    friend String operator+(const char* lhs, const String& rhs);
};
