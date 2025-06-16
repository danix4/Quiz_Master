
#pragma once
#include "questions.h"
#include "users.h"
#include "quiz.h"
#include <iostream>
#include <ctime>


using namespace std;


Administrator** admins = nullptr;
int admins_count = 0;

Player** players = nullptr;
int players_count = 0;
int players_capacity = 0;

bool is_player_logged_in = false;
int logined_player_id = -1;
bool is_admin_logged_in = false;
int logined_admin_id = -1;

int id_count = 0;

const int MAX_QUIZZES = 1000;

Quiz* approved_quizzes[MAX_QUIZZES];
Quiz* pending_quizzes[MAX_QUIZZES];

int approved_quizzes_count = 0;
int pending_quizzes_count = 0;



int to_int(const String& str) {
    int result = 0;
    int i = 0;
    bool is_negative = false;

    if (str.at(0) == '-') {
        is_negative = true;
        i = 1;
    }

    while (true) {
        char c = str.at(i);
        if (c < '0' || c > '9' || c == '\0')
            break;
        result = result * 10 + (c - '0');
        i++;
    }

    return is_negative ? -result : result;
}



bool strings_equal(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

void extract_first_word(const char* input, char* output) {
    int i = 0;
    while (input[i] != '\0' && input[i] != ' ') {
        output[i] = input[i];
        i++;
    }
    output[i] = '\0';
}

String* words_signup(const char* input) {
    auto result = new String[5];
    char word[100];
    int i = 6;
    int j = 0, w = 0;

    while (input[i] == ' ') ++i;

    while (input[i] != '\0' && w < 5) {
        j = 0;
        while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
        word[j] = '\0';
        result[w++] = String(word);
        while (input[i] == ' ') ++i;
    }

    return result;
}

void add_player(Player* new_player) {
    if (players_count >= players_capacity) {
        players_capacity = players_capacity == 0 ? 4 : players_capacity * 2;
        Player** new_arr = new Player*[players_capacity];
        for (int i = 0; i < players_count; ++i)
            new_arr[i] = players[i];
        delete[] players;
        players = new_arr;
    }
    players[players_count++] = new_player;
}

int login_user(const char* input) {

    char word[100];
    int i = 5, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String username(word);

    while (input[i] == ' ') ++i;
    j = 0;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';
    String password(word);

    for (int k = 0; k < players_count; ++k)
        if (players[k]->username == username && players[k]->password == password) {
            is_admin_logged_in = false;
            is_player_logged_in = true;
            return k;
        }

    for (int k = 0; k < admins_count; ++k)
        if (admins[k]->username == username && admins[k]->password == password) {
            is_admin_logged_in = true;
            is_player_logged_in = false;
            return k;
        }

    cout << "No such account" << endl;
    return -1;
}




void add_pending_quiz (Quiz* new_quiz) {
    if (pending_quizzes_count >= MAX_QUIZZES) {
        cout << "Pending quizzes list is full" << endl;
        return;
    }
    pending_quizzes[pending_quizzes_count++] = new_quiz;
}


void remove_pending_quiz (int quiz_id) {

    int index = -1;

    for (int i = 0; i < pending_quizzes_count; ++i) {
        if (pending_quizzes[i]->id == quiz_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Quiz with ID " << quiz_id << " not found in pending list" << endl;
        return;
    }

    for (int i = index; i < pending_quizzes_count - 1; ++i) {
        pending_quizzes[i] = pending_quizzes[i + 1];
    }

    pending_quizzes_count--;

}



void remove_approved_quiz (int quiz_id) {

    int index = -1;

    for (int i = 0; i < approved_quizzes_count; ++i) {
        if (approved_quizzes[i]->id == quiz_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Quiz with ID " << quiz_id << " not found in pending list" << endl;
        return;
    }

    for (int i = index; i < approved_quizzes_count - 1; ++i) {
        approved_quizzes[i] = approved_quizzes[i + 1];
    }

    approved_quizzes_count--;

}


String get_current_datetime() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);

    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);

    return String(buffer);
}

