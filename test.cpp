#include <iostream>
#include <sstream>

#include <vector>
#include <codecvt>
#include <set>
#include <bits/stdc++.h>
#include <vector>

#include <algorithm>
#include <math.h>
#include <set>

const std::string SPECIAL_CHARACTERS = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

template<typename T>
std::vector<T> pad_vector(std::vector<T> &vec, const int desired_length, const int filler=-10) {
    while (vec.size() < (unsigned long)desired_length) {
        vec.push_back((T)filler);
    }
    return vec;
}


double vector_dot_product(const std::vector<float> &a, const std::vector<float> &b) {
    if (! (a.size() == b.size())) return 0.0;
    double sum = 0;
    for (int i = 0; (unsigned long)i < a.size(); i++)
        sum += a[i] * b[i];
    return sum; 
}

double vector_magnitude(const std::vector<float> &vec) {
    double sum = 0;
    for (int i = 0; (unsigned long)i < vec.size(); i++)
        sum += pow(vec[i], 2);
    return std::sqrt(sum);
}

double cosine_similarity(const std::vector<float> &a, const std::vector<float> &b) {
    return vector_dot_product(a, b) / (vector_magnitude(a) * vector_magnitude(b));
};

double wcosine_similarity(std::vector<float> a, std::vector<float> b) {
    unsigned long desired = std::max(a.size(), b.size());
    a = pad_vector(a, desired, 0);
    b = pad_vector(b, desired, 0);
    return cosine_similarity(a, b);
}

void discover(const std::string &ngram, std::vector<std::string> &terms)
{
    if ((std::find(terms.begin(), terms.end(), ngram) == terms.end()))
        terms.push_back(ngram);
}

std::string remove_spaces(std::string str) {
    std::regex spaces_regex("[ ]+");
    return std::regex_replace(str, spaces_regex, "");
}


std::vector<std::string> to_ngrams(
    const std::string &sentence,
    std::vector<std::string> &terms,
    int n)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string utf32_sentence = converter.from_bytes(sentence);

    std::vector<std::string> ngrams;
    std::string ngram;

    int offset = 0;
    int counter = 1;
    while (offset < utf32_sentence.length()) {
        ngram = converter.to_bytes(utf32_sentence.substr(offset, counter));
        if (counter < n) counter++;
        offset += 1;
        ngrams.push_back(ngram);
        discover(ngram, terms);
    }
    return ngrams;
}

std::string remove_special_characters(std::string &text) {
    text.erase(std::remove_if(text.begin(), text.end(),
        [](char c) { return (SPECIAL_CHARACTERS.find(c) != std::string::npos);}),
        text.end());
    return text;
}

std::string tolower_curlocale(std::string &str) {
    std::locale loc("");

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wide = converter.from_bytes(str);

    std::transform(
        wide.begin(), wide.end(), wide.begin(), 
        [&loc](wchar_t c) {
            return std::tolower(c, loc);
    });
    return converter.to_bytes(wide);
}

std::string normalize_string(std::string &str) {
    str = tolower_curlocale(str);
    str = remove_special_characters(str);
    return str;
}

std::set<std::string> vector2set(const std::vector<std::string> &v) {
    std::set<std::string> set;
    for (auto e: v) set.insert(e);
    return set;
} 

std::vector<std::string> set2vector(const std::set<std::string> &s) {
    std::vector<std::string> vec(s.begin(), s.end());
    return vec;
}


std::vector<std::string> split_by_space(const std::string &str) {
    std::vector<std::string> terms;
    std::stringstream iss(str);
    while (1) {
        std::string buffer;
        if (!(iss >> buffer)) break;
        else terms.push_back(buffer);
    }
    return terms;
}

std::string vec2string(const std::vector<std::string> &vec, std::string delimiter=" ") {
    std::stringstream iss;
    for (int i = 0; (unsigned long)i < vec.size(); i++) 
        iss << vec[i] << delimiter;
    return iss.str();
}

float term_frequency(const std::string &term, const std::vector<std::string> &document) {
    int frequency_in_text = std::count(document.begin(), document.end(), term);
    int total_terms_in_text = document.size();
    return (float)frequency_in_text/total_terms_in_text;
}

