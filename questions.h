#pragma once
#include "String.h"
#include <iostream>

enum Mode { Test, Normal };

class Question {
public:
    String content;
    int points;

public:
    Question(const String& c = "", int p = 0);
    virtual ~Question();

    virtual void print_correct_answer() const = 0;
};

// Derived classes declarations

class TrueOrFalseQuestion : public Question {
    bool answer;

public:
    TrueOrFalseQuestion(const String& c, int p, bool a);
    void print_correct_answer() const override;
    bool get_answer() const;
};

class SingleChoiceQuestion : public Question {
private:
    String answer;
    String* all_answers;
    int answers_count;

public:
    SingleChoiceQuestion(const String& c, int p, String* answers, int count, const String& correct_answer);
    ~SingleChoiceQuestion();
    void print_correct_answer() const override;
    String get_answer() const;
    int get_answers_count() const;
    String get_answer_at_index(int i) const;
};

class MultipleChoiceQuestion : public Question {
    char* correct_answers;
    int correct_count;
    String* all_answers;
    int answers_count;

public:
    MultipleChoiceQuestion(const String& c, int p, String* answers, int count, char* correct_answers_input, int correct_count_input);
    ~MultipleChoiceQuestion() override;
    void print_correct_answer() const override;
    const char* get_answers() const;
    int get_correct_count() const;
    int get_answers_count() const;
    String get_answer_at_index(int i) const;
};

class ShortAnswerQuestion : public Question {
    String correct_answer;

public:
    ShortAnswerQuestion(const String& c, int p, const String& correct_answer);
    void print_correct_answer() const override;
    const String get_answer() const;
};

class MatchingPairsQuestion : public Question {
    int number_of_col1;
    String* col1;
    int number_of_col2;
    String* col2;
    String pairs_answer;

public:
    MatchingPairsQuestion(const String& c, int p, String* col1_input, String* col2_input, int n1, int n2, const String& pairs_answer);
    ~MatchingPairsQuestion() override;
    void print_correct_answer() const override;
    int get_col1_count() const;
    int get_col2_count() const;
    String get_col1_item(int index) const;
    String get_col2_item(int index) const;
    String get_pairs_answer() const;
};
