//
//  HTMLUtils.cpp
//  BBGradeCalc
//
//  Utilities for validating and parsing HTML documents.
//
//  Created by Austin Mayes on 11/14/19.
//  Copyright © 2019 Austin Mayes. All rights reserved.
//

#include "HTMLUtils.hpp"
namespace html_utils {
    /// A mapping of path descriptions to their corresponding parts of the full XPath
    std::pair<std::string, std::string> described_path[] = {
        {"base HTML root element", "/html"},
        {"main body tag", "body"},
        {"global navigation content area", "div[@id='globalNavPageContentArea']"},
        {"location pane", "div[@class='locationPane']"},
        {"main page content wrapper", "div[@role='main']"},
        {"shadow wrapper", "div[@class='shadow']"},
        {"edit mode wrapper", "div[@id='editmodeWrapper']"},
        {"main page content container", "div[@id='content']"},
        {"inner content wrapper", "div[@id='containerdiv']"},
        {"main grades table", "div[@class='gradeTableNew ']"},
        {"grades table inner content", "div[@id='grades_wrapper']"}
    };


    /// Validate the HTML document to ensure that it includes all of the needed elements.
    /// @param root node of the HTML document to validate
    void validate(xmlpp::Node* root) {
        xmlpp::Node* current = root;
        for(const std::pair<std::string, std::string> &pair : described_path){
            xmlpp::NodeSet found = current->find(pair.second);
            if (found.size() == 0) {
                throw std::invalid_argument("Missing required HTML section: " + pair.first + "!");
            }
            current = found[0];
        }
    }

    /// Create the full XPath object from the description array.
    std::string build_xpath() {
        std::string path = "";
        for(const std::pair<std::string, std::string> &pair : described_path){
            path.append(pair.second +"/");
        }
        path.append("*");
        return path;
    }

    /// Parse assignments from a Blackboard grades page
    /// @param root node of the HTML document of the grades page
    std::list<Assignment> parse(xmlpp::Node* root) {
        std::list<Assignment> res;
        
        for(xmlpp::Node* grade_node: root->find(build_xpath())){
            // BB assignment ID
            int id = (int)grade_node->eval_to_number("@id");
            std::string name;
            double current_grade = 0.0;
            double total_grade = 0.0;
            // Each element of the grade row
            for(xmlpp::Node* grade_element: grade_node->get_children()) {
                // type of element
                Glib::ustring clazz = grade_element->eval_to_string("@class");
                // Assignment info column (far left)
                if (clazz == "cell gradable") {
                    // Name of non-clickable assignments
                    name = misc_utils::sanitize(grade_element->eval_to_string("text()"));
                    if (name.empty()) {
                        // Name of clickable assignments
                        name = misc_utils::sanitize(grade_element->eval_to_string("a/text()"));
                    }
                    if (misc_utils::contains(name, "total") ||
                        misc_utils::contains(name, "attend") ||
                        misc_utils::contains(name, "absence")) {
                        // We don't care about totals
                        goto bottom;
                    }
                // Actual grade
                } else if (clazz == "cell grade") {
                    // Current assignment grade
                    current_grade = grade_element->eval_to_number("span[@class='grade']/text()");
                    // BB denotes empty grades as "-", which become -0
                    if (current_grade == -0) {
                        current_grade = -1;
                    }
                    // Have to parse as text to remove "/"
                    std::string total_text = misc_utils::replace(grade_element->eval_to_string("span[@class='pointsPossible clearfloats']/text()"), "/","");
                    // Empty - not graded
                    if (total_text.empty()) {
                        total_text = "-1";
                    }
                    // Parse total from string
                    total_grade = atof(total_text.c_str());
                }
            }
            res.push_back(Assignment(id, name, current_grade, total_grade));
            // Go to end of loop label for name check
            bottom:;
        }
        return res;
    }
}

