//
//  MiscUtils.cpp
//  BBGradeCalc
//
//  Miscellaneous utilities that don't go anywhere else nicely.
//
//  Created by Austin Mayes on 12/1/19.
//  Copyright © 2019 Austin Mayes. All rights reserved.
//

#include "MiscUtils.hpp"

namespace misc_utils {
    /// Clamo an ugly long double to a two decimal precision value with rounding
    /// @param to_clamp double to clamp to two decimals
    double clamp_to_two_dec(double to_clamp)
    {
        double value = (int)(to_clamp * 100 + .5);
        return (double)value / 100;
    }

    /// Generate a random double between two values
    /// @param min value allowed
    /// @param max value allowed
    double ramdom_clamped_double(double min, double max) {
        double random = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
        return random;
    }

    /// Convert a string to lowercase
    /// @param to_lower string to convert to fully lowerase
    std::string lower(std::string to_lower) {
        std::transform(to_lower.begin(),
                       to_lower.end(),
                       to_lower.begin(),
                       [](unsigned char c){ return std::tolower(c); }
                       );
        return to_lower;
    }

    /// Determine if a given string contains a given value, regardless of case
    /// @param base string to look in
    /// @param find to check for in the string
    bool contains(std::string base, const std::string& find) {
        return lower(base).find(lower(find)) != std::string::npos;
    }

    /// Convert a boolean to human friendly string
    /// @param b to stringify
    std::string stringify_bool(bool b) {
        return b ? "True" : "False";
    }

    /// Replace a given string for another in a string
    /// @param base string to find/replace in
    /// @param find text to search for
    /// @param replace text to replace the found text with
    std::string replace(std::string base, const std::string& find, const std::string& replace) {
        size_t pos = 0;
        while ((pos = base.find(find, pos)) != std::string::npos) {
             base.replace(pos, find.length(), replace);
             pos += replace.length();
        }
        return base;
    }

    /// Remove leading/following spaces from a string
    /// @param to_trim string to trim
    std::string trim(const std::string& to_trim)
    {
        size_t first = to_trim.find_first_not_of(' ');
        if (std::string::npos == first) {
            return "";
        }
        size_t last = to_trim.find_last_not_of(' ');
        return to_trim.substr(first, (last - first + 1));
    }

    /// Remove newlines and extra whitespace from a string
    /// @param to_sanitize string to clean
    std::string sanitize(const std::string& to_sanitize) {
        return trim(replace(to_sanitize, "\n", ""));
    }
}
