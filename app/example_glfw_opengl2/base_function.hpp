#pragma once
#include "imgui.h"
#include "imgui_stdlib.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <Windows.h>
#include <mmsystem.h>
#include "allclass.hpp"

#pragma comment(lib, "winmm.lib")

#define Log(x) std::cout<<x<<std::endl

constexpr auto SEC_VAL = 60;







//Variables
int calculate_centi_s(int& ms) {
    int ttl{}, stl{};

    ttl = (ms / 100) % 10;
    stl = (ms / 10) % 10;
    return (ttl * 10) + stl;
}

void stopwatch_print(std::string& my_str, int& num) {
    if (num == 0) {
        my_str = { "00" };
    }
    if (num > 0 && num < 10) {
        my_str = { "0" };
        my_str += std::to_string(num);
    }
}

bool is_even(int& num) {
    if (num % 2 == 0) {
        return true;
    }
    else {
        return false;
    }
}


//Workout
void temp_print_whole(std::vector<Body_part>& temp_workout) {
    std::cout << "\n\n=====================================================";
    if (temp_workout.size() == 0) {
        std::cout << "she's empty";
    }
    else {
        std::cout << std::endl;
        for (auto& t : temp_workout) {
            std::cout << "\n" << t.get_name() << "\n";
            t.print_exercise_vector();
        }
    }
}

void load_workout_ptr(std::unique_ptr<std::vector<Body_part>>& workout, std::fstream& exercise_file) {

    std::stringstream my_stream;
    std::string line_str, temp_str;
    bool first = true;
    std::vector<Exercise> temp_ex_vec;
    Body_part temp_body;

    if (!exercise_file.is_open()) {
        Log("Could not open exercise file");
    }
    else {
        while (getline(exercise_file, line_str)) {
            my_stream.str(line_str);
            first = true;
            while (my_stream >> temp_str) {
                if (first == true) {
                    temp_body.set_name(temp_str);
                    first = false;
                }
                else {
                    temp_ex_vec.push_back(Exercise{ temp_str });
                }
            }
            temp_body.fill_exercise_vector(temp_ex_vec);
            workout->push_back(temp_body);

            temp_body.clear_vector();
            temp_ex_vec.clear();
            my_stream.clear();
        }
    }
}

void write_to_workout_file(std::unique_ptr<std::vector<Body_part>>& workout, std::fstream& exercise_file) {

    if (!exercise_file.is_open()) {
        Log("Could not open exercise file");
    }
    else {
        for (auto& w : *workout) {
            exercise_file << w;
        }
    }
}

void fill_exercise_vector(std::vector<Exercise>& temp_vec) {
    std::string exercise_name;

    std::cout << "\nExercise name: ";
    std::cin >> exercise_name;

    temp_vec.push_back({ exercise_name });
}

//Logbook
void fill_func(std::unique_ptr<std::map<std::chrono::year_month_day, std::vector<Body_part>>>& logbook_ptr, std::vector<Body_part >& temp_body_vec, std::chrono::year_month_day& temp_date)
{
    if (!temp_body_vec.empty()) {
        logbook_ptr->emplace(temp_date, temp_body_vec);
        temp_body_vec.clear();
    }
}

std::chrono::year_month_day date_func(std::stringstream& my_stream) {

    std::string temp_str;
    int year{}, i{};
    unsigned month{}, day{};

    while (my_stream >> temp_str) {
        if (i == 0) {
            year = std::stoi(temp_str);
            i++;
        }
        else if (i == 1) {
            month = std::stoul(temp_str);
            i++;
        }
        else if (i == 2) {
            day = std::stoul(temp_str);
            i++;
        }
    }
    std::chrono::year_month_day temp_date{ std::chrono::year{year}, std::chrono::month{month}, std::chrono::day{day} };

    return temp_date;
}

void body_func(std::vector<Body_part >& temp_body_vec, std::stringstream& my_stream) {
    std::string temp_str;

    my_stream >> temp_str;

    temp_body_vec.push_back({ temp_str });
}

