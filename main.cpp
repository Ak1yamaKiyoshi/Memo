#include "include/memory/memory.cpp"
#include "include/vocablurary/vocablurary.cpp"
#include "include/levenshtain/levenshtain.cpp"
#include "src/search.cpp"
#include <iostream>
#include <stdlib.h>

// TODO:
// TODO: rewrite memories functions to vectors
// TODO: test <algorythms>
// TODO: test c++20
// TODO: add build script
// TODO: add configurable vocab and memories path to run anywhere
// TODO: add coloring
// TODO: add proper error warnings etc.
// TODO: normalize string before vocablurary

// TODO: fix remove due it's saving broken memories

// TODO: actually sort search results
// TODO: add tags to search (#tags)
// TODO: make actual unittests

// TODO: multithreaded QT app after 3d iteration

// Todo: rewrite using program state and separate functions;

// TODO: rewrite from C-style to C++ IO methods for memory
// TODO: Translate memory struct from char* to string 


struct program_state {
    std::string last_user_cli_input;
    std::string last_user_command;
    std::string last_user_command_arguments;
    
    std::vector<memory*> memories;
    std::string vocablurary;
};




bool __is_a_number(std::string s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}



int main()
{

    std::string user_input_string;
    std::string user_command;
    std::string parsed_user_command;
    std::string user_input_text;

    int command_offset;
    int backslash_position;

    // TODO: create if not exists
    // TODO: print how much loaded
    std::string vocablurary = from_file(FILEPATH_STORAGE_VOCABLURARY);
    int memories_amount = get_amount_of_cached_memories(FILEPATH_STORAGE_NOTES);
    struct memory *memories = from_file(FILEPATH_STORAGE_NOTES, &memories_amount);

    std::cout
        << "/add <text>\t\t\t\t adds note with text \n"
        << "/remove <id>\t\t\t\t removes note by id\n"
        << "/search <text>\t\t\t\t searches trough existing memories\n"
        << "/exit     \t\t\t\t exits the program"
        << std::endl;

    // TODO: make a fancy, informative line (time, memories loaded, etc.)
    // TODO: make it customasable
    char *time_buffer = (char *)malloc(sizeof(char) * TIME_BUFFER_MAXSIZE);
    to_string(time(0), TIME_FORMAT_FULL, time_buffer);

    while (
        std::cout << time_buffer << " [" << memories_amount << "]: " && getline(std::cin, user_input_string))
    {
        to_string(time(0), TIME_FORMAT_FULL, time_buffer);

        command_offset = user_input_string.find(" ");

        user_command = user_input_string.substr(0, command_offset);
        backslash_position = user_command.rfind("/");

        if (!backslash_position < 1)
        {
            std::cout << "You written command incorrectly; " << std::endl;
            continue;
        }
        parsed_user_command = user_command.substr(1, user_command.length() - 1);

        if (!parsed_user_command.compare("add"))
        {
            user_input_text = user_input_string.substr(command_offset + 1, user_input_string.length() - 1);
            memory *new_memory = create(user_input_text.c_str(), memories_amount);

            memories = add(memories, memories_amount, new_memory);
            to_file(FILEPATH_STORAGE_NOTES, memories, ++memories_amount);

            vocablurary = discover_words(user_input_text, vocablurary);
            to_file(FILEPATH_STORAGE_VOCABLURARY, vocablurary);
        }

        else if (!parsed_user_command.compare("remove"))
        {
            user_input_text = user_input_string.substr(command_offset + 1, user_input_string.length() - 1);
            if (!__is_a_number(user_input_text))
            {
                std::cout << "You entered not valid id of memory to remove. Yours: " << user_input_text << "\n";
            }
            int user_input_id_to_remove = atoi(user_input_text.c_str());

            // TODO: std::cout << "user input id parsed: " << user_input_id << "\n";
            // TODO: actually search if id exists or not;
            // TODO: do deletion only if such mem exists
            // TODO: write down in terminal memo deleting data.
            // TODO: REFACTOR TO F-ing vectors.
            memories = remove(memories, memories_amount, user_input_id_to_remove);
            to_file(FILEPATH_STORAGE_NOTES, memories, --memories_amount);
        }
        else if (!parsed_user_command.compare("edit"))
        {
            // ?
            std::cout << "edit";
        }
        else if (!parsed_user_command.compare("search"))
        {
            user_input_text = user_input_string.substr(command_offset + 1, user_input_string.length() - 1);

            std::vector<search_result> results = search(
                memories, memories_amount, user_input_text, vocablurary);

            search_result tmp_search_result;
            search_result max_search_result;

            std::vector<search_result> top_results;
            for (int i = 0; i < results.size(); i++)
            {
                tmp_search_result = results[i];
                if (i == 0 ||
                    max_search_result.confidence < tmp_search_result.confidence)
                {
                    max_search_result = tmp_search_result;
                    top_results.push_back(max_search_result);
                }
            }

            for (auto r : top_results)
            {
                char memory_search_buffer[512];
                to_string(r.mem, r.confidence, memory_search_buffer);
                std::cout << memory_search_buffer << std::endl;
            }

            std::cout << "search";
        }
        else if (!parsed_user_command.compare("exit"))
        {
            break;
        }
        else
        {
            std::cout << "Your command \'/" << parsed_user_command << " \'does not match any existing one!" << std::endl;
        }
    }

    return 0;
}



