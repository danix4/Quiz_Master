#include "users.h"

// User base
User::User(const String& name, const String& surname, const String& username, const String& password)
        : name(name), surname(surname), username(username), password(password) {}
User::~User() {}

// Administrator
Administrator::Administrator(const String& name, const String& surname, const String& username, const String& password)
        : User(name, surname, username, password), reports(nullptr), reports_count(0) {}

Administrator::~Administrator() {
    delete[] reports;
}

void Administrator::add_report(const String& report) {
    String* new_reports = new String[reports_count + 1];
    for (int i = 0; i < reports_count; ++i) {
        new_reports[i] = reports[i];
    }
    new_reports[reports_count] = report;
    delete[] reports;
    reports = new_reports;
    ++reports_count;
}

// Player
Player::Player(const String& name, const String& surname, const String& username, const String& password)
        : User(name, surname, username, password),
          collected_points(0),
          current_level(1),
          solved_test_quizzes(0),
          solved_normal_quizzes(0),
          created_approved_quizzes(0),
          finished_challenges(nullptr),
          ongoing_challenges(nullptr),
          messages(nullptr),
          messages_count(0),
          challenges(nullptr),
          challenges_count(0),
          liked_quizzes(nullptr),
          liked_quizzes_count(0),
          favourite_quizzes(nullptr),
          favourite_quizzes_count(0) {}

Player::~Player() {
    delete[] messages;
    delete[] liked_quizzes;
    delete[] challenges;
    delete[] favourite_quizzes;
}

void Player::add_message(const String& message) {
    String* new_messages = new String[messages_count + 1];
    for (int i = 0; i < messages_count; ++i) {
        new_messages[i] = messages[i];
    }
    new_messages[messages_count] = message;
    delete[] messages;
    messages = new_messages;
    ++messages_count;
}

void Player::add_challenge(const String& challenge) {
    String* new_challenges = new String[challenges_count + 1];
    for (int i = 0; i < challenges_count; ++i) {
        new_challenges[i] = challenges[i];
    }
    new_challenges[challenges_count] = challenge;
    delete[] challenges;
    challenges = new_challenges;
    ++challenges_count;
}

void Player::add_points(int added_points) {
    collected_points += added_points;
}

bool Player::has_liked(int quiz_id) const {
    for (int i = 0; i < liked_quizzes_count; ++i) {
        if (liked_quizzes[i] == quiz_id) return true;
    }
    return false;
}

bool Player::has_added_to_favs(int quiz_id) const {
    for (int i = 0; i < favourite_quizzes_count; ++i) {
        if (favourite_quizzes[i] == quiz_id) return true;
    }
    return false;
}

bool Player::has_not_liked(int quiz_id) const {
    return !has_liked(quiz_id);
}

bool Player::has_not_added_to_favs(int quiz_id) const {
    return !has_added_to_favs(quiz_id);
}

void Player::add_liked_quiz(int quiz_id) {
    int* new_liked = new int[liked_quizzes_count + 1];
    for (int i = 0; i < liked_quizzes_count; ++i) {
        new_liked[i] = liked_quizzes[i];
    }
    new_liked[liked_quizzes_count] = quiz_id;
    delete[] liked_quizzes;
    liked_quizzes = new_liked;
    ++liked_quizzes_count;
}

void Player::add_favourite_quiz(int quiz_id) {
    int* new_favourite = new int[favourite_quizzes_count + 1];
    for (int i = 0; i < favourite_quizzes_count; ++i) {
        new_favourite[i] = favourite_quizzes[i];
    }
    new_favourite[favourite_quizzes_count] = quiz_id;
    delete[] favourite_quizzes;
    favourite_quizzes = new_favourite;
    ++favourite_quizzes_count;
}

void Player::remove_liked_quiz(int quiz_id) {
    if (has_not_liked(quiz_id)) return;
    int* new_liked = new int[liked_quizzes_count - 1];
    int index = 0;
    for (int i = 0; i < liked_quizzes_count; ++i) {
        if (liked_quizzes[i] != quiz_id) {
            new_liked[index++] = liked_quizzes[i];
        }
    }
    delete[] liked_quizzes;
    liked_quizzes = new_liked;
    --liked_quizzes_count;
}

void Player::remove_favourite_quiz(int quiz_id) {
    if (has_not_added_to_favs(quiz_id)) return;
    int* new_favs = new int[favourite_quizzes_count - 1];
    int index = 0;
    for (int i = 0; i < favourite_quizzes_count; ++i) {
        if (favourite_quizzes[i] != quiz_id) {
            new_favs[index++] = favourite_quizzes[i];
        }
    }
    delete[] favourite_quizzes;
    favourite_quizzes = new_favs;
    --favourite_quizzes_count;
}