void add_approved_quiz (Quiz* new_quiz) {

    if (approved_quizzes_count >= MAX_QUIZZES) {
        cout << "Approved quizzes list is full" << endl;
        return;
    }
    approved_quizzes[approved_quizzes_count++] = new_quiz;

}

void level_check (Player* p) {

    int points = p->collected_points;
    int level = 1;
    int required_points = 0;

    while (true) {

        if (level <= 10) {
            required_points += 1000;
        } else if (level <= 20) {
            required_points += 2000;
        } else if (level <= 30) {
            required_points += 3000;
        } else {
            required_points += 4000;
        }

        if (points < required_points) {
            break;
        }

        level++;

    }

    if (level > p->current_level) {
        p->current_level = level;
        String msg = "Level " + String::from_int(level) + " reached";
        p->add_message(msg);
    }

}




void check_if__created_challenge_completed (Player* p){

    int n = p->created_approved_quizzes;

    String challenge = get_current_datetime();
    challenge = challenge + " | ";
    int added_points = 0;

    String message = "New challenge completed! You created ";

    if (n % 5 == 0 && n >= 5 && n <= 30) {

        challenge = challenge + "Created " + String::from_int(n) + " quizzes";
        added_points = (n * 10) / 2;
        p->add_challenge(challenge);

        message = message + String::from_int(n) + " quizzes!";
        p->add_message(message);

    }


    if (added_points > 0) {
        p->collected_points += added_points;
        level_check(p);
    }

}







void check_if__solved_normal_completed(Player* p){

    int n = p->solved_normal_quizzes;

    String challenge = get_current_datetime();
    challenge = challenge + " | ";
    int added_points = 0;

    String message = "New challenge completed! You solved ";

    if (n % 10 == 0 && n >= 10 && n <= 100) {

        challenge = challenge + "Completed " + String::from_int(n) + " quizzes in normal mode";
        added_points = 2 * (n * 10) / 3;
        p-> add_challenge (challenge);

        message = message + String::from_int(n) + " quizzes in normal mode!";
        p->add_message(message);

    }

    if (added_points > 0) {
        p->collected_points += added_points;
        level_check(p);
    }

}



void check_if__solved_test_completed(Player* p) {

    int n = p->solved_test_quizzes;

    String challenge = get_current_datetime();
    challenge = challenge + " | ";
    int added_points = 0;

    String message = "New challenge completed! You solved ";

    if (n % 10 == 0 && n >= 10 && n <= 100) {

        challenge = challenge + "Completed " + String::from_int(n) + " quizzes in test mode";
        added_points = (n * 10) / 3;
        p->add_challenge(challenge);

        message = message + String::from_int(n) + " quizzes in test mode!";
        p->add_message(message);
    }

    if (added_points > 0) {
        p->collected_points += added_points;
        level_check(p);
    }

}


void view_uncompleted_challenges(Player* p) {
    int created = p->created_approved_quizzes;
    int solved_normal = p->solved_normal_quizzes;
    int solved_test = p->solved_test_quizzes;

    for (int i = 5; i <= 30; i += 5) {
        if (created < i) {
            cout << "Create " << i << " quizzes (" << created << "/" << i << ")" << endl;
        }
    }

    for (int i = 10; i <= 100; i += 10) {
        if (solved_normal < i) {
            cout << "Complete " << i << " quizzes in normal mode (" << solved_normal << "/" << i << ")" << endl;
        }
    }

    for (int i = 10; i <= 100; i += 10) {
        if (solved_test < i) {
            cout << "Complete " << i << " quizzes in test mode (" << solved_test << "/" << i << ")" << endl;
        }
    }
}



