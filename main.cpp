//                                               ░░          ░░░░                                                                          
//         *                                   ▒▒▒▒          ░░▒▒░░                                                                        
//                                             ▒▒▒▒          ░░▒▒▒▒                                                                        
//                          *                  ▒▒▒▒          ░░▒▒▒▒                                                                        
//                                             ▒▒▒▒          ▒▒▒▒▒▒                                                                        
//          *                                  ▒▒▒▒▒▒░░▒▒    ▒▒▒▒▒▒                                                                        
//       *                                     ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒                                                                          
//                                               ░░▒▒▒▒▒▒▒▒▒▒▒▒                                                                            
//                                               ▒▒▒▒▒▒▒▒▒▒▒▒▓▓                                                                            
//                   *                           ▒▒▒▒▒▒██▓▓▒▒▒▒                                                                            
//                                               ░░▒▒▒▒▒▒▒▒▒▒▒▒                                                                            
//            *                       *          ░░▒▒▒▒▒▒▒▒▒▒▒▒                                                                            
//                                               ░░▒▒▒▒▒▒▒▒▒▒▒▒                                                                            
//                                               ░░▒▒▒▒▒▒▒▒▒▒▒▒                                                                            
//             *                                 ▒▒▓▓▓▓▒▒▒▒▓▓▓▓░░                                                                          
//                                               ░░▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒░░░░░░                                                                  
//                      *                        ░░▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░                                                          
//                                                 ▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░                                                      
//                                                 ▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░                                                    
//                                                 ▒▒▒▒▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒                                                    
//                                                 ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░                                                    
//                                                   ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒                                                      
//             *                                     ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒                                                      
//                            *                      ▒▒▒▒░░▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒                                                        
//                                                   ▒▒▒▒░░  ▒▒▒▒      ░░▒▒▒▒  ▒▒▒▒                                                        
//                                                   ░░▒▒    ▒▒▒▒        ▒▒░░░░▒▒▒▒                                                        
// ▓▓▓▓▓ ▓                                           ░░▒▒    ▒▒          ▒▒░░  ▒▒                                                          
// ▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓                ▒                    ░░     ░           ░     ░░        
// ░░░░░░░░░░▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒

#include "../include/utils.hpp"
#include "../include/memories.hpp"
#include "../include/repetitions.hpp"
#include "../include/search.hpp"
#include "../include/string_utils.hpp"
#include "../include/vocablurary.hpp"