int test_io_and_vector_functions_memory_rewritten() {
    std::vector <memory*> memories;
    memory *mem0 = create("text0", 1);
    memory *mem1 = create("text1", 2);
    memory *mem2 = create("text2", 3);
    
    memories = add(memories, mem0);
    memories = add(memories, mem1);
    memories = add(memories, mem2);

    for (auto mem: memories) {
        std::string text_memory_repr = to_string(mem);
        std::cout << text_memory_repr << std::endl;
    }

    memories = remove(memories, 2);
    std::cout << "\n";

    for (auto mem: memories) {
        std::string text_memory_repr = to_string(mem);
        std::cout << text_memory_repr << std::endl;
    }

    std::string text = "whoa";
    memories = edit(memories, text, 1);
    std::cout << "\n";

    for (auto mem: memories) {
        std::string text_memory_repr = to_string(mem);
        std::cout << text_memory_repr << std::endl;
    }

    mem_to_file(
        std::string("test.txt"), memories
    );

    std::cout << "\nFrom file: \n";
    std::vector<memory*> test;
    test = mem_from_file(std::string("test.txt"));
    for (auto mem: test) {
        std::string text_memory_repr = to_string(mem);
        std::cout << text_memory_repr << std::endl;
    }
}


int test_levenshtain_distance()
{
    // std::string str1 = "cat";
    // std::string str2 = "hat";
    // std::cout << "ldistance: " << levenshtain_distance(str1, str2) << "\n";

    std::vector<int> vec1 = {1, 5, 2, 5};
    std::vector<int> vec2 = {5, 5, 2, 7};
    std::cout << "ldistance: " << levenshtain_distance(vec1, vec2) << "\n";

    return 0;
}

int test_vocab_io()
{
    std::string vocablurary = "";

    // char sentence[] = "I love typin some messages idk whoa wow";
    // char another_sentence[] = "Halloo i would like some cola";
    char resulting_sentence[] = "Hallo I love typin wow cola";

    // vocablurary = discover_words(sentence, vocablurary);
    // vocablurary = discover_words(another_sentence, vocablurary);
    // vocablurary = discover_words(resulting_sentence, vocablurary);

    // std::cout << "vocab: " << vocablurary << "\n";
    // to_file("vocab.txt", vocablurary);
    vocablurary = from_file("vocab.txt");

    std::cout << "vocablurary: " << vocablurary << "\n";

    std::cout << "\n\n";
    int lenght_of_int_sentence_vector = 0;
    std::vector<int> vector = sentence_vector(resulting_sentence, vocablurary);

    for (auto i : vector)
        std::cout << i << "\n";

    return 0;
}

void test_to_string_time()
{
    char *output_buffer = new char[80];
    to_string(time(0), TIME_FORMAT_FULL, output_buffer);
    std::cout << output_buffer << "\n";
    free(output_buffer);
}
