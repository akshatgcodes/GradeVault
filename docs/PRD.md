# GradeVault — Product Requirements Document

**Status:** Final (v1 shipped)
**Owner:** Akshat Gupta
**One-liner:** A terminal grade manager in plain C that stores records in a file and writes a brutally honest class report card.

---

## 1. Problem

Grade calculators compute an average and forget it. A teacher or TA tracking a batch wants more than numbers: who's failing, who's one test away from it, which subject the class is weakest in, and whether this batch is doing better or worse than the last one. They want it without a spreadsheet, and in a form that's hard to ignore.

---

## 2. Goals (v1)

1. Add students with marks per subject, either labelled (`Math=88`) or positional (`88 76 91`).
2. Compute each student's average, letter grade and pass / borderline / fail status.
3. Save records to a file and reload them on the next run.
4. Print a class summary: average, pass/borderline/fail counts, weakest and strongest subject, trend against the last batch, and a verdict.
5. Export the roster and summary to a text file.
6. Run anywhere with a C compiler and no libraries.

---

## 3. Non-Goals (explicit scope cuts)

- **Editing or deleting students** — v1 is add-only. Fix mistakes by editing `gradevault.dat`.
- **Multiple classes** — one roster per working directory.
- **Weighted subjects** — every subject counts equally.
- **A GUI** — terminal only, with colour.

---

## 4. Users

**Primary:** Teachers, TAs and tutors who track a batch's marks and want a quick, blunt read on how it's going.

**Secondary:** Engineers reviewing a C project for file I/O, structs and string handling without external libraries.

---

## 5. User Stories

1. *As a teacher,* I type `add "Akshat" Math=88 Physics=76` so that a student is recorded with named subjects.
2. *As a teacher,* I type `add "Riya" 72 65 80` so that marks are labelled Math, Physics, Chemistry… automatically.
3. *As a teacher,* I run `view all` so that I see every student's average, grade and status, colour-coded.
4. *As a teacher,* I run `summary` so that I see the weakest subject and how many students are at risk.
5. *As a teacher,* I run `summary` again next term so that I see whether the average went up or down.
6. *As a teacher,* I run `export report.txt` so that I can share the report.

---

## 6. Functional Requirements

### 6.1 Records

- Up to 200 students, 10 subjects each, names up to 49 characters.
- Positional marks are labelled in order: Math, Physics, Chemistry, English, Biology, CS.
- Records are saved to `gradevault.dat` on `exit` / `quit` / end of input, and loaded at startup.

### 6.2 Grading

- Letter grade from the average: A ≥ 90, B ≥ 75, C ≥ 60, D ≥ 40, F < 40.
- Status: PASS ≥ 50, BORDERLINE 40–49.9, FAIL < 40.

### 6.3 Summary

- Class average, flagged when below 60.
- Count of failing, borderline and passing students.
- Weakest and strongest subject by the average of every mark recorded under that subject name.
- Trend against the average stored by the previous `summary` in `gradevault_history.dat`. The first run is the baseline.
- Verdict: "Most of this class is failing…" if failures outnumber passes, "A few students are sinking…" if any fail, "…one bad test from it" if any are borderline, otherwise "Solid batch…".

### 6.4 Export

- `export <file>` writes the roster and summary as plain text without colour codes. It doesn't update the trend history.

---

## 7. Non-Functional Requirements

- **Portability:** C standard library only. Builds with GCC or Clang on Linux and macOS, and MinGW on Windows.
- **Safety:** fixed-size buffers with bounded copies. Lines longer than the buffer or extra subjects are truncated, not overflowed.
- **Readability:** the data file is plain text and can be edited by hand.

---

## 8. Success Metrics

| Metric | Target |
|---|---|
| Build | `gcc -Wall -Wextra gradevault.c` compiles cleanly (checked in CI) |
| Persistence | A restart shows the same roster and a trend line |
| Report | `summary` names a real weakest subject and a verdict that matches the counts |

---

## 9. Risks & Open Questions

- **Save on exit only** — records are written when you `exit`, `quit` or input ends. Killing the program with Ctrl-C loses that session's changes.
- **Pipe characters** — a `|` or `:` in a name or subject would break the file format.
- **One history value** — only the previous average is kept, so there's no long-term trend.

---

## 10. v2 Candidates

- Save after every `add`, so a crash loses nothing.
- `edit` and `remove` commands.
- CSV import and export.
- A full history of class averages, for a trend over several batches.

---

<div align="center">

<h3>Akshat Gupta</h3>

<a href="https://akshatgcodes.in"><img src="https://img.shields.io/badge/Website-akshatgcodes.in-111111?style=flat-square&logo=googlechrome&logoColor=white" alt="Website"></a>
<a href="https://linkedin.com/in/akshatgcodes"><img src="https://img.shields.io/badge/LinkedIn-akshatgcodes-0A66C2?style=flat-square" alt="LinkedIn"></a>
<a href="https://github.com/akshatgcodes"><img src="https://img.shields.io/badge/GitHub-akshatgcodes-181717?style=flat-square&logo=github&logoColor=white" alt="GitHub"></a>
<a href="mailto:akshatgcodes@gmail.com"><img src="https://img.shields.io/badge/Email-akshatgcodes%40gmail.com-EA4335?style=flat-square&logo=gmail&logoColor=white" alt="Email"></a>

</div>