float inverse_document_frequency(
    const std::string &term,
    std::vector<std::vector<std::string>> documents
) {
    int documents_containing_term = 0;
    int total_number_of_docs = documents.size();
    for (const auto &document: documents)
        if ((std::find(document.begin(), document.end(), term) != document.end())) 
            documents_containing_term++;
    if (documents_containing_term == 0) return 0.00000001;

    return std::log(total_number_of_docs) / documents_containing_term;
}

std::vector<float> tfidf(std::string query, std::vector<std::vector<std::string>> documents, int splitsize=3, bool use_ngrams=true) {
    std::vector<std::string> corpus;
    std::vector<float> tfidf_query;
    if (use_ngrams) {
        for (auto &term: to_ngrams(normalize_string(query), corpus, splitsize)) {
            std::vector<std::string> document_terms = to_ngrams(normalize_string(query), corpus, splitsize);
            float tf = term_frequency(term, document_terms);
            float idf = inverse_document_frequency(term, documents);
            float _tfidf = tf * idf;
            tfidf_query.push_back(_tfidf);
        }
    } else {
        for (auto &term: split_by_space(normalize_string(query))) {
            std::vector<std::string> document_terms = split_by_space(normalize_string(query));
            float tf = term_frequency(term, document_terms);
            float idf = inverse_document_frequency(term, documents);
            float _tfidf = tf * idf;
            tfidf_query.push_back(_tfidf);
        }
    }

    return tfidf_query;
}
struct search_result {
    std::vector<std::string> document;
    float confidence;
};


std::vector<search_result> sort_results(std::vector<search_result> results) {
    for (int i = 0; i < (int)results.size()-1; i++) {
        for (int j = 0; j < (int)results.size()-1; j++) {
            if (results[j].confidence < results[j+1].confidence) {
                iter_swap(results.begin() + j, results.begin() + j + 1);
            }
        }
    }
    return results;
}




int word_to_id(const std::string &word, std::vector<std::string> &terms) {
    auto result = std::find(terms.begin(), terms.end(), word); 
    if ((result == terms.end())) return -1;
    else return (int)(result - terms.begin());
}

std::string remove_redundant_spaces(std::string str) {
    std::regex spaces_regex("[ ]+");
    return std::regex_replace(str, spaces_regex, " ");
}

std::vector<int> sentence_to_vector(std::string&sentence, std::vector<std::string> &terms) {
    std::vector<std::string> words = split_by_space(remove_redundant_spaces(remove_special_characters(sentence)));
    std::vector<int> resulting_vector;
    for (const auto &word: words) resulting_vector.push_back(word_to_id(word, terms));
    return resulting_vector;
}

std::vector<int> vectorize(std::string sentence, std::vector<std::string> &terms, int n =-1 ) {
    if (n != -1) {
        std::vector<std::string> ngrams = to_ngrams(sentence, terms, n);
        for (const std::string &term: ngrams) discover(term, terms);
        std::string ngrams_str = vec2string(ngrams);
        return sentence_to_vector(ngrams_str, terms);
    } else {
        std::string normalized = normalize_string(sentence);
        for (auto word: split_by_space(sentence)) discover(word, terms);
        return sentence_to_vector(normalized, terms);
    }
}

std::vector<int> filter(std::vector<int> a) {
    std::vector<int> b;
    std::copy_if (a.begin(), a.end(), std::back_inserter(b), [](int i){return i>=0;} );
    return b;
}

template <typename T>
float levenshtain_distance(
    std::vector<T> a,
    std::vector<T> b)
{
    int m = a.size();
    int n = b.size();

    std::vector<int> prevRow(n + 1, 0);
    std::vector<int> currRow(n + 1, 0);

    for (int j = 0; j <= n; j++)
        prevRow[j] = j;

    for (int i = 1; i <= m; i++)
    {
        currRow[0] = i;
        for (int j = 1; j <= n; j++)
        {
            if (a[i - 1] == b[j - 1])
                currRow[j] = prevRow[j - 1];
            else
            {
                currRow[j] = 1 + std::min(
                                     // Insert
                                     currRow[j - 1] * 1,
                                     std::min(
                                         // Remove
                                         prevRow[j] * 1,
                                         // Replace
                                         prevRow[j - 1] * 1));
            }
        }
        prevRow = currRow;
    }

    return currRow[n];
}


