# Local Setup — GradeVault

> **Project page:** [gradevault.akshatgcodes.in](https://gradevault.akshatgcodes.in)
> GradeVault is a terminal program, so it runs on your machine. This guide covers building and using it.

---

## Prerequisites

- A C compiler: GCC or Clang (Linux / macOS), or MinGW (Windows)

---

## 1. Clone and build

```bash
git clone https://github.com/akshatgcodes/GradeVault
cd GradeVault
gcc -Wall -Wextra gradevault.c -o gradevault
```

Windows (MinGW):

```bash
gcc -Wall gradevault.c -o gradevault.exe
```

---

## 2. Environment variables

None.

---

## 3. Run

```bash
./gradevault
```

### Commands

```
add "Name" Subject=Mark [Subject=Mark ...]   e.g. add "Akshat" Math=88 Physics=76 Chemistry=91 English=83
add "Name" Mark [Mark ...]                    e.g. add "Akshat" 88 76 91 83
                                              (labelled Math, Physics, Chemistry, English, Biology, CS in order)
view all                                      every student with average, grade and marks
summary                                       the class report card
export <file>                                 roster + summary to a plain-text file
help                                          command list
exit | quit                                   save and quit
```

### Sample session

```
$ ./gradevault
GradeVault - student grade management
Type 'help' for commands. 0 student record(s) loaded from gradevault.dat.
> add "Akshat" 88 76 91 83
Added Akshat - average 84.5, grade B [PASS]
> summary
⚠️  Class Average: 84.5 - Class is passing overall
🔴 0 students failing | 🟡 0 borderline | 🟢 1 passing
📉 Weakest subject: Physics (avg 76.0)
📈 Strongest subject: Chemistry (avg 91.0)
(No previous batch on record yet - this is the baseline.)
Solid batch - everyone is clearing the bar.
> exit
Saved 1 student record(s) to gradevault.dat. Goodbye.
```

Records are saved when you `exit` or `quit`. Ctrl-C quits without saving that session.

---

## 4. Project page (optional)

```bash
cd landing && npm install && npm run dev   # http://localhost:3000
```

See [DEPLOY.md](DEPLOY.md) for how it's hosted.

---

## Testing

There's no automated test suite yet. CI compiles `gradevault.c` with `-Wall -Wextra` and lints and builds the project page. To check a change by hand, pipe a session in:

```bash
printf 'add "A" 30 35\nadd "B" 45 48\nadd "C" 90 95\nsummary\nexit\n' | ./gradevault
```

and confirm the counts (1 failing, 1 borderline, 1 passing) and the verdict.

---

## Known limitations

- Data files are created in the current directory, so run it from the same folder to keep one roster.
- `|`, `:` or `,` in a name or subject breaks the data file.

---

<div align="center">

<h3>Akshat Gupta</h3>

<a href="https://akshatgcodes.in"><img src="https://img.shields.io/badge/Website-akshatgcodes.in-111111?style=flat-square&logo=googlechrome&logoColor=white" alt="Website"></a>
<a href="https://linkedin.com/in/akshatgcodes"><img src="https://img.shields.io/badge/LinkedIn-akshatgcodes-0A66C2?style=flat-square" alt="LinkedIn"></a>
<a href="https://github.com/akshatgcodes"><img src="https://img.shields.io/badge/GitHub-akshatgcodes-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub"></a>
<a href="mailto:akshatgcodes@gmail.com"><img src="https://img.shields.io/badge/Email-akshatgcodes%40gmail.com-EA4335?style=flat-square&logo=gmail&logoColor=white" alt="Email"></a>

</div>
