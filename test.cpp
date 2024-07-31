#include <iostream>
#include <sstream>

#include <vector>
#include <codecvt>
#include <set>
#include <bits/stdc++.h>
#include <vector>
#include <math.h>

const std::string SPECIAL_CHARACTERS = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";


std::vector<float> pad_vector(std::vector<float> &vec, const int desired_length, const int filler=-10) {
    while (vec.size() < (unsigned long)desired_length) {
        vec.push_back(filler);
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
    if (!(std::find(terms.begin(), terms.end(), ngram) != terms.end()))
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
    while (offset < utf32_sentence.length()) {
        ngram = converter.to_bytes(utf32_sentence.substr(offset, n));
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

int main() {
std::string query = "the cossack era is a bright period in the history of th";

// Creating 25 example documents
std::vector<std::string> texts = {
    "The Baptism of Rus is one of the most important historical events in Ukraine",
    "The Battle of Berestechko was crucial for the history of Ukraine",
    "The Ukrainian Revolution of 1917-1921 radically changed the political landscape",
    "Kievan Rus was one of the most powerful states in medieval Europe",
    "The Liberation War led by Bohdan Khmelnytsky determined the fate of the Ukrainian people",
    "The Ruin period was a time of great turmoil and political instability in Ukraine",
    "The Holodomor of 1932-1933 became a tragic page in Ukrainian history",
    "Ukraine gained independence on August 24, 1991, after the collapse of the USSR",
    "The Revolution of Dignity in 2014 changed the development vector of Ukraine",
    "The Cossack era is a bright period in the history of the Ukrainian people",
    "The cultural revival in Ukraine in the 19th century became an important epoch for national identity",
    "The First World War influenced the political and social structure of Ukraine",
    "Petro Mohyla was a prominent figure in Ukrainian culture and education",
    "The Act of Unification in 1919 united the UNR and ZUNR into a single state",
    "The Pereyaslav Council of 1654 had a significant impact on Ukraine's future",
    "The princes of Kyiv were important figures in the history of Eastern Europe",
    "The Chernobyl disaster of 1986 became one of the largest technogenic accidents in history",
    "Taras Shevchenko is a symbol of national culture and the struggle for freedom",
    "The Batkivshchyna tragedy of 1708 left an indelible mark on Ukrainian history",
    "The Ostroh Academy was one of the most important centers of education in Ukraine",
    "Ukrainian Sich Riflemen participated in the liberation struggle of the early 20th century",
    "The collapse of Kievan Rus led to political disintegration in Ukrainian lands",
    "The history of Zaporizhzhya Sich is connected with the formation of the Ukrainian Cossacks",
    "Mykhailo Hrushevsky became the first president of Ukraine in 1918",
    "Saint Sophia Cathedral is a significant monument of architecture and history"
};

    std::vector<std::string> corpus;
    
    const int SPLIT = 3;
    std::vector<std::vector<std::string>> documents;
    for (auto text : texts) {
        std::vector<std::string> normalized_text = split_by_space(normalize_string(text)); //to_ngrams(normalize_string(text), corpus, SPLIT);
        documents.push_back(normalized_text);
    }

    std::vector<std::vector<float>> documents_tfidf;
    std::vector<float> query_tfidf = tfidf(query, documents, SPLIT);
    for (auto &document: documents) {
        std::string buffer = vec2string(document, " ");
        std::vector<float> tfidf_res = tfidf(buffer, documents);
        documents_tfidf.push_back(tfidf_res);
        
    }

    std::cout << "Query tfidf: " << std::endl;

    for (auto f: query_tfidf) std::cout << f <<" ";
    std::cout << std::endl;
    std::vector<search_result> results;

    for (int i = 0; (unsigned long)i < documents_tfidf.size(); i++) {
        
        struct search_result res;

        res.confidence = wcosine_similarity(query_tfidf, documents_tfidf[i]);
        res.document = documents[i];
        results.push_back(res);
    }

    results = sort_results(results);    

    std::cout << "Query: " << query << std::endl;
    for (int i = 0; i < (int)documents_tfidf.size(); i++) {
        std::cout  << "[" << results[i].confidence << "] " << vec2string(results[i].document) << std::endl;
    }

    return 0;
}

/*


int main_while_writing() {
    std::string text = "магазин, магазини, я і є магазин";
    std::string text0 = "Я сходив у магазин учора";
    std::vector<std::vector<std::string>> documents = {{text, text0}};

    text = normalize_string(text);
    const int SPLIT = 5;

    
    std::vector<std::string> corpus;
    std::vector<std::string> ngrams = to_ngrams(text, corpus, SPLIT);

    std::cout << "Sentence: " << text << std::endl << "Ngrams: ";
    for (auto &ngram: ngrams) std::cout << ngram << " ";
    std::cout << std::endl;

    std::cout << "Occurences: ";
    for (auto &ngram: ngrams) {
        int amount = std::count(ngrams.begin(), ngrams.end(), ngram);
        std::cout << amount << " ";
    }
    std::cout << std::endl; 

    std::vector<std::string> unique_ngrams = set2vector(vector2set(ngrams));
    std::cout << "Unique ngrams total: " << unique_ngrams.size();
    std::cout << std::endl;

    std::cout << "split_by_space: ";
    for (auto &term: split_by_space(text)) std::cout << term << "|";
    std::cout << std::endl;

    for (auto &term: to_ngrams(normalize_string(text), corpus, SPLIT)) {
        std::vector<std::string> document_terms = to_ngrams(normalize_string(text), corpus, SPLIT);
        float tf = term_frequency(term, document_terms);
        float idf = inverse_document_frequency(term, documents);
        float tfidf = tf * idf;
        std::cout << "term: '" << term << "' tfidf: " << tfidf <<std::endl;
    }

    

    return 0;
}

*/

/*
Ngram is n-character or word wide window that splits string.
 n=2, data -> da at ta
TF-IDF
 TermFrequency(term, text) = Number of times term appears in text / total number of terms in text
 InvereDocumentFrequency(term, corpus) = log(total number of texts in corpus) / number of texts containing term t
 TF-IDF(term, text, corpus) = TF(term, text) * IDF(term, corpus)
*/