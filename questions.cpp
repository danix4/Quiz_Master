#include "questions.h"
#include <iostream>


using namespace std;

// Question base class
Question::Question(const String& c, int p) : content(c), points(p) {}
Question::~Question() {}

// TrueOrFalseQuestion
TrueOrFalseQuestion::TrueOrFalseQuestion(const String& c, int p, bool a)
        : Question(c, p), answer(a) {}
void TrueOrFalseQuestion::print_correct_answer() const {
    cout << (answer ? "True" : "False") << endl;
}
bool TrueOrFalseQuestion::get_answer() const {
    return answer;
}

// SingleChoiceQuestion
SingleChoiceQuestion::SingleChoiceQuestion(const String& c, int p, String* answers, int count, const String& correct_answer)
        : Question(c, p), answers_count(count), answer(correct_answer) {
    all_answers = new String[answers_count];
    for (int i = 0; i < answers_count; ++i) {
        all_answers[i] = answers[i];
    }
}
SingleChoiceQuestion::~SingleChoiceQuestion() {
    delete[] all_answers;
}
void SingleChoiceQuestion::print_correct_answer() const {
    cout << answer << endl;
}
String SingleChoiceQuestion::get_answer() const {
    return answer;
}
int SingleChoiceQuestion::get_answers_count() const {
    return answers_count;
}
String SingleChoiceQuestion::get_answer_at_index(int i) const {
    return all_answers[i];
}

// MultipleChoiceQuestion
MultipleChoiceQuestion::MultipleChoiceQuestion(const String& c, int p, String* answers, int count, char* correct_answers_input, int correct_count_input)
        : Question(c, p), answers_count(count), correct_count(correct_count_input) {
    all_answers = new String[answers_count];
    for (int i = 0; i < answers_count; ++i) {
        all_answers[i] = answers[i];
    }
    correct_answers = new char[correct_count];
    for (int i = 0; i < correct_count; ++i) {
        correct_answers[i] = correct_answers_input[i];
    }
}
MultipleChoiceQuestion::~MultipleChoiceQuestion() {
    delete[] all_answers;
    delete[] correct_answers;
}
void MultipleChoiceQuestion::print_correct_answer() const {
    for (int i = 0; i < correct_count; ++i) {
        cout << correct_answers[i];
        if (i < correct_count - 1) cout << ", ";
    }
    cout << endl;
}
const char* MultipleChoiceQuestion::get_answers() const {
    return correct_answers;
}
int MultipleChoiceQuestion::get_correct_count() const {
    return correct_count;
}
int MultipleChoiceQuestion::get_answers_count() const {
    return answers_count;
}
String MultipleChoiceQuestion::get_answer_at_index(int i) const {
    return all_answers[i];
}

// ShortAnswerQuestion
ShortAnswerQuestion::ShortAnswerQuestion(const String& c, int p, const String& correct_answer)
        : Question(c, p), correct_answer(correct_answer) {}
void ShortAnswerQuestion::print_correct_answer() const {
    cout << correct_answer << endl;
}
const String ShortAnswerQuestion::get_answer() const {
    return correct_answer;
}

// MatchingPairsQuestion
MatchingPairsQuestion::MatchingPairsQuestion(const String& c, int p, String* col1_input, String* col2_input, int n1, int n2, const String& pairs_answer)
        : Question(c, p), number_of_col1(n1), number_of_col2(n2), pairs_answer(pairs_answer) {
    col1 = new String[n1];
    for (int i = 0; i < n1; ++i) {
        col1[i] = col1_input[i];
    }
    col2 = new String[n2];
    for (int i = 0; i < n2; ++i) {
        col2[i] = col2_input[i];
    }
}
MatchingPairsQuestion::~MatchingPairsQuestion() {
    delete[] col1;
    delete[] col2;
}
void MatchingPairsQuestion::print_correct_answer() const {
    cout << pairs_answer << endl;
}
int MatchingPairsQuestion::get_col1_count() const {
    return number_of_col1;
}
int MatchingPairsQuestion::get_col2_count() const {
    return number_of_col2;
}
String MatchingPairsQuestion::get_col1_item(int index) const {
    return col1[index];
}
String MatchingPairsQuestion::get_col2_item(int index) const {
    return col2[index];
}
String MatchingPairsQuestion::get_pairs_answer() const {
    return pairs_answer;
}
