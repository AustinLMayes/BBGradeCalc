#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>
 
#include <iostream>
#include <fstream>
#include <string>
#include "HTMLUtils.cpp"
#include "GradeUtils.cpp"

int main() {
    
    std::string file_path = "/Users/austinmayes/Desktop/grades.html";
    double wanted = 20000;
    double avg_min = .80;
    double avg_max = 1.0;
    
    // Load in file
    std::ifstream ifs(file_path);
    std::string re( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
 
    // Parse HTML and create a DOM tree
    xmlDoc* doc = htmlReadDoc((xmlChar*)re.c_str(), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
 
    // Encapsulate raw libxml document in a libxml++ wrapper
    xmlNode* r = xmlDocGetRootElement(doc);
    xmlpp::Element* root = new xmlpp::Element(r);
 
    // Ensure the document is actually correct
    html_utils::validate(root);
    
    // Parse in assignments from the gradebook
    std::list<Assignment> assignments = html_utils::parse(root);
    
    // Debug
    std::cout << "Assignments:\n";
    for (Assignment ass: assignments) {
        std::cout << ass.describe() << std::endl;
    }
    
    std::cout << "\n\n\n\n";
    
    // Calculate needed grades
    std::pair<std::list<std::pair<Assignment, double>>, double> needed = grade_utils::calc_needed_grade(assignments, wanted, avg_min, avg_max);
    for (std::pair<Assignment, double> section: needed.first) {
        std::cout << "Need " << section.second << " on " << section.first.name << std::endl;
    }
    
    // Uh oh :(
    if (needed.second > 0) {
        std::cout << "Still need " << needed.second << std::endl;
    }
    
    
    // Clean up memory
    delete root;
    xmlFreeDoc(doc);
 
    return 0;
}
