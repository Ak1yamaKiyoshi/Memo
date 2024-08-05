#include "../include/memories.hpp"
#include "../include/string_utils.hpp"
#include "../include/search.hpp"
#include "../include/vocablurary.hpp"

#include <string>
#include <vector>
#include <set>



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

double wjaccardSimilarity(const std::vector<int> &a, const std::vector<int> b) {
    std::set<int> example_set(a.begin(), a.end());
    std::set<int> query_set(b.begin(), b.end());
    return jaccardSimilarity(example_set, query_set);
}


void discover(const std::string &ngram, std::vector<std::string> &terms)
{
    if ((std::find(terms.begin(), terms.end(), ngram) == terms.end()))
        terms.push_back(ngram);
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

std::vector<int> sentence_to_vector(std::string&sentence, std::vector<std::string> &terms) {
    std::vector<std::string> words = split_by_space(remove_redundant_spaces(remove_special_characters(sentence)));
    std::vector<int> resulting_vector;
    for (const auto &word: words) resulting_vector.push_back(word_to_id(word, terms));
    return resulting_vector;
}

std::vector<int> vectorize(std::string sentence, std::vector<std::string> &terms, int n) {
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
    for (int i = 1; i <= m; i++) {
        currRow[0] = i;
        for (int j = 1; j <= n; j++) 
            if (a[i - 1] == b[j - 1])
                currRow[j] = prevRow[j - 1];
            else
                currRow[j] = 1 + std::min(currRow[j - 1] * 1, std::min(prevRow[j] * 1, prevRow[j - 1] * 1));
        prevRow = currRow;
    }
    return currRow[n];
}

std::vector<search_result> search(
    const std::string &query, const std::vector<memo*> &memories, 
    std::vector<std::string> &terms, int n
) {

    // TODO: parse dates from string and search most recent too;
    // if date is present, normalize time diff between query and best match 
    // add it to confidence; 
    std::vector<std::string> tags_vocab;

    std::vector<search_result> results;
    std::vector<std::string> filter_tags = parse_tags_from_string(query);
    std::string normalized_query = remove_tags_from_string(query);
    normalized_query = normalize_string(normalized_query);
    std::vector<int> query_vec = vectorize(normalized_query, terms, n);

    auto query_tags_vectorized = vectorize(join(filter_tags, " "), tags_vocab);
    for (const auto &mem: memories) {
        auto doc_tags_vectorized = vectorize(join(mem->tags, " "), tags_vocab);
        std::set<std::string> tags_doc_set = vec2set(mem->tags);

        if (filter_tags.size() > 0) {
            float tags_score = wjaccardSimilarity(query_tags_vectorized, doc_tags_vectorized);
            if (!tags_score) continue; 
        }

        std::string buffer = mem->text;
        std::vector<int> document_vec = vectorize(buffer, terms, n);
        search_result sr;
        sr.confidence = (float)wjaccardSimilarity(query_vec, document_vec);
        sr.reference = mem;
        results.push_back(sr);
    }
    results = sort_results(results);
    return results;
}  