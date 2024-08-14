### Memo

**Memo** is a stupidly simple note-taking app for me. Its key idea is to prevent users from viewing all their memories at once and instead to search them through keywords or exact matches. The key point is to avoid getting overwhelmed by a lot of facts or information but still be able to remember something in a small portion.

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
mkdir build && cd build
cmake ..
make
```

**Run:**

cd build
./bin/memo
```

**Commands:**

- **/add <text>** (or **/a**)
  - Add a new memory. `<text>` can have tags (e.g., `#tag`) used for filtering in search.

- **/repetition** (or **/i**)
  - Not implemented yet.

- **/search <text>** (or **/s**)
  - Search memories using `<text>`. You can filter by tags like `#tag`.

- **/clear** (or **/c**)
  - Clear the screen.

- **/help** (or **/h**)
  - Display this help message.

- **/exit** (or **/e**)
  - Exit the program.

- **/remove <id>** (or **/r**)
  - Remove a memory with the specified `<id>`.

- **/update <id> <text>** (or **/u**)
  - Update a memory with the specified `<id>` to the new `<text>`.

- **/all**
  - Display all memories.