template<typename T>
int hamming_distance(std::vector<T> a, std::vector<T> b) {
    int distance = 0;
    for (int i = 0; i < a.size(); i++)
        if (a[i] != b[i]) distance++;
    return distance;
}

template<typename T>
int whamming_disstance(std::vector<T> a, std::vector<T> b) {
    int desired = std::max(a.size(), b.size());
    a = pad_vector(a, desired);
    b = pad_vector(b, desired);
    return hamming_distance(a, b);
}

double jaccardSimilarity(const std::set<int>& setA, const std::set<int>& setB) {

    std::set<int> intersection;
    std::set_intersection(setA.begin(), setA.end(), setB.begin(), setB.end(),
                          std::inserter(intersection, intersection.begin()));

    std::set<int> unionSet;
    std::set_union(setA.begin(), setA.end(), setB.begin(), setB.end(),
                   std::inserter(unionSet, unionSet.begin()));
    
    double similarity = static_cast<double>(intersection.size()) / unionSet.size();
    return similarity;
}




#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>


// Include all necessary functions from the original code here
// (normalize_string, vectorize, levenshtain_distance, etc.)

int main() {
    std::vector<std::string> examples = {
        "магазин", "магазини", "я і є магазин", "сходив у магазин учора", "купив хліб", 
        "сьогодні гарний день", "я люблю програмування", "це мій новий код", 
        "вечеря була смачною", "у мене є кіт", "погода сьогодні сонячна",
        "я вивчаю C++", "він був у магазині", "ми пішли на прогулянку", 
        "робота закінчена", "читаю цікаву книгу", "поїхав у відпустку", 
        "закінчив проект", "побачив старого друга", "зустріч була успішною", 
        "буду працювати пізніше", "сніданок був смачним", "пишу код", 
        "це було чудове кіно", "планую поїздку"
    };

    
    const int ENSHITIFICATION = 3;

    std::vector<std::string> terms;



    
    std::string query;
    std::cout << "Enter your query: ";
    std::getline(std::cin, query);

    query = normalize_string(query);
    std::vector<int> query_vec = vectorize(query, terms, ENSHITIFICATION);

    std::vector<std::pair<float, std::string>> matches;
    std::vector<std::pair<float, std::string>> matches_alt;

    for ( auto& example : examples) {
        std::string normalized_example = normalize_string(example);
        std::vector<int> example_vec = vectorize(normalized_example, terms, ENSHITIFICATION);
    


        // Metric 1: 
        float keywordScore = 0;
        for (int i = 0; i < query_vec.size(); i++) {
            if (std::find(example_vec.begin(), example_vec.end(), query_vec[i]) != example_vec.end()) {
                keywordScore += 1.0f / query_vec.size();
            }
        }
        float levenshtein_dist = levenshtain_distance(query_vec, example_vec);
        float metric1 = (keywordScore / levenshtein_dist);
        std::cout << example << "| keyword: " << keywordScore << "levenshtain: " << levenshtein_dist << std::endl;

        // Metric 2
        
        std::set<int> example_set(example_vec.begin(), example_vec.end());
        std::set<int> query_set(query_vec.begin(), query_vec.end());
        
        float metric2 = jaccardSimilarity(example_set, query_set);


        //std::cout << example << "| keyword: " << keywordScore << "levenshtain: " << levenshtein_dist << std::endl;
        matches.push_back(std::make_pair(metric1, example));
        matches_alt.push_back(std::make_pair(metric2, example));
    }

    
    std::sort(matches.begin(), matches.end(), std::less_equal<>());
    std::sort(matches_alt.begin(), matches_alt.end(), std::less_equal<>());

    std::cout << "Query: " << query << std::endl;
    std::cout << "\nTop matches (Metric 1):" << std::endl;
    for (int i = 0; i < 1115 && i < matches.size(); ++i) {
        std::cout << matches[i].second << " | Score: " << matches[i].first << std::endl;
    }

    std::cout << "\nTop matches (Metric 2):" << std::endl;
    for (int i = 0; i < 1115 && i < matches_alt.size(); ++i) {
        std::cout << matches_alt[i].second << " | Score: " << matches_alt[i].first << std::endl;
    }

    return 0;
}


/*
Current good search: 
    1 / keywordScore / levenshtein_dist

Proposed one: 
    delete words (ngrams) that are not in query






*/
