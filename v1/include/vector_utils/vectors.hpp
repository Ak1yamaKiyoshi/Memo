#pragma once
#include <vector>
#include <math.h>

std::vector<int> pad_vector(std::vector<int> &vec, const int desired_legnth, const int filler=0) {
    for (int i = 0; (unsigned long)i < vec.size() - desired_legnth; i++) vec.push_back(filler);
    return vec;
}

double vector_dot_product(const std::vector<int> &a, const std::vector<int> &b) {
    if (! (a.size() == b.size())) return 0.0;
    double sum = 0;
    for (int i = 0; (unsigned long)i < a.size(); i++)
        sum += a[i] * b[i];
    return sum; 
}

double vector_magnitude(const std::vector<int> &vec) {
    double sum = 0;
    for (int i = 0; (unsigned long)i < vec.size(); i++)
        sum += pow(vec[i], 2);
    return std::sqrt(sum);
}

double cosine_similarity(const std::vector<int> &a, const std::vector<int> &b) {
    return vector_dot_product(a, b) / (vector_magnitude(a) * vector_magnitude(b));
};

double wcosine_similarity(std::vector<int> a, std::vector<int> b) {
    unsigned long desired = std::max(a.size(), b.size());
    a = pad_vector(a, desired, 0);
    b = pad_vector(b, desired, 0);
    return cosine_similarity(a, b);
}

double euclidian_distance(const std::vector<int> &a, const std::vector<int> b) {
    int sum = 0;
    for (int i = 0; (unsigned long)i < a.size(); i++)
        sum += pow(a[i] - b[i], 2);
    return std::sqrt(sum);
}

double weuclidian_distance(std::vector<int> a, std::vector<int> b) {
    unsigned long desired = std::max(a.size(), b.size());
    a = pad_vector(a, desired, 0);
    b = pad_vector(b, desired, 0);
    return euclidian_distance(a, b);
}