void view_profile(Player* p) {
    cout << "User " << p->name << " @" << p->username << endl;
    cout << "Level: " << p->current_level << endl;


    cout << "Created Quizzes:" << endl;
    for (int i = 0; i < approved_quizzes_count; ++i) {
        if (approved_quizzes[i]->creator == p->username) {
            cout << "[" << approved_quizzes[i]->id << "] " << approved_quizzes[i]->title << endl;
        }
    }


    cout << p->collected_points << "points collected" << endl;

    cout << "Liked Quizzes: ";
    for (int i = 0; i < p->liked_quizzes_count; ++i) {
        cout << "[" << p->liked_quizzes[i] << "]";
        if (i < p->liked_quizzes_count - 1) cout << " ";
    }
    cout << endl;


    cout << "Favourite Quizzes: ";
    for (int i = 0; i < p->favourite_quizzes_count; ++i) {
        cout << "[" << p->favourite_quizzes[i] << "]";
        if (i < p->favourite_quizzes_count - 1) cout << " ";
    }
    cout << endl;
}





void view_user(const char* input) {

    char word[100];
    int i = 5, j = 0;

    while (input[i] == ' ') ++i;

    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String username(word);


    Player* target = nullptr;
    for (int k = 0; k < players_count; ++k) {
        if (players[k]->username == username) {
            target = players[k];
            break;
        }
    }

    if (target == nullptr) {
        cout << "No such user found." << endl;
        return;
    }


    cout << "User " << target->name << " @" << target->username << endl;
    cout << "Level: " << target->current_level << endl;


    cout << "Created Quizzes:" << endl;
    for (int k = 0; k < approved_quizzes_count; ++k) {
        if (approved_quizzes[k]->creator == username) {
            cout << "[" << approved_quizzes[k]->id << "]" << endl;
            cout << approved_quizzes[k]->title << endl;
        }
    }
}






void approve_quiz (const char* input){

    char word[100];

    int i = 13, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String id (word);

    int quiz_id = to_int(id);

    Quiz* approved_quiz = nullptr;

    for (int k = 0; k < pending_quizzes_count; ++k) {
        if (pending_quizzes[k]->id == quiz_id) {
            approved_quiz = pending_quizzes[k];
            break;
        }
    }

    const String& creator_nickname = approved_quiz->creator;

    for (int k = 0; k < players_count; ++k) {
        if (players[k]->username == creator_nickname) {

            Player* p = players[k];

            p->created_approved_quizzes++;
            check_if__created_challenge_completed(p);

        }
    }

    remove_pending_quiz (approved_quiz->id);
    add_approved_quiz(approved_quiz);

}






void show_quiz_with_id (const char* input){

    char word[100];

    int i = 8, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String username (word);

    for(int k=0; k<approved_quizzes_count; k++){
        if(approved_quizzes[k]->creator == username){
            cout<<approved_quizzes[k]->id<<" | "<<approved_quizzes[k]->title<<" | "<<approved_quizzes[k]->creator<<" | "<<approved_quizzes[k]->capacity<<" Questions | "<<approved_quizzes[k]->likes<<" Likes"<<endl;
        }
    }

}