void ex_func(std::vector<Body_part >& temp_body_vec, std::stringstream& my_stream) {

    std::string temp_str{}, name{};
    int i{}, sets{}, reps{}, weight{};

    while (my_stream >> temp_str) {
        if (i == 0) {
            name = temp_str;
            i++;
        }
        else if (i == 1) {
            sets = std::stoi(temp_str);
            i++;
        }
        else if (i == 2) {
            reps = std::stoi(temp_str);
            i++;
        }
        else if (i == 3) {
            weight = std::stoi(temp_str);
            i++;
        }
    }
    std::vector<Exercise>temp_ex_vec{ {name, sets, reps, weight} };
    temp_body_vec.at(temp_body_vec.size() - 1).fill_exercise_vector(temp_ex_vec);
}

void load_logbook_ptr(std::unique_ptr<std::map<std::chrono::year_month_day, std::vector<Body_part>>>& logbook_ptr, std::fstream& logbook) {

    std::string line_str, temp_str;
    std::stringstream my_stream;

    std::vector<Body_part > temp_body_vec;
    std::chrono::year_month_day temp_date{};

    if (!logbook.is_open()) {
        Log("Could not open exercise file");
    }
    else {
        while (getline(logbook, line_str)) {
            my_stream.str(line_str);
            while (my_stream >> temp_str) {
                if (temp_str == "D") {
                    fill_func(logbook_ptr, temp_body_vec, temp_date);
                    temp_date = date_func(my_stream);
                }
                else if (temp_str == "B") {
                    body_func(temp_body_vec, my_stream);
                }
                else if (temp_str == "E") {
                    ex_func(temp_body_vec, my_stream);
                }
            }
            my_stream.clear();
        }
        if (logbook.eof()) {
            fill_func(logbook_ptr, temp_body_vec, temp_date);
        }
    }
}


void write_to_logbook_file(std::unique_ptr<std::map<std::chrono::year_month_day, std::vector<Body_part>>>& logbook_ptr, std::fstream& logbook) {


    if (!logbook.is_open()) {
        Log("Could not open exercise file");
    }
    else {
        for (auto& i : *logbook_ptr) {
            logbook << "\n" << "D " << i.first.year() << " " << static_cast<unsigned>(i.first.month()) << " " << static_cast<unsigned>(i.first.day()) << "\n";
            for (auto j : i.second) {
                logbook << "B " << j.get_name();
                j.write_exercise_vector_to_logbook(logbook);
                logbook << "\n";
            }
        }
        logbook << "\n";
    }
}

//Notes
void load_notes_buff(char* buff, std::fstream& notes) {

    std::string line{};

    if (!notes.is_open()) {
        Log("Could not open notes");
    }
    else {
        while (getline(notes, line)) {
            if (line.size() == 0) {
                strcat(buff, "\n");
            }
            else {
                strcat(buff, line.c_str());
                strcat(buff, "\n");
            }
        }
    }

}

void write_to_notes_file(char* buff, std::fstream& notes) {

    std::string s_buff = buff;

    if (!notes.is_open()) {
        Log("Could not open notes");
    }
    else {
        for (auto s : s_buff) {
            notes << s;
        }
    }
}

//GUI functions

int find_index(std::vector<Body_part>& temp_workout, std::unique_ptr<std::vector<Body_part>>& workout, int row_n) {

    //finds out at what index in temp vec workout->at(row_n).get_name() is at
    auto it = std::find(temp_workout.begin(), temp_workout.end(), workout->at(row_n).get_name());

    return std::distance(temp_workout.begin(), it);
}

int find_ex_index(std::vector<Body_part>& temp_workout, std::unique_ptr<std::vector<Body_part>>& workout, int row_n, int n) {

    //same but for the ex vector
    auto it = std::find(temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().begin(), temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().end(), workout->at(row_n).get_list().at(n));

    return std::distance(temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().begin(), it);
}


void add_temp_workout(std::vector<Body_part>& temp_workout, std::unique_ptr<std::vector<Body_part>>& workout, int row_n, int n) {

    //find body part name in temp vec
    auto it_body = std::find(temp_workout.begin(), temp_workout.end(), workout->at(row_n).get_name());

    if (it_body == temp_workout.end()) {
        temp_workout.push_back(workout->at(row_n).get_name());
        temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().push_back(workout->at(row_n).get_list().at(n).get_name());
    }
    else {
        //if body part was found find if exercise exists in list
        auto it_ex = std::find(temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().begin(), temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().end(), workout->at(row_n).get_list().at(n).get_name());

        if (it_ex == temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().end()) {
            temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().push_back(workout->at(row_n).get_list().at(n).get_name());
        }
        else {
            std::cout << "\nis already in";
        }
    }
}


