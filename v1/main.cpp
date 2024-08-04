#include "include/memory/memory.cpp"
#include "include/vocablurary/vocablurary.cpp"
#include "include/levenshtain/levenshtain.cpp"
#include "src/search.cpp"
#include "include/coloring.hpp"

#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include <string>
#include <algorithm>
#include <cstdlib>

void clear_screen()
{
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system("clear");
#endif
}

// IMPLEMENT term frequency-inverse document frequency instead of BOW
// TODO: fix remove due it's saving broken memories
// TODO: add tags to search (#tags)
// TODO: make actual unittests
// TODO: multithreaded QT app after 3d iteration
// TODO: undo option and cache to multiple files by sessions;
// TODO: Translate memory struct from char* to string
// TODO: ASCII art on start
// TODO: case when removing last memory segfaults.

enum command_codes
{
    code_ok = 0,
    code_exit = 1,
    code_error = 2
};

enum message_codes
{
    message_ok = 'i',
    message_error = 'e',
    message_fail = 'x',
    message_neutral = 'n'
};

bool __is_a_number(std::string s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int __is_file_exist(std::string filename)
{
    std::ifstream file(filename.c_str());
    if (file.good())
    {
        return code_ok;
    }
    else
    {
        return code_error;
    }
}

int __touch(std::string filename)
{
    std::ofstream outfile(filename);
    outfile << " " << std::endl;
    outfile.close();
    return code_ok;
}

/**/
void message(message_codes code, std::string message)
{
    std::string color;
    switch (code)
    {
    case message_error:
        color = ANSI_YELLOW;
        break;
    case message_fail:
        color = ANSI_RED;
        break;
    case message_ok:
        color = ANSI_GREEN;
        break;
    default:
        color = ANSI_WHITE;
        break;
    }

    std::string resulting_message = color + std::string(" ") + (char)code + " " + message + ANSI_RESET + "\n";
    std::cout << resulting_message;
}

struct program_state
{
    std::string last_user_cli_input;
    std::string last_user_command;
    std::string last_user_command_arguments;

    std::vector<memory *> memories;
    std::string vocablurary;
};

int get_id(std::vector<memory *> memories)
{
    int max_id;
    int idx = 0;
    for (auto &mem : memories)
    {
        if (idx == 0 || mem->id > max_id)
        {
            max_id = mem->id;
        }
        idx++;
    }

    return max_id + 1;
}

void print_command(const std::string &command, const std::string &description)
{
    std::cout << ANSI_BRIGHT_WHITE << command << ANSI_RESET << " - " << ANSI_GREEN << description << ANSI_RESET << "\n";
}

int command_help()
{
    std::cout << "Command list:\n";
    print_command("/help", "Displays help information.");
    print_command("/add <text>", "Adds a new memory with optional tags.");
    print_command("/remove <id>", "Deletes a memory by its ID.");
    print_command("/edit <id> <text>", "Edits a memory with a new text.");
    print_command("/clear", "Clears terminal\n");
    return code_ok;
}

int command_add(program_state &state)
{
    if (state.last_user_command_arguments.length() > 1)
    {
        char *memory_text = new char[state.last_user_command_arguments.length()];
        strcpy(memory_text, state.last_user_command_arguments.c_str());

        memory *new_memory = create(memory_text, get_id(state.memories));

        state.vocablurary = discover_words(state.last_user_command_arguments, state.vocablurary);
        to_file(FILEPATH_STORAGE_VOCABLURARY, state.vocablurary);

        state.memories = add(state.memories, new_memory);

        mem_to_file(FILEPATH_STORAGE_NOTES, state.memories);
        message(message_ok, "Memory created and saved successfuly!");
        message(message_neutral, to_string(new_memory));

        return code_ok;
    }
    else
    {
        return code_error;
    }
}

struct memory *get_by_id(std::vector<memory *> memories, int to_search)
{
    // TODO: Rewrite to modern syntax
    for (auto &mem : memories)
    {
        if (mem->id == to_search)
        {
            return mem;
        }
    }
    return nullptr;
}

int command_edit(program_state &state)
{
    std::string id_string, text;
    std::stringstream iss(state.last_user_command_arguments);
    iss >> id_string >> text;
    if (__is_a_number(id_string)) {
        int id = std::stoi(id_string);
        
        struct memory *desired_mem = get_by_id(state.memories, id);
        if (desired_mem) {
            std::string desired_mem_repr = to_string(desired_mem);
            edit(state.memories, text, id);
            message(message_ok, "Message edited succesfuly.");
            message(message_neutral, "Before: " + desired_mem_repr);
            desired_mem = get_by_id(state.memories, id);
            message(message_neutral, "After:  " + to_string(desired_mem));
        } else {
            message(message_error, "There is no memory with such id.");
        }
    } else {
        message(message_error, "Entered id is not a number");
    }
    return code_ok;
}

int command_remove(program_state &state)
{
    if (!__is_a_number(state.last_user_command_arguments))
    {
        message(message_error,
                "You entered not valid id of memory to remove; Yours: " + state.last_user_command_arguments + "\n");
        return code_error;
    }

    // Todo: add confirmation and printing of deleted memo;
    // Todo: check if idx exists
    // TODO: THERE IS A BUG WITH ID'S; ID"S CHANGING!
    // Now thats a feature.
    int user_input_id_to_remove = atoi(state.last_user_command_arguments.c_str());

    struct memory *desired_mem = get_by_id(state.memories, user_input_id_to_remove);
    state.memories = remove(state.memories, user_input_id_to_remove);

    int cur_size = state.memories.size();
    mem_to_file(FILEPATH_STORAGE_NOTES, state.memories);
    std::string id_str = std::to_string((int)user_input_id_to_remove);
    if (!desired_mem)
    {
        message(message_error,
                "Memory with id '" + id_str + "' does not exist.");
    }
    else
    {
        message(message_ok,
                "Memory \"\"\" " + to_string(desired_mem) + "\"\"\" is deleted/");
    }

    return code_ok;
}

int command_clear(program_state &state)
{
    clear_screen();
    return code_ok;
}

int command_search(program_state &state)
{
    std::vector<search_result> results = search(
        state.memories,
        state.last_user_command_arguments,
        state.vocablurary);

    std::string num = std::to_string(results.size());
    message(message_ok, "Search results (" + num + std::string("): "));
    for (auto r : results)
    {
        char memory_search_buffer[512];
        to_string(r.mem, r.confidence, memory_search_buffer);
        std::cout << memory_search_buffer << std::endl;
    }
    return code_ok;
}

int command_exit(program_state &state) {
    return code_exit;
}

int utility_get_input(program_state &state)
{

    char *time_buffer = (char *)malloc(sizeof(char) * TIME_BUFFER_MAXSIZE);
    int command_offset = 0, backslash_position = 0;

    to_string(time(0), TIME_FORMAT_FULL, time_buffer);

    std::cout
        << "f("
        << ANSI_BLUE << time_buffer
        << ANSI_RESET << ", "
        << ANSI_YELLOW << state.memories.size()
        << ANSI_RESET << ") is ";
    getline(std::cin, state.last_user_cli_input);

    free(time_buffer);

    command_offset = state.last_user_cli_input.find(" ");
    state.last_user_command = state.last_user_cli_input.substr(0, command_offset);
    backslash_position = state.last_user_command.rfind("/");

    if (!backslash_position < 1)
    {
        message(
            message_error,
            "You may forgotten '/' when writing a command. ");
        return code_error;
    }

    state.last_user_command = state.last_user_command.substr(1, state.last_user_command.length() - 1);
    state.last_user_command_arguments = state.last_user_cli_input.substr(
        command_offset + 1, state.last_user_cli_input.length() - 1);

    // TODO: still possible to add /ad with '/add' only
    // TODO: commands can be executed without a slash.
    // TODO: exit codes are ignored 
    if (state.last_user_command_arguments.length() < 1)
    {
        message(message_fail, "The input is to short! Don't forget parameters.");
        return code_error;
    }
    return code_ok;
}

int utility_setup(program_state &state)
{
    if (__is_file_exist(FILEPATH_STORAGE_VOCABLURARY) == code_ok)
    {
        state.vocablurary = from_file(FILEPATH_STORAGE_VOCABLURARY);
    }
    else
    {
        __touch(FILEPATH_STORAGE_VOCABLURARY);
        message(message_error,
                "Vocablurary file '" + std::string(FILEPATH_STORAGE_VOCABLURARY) + "' does not exist. New one is created.");
    }
    if (__is_file_exist(FILEPATH_STORAGE_NOTES) == code_ok)
    {
        state.memories = mem_from_file(FILEPATH_STORAGE_NOTES);
    }
    else
    {
        __touch(FILEPATH_STORAGE_NOTES);
        message(message_error,
                "Memory storage file '" + std::string(FILEPATH_STORAGE_NOTES) + "' does not exist. New one is created.");
    }
    return code_ok;
}

int utility_parse_command(program_state &state)
{
    int output = -1;
    if (!state.last_user_command.compare("add"))
    {
        // command_clear(state);
        output = command_add(state);
    }
    else if (!state.last_user_command.compare("remove"))
    {
        // command_clear(state);
        output = command_remove(state);
    }
    else if (!state.last_user_command.compare("edit"))
    {
        //command_clear(state);
        output = command_edit(state);
    }
    else if (!state.last_user_command.compare("search"))
    {
        //command_clear(state);
        output = command_search(state);
    }
    else if (!state.last_user_command.compare("exit"))
    {
        // command_clear(state);
        output = command_exit(state);
    }
    else if (!state.last_user_command.compare("clear"))
    {
        // command_clear(state);
        output = command_clear(state);
    }
    else if (!state.last_user_command.compare("help"))
    {
        // command_clear(state);
        output = command_help();
    }
    else
    {
        message(message_error,
                "Your command does not match any of existing ones. Your command is '" + state.last_user_command + "'.");
    }
    return output;
}

int main()
{
    program_state state;
    command_help();
    utility_setup(state);
    while (1)
    {

        utility_get_input(state);
        if (utility_parse_command(state) == code_exit)
            break;
    }
    return code_ok;
}

int _main_v1()
{
    std::string user_input_string;
    std::string user_command;
    std::string parsed_user_command;
    std::string user_input_text;

    int command_offset;
    int backslash_position;

    std::string vocablurary = from_file(FILEPATH_STORAGE_VOCABLURARY);
    int memories_amount = get_amount_of_cached_memories(FILEPATH_STORAGE_NOTES);
    struct memory *memories = from_file(FILEPATH_STORAGE_NOTES, &memories_amount);

    std::cout
        << "/add <text>\t\t\t\t adds note with text \n"
        << "/remove <id>\t\t\t\t removes note by id\n"
        << "/search <text>\t\t\t\t searches trough existing memories\n"
        << "/exit     \t\t\t\t exits the program"
        << std::endl;

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

int test_io_and_vector_functions_memory_rewritten()
{
    std::vector<memory *> memories;
    memory *mem0 = create("text0", 1);
    memory *mem1 = create("text1", 2);
    memory *mem2 = create("text2", 3);

    memories = add(memories, mem0);
    memories = add(memories, mem1);
    memories = add(memories, mem2);

    for (auto mem : memories)
    {
        std::string text_memory_repr = to_string(mem);
        std::cout << text_memory_repr << std::endl;
    }

    memories = remove(memories, 2);
    std::cout << "\n";

    for (auto mem : memories)
    {
        std::string text_memory_repr = to_string(mem);
        std::cout << text_memory_repr << std::endl;
    }

    std::string text = "whoa";
    memories = edit(memories, text, 1);
    std::cout << "\n";

    for (auto mem : memories)
    {
        std::string text_memory_repr = to_string(mem);
        std::cout << text_memory_repr << std::endl;
    }

    mem_to_file(
        std::string("test.txt"), memories);

    std::cout << "\nFrom file: \n";
    std::vector<memory *> test;
    test = mem_from_file(std::string("test.txt"));
    for (auto mem : test)
    {
        std::string text_memory_repr = to_string(mem);
        std::cout << text_memory_repr << std::endl;
    }
    return 0;
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