int main() {
    std::string filename_memories = "memories.txt";
    if (!is_file_exist(filename_memories)) {
        file_create(filename_memories);
        message(ok, "Memories files not found. Created new one: " + filename_memories);
    }

    const int N_FOR_NGRAMS= 3;
    std::vector<std::string> vocablurary;
    std::string user_input, command, arguments;
    std::vector<memo*> memories = memories_read_all_w(filename_memories);
    
    time_t a = time(0)-10000;
    bool is_running = true;
    while (is_running) {
        time_t b = a;
        a = time(0);
        double diff = difftime(a, b);
        if (diff < 0.00001) {
            message(error_program_side, "Too fast iteration. Suspect of endless loop. Exiting."); 
            return -1;
        } 

        memories_write_all_w(filename_memories, memories);
        memories = memories_read_all_w(filename_memories);
        
        std::cin.sync();
        std::cout << "|" << memories.size() << "> ";
        getline(std::cin, user_input);

        command = parse_command(user_input);
        arguments = remove_commands(user_input);
        if (command.length() == 0) {
            message(error_user_side, "Entered command is too short.");
            continue;
        }
        if (!command.compare("/add") || !command.compare("/a")) {
            if (trim(arguments).length() == 0) {
                message(error_user_side, "You provided no arguments.");
                continue;
            }
            int id = memories_get_new_id(memories);
            memo* mem = create_memo(arguments, id);
            if (!memories_add(mem, memories)) {
                message(error_program_side, "Something went wrong.");
            } else {
                message(ok, "Memory successfuly created");
                message(display, "Created memory: " + to_view_string(memo_to_stringc(mem)));
            }
        
        } else if (!command.compare("/repetition") || !command.compare("/i")) {
            message(error_program_side, "Not implemented yet!"); 
            continue;
        } else if (!command.compare("/search") || !command.compare("/s"))     {
            if (trim(arguments).length() == 0) {
                message(error_user_side, "You provided search query.");
                continue;
            }
            
            time_t search_start = time(0);
            std::vector<search_result> rs = search(arguments, memories, vocablurary);
            time_t search_end = time(0);
            time_t diff = search_end - search_start;
            float time_taken = static_cast<float>(diff);

            message(ok, "Search completed in " + std::to_string(time_taken) + "s. ");
            message(display, "Results: ");


            int i = 0;
            for (auto result: rs) {
                if (i++ > 5) break;

                std::cout << "[" << ANSI_BRIGHT_WHITE<< std::fixed << std::setprecision(2) 
                << std::setw(5) << std::setfill(' ')  << result.confidence << " ] "  << ANSI_RESET << memo_to_stringc( result.reference) << std::endl; 
            }

        } else if (!command.compare("/clear") || !command.compare("/c"))     {
            clear_screen();
        } else if (!command.compare("/help") || !command.compare("/h"))     {
            const int command_width = 20;
            const int description_width = 80;
            std::stringstream help_message;
            help_message 
                << "Available commands:\n"
                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/add <text>" << ANSI_RESET 
                << std::setw(description_width) << std::left << "(or /a) - Add a new memory. <text> can have tags (e.g., #tag) used for filtering in search." << "\n"
                
                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/repetition" << ANSI_RESET 
                << std::setw(description_width) << std::left << "(or /i) - (Not implemented yet)." << "\n"

                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/search <text>" << ANSI_RESET 
                << std::setw(description_width) << std::left << "(or /s) - Search memories using <text>. You can filter by tags like #tag." << "\n"

                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/clear" << ANSI_RESET 
                << std::setw(description_width) << std::left << "(or /c) - Clear the screen." << "\n"

                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/help" << ANSI_RESET 
                << std::setw(description_width) << std::left << "(or /h) - Display this help message." << "\n"

                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/exit" << ANSI_RESET 
                << std::setw(description_width) << std::left << "(or /e) - Exit the program." << "\n"

                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/remove <id>" << ANSI_RESET 
                << std::setw(description_width) << std::left << "(or /r) - Remove a memory with the specified <id>." << "\n"

                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/update <id> <text>" << ANSI_RESET 
                << std::setw(description_width) << std::left << "(or /u) - Update a memory with the specified <id> to the new <text>." << "\n"

                << ANSI_BRIGHT_WHITE << std::setw(command_width) << std::left << "/all" << ANSI_RESET 
                << std::setw(description_width) << std::left << "- Display all memories." << "\n";

            message(display, help_message.str());
            
        } else if (!command.compare("/exit") || !command.compare("/e"))     {
            break;
        } else if (!command.compare("/remove") || !command.compare("/r"))     {
            if (trim(arguments).length() == 0) {
                message(error_user_side, "You provided no arguments.");
                continue;
            }
            std::stringstream iss(arguments);
            std::string id_str;
            int id = 0; iss >> id_str;
            id = std::stoi(id_str);

            memo* mem = memories_get_by_id(memories, id); 
            if (mem == nullptr) {
                message(error_user_side, "There is no memory with such id");
                continue;
            }

            if (!memories_remove_by_id(memories, id)) {
                message(error_program_side, "Something went wrong");
                continue;
            }

            std::string buffer = "Memory " + to_view_string(memo_to_stringc(mem)) + " has been removed";
            message(ok, buffer);

        } else if (!command.compare("/update") || !command.compare("/u")) {
            if (trim(arguments).length() == 0) {
                message(error_user_side, "You provided no arguments.");
                continue;
            }
            std::stringstream iss(arguments);
            std::string id_str, text;
            int id = 0; iss >> id_str;
            id = std::stoi(id_str);
            getline(iss, text);

            memo* mem = memories_get_by_id(memories, id); 
            std::string old_memory_string = memo_to_stringc(mem);
            
            mem->text = mem->text;
            if (mem == nullptr) {
                message(error_user_side, "There is no memory with such id");
                continue;
            }

            if (!memories_update_by_id(memories, id, text)) {
                message(error_program_side, "Something went wrong");
                continue;
            }

            std::string message_str = "Memory    : "+ to_view_string(old_memory_string) + " successfuly";
            message(ok, message_str);
            message(display,          "updated to: " + to_view_string(memo_to_stringc(mem)));
            
        } else if (!command.compare("/all")) {
            message(ok, "Displaying all memories.");
            for (const auto &mem: memories) {
                std::cout << to_view_string(memo_to_stringc(mem)) << std::endl;
            }

        } else {
            message(error_user_side, "There is no such command.");
        }
    };

    return 0;
}