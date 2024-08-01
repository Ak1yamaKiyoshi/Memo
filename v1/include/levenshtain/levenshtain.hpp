#pragma once

#include <string>
#include <vector>



const float COST_INSERT = 5;
const float COST_REPLACE = 3;
const float COST_REMOVE = 10;
const float COST_MAX_OF_THEM = COST_REMOVE;


int levenshtain_distance(
    const std::string& str1, const std::string& str2);


float levenshtain_distance(
    std::vector<int> a,
    std::vector<int> b
);
