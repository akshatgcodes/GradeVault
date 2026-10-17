# Engineering Decisions — GradeVault

---

## Decision 1 — A flat text file instead of a database

**Context:** Records have to survive between sessions, but the tool should stay one file that compiles anywhere.

**Decision:** `gradevault.dat` stores one student per line as `name|count|Subject:mark,...`, written with `fprintf` and read back with `fgets` and manual parsing.

**Reason:** No SQLite or other dependency, so `gcc gradevault.c` is the whole build. The file is human-readable and fixable in any editor.

**Tradeoff:** `|`, `:` and `,` can't appear in names or subjects, and the whole file is rewritten on every save.

---

## Decision 2 — Trend from one stored number

**Context:** The summary should say whether this batch is better or worse than the last.

**Decision:** Each on-screen `summary` writes the class average to `gradevault_history.dat`. The next one reads it and prints the difference. `export` reads it but doesn't overwrite it.

**Reason:** One number is all that "dropped / improved N points from last batch" needs. Exporting a report shouldn't move the baseline.

**Tradeoff:** Only the most recent comparison is possible, with no multi-batch history.

---

## Decision 3 — The verdict is computed, not decorative

**Context:** The summary's closing line is meant to be blunt and accurate.

**Decision:** Four verdicts, chosen from the counts: failures outnumber passes, any failures, any borderline, or none of those.

**Reason:** The line always matches the numbers above it, so it reads like a judgement on this class, not a slogan.

**Tradeoff:** Only four possible lines.

---

## Decision 4 — Fixed-size arrays and bounded copies

**Context:** C string handling is where small tools usually break.

**Decision:** Static arrays (`MAX_STUDENTS 200`, `MAX_SUBJECTS 10`, `MAX_NAME_LEN 50`), `fgets` with a 1,024-byte line buffer, and explicit length limits on every copy.

**Reason:** No `malloc` / `free` bookkeeping and no overflow paths. The limits comfortably fit a class.

**Tradeoff:** Hard caps. A 201st student or an 11th subject is ignored.

---

## Decision 5 — Raw ANSI colour codes, and no colour in exports

**Context:** Status should be readable at a glance: green, yellow, red.

**Decision:** Hand-written `\033[..m` codes. `run_summary()` takes a `use_color` flag, so the same routine prints coloured output to the terminal and plain text to export files.

**Reason:** No ncurses dependency, and exported reports don't fill up with escape sequences.

**Tradeoff:** Old Windows consoles without ANSI support show the raw codes.

---

## What I'd do differently in v2

- **Save after every change** instead of only on exit, so Ctrl-C can't lose a session.
- **Edit and remove commands**, so a typo doesn't mean editing the data file by hand.
- **Escape delimiters** in names and subjects, or switch to quoted CSV.

---

## Explicit non-decisions (deferred to v2)

| Feature | Why deferred |
|---|---|
| Multiple classes | One roster per folder already gives separation |
| Weighted subjects | Needs a per-subject weight config and changes every average |
| Long-term trend history | One stored value covers "vs last batch", the main question |

---

<div align="center">

<h3>Akshat Gupta</h3>

<a href="https://akshatgcodes.in"><img src="https://img.shields.io/badge/Website-akshatgcodes.in-111111?style=flat-square&logo=googlechrome&logoColor=white" alt="Website"></a>
<a href="https://linkedin.com/in/akshatgcodes"><img src="https://img.shields.io/badge/LinkedIn-akshatgcodes-0A66C2?style=flat-square" alt="LinkedIn"></a>
<a href="https://github.com/akshatgcodes"><img src="https://img.shields.io/badge/GitHub-akshatgcodes-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub"></a>
<a href="mailto:akshatgcodes@gmail.com"><img src="https://img.shields.io/badge/Email-akshatgcodes%40gmail.com-EA4335?style=flat-square&logo=gmail&logoColor=white" alt="Email"></a>

</div>