void remove_temp_workout(std::vector<Body_part>& temp_workout, std::unique_ptr<std::vector<Body_part>>& workout, int row_n, int n) {

    //find ody part name in temp vec
    auto it_body = std::find(temp_workout.begin(), temp_workout.end(), workout->at(row_n).get_name());

    if (it_body == temp_workout.end()) {
        std::cout << "couldn't find body part" << std::endl;
    }
    else {
        //if body part was found, find if exercise exists in list
        auto it_ex = std::find(temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().begin(), temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().end(), workout->at(row_n).get_list().at(n).get_name());
        if (it_ex == temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().end()) {
            std::cout << "couldn't find exercise" << std::endl;
        }
        else {
            temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().erase(temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().begin() + find_ex_index(temp_workout, workout, row_n, n));

        }
    }
    if (temp_workout.at(find_index(temp_workout, workout, row_n)).get_list().size() == 0) {
        temp_workout.erase(temp_workout.begin() + find_index(temp_workout, workout, row_n));
    }
}

void print_logbook_contents(std::unique_ptr<std::map<std::chrono::year_month_day, std::vector<Body_part>>>& logbook_ptr) {

    std::stringstream my_stream;
    std::string str;

    for (auto& log : *logbook_ptr) {

        my_stream << log.first;
        my_stream >> str;
        my_stream.clear();
        str.insert(0, "\n");
        ImGui::Text(str.c_str());

        for (auto& l : log.second) {
            str = l.get_name();
            ImGui::Text(str.c_str());
            for (auto& list : l.get_list()) {
                str = list.get_name() + " Sets:" + std::to_string(list.get_sets()) + " Reps: " + std::to_string(list.get_reps()) + " Weight: " + std::to_string(list.get_weight()) + "\n";
                ImGui::Text(str.c_str());
            }
        }
    }

}

struct img_struct {
    GLuint my_image_texture;
    int my_image_width;
    int my_image_height;
};

