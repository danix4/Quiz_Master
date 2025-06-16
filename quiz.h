#pragma once
#include "questions.h"

class Quiz {
public:
    int id;
    String title;
    Question** questions;
    int capacity;
    int question_count;
    Mode mode;
    int likes;
    String creator;

    Quiz(int _id = -1, const String& _title = "", int _capacity = 1, Mode _mode = Test, int _likes = 0, const String& creator = "");
    ~Quiz();

    void add_question(Question* q);
    void shuffle_questions();
};
