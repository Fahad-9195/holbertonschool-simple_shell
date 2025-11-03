# 🐚 C - Simple Shell

> **A UNIX command line interpreter written in C**  
> _Developed as part of the Holberton School curriculum._

---

## 👥 Team Members
| Name | GitHub |
|------|---------|
| **Fahad Abdulaziz Alghamdi** | [Fahad-9195](https://github.com/Fahad-9195) |
| **Nabel Nasser Aldwese** | [Nabelaldwese](https://github.com/Nabelaldwese) |

---

## 🧠 Project Overview
The **Simple Shell** project is a collaborative implementation of a basic UNIX shell written in C.  
It replicates the behavior of `/bin/sh` for simple commands and demonstrates fundamental concepts of process creation, environment management, and command execution.

---

## 🎯 Learning Objectives
By completing this project, you will understand:
- How a **shell** works internally.
- The difference between **functions** and **system calls**.
- How to use `fork`, `execve`, `wait`, and `exit`.
- How the shell locates programs via the **PATH**.
- How to handle **environment variables**, **EOF (Ctrl+D)**, and **signals**.

---

## ⚙️ Requirements
- **OS:** Ubuntu 20.04 LTS  
- **Compiler:**  
  ```bash
  gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
