# GradeVault — Architecture

---

## 1. Stack

| Layer | Tech |
|---|---|
| Language | C99, standard library only (`stdio`, `stdlib`, `string`, `ctype`) |
| Storage | Two plain-text files in the working directory |
| Output | ANSI escape codes for colour |
| Project page | Next.js 16 static site in `landing/`, on Vercel |

---

## 2. Components

```
gradevault.c   the whole program: data model, file I/O, grading, commands, REPL
landing/       Next.js project page (separate from the CLI)
```

Inside `gradevault.c`:

| Section | What it does |
|---|---|
| `Student` struct + `students[200]` | name, subject count, subject names, marks |
| `student_average`, `letter_grade`, `student_status` | per-student maths, using the thresholds in the `#define`s |
| `load_students` / `save_students` | read and write `gradevault.dat` |
| `load_history` / `save_history` | read and write the previous class average |
| `cmd_add`, `cmd_view_all`, `cmd_export` | command handlers |
| `run_summary(out, use_color, update_history)` | one summary routine for both screen and export |
| `main` | REPL: read a line, dispatch, save on exit |

---

## 3. File Formats

`gradevault.dat`, one student per line:

```
Akshat|4|Math:88.00,Physics:76.00,Chemistry:91.00,English:83.00
```

`gradevault_history.dat`, one number:

```
84.5000
```

Both are gitignored runtime state.

---

## 4. Data Flow

```
start --> load_students(gradevault.dat)
loop:  "> " fgets(line)
         add ...      --> parse name + marks --> students[n++]
         view all     --> print each with avg / grade / coloured status
         summary      --> run_summary(stdout, colour, update_history=1)
                            --> per-subject averages, counts, trend vs history
                            --> save_history(class_avg)
         export f     --> run_summary(file, no colour, update_history=0)
         exit | EOF   --> break
end   --> save_students(gradevault.dat)
```

---

## 5. Summary Logic

1. Class average = mean of student averages. Below 60 prints a ⚠️ warning.
2. Each student's status is counted (fail < 40, borderline < 50, otherwise pass).
3. Subject averages: every mark is grouped by subject name across all students, and the lowest and highest are reported.
4. Trend: if `gradevault_history.dat` has a value, the difference is printed as "dropped" or "improved". The current average is then stored (on screen only, not on export).
5. Verdict: picked from the fail, borderline and pass counts, never a fixed string.

---

## 6. Security

- **Buffers:** names and subjects are copied with explicit length limits. Input lines are read with `fgets` into a 1,024-byte buffer.
- **Limits:** 200 students and 10 subjects per student. Extra input is ignored, not written past the arrays.
- **Files:** only `gradevault.dat`, `gradevault_history.dat` and the export path you give are touched, all relative to the current directory.

---

## 7. Error Handling

| Failure | Behaviour |
|---|---|
| Unknown command | Message pointing to `help` |
| `add` with no marks or a bad mark | Error message, nothing added |
| Data file can't be opened | Starts empty (load) or prints an error (save) |
| `export` without a file name | Usage message |
| Input ends (piped input) | Treated like `exit`, so records are saved |

---

## 8. Deployment

The CLI isn't hosted. It's built and run locally. The project page in `landing/` is a Vercel project with root directory `landing`, deployed on every push to `master` (see [DEPLOY.md](DEPLOY.md)).

---

## 9. Explicit Scope Cuts

- **Edit / delete** — add-only in v1.
- **Database** — flat files only.
- **Dynamic allocation** — fixed arrays sized for one class.

---

<div align="center">

<h3>Akshat Gupta</h3>

<a href="https://akshatgcodes.in"><img src="https://img.shields.io/badge/Website-akshatgcodes.in-111111?style=flat-square&logo=googlechrome&logoColor=white" alt="Website"></a>
<a href="https://linkedin.com/in/akshatgcodes"><img src="https://img.shields.io/badge/LinkedIn-akshatgcodes-0A66C2?style=flat-square" alt="LinkedIn"></a>
<a href="https://github.com/akshatgcodes"><img src="https://img.shields.io/badge/GitHub-akshatgcodes-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub"></a>
<a href="mailto:akshatgcodes@gmail.com"><img src="https://img.shields.io/badge/Email-akshatgcodes%40gmail.com-EA4335?style=flat-square&logo=gmail&logoColor=white" alt="Email"></a>

</div>
