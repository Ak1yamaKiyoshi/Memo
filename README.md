### Memo

**Memo** is a stupidly simple note-taking app for me. Its key idea is to prevent users from viewing all their memories at once and instead to search them through keywords or exact matches. The key point is to avoid getting overwhelmed by a lot of facts or information but still be able to remember something in a small portion.

### State

As of now, the only possibility is to add memories and search through them. In the near future, a tag system will be added to filter the results, and there will be an ability to link memories like graphs.

### Motivation

I started this project because:
- I don’t like the current existing alternatives for such a case (Obsidian, Notion, etc.) because they are completely bloated.
- I regularly forget something important.
- I want to practice modern C++, since I have experience primarily with C, and switching to C++ is a bit tough without practice.

Since this app serves my own purpose for taking notes, you can expect me to maintain the repository over time.

### Disclaimer

The code is messy and buggy. Use at your own risk. All of us have a life outside of the computer, and so do I (not that often, really).

### How to Set Up and Use

**Compile:**

```bash
g++ -o memories ./main.cpp
```

**Run:**

```bash
./memories
```

**Commands:**

- `/add <text>`: Adds a note with the specified text.
- `/remove <id>`: Removes a note by ID.
- `/search <text>`: Searches through existing memories.
- `/exit`: Exits the program.