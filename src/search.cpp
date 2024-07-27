#include "search.hpp"



std::vector<search_result> sort_results(std::vector<search_result> results) {
    // for (int i = 0; i < results.size(); i++) {
    //     for (int j = 0; j < results.size(); j++) {
    //         if (results[i].confidence < results[i+1].confidence) {
    //             std::swap( results[i], results[i+1]);
    //         }
    //     }
    // }

    // return results;
}


std::vector<search_result> search(struct memory* memories, int len, std::string text_query, std::string vocablurary) {
    std::vector<search_result> results;

    // std::cout <<"test-1";
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