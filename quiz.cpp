#include "quiz.h"
#include <ctime>
#include <cstdlib>

Quiz::Quiz(int _id, const String& _title, int _capacity, Mode _mode, int _likes, const String& creator)
        : id(_id), title(_title), capacity(_capacity), mode(_mode), likes(_likes), question_count(0), creator(creator) {
    questions = new Question*[capacity];
    for (int i = 0; i < capacity; ++i)
        questions[i] = nullptr;
}

Quiz::~Quiz() {
    for (int i = 0; i < question_count; ++i)
        delete questions[i];
    delete[] questions;
}

void Quiz::add_question(Question* q) {
    if (question_count < capacity)
        questions[question_count++] = q;
}

void Quiz::shuffle_questions() {
    srand(time(nullptr));
    for (int i = question_count - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        Question* temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}
