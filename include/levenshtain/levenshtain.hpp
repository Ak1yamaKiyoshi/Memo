#pragma once

#include <string>
#include <vector>



const float COST_INSERT = 1;
const float COST_REPLACE = 1;
const float COST_REMOVE = 1;


int levenshtain_distance(
    const std::string& str1, const std::string& str2);


float levenshtain_distance(
    std::vector<int> a,
    std::vector<int> b
);
