//
//  Assignment.cpp
//  BBGradeCalc
//
//  A single assignment in the gradebook.
//
//  Created by Austin Mayes on 11/22/19.
//  Copyright © 2019 Austin Mayes. All rights reserved.
//

#include "Assignment.hpp"

class Assignment {
public:
    int id;
    std::string name;
    double current_grade;
    double total_grade;
   
    /// Constructor
    ///
    /// If the gradebook does not contain data for the specified field, -1 should be used to denote empty data.
    ///
    /// @param id of the assignment in the Blackboard database
    /// @param name of the assignment
    /// @param current_grade of the assignment in the grade book
    /// @param total_grade possible for the assignment
    Assignment(int id, std::string name, double current_grade, double total_grade) {
        this->id = id;
        this->name = name;
        this->current_grade = current_grade;
        this->total_grade = total_grade;
    }
    
    /// Determine if the assignment has a grade for it
    bool is_graded() {
        return current_grade > -1;
    }
    
    /// Determine if the assignment should be considered as extra credit
    bool is_extra_credit() {
        return is_graded() && total_grade == -1;
    }
    
    /// Get the current percentage that the assignment represents
    double percentage() {
        if (!is_graded()) {
            throw std::logic_error("tried to get percentage of ungraded assignment");
        }
        if (is_extra_credit()) {
            return current_grade * 100;
        }
        return current_grade / total_grade;
    }
    
    /// Describe all attributes of this assignment
    std::string describe() {
        std::string res = "Assignment {";
        res += "\n  ID: " + std::to_string(id);
        res += "\n  Name: " + name;
        res += "\n  Raw Current: " + std::to_string(current_grade);
        res += "\n  Raw Total: " + std::to_string(total_grade);
        res += "\n  Graded?: " + misc_utils::stringify_bool(is_graded());
        res += "\n  Extra Credit?: " + misc_utils::stringify_bool(is_extra_credit());
        if (is_graded()) {
            res += "\n  Calculated Percentage: " + std::to_string(percentage() * 100) + "%";
        }
        return res + "\n}";
    }
};
