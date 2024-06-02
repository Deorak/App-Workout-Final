#pragma once

class Exercise {
    friend std::ostream& operator<<(std::ostream& os, const Exercise& obj) {
        os << " " << obj.ex_name << " " << obj.ex_sets << " " << obj.ex_reps << " " << obj.ex_weight << " ";
        return os;
    }
private:
    std::string ex_name;
    int ex_sets;
    int ex_reps;
    int ex_weight;

public:
    Exercise(std::string ex_name = "N/A", int ex_sets = 0, int ex_reps = 0, int ex_weight = 0) :
        ex_name(ex_name), ex_sets(ex_sets), ex_reps(ex_reps), ex_weight(ex_weight) {};

    bool operator==(const Exercise& rhs)const {
        return (this->ex_name == rhs.ex_name);
    }

    std::string get_name() {
        return ex_name;
    }

    int get_sets() {
        return ex_sets;
    }

    int get_reps() {
        return ex_reps;
    }

    int get_weight() {
        return ex_weight;
    }

    void set_name(std::string name) {
        ex_name = name;
    }

    void set_sets(int sets) {
        ex_sets = sets;
    }

    void set_reps(int reps) {
        ex_reps = reps;
    }

    void set_weight(int weight) {
        ex_weight = weight;
    }

    void set_all_stats() {
        int sets{}, reps{}, weight{};
        std::cout << "For: " << ex_name << "\n";
        std::cout << "Sets: ";
        std::cin >> sets;
        this->ex_sets = sets;
        std::cout << "Reps: ";
        std::cin >> reps;
        this->ex_reps = reps;
        std::cout << "Weight: ";
        std::cin >> weight;
        this->ex_weight = weight;
    }

};




class Body_part {
    friend std::ostream& operator<<(std::ostream& os, const Body_part& obj) {
        os << obj.body_part_name << " ";
        for (auto l : obj.the_list) {
            os << l.get_name() << " ";
        }
        os << "\n";
        return os;
    }
private:
    std::string body_part_name;
    std::vector<Exercise> the_list;
public:
    Body_part(std::string body_part_name = "N/A") :body_part_name(body_part_name) {};
    ~Body_part() {};

    bool operator==(const Body_part& rhs) const {
        return (this->body_part_name == rhs.body_part_name);
    }

    std::string& get_name() {
        return body_part_name;
    }
    std::vector<Exercise>& get_list() {
        return the_list;
    }

    void set_name(std::string name) {
        body_part_name = name;
    }

    void set_exercise_list() {
        for (auto& t : the_list)
            t.set_all_stats();
    }

    void fill_exercise_vector(std::vector<Exercise> temp_v) {
        for (auto t : temp_v) {
            this->the_list.push_back(t);
        }
    }

    void add_new_exercise(Exercise exercise_name) {

        this->the_list.push_back(exercise_name);

    }

    void remove_at_index(int pos) {
        if (pos < this->get_list().size()) {

            this->the_list.erase(this->get_list().begin() + pos);
            std::cout << "deleted\n";
        }
        else {
            std::cout << "invalid";
        }
    }



    void print_exercise_vector() {
        if (this->the_list.size() == 0) {
            std::cout << "List is empty";
        }
        else {
            for (auto t : the_list) {
                std::cout << "\n" << "E " << t.get_name() << " ";
                std::cout << t.get_sets() << " ";
                std::cout << t.get_reps() << " ";
                std::cout << t.get_weight() << " ";
            }
        }
    }

    void write_exercise_vector_to_logbook(std::fstream& logbook) {
        for (auto t : the_list) {
            logbook << "\n" << "E " << t.get_name() << " ";
            logbook << t.get_sets() << " ";
            logbook << t.get_reps() << " ";
            logbook << t.get_weight() << " ";
        }
    }

    void clear_vector() {
        this->the_list.clear();
    }
};



