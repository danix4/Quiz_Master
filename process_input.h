
#pragma once
#include "func.h"
#include <iostream>


using namespace std;

void process_input(const char* input) {

    char command[30];

    extract_first_word (input, command);

    if ((strings_equal(command, "signup") || strings_equal(command, "login"))
    && (is_admin_logged_in || is_player_logged_in)) {
        cout << "Log out first." << endl;
        return;
    }


    if (strings_equal (command, "signup")) {

        String* signup_fields = words_signup(input);

        if (!(signup_fields[3] == signup_fields[4]))
            cout << "Passwords do not match!\n";
        else {
            auto new_player = new Player(signup_fields[0], signup_fields[1], signup_fields[2], signup_fields[3]);
            add_player(new_player);
            is_player_logged_in = true;
            logined_player_id = players_count - 1;
            cout << "You are in account " << logined_player_id + 1 << endl;
        }

        delete[] signup_fields;
    }

    else if (strings_equal(command, "login")) {

        int user_id = login_user(input);

        if (user_id != -1) {

            if (is_admin_logged_in) {
                logined_admin_id = user_id;
                cout << "You are in admin account " << logined_admin_id + 1 << endl;
            } else {
                logined_player_id = user_id;
                cout << "You are in account " << logined_player_id + 1 << endl;
            }
        }

    }


    else if (strings_equal(command, "view-profile") && is_player_logged_in){

        view_profile (players[logined_player_id]);

    }

    else if (strings_equal(command, "create-quiz") && is_player_logged_in) {

        Quiz* new_quiz = new Quiz();

        cout << "Enter quiz title: ";
        char quiz_title[256];
        cin.getline (quiz_title, 256);
        new_quiz->title = String(quiz_title);
        new_quiz->id = 100 + id_count++;
        new_quiz->creator = players[logined_player_id]->username;


        cout << "Enter number of questions: ";
        int questions_count;
        cin >> questions_count;
        cin.ignore();
        new_quiz->capacity = questions_count;
        delete[] new_quiz->questions;
        new_quiz->questions = new Question*[questions_count];

        for (int i = 0; i < questions_count; ++i) {

            cout << "Enter question " << i + 1 << " type (T/F, SC, MC, ShA, MP): ";

            char qtype[16];
            cin.getline(qtype, 16);

            char description[256];
            int points;

            if (strings_equal(qtype, "T/F")) {

                cout << "Enter description: ";
                cin.getline(description, 256);
                cout << "Enter points: ";
                cin >> points;
                cin.ignore();

                ask_answer:
                char ans[16];
                cout << "Enter correct answer (True/False): ";
                cin.getline(ans, 16);
                if(strings_equal(ans,"True")){
                    new_quiz->add_question(new TrueOrFalseQuestion(String(description), points, "True"));
                }
                else if (strings_equal(ans,"False")) {
                    new_quiz->add_question(new TrueOrFalseQuestion(String(description), points, "False"));
                }
                else{
                    cout<<"Wrong written answer"<<endl;
                    goto ask_answer;
                }
            }

            else if (strings_equal(qtype, "SC")){

                char answer_a[256];
                char answer_b[256];
                char answer_c[256];
                char answer_d[256];
                char correct_answer[16];

                cout << "Enter description: ";
                cin.getline(description, 256);

                cout << "Enter points: ";
                cin >> points;
                cin.ignore();

                cout << "Enter answer A: ";
                cin.getline(answer_a, 256);

                cout << "Enter answer B: ";
                cin.getline(answer_b, 256);

                cout << "Enter answer C: ";
                cin.getline(answer_c, 256);

                cout << "Enter answer D: ";
                cin.getline(answer_d, 256);

                String answers[4] = {
                        String(answer_a),
                        String(answer_b),
                        String(answer_c),
                        String(answer_d)
                };

                ask_answer_SC:
                cout << "Enter correct answer (A,B,C,D): ";
                cin.getline(correct_answer,16);

                if(strings_equal(correct_answer,"A")){
                    new_quiz->add_question(new SingleChoiceQuestion(String(description), points, answers, 4, "A"));
                }
                else if(strings_equal(correct_answer,"B")){
                    new_quiz->add_question(new SingleChoiceQuestion(String(description), points, answers, 4, "B"));
                }
                else if(strings_equal(correct_answer,"C")){
                    new_quiz->add_question(new SingleChoiceQuestion(String(description), points, answers, 4, "C"));
                }
                else if(strings_equal(correct_answer,"D")){
                    new_quiz->add_question(new SingleChoiceQuestion(String(description), points, answers, 4, "D"));
                }
                else {
                    cout<<"Invalid answer. Enter again"<<endl;
                    goto ask_answer_SC;
                }
            }

            else if (strings_equal(qtype, "MC")) {

                int answers_count;
                int correct_count;
                char options[256];

                cout << "Enter description: ";
                cin.getline(description, 256);

                cout << "Enter possible answers count: (at least 4)";
                cin >> answers_count;
                cin.ignore();

                auto answers = new String[answers_count];

                for (int h = 0; h < answers_count; h++) {
                    char label = 'A' + h;
                    cout << "Enter answer option " << label << ": ";
                    cin.getline(options, 256);
                    answers[h] = String(options);
                }

                cout << "How many correct answers are there? ";
                cin >> correct_count;
                cin.ignore();


                char* correct_answers = new char[correct_count];

                for (int m = 0; m < correct_count; ) {
                    cout << "Enter correct answer " << (m + 1) << " (must be one of ";
                    for (int j = 0; j < answers_count; ++j) {
                        cout << static_cast<char>('A' + j);
                        if (j < answers_count - 1) cout << ", ";
                    }
                    cout << "): ";

                    char Input[256];
                    cin.getline(Input, 256);

                    String input_(Input);

                    if (input_.size() == 1) {
                        char letter = input_[0];
                        bool valid = false;
                        for (int j = 0; j < answers_count; ++j) {
                            if (letter == ('A' + j)) {
                                valid = true;
                                break;
                            }
                        }

                        if (valid) {
                            correct_answers[m] = letter;
                            ++i;
                        } else {
                            cout << "Invalid answer. Please enter a valid letter option.\n";
                        }
                    } else {
                        cout << "Invalid input. Enter exactly one letter.\n";
                    }
                }

                cout << "Enter points: ";
                cin >> points;
                cin.ignore();


                new_quiz->add_question(new MultipleChoiceQuestion(
                        String(description), points, answers, answers_count, correct_answers, correct_count));

                delete[] correct_answers;
            }



            else if(strings_equal(qtype, "ShA")){

                char correct_answer[256];

                cout << "Enter description: ";
                cin.getline(description, 256);

                cout<<"Enter correct answer: ";
                cin.getline(correct_answer,256);

                cout << "Enter points: ";
                cin >> points;
                cin.ignore();

                new_quiz->add_question(new ShortAnswerQuestion(String(description), points, String(correct_answer)));

            }
            else if(strings_equal(qtype, "MP")) {

                int left, right;
                char options[256];
                char correct_pairs[256];


                cout << "Enter description: ";
                cin.getline(description, 256);

                cout << "Enter left column values count: ";
                cin>>left;

                auto answers_left = new String[left];

                cin.ignore(10000, '\n');

                for (int f = 0; f < left; f++) {
                    char label = 'A' + f; // Calculate the letter label
                    cout << "Enter value " << label << ": ";
                    cin.getline(options, 256);
                    answers_left[f] = String(options);
                }


                cout << "Enter right column values count: ";
                cin>>right;


                auto answers_right = new String[right];

                cin.ignore(10000, '\n');

                for (int g = 0; g < right; g++) {
                    char label = 'a' + g; // Calculate the letter label
                    cout << "Enter value " << label << ": ";
                    cin.getline(options, 256);
                    answers_right[g] = String(options);
                }

                cout<<"Enter correct pairs: ";
                cin.getline(correct_pairs, 256);


                cout << "Enter points: ";
                cin >> points;
                cin.ignore();


                new_quiz->add_question(new MatchingPairsQuestion(String(description), points, answers_left, answers_right, left, right, String(correct_pairs)));

            }
            else {
                cout<<"Wrong question type. Enter again:"<<endl;
                i--;
            }
        }

        add_pending_quiz(new_quiz);



        cout<<endl;
        cout << "Quiz \"" << new_quiz->title << "\" with ID " << new_quiz->id << " sent for admin approval"<<endl;
        cout<<endl;
    }

    else if (strings_equal(command, "pending") && is_admin_logged_in) {

        for(int i=0; i<pending_quizzes_count; i++){
            cout<<"["<<pending_quizzes[i]->id<<"] "<<pending_quizzes[i]->title<<" by "<<pending_quizzes[i]->creator<<endl;
        }

    }

    else if (strings_equal(command, "reject-quiz") && is_admin_logged_in) {

        reject_quiz (input);

    }

    else if (strings_equal(command, "approve-quiz") && is_admin_logged_in){

        approve_quiz (input);

    }

    else if (strings_equal(command, "remove-quiz") && is_admin_logged_in) {

        remove_quiz (input);

    }

    else if (strings_equal(command, "report-quiz") && is_player_logged_in){

        String reporter = players[logined_player_id]->username;
        report_quiz(input, reporter);

    }

    else if (strings_equal(command, "view-reports") && is_admin_logged_in){

        for(int i=0; i<admins[logined_admin_id]->reports_count; i++){
            cout<<admins[logined_admin_id]->reports[i]<<endl;
        }

    }

    else if (strings_equal(command, "ban") && is_admin_logged_in){

        ban_user(input);

    }


    else if (strings_equal(command, "messages") && is_player_logged_in){

        for(int i=0; i<players[logined_player_id]->messages_count; i++){
            cout<<players[logined_player_id]->messages[i]<<endl;
        }

    }


    else if (strings_equal(command, "view-challenges") && is_player_logged_in){

        view_uncompleted_challenges (players[logined_player_id]);

    }

    else if (strings_equal(command, "view-finished-challenges") && is_player_logged_in){

        for(int i=0; i<players[logined_player_id]->challenges_count; i++){
            cout<<players[logined_player_id]->challenges[i]<<endl;
        }

    }

    else if (strings_equal(command, "quizzes") && is_player_logged_in) {

        int i = 7;
        while (input[i] == ' ') ++i;

        if (input[i] == '\0') {
            for (int v = 0; v < approved_quizzes_count; v++) {
                cout << approved_quizzes[v]->id << " | "
                     << approved_quizzes[v]->title << " | "
                     << approved_quizzes[v]->creator << " | "
                     << approved_quizzes[v]->capacity << " Questions | "
                     << approved_quizzes[v]->likes << " Likes" << endl;
            }
        } else {
            show_quiz_with_id(input);
        }

    }


    else if (strings_equal(command, "view") && is_player_logged_in) {

        view_user (input);

    }



    else if (strings_equal(command, "like-quiz") && is_player_logged_in){

        String username = players[logined_player_id]->username;
        like_quiz (input, username);

    }

    else if (strings_equal(command, "unlike-quiz") && is_player_logged_in){

        String username = players[logined_player_id]->username;
        unlike_quiz (input, username);

    }

    else if (strings_equal(command, "add-to-favs") && is_player_logged_in){

        String username = players[logined_player_id]->username;
        add_to_favs (input, username);

        for(int i=0; i<players[logined_player_id]->favourite_quizzes_count; i++){
            cout<<players[logined_player_id]->favourite_quizzes[i]<<endl;
        }

    }


    else if (strings_equal(command, "remove-from-favs") && is_player_logged_in){

        String username = players[logined_player_id]->username;
        remove_from_favs (input, username);

        for(int i=0; i<players[logined_player_id]->favourite_quizzes_count; i++){
            cout<<players[logined_player_id]->favourite_quizzes[i]<<endl;
        }

    }


    else if (strings_equal(command, "start-quiz") && is_player_logged_in){

        String username = players[logined_player_id]->username;

        start_quiz(input, username);

    }



    else if (strings_equal(command, "help") && is_admin_logged_in) {

        cout << "Here is a list of the commands you can use:\n"
             << "• pending: shows all created quizzes that require administrator approval\n"
             << "• approve-quiz <quiz-id>: approves a quiz with the given ID\n"
             << "• reject-quiz <quiz-id> <reason>: rejects a quiz with the given ID and sends a short message to the quiz author explaining the reason\n"
             << "• view-reports: views all submitted reports\n"
             << "• remove-quiz <quiz-id> <reason>: removes an existing quiz with the given ID and sends a short message to the quiz author explaining the reason\n"
             << "• ban <username>: bans a user with the given username. Quizzes they created will remain in the system\n";


    }

    else if (strings_equal(command, "help") && is_player_logged_in) {

        cout << "Here is a list of user commands you can use:\n"
             << "• signup <first-name> <last-name> <username> <password1> <password2>: registers a new user\n"
             << "• view-profile: displays all user profile information\n"
             << "• view-challenges: displays all unfinished challenges and their progress\n"
             << "• view-finished-challenges: displays all completed challenges\n"
             << "• view <username>: displays all information about the specified user\n"
             << "• messages: shows all received messages from the system and administrators\n"
             << "• create-quiz: opens the quiz creation interface; once created, the quiz is sent for admin review and approval\n"
             << "• quizzes: shows all quizzes on the platform with title, author, number of questions, and number of matches\n"
             << "• quizzes <username>: shows all quizzes created by the specified user\n"
             << "• like-quiz <quiz-id>: likes the specified quiz\n"
             << "• unlike-quiz <quiz-id>: removes your like from the specified quiz\n"
             << "• add-to-favs <quiz-id>: adds the specified quiz to your favorites\n"
             << "• remove-from-favs <quiz-id>: removes the specified quiz from your favorites\n"
             << "• start-quiz <quiz-id> test|normal (shuffle): starts the quiz in test or normal mode; optionally enables question shuffling\n"
             << "• save-quiz <quiz-id> <filepath>: saves the quiz questions to a text file in a suitable format\n"
             << "• report-quiz <quiz-id>: reports the specified quiz to an administrator; the report includes the date and the reason\n";

    }

    else if (strings_equal(command, "logout")) {

        is_admin_logged_in = false;
        is_player_logged_in = false;
    }

    else {
        cout << "Unknown command!" << endl;
    }

}