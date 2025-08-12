# C Input Utilities Boilerplate
This project provides a simple C boilerplate with reusable input functions (`get_int`, `get_double`, `get_string`, `get_bool`, `get_char`, `get_float`) for safe and user-friendly input handling.

## Getting Started

### 1. Fork and Clone the Repository

First, fork the repository to your own GitHub account using the "Fork" button on the repository page. Then, clone your forked repository:

```sh
git clone https://github.com/YOUR-USERNAME/c-boilerplate.git project-name
cd project-name
```
Replace `YOUR-USERNAME` with your GitHub username.

### 2. Build the Project

If a `Makefile` is provided, simply run:

```sh
make
```

You can also build a specific executable (for example, `main`) with:

```sh
make main
```

### 3. Run the Program

After building, run the executable:

```sh
./main
```

### 4. Project Structure

- `main.c` — Example usage of the input utility functions.
- `input_utils.h` — Declarations for all input functions.
- `input_utils.c` — Implementations of the input functions.

### 5. Extending the Boilerplate

You can add your own logic to `main.c` or create new files as needed. Use the input functions for safe and consistent user input.
For example, to use this boilerplate for your Capstone Project, create new file such as `calculator.c`

```sh
code calculator.c
```
To compile and run it, use the `make` command as above

```sh
make calculator
```


### 6. Publishing Your Project
Once you're satisfied with your project, you can publish it on GitHub.
Steps to publish:
1. Ensure your project is complete and tested.
2. Commit all changes:
```sh
git add .
git commit -m "Final version ready for publishing"
```
3. Push to your forked repository:
```sh
git push origin main
```
### 7 Sharing Your project link
To share your repository, click the "Code" button on your repository page and copy the URL provided.
Share the link on the Group

---

Feel free to open issues or pull requests if you have suggestions or improvements.

Credit: [@hexxondiv](https://github.com/hexxondiv)
