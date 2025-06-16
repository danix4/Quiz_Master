
#pragma once
#include "String.h"

class User {
public:
    String name, surname, username, password;

    User(const String& name, const String& surname, const String& username, const String& password);
    virtual ~User();
};

class Administrator : public User {
public:
    String* reports;
    int reports_count;

    Administrator(const String& name, const String& surname, const String& username, const String& password);
    ~Administrator();

    void add_report(const String& report);
};

class Player : public User {
public:
    int collected_points;
    int current_level;
    int solved_test_quizzes;
    int solved_normal_quizzes;
    int created_approved_quizzes;
    String* finished_challenges;
    String* ongoing_challenges;
    String* messages;
    int messages_count;
    String* challenges;
    int challenges_count;
    int* liked_quizzes;
    int liked_quizzes_count;
    int* favourite_quizzes;
    int favourite_quizzes_count;

    Player(const String& name, const String& surname, const String& username, const String& password);
    ~Player();

    void add_message(const String& message);
    void add_challenge(const String& challenge);
    void add_points(int added_points);

    bool has_liked(int quiz_id) const;
    bool has_added_to_favs(int quiz_id) const;
    bool has_not_liked(int quiz_id) const;
    bool has_not_added_to_favs(int quiz_id) const;

    void add_liked_quiz(int quiz_id);
    void add_favourite_quiz(int quiz_id);
    void remove_liked_quiz(int quiz_id);
    void remove_favourite_quiz(int quiz_id);
};