//GUI base function
void base_function(std::unique_ptr<std::map<std::chrono::year_month_day, std::vector<Body_part>>>& logbook_ptr,

    std::unique_ptr<std::vector<Body_part>>& workout, std::vector<Body_part>& temp_workout, std::fstream& exercise_file,
    std::fstream& logbook, std::fstream& notes, std::vector<ImFont*>fonts, std::vector<img_struct>& images) {

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 9.f;
    style.FramePadding.x = 15.f;
    style.FramePadding.y = 6.f;
    ImGui::PushFont(fonts.at(0));

    static ImGuiTableFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

    ImVec2 center = ImGui::GetMainViewport()->GetWorkCenter();
    ImVec2 viewportPos = ImGui::GetMainViewport()->Pos;

    ImGuiIO& io = ImGui::GetIO();


    //Workout block

    ImGui::SetWindowSize("window_right", ImVec2(540, io.DisplaySize.y));
    ImGui::SetWindowPos("window_right", ImVec2(viewportPos.x+1380, viewportPos.y));
    
    ImGui::Begin("window_right", nullptr, window_flags);


    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_ScrollY;


    ImGui::BeginTabBar("LC");
    
    if (ImGui::BeginTabItem("Workout List")) {
       
        ImVec2 buttonSize(100.f, 0.f);
        ImVec2 contentRegion = ImGui::GetContentRegionAvail();

        float buttonX = contentRegion.x - buttonSize.x;
        float buttonSaveChanges = contentRegion.x - buttonSize.x;
        float buttonAddToTemp = contentRegion.x - buttonSize.x;

        if (ImGui::BeginTable("table_sorting", 3, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 44.5), 0.0f))
        {
            // Declare columns
            ImGui::TableSetupColumn("Body_part", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
            ImGui::TableSetupColumn("Edit exercises", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
            ImGui::TableSetupColumn("Exercise", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
            ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
            ImGui::TableHeadersRow();

            // Using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(workout->size());

            while (clipper.Step())
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                {
                    // Display a data item
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(workout->at(row_n).get_name().c_str());

                    ImGui::TableNextColumn();
                    if (ImGui::SmallButton(("Edit##" + std::to_string(row_n)).c_str()))
                        ImGui::OpenPopup(("Edit##" + std::to_string(row_n)).c_str());

                    if (ImGui::BeginPopupModal(("Edit##" + std::to_string(row_n)).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        //Adding new exercise
                        ImGui::SeparatorText("What for entering new exercise");

                        static std::string exercise_name;
                        static char buffer[256] = { 0 };

                        ImGui::Spacing();
                        if (ImGui::InputText("##", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                            exercise_name = buffer;
                            ImGui::SetKeyboardFocusHere(-1);
                        }

                        if (ImGui::Button("Add", ImVec2(120, 0))) {
                            exercise_name = buffer;
                            workout->at(row_n).add_new_exercise({ exercise_name });
                            buffer[0] = '\0';
                        }

                        //Delete existing exercise
                        ImGui::Spacing();
                        ImGui::SeparatorText("Delete highlighted exercise");

                        static int selected = -1;
                        ImGui::Spacing();
                        for (int n = 0; n < workout->at(row_n).get_list().size(); n++)
                        {

                            if (ImGui::Selectable(workout->at(row_n).get_list().at(n).get_name().c_str(), selected == n, ImGuiSelectableFlags_DontClosePopups))
                            {
                                selected = n;
                            }
                        }

                        ImGui::Spacing();
                        if (ImGui::Button("Delete", ImVec2(120, 0))) {

                            workout->at(row_n).remove_at_index(selected);

                        }

                        ImGui::SetItemDefaultFocus();
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(buttonX);
                        if (ImGui::Button("Done", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                        ImGui::EndPopup();
                    }

                    ImGui::TableNextColumn();
                    for (auto& wtf : workout->at(row_n).get_list()) {
                        ImGui::TextUnformatted(wtf.get_name().c_str());
                    }
                }

            ImGui::EndTable();
        }

        //Add body part
        if (ImGui::Button("Add body part"))
            ImGui::OpenPopup("Add body part");


        if (ImGui::BeginPopupModal("Add body part", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Spacing();
            ImGui::SeparatorText("");
            ImGui::Spacing();

            static char buffer[256] = { 0 };
            std::string body_part_name;

            if (ImGui::InputText("##", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                body_part_name = buffer;
                ImGui::SetKeyboardFocusHere(-1);
            }

            ImGui::Spacing();

            if (ImGui::Button("Add", ImVec2(60, 0))) {
                body_part_name = buffer;
                workout->push_back({ body_part_name });
                buffer[0] = '\0';
            }

            ImGui::SameLine();
            ImGui::SetCursorPosX(219);
            if (ImGui::Button("Done", ImVec2(60, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        //Remove body part
        ImGui::SameLine();
        if (ImGui::Button("Remove body part"))
            ImGui::OpenPopup("Remove body part");

        if (ImGui::BeginPopupModal("Remove body part", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Spacing();
            ImGui::SeparatorText("");

            static int selected = -1;

            ImGui::Spacing();
            for (int n = 0; n < workout->size(); n++)
            {
                if (ImGui::Selectable(workout->at(n).get_name().c_str(), selected == n, ImGuiSelectableFlags_DontClosePopups))
                {
                    selected = n;
                }
            }

            ImGui::Spacing();
            if (ImGui::Button("Remove", ImVec2(120, 0))) {
                workout->erase(workout->begin() + selected);
            }

            ImGui::SameLine();
            if (ImGui::Button("Done", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        //Save changes
        ImGui::SameLine();
        ImGui::SetCursorPosX(402.f);
        if (ImGui::Button("Save Changes", ImVec2(120.f, 0.f))) {

            exercise_file.close();
            exercise_file.open("Wexercise_list.txt", std::fstream::out);
            exercise_file.close();
            exercise_file.open("Wexercise_list.txt", std::fstream::in | std::fstream::out | std::fstream::app);
            write_to_workout_file(workout, exercise_file);
        }
        ImGui::EndTabItem();
    }





    if (ImGui::BeginTabItem("Current Workout")) {
      
        ImGui::SeparatorText("Choose exercise from workout list");
        if (ImGui::BeginTable("table_sorting_list", 3, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 14.9), 0.0f))
        {
            // Declare columns
            ImGui::TableSetupColumn("Body_part", ImGuiTableColumnFlags_WidthFixed, 100.0f, 0);
            ImGui::TableSetupColumn("Exercise", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
            ImGui::TableSetupScrollFreeze(0, 0); // Make row always visible
            ImGui::TableHeadersRow();

            // Using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(workout->size());

            while (clipper.Step())
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                {
                    // Display a data item
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(workout->at(row_n).get_name().c_str());

                    ImGui::TableNextColumn();
                    ImGui::Spacing();
                    static int selected = -1;
                    ImGui::Spacing();
                    for (int n = 0; n < workout->at(row_n).get_list().size(); n++)
                    {
                        ImGui::TextUnformatted(workout->at(row_n).get_list().at(n).get_name().c_str());

                        ImGui::SameLine();
                        ImGui::SetCursorPosX(300.f);
                        if (ImGui::SmallButton(("Add##" + workout->at(row_n).get_list().at(n).get_name()).c_str())) {

                            add_temp_workout(temp_workout, workout, row_n, n);

                        }
                        ImGui::SameLine();
                        if (ImGui::SmallButton(("Remove##" + workout->at(row_n).get_list().at(n).get_name()).c_str())) {

                            remove_temp_workout(temp_workout, workout, row_n, n);
                        }

                    }
                    ImGui::TableNextColumn();
                }
            ImGui::EndTable();
        }

        ImGui::SeparatorText("Today's workout");

        if (ImGui::BeginTable("table_sorting", 2, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 14.9), 0.0f))
        {
            // Declare columns
            ImGui::TableSetupColumn("Body_part", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
            ImGui::TableSetupColumn("Exercise", ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
            ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
            ImGui::TableHeadersRow();

            // Using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(temp_workout.size());

            static int i_reps{};
            static int i_sets{};
            static int i_weight{};

            while (clipper.Step())
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                {

                    // Display a data item
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(temp_workout.at(row_n).get_name().c_str());

                    ImGui::TableNextColumn();
                    static int selected = -1;
                    ImGui::Spacing();

                    for (int n = 0; n < temp_workout.at(row_n).get_list().size(); n++)
                    {

                        char buffer[256]{};

                        strcat(buffer, temp_workout.at(row_n).get_list().at(n).get_name().c_str());
                        strcat(buffer, ": sets-");
                        strcat(buffer, std::to_string(temp_workout.at(row_n).get_list().at(n).get_sets()).c_str());
                        strcat(buffer, " reps-");
                        strcat(buffer, std::to_string(temp_workout.at(row_n).get_list().at(n).get_reps()).c_str());
                        strcat(buffer, " weight-");
                        strcat(buffer, std::to_string(temp_workout.at(row_n).get_list().at(n).get_weight()).c_str());

                        ImGui::TextUnformatted(buffer);

                        ImGui::SameLine();
                        ImGui::SetCursorPosX(370.f);
                        if (ImGui::SmallButton(("Add##" + temp_workout.at(row_n).get_list().at(n).get_name()).c_str())) {

                            temp_workout.at(row_n).get_list().at(n).set_sets(i_sets);
                            temp_workout.at(row_n).get_list().at(n).set_reps(i_reps);
                            temp_workout.at(row_n).get_list().at(n).set_weight(i_weight);
                        }

                        ImGui::SameLine();
                        if (ImGui::SmallButton(("Clear##" + temp_workout.at(row_n).get_list().at(n).get_name()).c_str())) {

                            temp_workout.at(row_n).get_list().at(n).set_sets(0);
                            temp_workout.at(row_n).get_list().at(n).set_reps(0);
                            temp_workout.at(row_n).get_list().at(n).set_weight(0);

                        }
                    }

                }
            ImGui::EndTable();


            ImGui::SeparatorText("Sets reps weight");
            ImGui::PushItemWidth(120);
            ImGui::SetCursorPosX(180);
            ImGui::InputInt("Input Sets", &i_sets);
            ImGui::SetCursorPosX(180);
            ImGui::InputInt("Input Reps", &i_reps);
            ImGui::SetCursorPosX(180);
            ImGui::InputInt("Input Weight", &i_weight);
            ImGui::PopItemWidth();

            ImGui::SetCursorPosX(420);
            if (ImGui::Button("Set all to 0")) {
                i_sets = 0;
                i_reps = 0;
                i_weight = 0;
            }
            ImGui::SeparatorText("Date");

            const std::chrono::time_point chro_now{ std::chrono::system_clock::now() };
            const std::chrono::year_month_day chro_today{ std::chrono::floor<std::chrono::days>(chro_now) };


            const char* days[] = { "1", "2", "3","4","5" ,"6" ,"7" ,"8" ,"9" ,"10" ,"11" ,"12" ,"13" ,"14" ,"15" ,"16" ,"17" ,"18" ,"19" ,"20" ,"21" ,"22" ,"23" ,"24" ,"25" ,"26" ,"27" ,"28" ,"29" ,"30" ,"31" };
            static int days_current_index = static_cast<unsigned>(chro_today.day()) - 1;
            const char* days_preview_value = days[days_current_index];

            const char* months[] = { "January", "February", "March","April","May" ,"June" ,"July" ,"August" ,"September" ,"October" ,"November" ,"December" };
            static int months_current_index = static_cast<unsigned>(chro_today.month()) - 1;
            const char* months_preview_value = months[months_current_index];

            const char* years[] = { "2024", "2025", "2026", "2027", "2028", "2029", "2030" };
            static int years_current_index = static_cast<int>(chro_today.year()) - 2024;
            const char* years_preview_value = years[years_current_index];

            static int log_year = static_cast<int>(chro_today.year());
            static unsigned log_month = static_cast<unsigned>(chro_today.month()), log_day = static_cast<unsigned>(chro_today.day());



            ImGui::SetNextItemWidth(90);
            ImGui::SetCursorPosX(80);
            if (ImGui::BeginCombo("Day", days_preview_value, ImGuiComboFlags_HeightLargest))
            {
                for (int n = 0; n < IM_ARRAYSIZE(days); n++)
                {
                    const bool is_selected = (days_current_index == n);
                    if (ImGui::Selectable(days[n], is_selected))
                        days_current_index = n;
                    log_day = days_current_index + 1;

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SetNextItemWidth(90);
            ImGui::SameLine();
            if (ImGui::BeginCombo("Month", months_preview_value, ImGuiComboFlags_HeightLargest))
            {
                for (int n = 0; n < IM_ARRAYSIZE(months); n++)
                {
                    const bool is_selected = (months_current_index == n);
                    if (ImGui::Selectable(months[n], is_selected))
                        months_current_index = n;
                    log_month = months_current_index + 1;

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SetNextItemWidth(90);
            ImGui::SameLine();
            if (ImGui::BeginCombo("Year", years_preview_value, ImGuiComboFlags_HeightLargest))
            {
                for (int n = 0; n < IM_ARRAYSIZE(years); n++)
                {
                    const bool is_selected = (years_current_index == n);
                    if (ImGui::Selectable(years[n], is_selected))
                        years_current_index = n;
                    log_year = years_current_index + 2024;

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SetCursorPosX(195);
            if (ImGui::Button("Save to logbook")) {
                std::chrono::year_month_day log_date{ std::chrono::year{log_year}, std::chrono::month{log_month}, std::chrono::day{log_day} };

                logbook_ptr->emplace(log_date, temp_workout);

                logbook.close();
                logbook.open("Wlogbook.txt", std::fstream::out);
                logbook.close();
                logbook.open("Wlogbook.txt", std::fstream::in | std::fstream::out | std::fstream::app);
                write_to_logbook_file(logbook_ptr, logbook);
            }
            ImGui::SetCursorPosX(150);
            if (ImGui::Button("Delete workout at current date")) {
                std::chrono::year_month_day log_date{ std::chrono::year{log_year}, std::chrono::month{log_month}, std::chrono::day{log_day} };

                logbook_ptr->erase(log_date);
            }
        }
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();






    ImGui::SetWindowSize("window_middles", ImVec2(io.DisplaySize.x-1040, io.DisplaySize.y));
    ImGui::SetWindowPos("window_middles", ImVec2(viewportPos.x+500, viewportPos.y));

    ImGui::Begin("window_middles", nullptr, window_flags);

    ImVec2 windowPos = ImGui::GetWindowViewport()->Pos;



    static auto stopwatch_start = std::chrono::system_clock::now();
    auto now = std::chrono::system_clock::now();

    std::chrono::system_clock::duration stopwatch_duration = now - stopwatch_start;

    static bool is_started = false;
    static bool is_paused = false;

    static std::string s_hours{};
    static std::string s_minutes{};
    static std::string s_seconds{};
    static std::string s_centi{};

    static int hours{};
    static int minutes{};
    static int seconds{};
    static int centi{};

    static int main_ms{};
    static int pause_ms{};

    static int button_clicked{ -1 };
    std::string start_pause_resume{ "Start" };
    static int image_s{};




    if (is_started == true && is_paused == false) {

        main_ms = std::chrono::duration_cast<std::chrono::milliseconds>(stopwatch_duration).count() + pause_ms;
        centi = calculate_centi_s(main_ms);
        if (main_ms >= 1000) {
            pause_ms = 0;
            seconds++;
            if (seconds >= SEC_VAL) {
                seconds = 0;
                minutes++;
                if (minutes >= 60) {
                    minutes = 0;
                    hours++;
                }
            }
            stopwatch_start = std::chrono::system_clock::now();
        }
    }

    if (is_started == false) {
        hours = 0;
        minutes = 0;
        seconds = 0;
        centi = 0;
    }

    if (is_even(button_clicked) == true) {
        start_pause_resume = "Pause";
        image_s = 1;
    }
    else {
        start_pause_resume = "Resume";
        image_s = 0;
    }

    if (button_clicked == -1) {
        image_s = 0;
        start_pause_resume = "Start";
        is_started = false;
        is_paused = false;
    }





    ImGui::PushFont(fonts.at(1));

    s_hours = std::to_string(hours);
    s_minutes = std::to_string(minutes);
    s_seconds = std::to_string(seconds);
    s_centi = std::to_string(centi);

    stopwatch_print(s_hours, hours);
    stopwatch_print(s_minutes, minutes);
    stopwatch_print(s_seconds, seconds);
    stopwatch_print(s_centi, centi);




    auto stopwatch_window = ImGui::GetWindowSize().x;
    auto stopwatch_text = ImGui::CalcTextSize(s_hours.c_str()).x + ImGui::CalcTextSize(s_minutes.c_str()).x + ImGui::CalcTextSize(s_seconds.c_str()).x + ImGui::CalcTextSize(s_centi.c_str()).x;


    ImGui::SetCursorPos(ImVec2((stopwatch_window - stopwatch_text - 60.f)*0.49f, 50.f));

    ImGui::Text("%s : %s : %s.", s_hours.c_str(), s_minutes.c_str(), s_seconds.c_str(), s_centi.c_str());
    ImGui::PopFont();

    ImGui::PushFont(fonts.at(2));
    ImGui::SameLine();
    ImGui::SetCursorPosY(125);
    ImGui::Text("%s ", s_centi.c_str());
    ImGui::PopFont();


  
    ImGui::SetCursorPosX(390);
    ImGui::SetCursorPosY(228);
    if (ImGui::ImageButton((void*)(intptr_t)images.at(image_s).my_image_texture, ImVec2(images.at(image_s).my_image_width, images.at(image_s).my_image_height))) {
        if (start_pause_resume == "Start") {
            stopwatch_start = std::chrono::system_clock::now();
            is_started = true;
        }
        if (start_pause_resume == "Resume") {
            stopwatch_start = std::chrono::system_clock::now();
            is_started = true;
            is_paused = false;
        }
        if (start_pause_resume == "Pause") {
            is_paused = true;
            pause_ms = main_ms;
        }
        button_clicked++;
    }

    ImGui::SameLine();
    ImGui::SetCursorPosY(257);
    if (ImGui::ImageButton((void*)(intptr_t)images.at(2).my_image_texture, ImVec2(images.at(2).my_image_width, images.at(2).my_image_height))) {
        button_clicked = -1;
    }






    static int beep_minutes{}, beep_seconds{}, i_beep_minutes{}, i_beep_seconds{};
    static bool set_alarm = false, has_beeped = false, beep_once = false, alarm_check = false;

    static int initial_seconds{}, total_seconds{};
    static int when_beep_min{}, when_beep_sec{};


    when_beep_min = total_seconds / SEC_VAL;
    when_beep_sec = total_seconds % SEC_VAL;



    //recurring
    if (minutes == when_beep_min && seconds == when_beep_sec) {
        has_beeped = false;
    }

    if (set_alarm == true && minutes == when_beep_min && seconds == when_beep_sec) {

        if (has_beeped == false) {
            PlaySound(TEXT("beep.wav"), NULL, SND_ASYNC);
            total_seconds += initial_seconds;
            has_beeped = true;
        }

    }

    //once
    if (beep_once == true && minutes == beep_minutes && seconds == beep_seconds) {
        if (has_beeped == false) {
            PlaySound(TEXT("beep.wav"), NULL, SND_ASYNC);
            has_beeped = true;
        }
    }



    ImGui::SetCursorPosY(325);
    ImGui::SeparatorText("##");


    ImGui::SetCursorPosX(70);
    ImGui::SetCursorPosY(380);
    ImGui::PushFont(fonts.at(2));
    if (set_alarm == true) {
        ImGui::Text("Alarm set for every: %dm - %ds", beep_minutes, beep_seconds);
    }
    else if (beep_once == true) {
        ImGui::Text("Alarm set for: %dm - %ds", beep_minutes, beep_seconds);
    }
    else {
        ImGui::Text("Alarm not set");
    }
    ImGui::PopFont();



    ImGui::PushItemWidth(110);
    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(430);
    ImGui::InputInt("Minutes", &i_beep_minutes);

    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(465);
    ImGui::InputInt("Seconds", &i_beep_seconds);
    ImGui::PopItemWidth();



    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(505);
    if (ImGui::Button("Set alarm")) {

        set_alarm = false;
        beep_once = false;
        has_beeped = false;

        initial_seconds = 0;
        total_seconds = 0;

        //recurring
        if (alarm_check == true && (i_beep_minutes > 0 || i_beep_seconds > 0)) {
            set_alarm = true;
            beep_minutes = i_beep_minutes;
            beep_seconds = i_beep_seconds;
            initial_seconds = (beep_minutes * SEC_VAL) + beep_seconds;

            if (initial_seconds < (minutes * SEC_VAL) + seconds) {
                total_seconds = initial_seconds + (minutes * SEC_VAL) + seconds;
            }
        }

        //once
        if (alarm_check == false && (i_beep_minutes > 0 || i_beep_seconds > 0)) {
            beep_once = true;
            beep_minutes = i_beep_minutes;
            beep_seconds = i_beep_seconds;
            initial_seconds = (beep_minutes * SEC_VAL) + beep_seconds;
        }
    }

    ImGui::SameLine();
    ImGui::Checkbox("Recurring", &alarm_check);



    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(540);
    if (ImGui::Button("Reset alarms")) {
        set_alarm = false;
        beep_once = false;
        has_beeped = false;
        beep_minutes = 0;
        beep_seconds = 0;
        total_seconds = 0;
        initial_seconds = 0;
        when_beep_min = 0;
        when_beep_sec = 0;
    }

   

    static int rep_index{};
    static int rep_inc{};


    ImGui::PushFont(fonts.at(2));
    ImGui::SetCursorPosX(70);
    ImGui::SetCursorPosY(600);
    ImGui::Text("Reps: %d", rep_index);
    ImGui::PopFont();


    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(650);
    if (ImGui::Button("-")) {
        rep_index -= rep_inc;
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(44);
    ImGui::InputInt("##", &rep_inc, 0);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::Button("+")) {
        rep_index += rep_inc;
    }

    ImGui::SetCursorPosX(50);
    ImGui::SetCursorPosY(685);
    if (ImGui::Button("Reset counter## +rep_inc")) {
        rep_index = 0;
        rep_inc = 0;
    }
    ImGui::End();

  



    ImGui::SetWindowSize("window_left", ImVec2(500, io.DisplaySize.y));
    ImGui::SetWindowPos("window_left", ImVec2(viewportPos.x, viewportPos.y));

    ImGui::Begin("window_left", nullptr, window_flags);
    
    ImGui::BeginTabBar("LN");
   

    if (ImGui::BeginTabItem("Print Logbook"))
    {

        if (ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 55), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY)) {

            print_logbook_contents(logbook_ptr);

        }
        ImGui::EndChild();
        ImGui::EndTabItem();
    }


   if( ImGui::BeginTabItem("Notes"))
   {
        static char buff[1024];

    load_notes_buff(buff, notes);

    ImGui::InputTextMultiline("##source", buff, IM_ARRAYSIZE(buff), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 55), ImGuiInputTextFlags_AllowTabInput);

    if (ImGui::Button("Save")) {
        std::cout << buff;
        notes.close();
        notes.open("Wnotes.txt", std::fstream::out);
        notes.close();
        notes.open("Wnotes.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        write_to_notes_file(buff, notes);
    }

    ImGui::EndTabItem();
}
    ImGui::EndTabBar();
    ImGui::End();


    ImGui::PopFont();
}
