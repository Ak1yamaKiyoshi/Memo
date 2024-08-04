

#include <vector>
#include <ctime>
#include <ctime>
#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <stdlib.h> 
#include <vector>
#include <string>



std::string weekday_to_string(int i) {
    std::vector<std::string> weekdays = {"Monday", "Tueday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};


    std::string buffer;
    if (i < 7)
        buffer += weekdays[i];
    return buffer;
}

std::string month_to_string(int i) {
    std::vector<std::string> months = {
        "January", 
        "February", 
        "March", 
        "April", 
        "May", 
        "June", 
        "July", 
        "August", 
        "September", 
        "October", 
        "November", 
        "December"
    };

    std::string buffer;
    if (i < 12)
        buffer += months[i];
    return buffer;
}

std::string time_to_string(const time_t timestamp, const char *format) {
    char *output_buffer = new char[90];
    tm tstruct = *localtime(&timestamp);
    strftime(output_buffer, 90, format, &tstruct);
    return std::string(output_buffer);
}

std::string center(std::string input, int width = 113) { 
    return std::string((width - input.length()) / 2, ' ') + input;
}



std::string make_fixed_width_string(const std::string& input, size_t width) {
    std::ostringstream oss;
    oss << std::left << std::setw(width) << std::setfill(' ') << input;
    return oss.str().substr(0, width);
}




std::string time_to_string_verbose(const time_t& current, const time_t& past) {
    std::tm curs{};
    std::tm pasts{};
    
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&curs, &current);
        localtime_s(&pasts, &past);
    #else
        localtime_r(&current, &curs);
        localtime_r(&past, &pasts);
    #endif

    std::stringstream iss;

    int days_ago_same_year = curs.tm_yday - pasts.tm_yday;
    
    if (curs.tm_yday == pasts.tm_yday && curs.tm_year == pasts.tm_year) {
        iss << "Today at " << std::setw(2) << std::setfill('0')
            << pasts.tm_hour << ":" << std::setw(2) << std::setfill('0')
            << pasts.tm_min;
    } else if (days_ago_same_year == 1 && curs.tm_year == pasts.tm_year) {
        iss << "Yesterday at "
            << std::setw(2) << std::setfill('0') << pasts.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << pasts.tm_min;
    } else if (curs.tm_yday / 7 == pasts.tm_yday / 7 && curs.tm_year == pasts.tm_year) {
        iss << weekday_to_string(pasts.tm_wday) << " "
            << std::setw(2) << std::setfill('0') << pasts.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << pasts.tm_min;
    } else if (curs.tm_mon == pasts.tm_mon) {
        iss << days_ago_same_year << " days ago at "
            << pasts.tm_mday << (pasts.tm_mday == 1 ? "st" : 
                                 pasts.tm_mday == 2 ? "nd" : 
                                 pasts.tm_mday == 3 ? "rd" : "th") << " "
            << std::setw(2) << std::setfill('0') << pasts.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << pasts.tm_min;

    } else if (curs.tm_year == pasts.tm_year) {
        iss << pasts.tm_mday << (pasts.tm_mday == 1 ? "st" : 
                                 pasts.tm_mday == 2 ? "nd" : 
                                 pasts.tm_mday == 3 ? "rd" : "th") << " of "
            << month_to_string(pasts.tm_mon) + " "
            << std::setw(2) << std::setfill('0') << pasts.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << pasts.tm_min;
    } else {
        iss << time_to_string(past, "%Y-%m-%d %H:%M:%S");
    }

    return make_fixed_width_string(center(iss.str(), 25), 25);
}


/*
    default: year, month, day, minutes
    if current year: 
        month, day, hours, minutes
    if current month: 
        n-days ago, day, hours, minutes
    if current week:
        weekday, hours, minutes
    if yesterday:
        'yesterday', hours, minutes
*/




int main() {
    // Test case 1: Same Day, Different Time
    time_t current1 = 1725364200; // 2024-08-03 14:30:00
    time_t past1 = 1725350100;    // 2024-08-03 10:15:00
    std::cout << "Test 1: " << time_to_string_verbose(current1, past1) << std::endl;

    // Test case 2: Yesterday
    time_t current2 = 1725364200; // 2024-08-03 14:30:00
    time_t past2 = 1725278700;    // 2024-08-02 18:45:00
    std::cout << "Test 2: " << time_to_string_verbose(current2, past2) << std::endl;

    // Test case 3: Same Week, Different Day
    time_t current3 = 1725364200; // 2024-08-03 14:30:00
    time_t past3 = 1725039600;    // 2024-07-31 09:00:00
    std::cout << "Test 3: " << time_to_string_verbose(current3, past3) << std::endl;

    // Test case 4: Same Month, Different Week 
    time_t current4 = time(0); // 2024-08-30 14:30:00
    time_t past4 = 1722476168;    // 2024-08-10 12:00:00
    std::cout << "Test 4: " << time_to_string_verbose(current4, past4) << std::endl;

    time_t current5 = time(0)+1000000; // 2024-08-03 14:30:00
    time_t past5 = 1722276168-100000+1000000;    // 2024-05-22 08:20:00
    std::cout << "Test 5: " << time_to_string_verbose(current5, past5) << std::endl;

    // Test case 6: Different Year
    time_t current6 = 1725364200; // 2024-08-03 14:30:00
    time_t past6 = 1708820400;    // 2023-12-25 17:00:00
    std::cout << "Test 6: " << time_to_string_verbose(current6, past6) << std::endl;

    // Test case 7: New Year's Eve to New Year's Day
    time_t current7 = 1704080400; // 2024-01-01 01:00:00
    time_t past7 = 1704074340;    // 2023-12-31 23:59:00
    std::cout << "Test 7: " << time_to_string_verbose(current7, past7) << std::endl;

    // Test case 8: More Than a Year Ago
    time_t current8 = 1725364200; // 2024-08-03 14:30:00
    time_t past8 = 1655273100;    // 2022-06-15 07:45:00
    std::cout << "Test 8: " << time_to_string_verbose(current8, past8) << std::endl;

    return 0;
}