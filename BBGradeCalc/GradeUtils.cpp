//
//  GradeUtils.cpp
//  BBGradeCalc
//
//  Utilities for calculating grades.
//
//  Created by Austin Mayes on 11/13/19.
//  Copyright © 2019 Austin Mayes. All rights reserved.
//

#include "GradeUtils.hpp"
namespace grade_utils {
    /// Determine the maximum grade possible in the course given the current assignments
    /// @param assignments in the gradebook
    double determine_max_grade(std::list<Assignment> assignments) {
        double res = 0.0;
        for (Assignment assign: assignments) {
            if (!assign.is_extra_credit()) {
                res += assign.total_grade;
            }
        }
        return res;
    }

/// Calculate the needed grades on each un-graded assignment in order to receive a desired grade
///
/// Returns a pair containing a pair (woa)  of assignments to needed grades along with a double representing any remaining points not able to be made given the paramaters
///
/// This functions supports an average grade generation algorithm which takes a minimum and maximum expected average percentage on each assignment and picks a grade following in line with these paramaters
///
/// @param assignments in the gradebook
/// @param wanted grade in the course, no more than the max possible in the course
/// @param avg_grade_min minimum clamp of the average grade calculate
/// @param avg_grade_max maximum clamp of the average grade calculation
    std::pair<std::list<std::pair<Assignment, double>>, double> calc_needed_grade(std::list<Assignment> assignments, double wanted, double avg_grade_min, double avg_grade_max) {
        std::list<std::pair<Assignment, double>> res;
        double max = determine_max_grade(assignments);
        // clamp wanted grade to ensure its possible
        wanted = std::max(0.0, std::min(max, wanted));
        // Calculate the current total grade
        double current = 0.0;
        for (Assignment assign: assignments) {
            // Not yet graded, isn't counted
            if (!assign.is_graded()) {
                continue;
            }
            current += assign.current_grade;
        }
        double needed = wanted - current;
        // Debug
        std::cout << "Wanted: " << wanted << std::endl;
        std::cout << "Current: " << current << std::endl;
        std::cout << "Needed: " << needed << std::endl;
        if (needed < 0) {
            // Has wanted grade
            return {res, 0.0};
        }
        for (Assignment assign: assignments) {
            // Hit wanted grade
            if (needed <= 0) {
                break;
            }
            // Already graded, can't change - extra credit handled below
            if (assign.is_graded() || assign.is_extra_credit()) {
                continue;
            }
            // Calculated the needed grade on the assignment, using min/max
            double needed_on_assign = std::min(
                                               needed,
                                               misc_utils::clamp_to_two_dec(assign.total_grade * misc_utils::ramdom_clamped_double(avg_grade_min, avg_grade_max))
                                               );
            needed -= needed_on_assign;
            res.push_back({assign, needed_on_assign});
        }
        if (needed > 0) {
            // Last resort, see if we have any extra credit
            for (Assignment assign: assignments) {
                // Hit wanted grade
                if (needed <= 0) {
                    break;
                }
                // Not extra credit or graded, not needed here
                if (!assign.is_extra_credit() || assign.is_graded()) {
                    continue;
                }
                double needed_on_assign = misc_utils::clamp_to_two_dec(needed * misc_utils::ramdom_clamped_double(avg_grade_min, avg_grade_max));
                needed -= needed_on_assign;
                res.push_back({assign, needed_on_assign});
            }
        }
        return {res, needed};
    }
}
