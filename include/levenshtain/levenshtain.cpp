#include "levenshtain.hpp"

int levenshtain_distance(const std::string &str1, const std::string &str2)
{
    int m = str1.length();
    int n = str2.length();

    std::vector<int> prevRow(n + 1, 0);
    std::vector<int> currRow(n + 1, 0);

    for (int j = 0; j <= n; j++)
        prevRow[j] = j;

    for (int i = 1; i <= m; i++)
    {
        currRow[0] = i;
        for (int j = 1; j <= n; j++)
        {
            if (str1[i - 1] == str2[j - 1])
                currRow[j] = prevRow[j - 1];
            else
            {
                currRow[j] = 1 + std::min(
                                     // Insert
                                     currRow[j - 1] * COST_INSERT,
                                     std::min(
                                         // Remove
                                         prevRow[j] * COST_REMOVE,
                                         // Replace
                                         prevRow[j - 1] * COST_REPLACE));
            }
        }
        prevRow = currRow;
    }

    return currRow[n];
}

float levenshtain_distance(
    std::vector<int> a,
    std::vector<int> b)
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
                                     currRow[j - 1] * COST_INSERT,
                                     std::min(
                                         // Remove
                                         prevRow[j] * COST_REMOVE,
                                         // Replace
                                         prevRow[j - 1] * COST_REPLACE));
            }
        }
        prevRow = currRow;
    }

    return currRow[n];
}