void like_quiz (const char* input, const String& username_) {
    char word[100];
    int i = 10, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    int quiz_id = to_int(String(word));
    Player* player = nullptr;


    for (int p = 0; p < players_count; p++) {
        if (players[p]->username == username_) {
            player = players[p];
            if (player->has_liked(quiz_id)) {
                cout << "You have already liked this quiz." << endl;
                return;
            }
            break;
        }
    }


    bool found = false;
    for (int k = 0; k < approved_quizzes_count; k++) {
        if (approved_quizzes[k]->id == quiz_id) {
            approved_quizzes[k]->likes++;
            player->add_liked_quiz(quiz_id);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Quiz with ID " << quiz_id << " not found." << endl;
    }
}



void add_to_favs (const char* input, const String& username_) {

    char word[100];
    int i = 12, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    int quiz_id = to_int(String(word));
    Player* player = nullptr;

    for (int p = 0; p < players_count; p++) {
        if (players[p]->username == username_) {
            player = players[p];
            if (player->has_added_to_favs (quiz_id)) {
                cout << "You have already marked as favorite." << endl;
                return;
            }
            break;
        }
    }

    bool found = false;

    for (int k = 0; k < approved_quizzes_count; k++) {
        if (approved_quizzes[k]->id == quiz_id) {
            player->add_favourite_quiz(quiz_id);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Quiz with ID " << quiz_id << " not found." << endl;
    }
}




void unlike_quiz(const char* input, const String& username_) {
    char word[100];
    int i = 12, j = 0;

    // Skip initial spaces
    while (input[i] == ' ') ++i;
    // Extract the quiz ID string
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    int quiz_id = to_int(String(word));
    Player* player = nullptr;


    for (int p = 0; p < players_count; p++) {
        if (players[p]->username == username_) {
            player = players[p];
            if (player->has_not_liked(quiz_id)) {
                cout << "You have not yet liked this quiz." << endl;
                return;
            }
            break;
        }
    }

    bool found = false;
    for (int k = 0; k < approved_quizzes_count; k++) {
        if (approved_quizzes[k]->id == quiz_id) {
            if (approved_quizzes[k]->likes > 0) {
                approved_quizzes[k]->likes--;
            }
            player->remove_liked_quiz(quiz_id);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Quiz with ID " << quiz_id << " not found." << endl;
    }
}





void remove_from_favs (const char* input, const String& username_) {
    char word[100];
    int i = 17, j = 0;

    // Skip initial spaces
    while (input[i] == ' ') ++i;
    // Extract the quiz ID string
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    int quiz_id = to_int(String(word));
    Player* player = nullptr;

    for (int p = 0; p < players_count; p++) {
        if (players[p]->username == username_) {
            player = players[p];
            if (player->has_not_added_to_favs (quiz_id)) {
                cout << "You have not added to favourite this quiz." << endl;
                return;
            }
            break;
        }
    }

    bool found = false;
    for (int k = 0; k < approved_quizzes_count; k++) {
        if (approved_quizzes[k]->id == quiz_id) {
            player->remove_favourite_quiz(quiz_id);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Quiz with ID " << quiz_id << " not found." << endl;
    }
}





void ban_user (const char* input){

    char word[100];

    int i = 4, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String username_ (word);


    char word_[21];

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int charset_size = sizeof(charset) - 1;

    for (int k = 0; k < 20; ++k) {
        int index = rand() % charset_size;
        word_[k] = charset[index];
    }
    word_[20] = '\0';

    String result(word_);

    for(int k = 0; k < players_count; k++){
        if(players[k]->username == username_){
            players[k]->password = result;
        }
    }

}



void remove_quiz (const char* input){

    char word[100];
    int i = 12, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String id (word);
    int quiz_id = to_int(id);

    Quiz* removed_quiz = nullptr;

    for (int k = 0; k < approved_quizzes_count; ++k) {
        if (approved_quizzes[k]->id == quiz_id) {
            removed_quiz = approved_quizzes[k];
            break;
        }
    }

    while (input[i] == ' ') ++i;
    String message(input + i);
    String creator = removed_quiz->creator;
    remove_approved_quiz (removed_quiz->id);

    String final_message = "Quiz with id: " + String("") + quiz_id + " was removed for " + message;

    for (int l = 0; l < players_count; ++l) {
        if (players[l]->username == creator) {
            players[l]->add_message(final_message);
        }
    }
}





void reject_quiz (const char* input) {

    char word[100];
    int i = 12, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String id (word);
    int quiz_id = to_int(id);

    Quiz* rejected_quiz = nullptr;

    for (int k = 0; k < pending_quizzes_count; ++k) {
        if (pending_quizzes[k]->id == quiz_id) {
            rejected_quiz = pending_quizzes[k];
            break;
        }
    }

    while (input[i] == ' ') ++i;
    String message(input + i);

    String creator = rejected_quiz->creator;
    remove_pending_quiz (rejected_quiz->id);

    String final_message = "Quiz with id: " + String("") + quiz_id + " was rejected for " + message;

    for (int l = 0; l < players_count; ++l) {
        if (players[l]->username == creator) {
            players[l]->add_message(final_message);
        }
    }
}





void report_quiz (const char* input, const String& reporter) {

    char word[100];
    int i = 12, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String id(word);
    int quiz_id = to_int(id);

    Quiz* reported_quiz = nullptr;

    for (int k = 0; k < approved_quizzes_count; ++k) {
        if (approved_quizzes[k]->id == quiz_id) {
            reported_quiz = approved_quizzes[k];
            break;
        }
    }

    while (input[i] == ' ') ++i;
    String message(input + i);

    String creator = reported_quiz->creator;

    time_t now = time(0);
    tm* ltm = localtime(&now);

    char date_buffer[11];
    sprintf(date_buffer, "%02d.%02d.%04d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    String date(date_buffer);

    String final_message = date + " | sent by " + reporter +
                           " | quiz id: " + String("") + quiz_id +
                           " by " + creator +
                           " | reason - " + message;

    for (int l = 0; l < admins_count; ++l) {
        admins[l]->add_report(final_message);
    }
}


int max_points(const Quiz* quiz) {
    int total_points = 0;

    for (int i = 0; i < quiz->question_count; ++i) {
        if (quiz->questions[i] != nullptr) {
            total_points += quiz->questions[i]->points;
        }
    }

    return total_points;
}





int compare_answers_TF(const TrueOrFalseQuestion* q, const String& user_answer) {

    bool correct_answer = q->get_answer();
    bool user_actual_answer;

    if (user_answer == "True") {
        user_actual_answer = true;
    }  {
        user_actual_answer = false;
    }

    return (correct_answer == user_actual_answer) ? q->points : 0;

}


int compare_answers_SC (const SingleChoiceQuestion* q, const String& user_answer){

    String correct_answer = q->get_answer();

    return (correct_answer == user_answer) ? q->points : 0;

}

int compare_answers_MC(const MultipleChoiceQuestion* q, const char* user_answers) {
    int correct_count = q->get_correct_count();
    const char* correct_answers = q->get_answers();


    int matched = 0;


    for (int i = 0; i < correct_count; ++i) {
        char user_ans = user_answers[i];

        for (int j = 0; j < correct_count; ++j) {
            if (user_ans == correct_answers[j]) {
                matched++;
                break;
            }
        }
    }

    if (matched == correct_count) {
        return q->points;
    } else if (matched >= (correct_count + 1) / 2) {

        return q->points / 2;
    } else {
        return 0;
    }
}



int compare_answers_ShA(const ShortAnswerQuestion* q, const String& user_answer) {

    String correct = q->get_answer();

    for (int i = 0; i < correct.size(); ++i) {
        correct[i] = tolower(correct[i]);
    }

    String user = user_answer;
    for (int i = 0; i < user.size(); ++i) {
        user[i] = tolower(user[i]);
    }

    if (correct == user) {
        return q->points;
    }

    return 0;
}


int compare_answers_MP(const MatchingPairsQuestion* q, const String& user_input) {

    const int MAX_PAIRS = 20;
    char user_left[MAX_PAIRS];
    char user_right[MAX_PAIRS];
    int user_count = 0;

    for (int i = 0; i < user_input.size(); ++i) {
        if (user_input[i] == '(' && user_input[i+2] == ',' && user_input[i+4] == ')') {
            if (user_count < MAX_PAIRS) {
                user_left[user_count] = user_input[i+1];
                user_right[user_count] = user_input[i+3];
                ++user_count;
            }
        }
    }

    String correct = q->get_pairs_answer();
    char correct_left[MAX_PAIRS];
    char correct_right[MAX_PAIRS];
    int correct_count = 0;

    for (int i = 0; i < correct.size(); ++i) {
        if (correct[i] == '(' && correct[i+2] == ',' && correct[i+4] == ')') {
            if (correct_count < MAX_PAIRS) {
                correct_left[correct_count] = correct[i+1];
                correct_right[correct_count] = correct[i+3];
                ++correct_count;
            }
        }
    }

    int matches = 0;
    for (int i = 0; i < user_count; ++i) {
        for (int j = 0; j < correct_count; ++j) {
            if (user_left[i] == correct_left[j] && user_right[i] == correct_right[j]) {
                ++matches;
                break;
            }
        }
    }


    if (matches == correct_count) {
        return q->points;
    } else if (matches >= correct_count / 2.0) {
        return q->points / 2;
    } else {
        return 0;
    }
}


void show_answers_TF (const TrueOrFalseQuestion* q){
    cout<<"Select True or False"<<endl;
}

void show_answers_SC (const SingleChoiceQuestion* q){
    int count = q->get_answers_count();
    for (int i=0; i < count; i++){
        cout << static_cast<char>('A' + i) << ") " << q->get_answer_at_index(i) << endl;
    }
}

void show_answers_MC(const MultipleChoiceQuestion* q) {
    int count = q->get_answers_count();
    for (int i = 0; i < count; i++) {
        cout << static_cast<char>('A' + i) << ") " << q->get_answer_at_index(i) << endl;
    }
}

void show_answers_MP(const MatchingPairsQuestion* q) {
    int col1_count = q->get_col1_count();
    int col2_count = q->get_col2_count();

    cout << "Column 1:" << endl;
    for (int i = 0; i < col1_count; ++i) {
        cout << static_cast<char>('A' + i) << ") " << q->get_col1_item(i) << endl;
    }

    cout << "Column 2:" << endl;
    for (int i = 0; i < col2_count; ++i) {
        cout << (i + 1) << ") " << q->get_col2_item(i) << endl;
    }
}



void start_quiz (const char* input, const String& player_username) {
    char word[100];
    int i = 11, j = 0;

    while (input[i] == ' ') ++i;
    while (input[i] != ' ' && input[i] != '\0') word[j++] = input[i++];
    word[j] = '\0';

    String id(word);
    int quiz_id = to_int(id);

    Quiz* started_quiz = nullptr;

    for (int k = 0; k < approved_quizzes_count; ++k) {
        if (approved_quizzes[k]->id == quiz_id) {
            started_quiz = approved_quizzes[k];
            break;
        }
    }

    if (started_quiz == nullptr) {
        cout << "Quiz with ID " << quiz_id << " not found." << endl;
        return;
    }

    while (input[i] == ' ') ++i;

    char mode_word[20];
    j = 0;
    while (input[i] != ' ' && input[i] != '\0') mode_word[j++] = input[i++];
    mode_word[j] = '\0';
    String mode_str(mode_word);

    bool is_test = false;
    if (mode_str == "test") {
        is_test = true;
    } else if (mode_str == "normal") {
        is_test = false;
    } else {
        std::cout << "Invalid mode. Use 'test' or 'normal'." << std::endl;
        return;
    }

    while (input[i] == ' ') ++i;


    bool do_shuffle = false;
    if (input[i] != '\0') {
        char shuffle_word[20];
        j = 0;
        while (input[i] != ' ' && input[i] != '\0') shuffle_word[j++] = input[i++];
        shuffle_word[j] = '\0';
        String shuffle_str(shuffle_word);

        if (shuffle_str == "shuffle") {
            do_shuffle = true;
        } else {
            cout << "Invalid argument after mode. Use 'shuffle' or leave empty." << endl;
            return;
        }
    }

    if (do_shuffle) {
        started_quiz->shuffle_questions();
    }


    if (is_test) {


        for (int h = 0; h < started_quiz->question_count; h++) {

            Question* q = started_quiz->questions[h];

            char user_answer[256];

            cout << h + 1 << ") " << q->content << endl;

            if (auto mc = dynamic_cast<MultipleChoiceQuestion*>(q)){

                int count = mc->get_correct_count();
                char* correct_answers = new char[count];
                char c;
                show_answers_MC (mc);

                cout<<"There are "<<count<<" correct answers. Enter them: "<<endl;

                for(int l = 0; l < count; l++){
                    cout<<l+1<<": ";
                    cin>>c;
                    correct_answers[l] = c;
                }

                cin.ignore();

                cout << "Correct answer is: ";
                started_quiz->questions[h]->print_correct_answer();

                delete[] correct_answers;

            }

            else if (auto mp = dynamic_cast<MatchingPairsQuestion*>(q)){

                show_answers_MP (mp);
                cout << "Enter your pairs here: ";
                cin.getline(user_answer, 256);
                cout << "Correct answer is: ";
                started_quiz->questions[h]->print_correct_answer();

            }

            else if (auto tf = dynamic_cast<TrueOrFalseQuestion*>(q)) {

                show_answers_TF (tf);
                cout << "Enter your answer here: ";
                cin.getline(user_answer, 256);
                cout << "Correct answer is: ";
                started_quiz->questions[h]->print_correct_answer();

            }

            else if (auto sc = dynamic_cast<SingleChoiceQuestion*>(q)) {

                show_answers_SC (sc);
                cout << "Enter your answer here: ";
                cin.getline(user_answer, 256);
                cout << "Correct answer is: ";
                started_quiz->questions[h]->print_correct_answer();

            } else if (auto sa = dynamic_cast<ShortAnswerQuestion*>(q)){

                cout << "Enter your answer here: ";
                cin.getline(user_answer, 256);
                cout << "Correct answer is: ";
                started_quiz->questions[h]->print_correct_answer();

            }

        }


        const String& creator_nickname = started_quiz->creator;

        for (int m = 0; m < players_count; ++m) {
            if (players[m]->username == creator_nickname) {

                auto p = dynamic_cast<Player*>(players[m]);

                p->solved_test_quizzes++;

            }
        }


    } else {

        int user_points = 0;

        for (int h = 0; h < started_quiz->question_count; h++) {

            Question* q = started_quiz->questions[h];

            char user_answer[256];

            cout << h + 1 << ") " << q->content << endl;

            if (auto mc = dynamic_cast<MultipleChoiceQuestion*>(q)){

                int count = mc->get_correct_count();
                char* correct_answers = new char[count];
                char c;
                show_answers_MC (mc);

                cout<<"There are "<<count<<" correct answers. Enter them: "<<endl;

                for(int l = 0; l < count; l++){
                    cout<<l+1<<": ";
                    cin>>c;
                    correct_answers[l] = c;
                }

                user_points += compare_answers_MC (mc, correct_answers);

                delete[] correct_answers;

            }

            else if (auto mp = dynamic_cast<MatchingPairsQuestion*>(q)){

                show_answers_MP (mp);
                cout << "Enter your pairs here: ";
                cin.getline(user_answer, 256);
                user_points += compare_answers_MP (mp, String(user_answer));

            }

            else if (auto tf = dynamic_cast<TrueOrFalseQuestion*>(q)) {

                show_answers_TF (tf);
                cout << "Enter your answer here: ";
                cin.getline(user_answer, 256);
                user_points += compare_answers_TF(tf, user_answer);

            }

            else if (auto sc = dynamic_cast<SingleChoiceQuestion*>(q)) {

                show_answers_SC (sc);
                cout << "Enter your answer here: ";
                cin.getline(user_answer, 256);
                user_points += compare_answers_SC(sc, user_answer);

            } else if (auto sa = dynamic_cast<ShortAnswerQuestion*>(q)){

                cout << "Enter your answer here: ";
                cin.getline(user_answer, 256);
                user_points += compare_answers_ShA(sa, user_answer);

            }

        }

        cout<<endl;

        cout << "Your quiz score is: " << user_points << "/" << max_points (started_quiz) << endl;

        const String& creator_nickname = started_quiz->creator;

        for (int m = 0; m < players_count; ++m) {
            if (players[m]->username == creator_nickname) {

                auto p = dynamic_cast<Player*>(players[m]);

                p->solved_normal_quizzes++;

            }
        }
    }
}
