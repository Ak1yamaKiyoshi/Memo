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

### Thoughts 
As soon as simply writing notes does not really solves the problem of forgetting everything
I want to stir the ship: using spaced repetition techniques to actually remember stuff!

For that i need more data in each memory:
  - vector<time_t> viewed; // when memory was viewed. 

So, if there is linked memories to some other, I can show them and prompt user
 to recall content of desired one. 

It would be as separate mode, but sounds kinda interesting, allowing the app to be used as learning platform too. But not as effective as "Anki".

Also, the interesting part is analysis of notes, statistics:
 - repeated words, tags, most viewed notes, defining about what user actually thinks the most 

### Disclaimer

The code is messy and buggy. Use at your own risk. All of us have a life outside of the computer, and so do I (not that often, really).

### How to Set Up and Use

**Compile:**

```bash
chmod +x ./build.sh
./build.sh
```

**Run:**

```bash
chmod +x ./run.sh
./run.sh
```

**Commands:**

- `/add <text>`: Adds a note with the specified text.
- `/remove <id>`: Removes a note by ID.
- `/search <text>`: Searches through existing memories.
- `/exit`: Exits the program.