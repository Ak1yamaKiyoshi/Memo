#include "search.hpp"
#include <algorithm>



std::vector<search_result> sort_results(std::vector<search_result> results) {
    for (int i = 0; i < results.size()-1; i++) {
        for (int j = 0; j < results.size()-1; j++) {
            if (results[j].confidence < results[j+1].confidence) {
                //std::swap( results[i], results[i+1]);
                iter_swap(results.begin() + j, results.begin() + j + 1);
            }
        }
    }
    return results;
}

std::vector<search_result> filter_results(std::vector<search_result> results) {
    std::vector<search_result> new_filtered_results;
    for (auto &res: results)
        if (res.confidence > 0)
            new_filtered_results.push_back(res);
    return new_filtered_results;
}


std::vector<search_result> search(struct memory* memories, int len, std::string text_query, std::string vocablurary) {
    std::vector<search_result> results;

    for (int i = 0; i < len; i++) {
        memory mem = memories[i];
        struct search_result result;
        result.mem = (memory*)malloc(sizeof(memory));
            result.mem->edit = memories[i].edit;
            result.mem->create = memories[i].create;
            result.mem->id = memories[i].id;
            result.mem->text = new char[strlen(memories[i].text)+1];
            strcpy(result.mem->text, memories[i].text);   
        

        std::vector<int> query = sentence_vector(text_query, vocablurary);
        std::vector<int> voc = sentence_vector(mem.text, vocablurary);

        int queryTokenLength = query.size();
        int vocTokenLength = voc.size();
        int queryStringLength = text_query.length();
        int vocStringLength = strlen(mem.text);

        // Todo: use somehow symbolwise too
        // idk thats kinda magic to me
        float confidence = (float)levenshtain_distance(query, voc);
        float confidence_symbolwise = levenshtain_distance(mem.text, text_query);
        float normalizedConfidence = confidence / (float)std::max(queryTokenLength, vocTokenLength);

        float keywordScore = 0;
        for (int i = 0; i < queryTokenLength; i++) {
            if (std::find(voc.begin(), voc.end(), query[i]) != voc.end()) {
                keywordScore += 1.0f / queryTokenLength;
            }
        }

        result.confidence = keywordScore*1;
        results.push_back(result);
    }

    return results;
}


std::vector<search_result> search(
    std::vector<memory*> memories,
    std::string text_query, std::string vocablurary
)  {
    std::vector<search_result> results;
    if (memories.size() == 0) {
        return results;
    }
    
    for (auto mem: memories) {

        search_result result;
        result.mem = mem;
        std::vector<int> query = sentence_vector(text_query, vocablurary);
        std::vector<int> voc = sentence_vector(trim(mem->text), vocablurary);
             
        int queryTokenLength = query.size();
        int vocTokenLength = voc.size();
        int queryStringLength = text_query.length();
        int vocStringLength = strlen(mem->text);   

        float confidence = (float)levenshtain_distance(query, voc) / ((float)query.size()*COST_MAX_OF_THEM );
        float confidence_symbolwise = levenshtain_distance(mem->text, text_query) / (text_query.length()*COST_MAX_OF_THEM);


        float keywordScore = 0;
        for (int i = 0; i < queryTokenLength; i++) {
            if (std::find(voc.begin(), voc.end(), query[i]) != voc.end()) {
                keywordScore += 1.0f / queryTokenLength;
            }
        }

        
        float condifence_normalized = (confidence_symbolwise*0.2+ confidence*0.2 + keywordScore*0.6)/2;
        std::cout << "= = = - " << trim(mem->text) << std::endl;
        std::cout << "confidence   : " << confidence << std::endl;
        std::cout << "symbolwise   : " << confidence_symbolwise << std::endl;
        std::cout << "normalized   : " << condifence_normalized << std::endl;
        result.confidence = condifence_normalized;
        results.push_back(result);
    }

    results = sort_results(results);
    results = filter_results(results);
    return results;
}
